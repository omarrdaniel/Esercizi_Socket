#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 255
#define  PORT 3333

void die(char *);

int main() {

	int socketdescriptor;
	char sendbuff[MAXLEN], recvbuff[MAXLEN];
	
	struct sockaddr_in server_ip_port;
	int server_ip_port_length = sizeof(server_ip_port);
	
	memset(sendbuff, 0, MAXLEN);
	memset(recvbuff, 0, MAXLEN);
	
	/*Creo la scoket*/
	socketdescriptor = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (socketdescriptor < 0) die("socket() error");
	printf("socket() ok.\n");
	
	printf("Inserisci stringa: ");
	gets(sendbuff);
	
	/*Valorizzo indirizzo porta del server*/
	server_ip_port.sin_family = AF_INET;
	server_ip_port.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_ip_port.sin_port = htons(PORT);
	
	/*Siamo su UDP quindi specifico anche gli indirizzi*/
	int bytesent = sendto(socketdescriptor, sendbuff, MAXLEN, 0, (struct sockaddr *) &server_ip_port, server_ip_port_length);
	
	if (bytesent <= 0) die("sendto() error");
	printf("%d byte sent.\n", bytesent);
	
	int byterecvd = recvfrom(socketdescriptor, recvbuff, MAXLEN, 0, (struct sockaddr *) &server_ip_port, &server_ip_port_length);
	
	if (byterecvd <= 0) die("recvfrom() error");
	printf("%d byte received.\n", byterecvd);
	
	printf("%s\n", recvbuff);
	
	close(socketdescriptor);	

	return 0;

}

void die(char *error) {
	
	fprintf(stderr, "%s.\n", error);
	exit(1);
	
}
