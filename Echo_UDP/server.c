#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 255
#define PORT 3333

void die(char *);
void handleClient(int, struct sockaddr *, int);

int main() {
	
	int socketdescriptor;
	struct sockaddr_in bind_ip_port, client_ip_port;
	int bind_ip_port_length = sizeof(bind_ip_port);
	int client_ip_port_length = sizeof(client_ip_port);

	socketdescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (socketdescriptor < 0) die("socket() error");
	printf("socket() ok.\n");
	
	/*Valorizzo l'indirizzo e la porta*/
	bind_ip_port.sin_family = AF_INET;
	bind_ip_port.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind_ip_port.sin_port = htons(PORT);
	
	/*Attenzione al cast che va fatto al secondo parametro*/
	if (bind(socketdescriptor, (struct sockaddr *) &bind_ip_port, bind_ip_port_length) < 0) die("bind() error");
	printf("bind() ok.\n");
	
	/*While che permette di andare a gestire i client*/
	while (1) {
		
		handleClient(socketdescriptor, (struct sockaddr *) &client_ip_port, client_ip_port_length);
		
	}
	
	close(socketdescriptor);
	
	return 0;
	
}

void handleClient(int sd, struct sockaddr *sa, int length) {
	
	char sendbuff[MAXLEN], recvbuff[MAXLEN];
	
	memset(sendbuff, 0, MAXLEN);
	memset(recvbuff, 0, MAXLEN);
	
	/*Ricevo dal client*/
	int byterecvd = recvfrom(sd, recvbuff, MAXLEN, 0, sa, &length);
	if (byterecvd <= 0) die("recvfrom() error");
	printf("%d byte received.\n", byterecvd);
		
	strcpy(sendbuff, recvbuff);
	
	/*Vado in risposta verso il client*/
	int bytesent = sendto(sd, sendbuff, MAXLEN, 0, sa, length);
	if (bytesent <= 0) die("sendto() error");
	printf("%d byte sent.\n", bytesent);
	
}

void die(char *error) {
	
	fprintf(stderr, "%s.\n", error);
	exit(1);
	
}
