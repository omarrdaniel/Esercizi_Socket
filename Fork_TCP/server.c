#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 255
#define PORT 3333
#define LISTEN 5

void die(char *);
void handleClient(int);

int main()
{
	pid_t pid;

	/*Creo i due sd*/
	int socketdescriptor, csd;
	/*Creo le due strutture per i due indirizzi*/
	struct sockaddr_in bind_ip_port, client_ip_port;
	/*Trovo le due lunghezze*/
	int bind_ip_port_length = sizeof(bind_ip_port);
	int client_ip_port_length = sizeof(client_ip_port);

	/*Creo la scoket a cui passo dominio, tipo STREAM è TCP e il protocollo*/
	socketdescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(socketdescriptor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
		die("setsockopt(SO_REUSEADDR) failed");
	/*if(setsockopt(socketdescriptor, SOL_SOCKET, SO_REUSEPORT, &(int){ 1 }, sizeof(int)) < 0)
		die("setsockopt(SO_REUSEPORT) failed");*/

	/*Controllo se la socket è stata creata*/
	if (socketdescriptor < 0)
		die("socket() error");
	printf("socket() ok.\n");

	/*Valorizzo indirizzo porta del server*/
	bind_ip_port.sin_family = AF_INET;
	bind_ip_port.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind_ip_port.sin_port = htons(PORT);

	/*Faccio la connect a cui passo sd, l'indirizzo del server e la lunghezza dell'indirizzo */
	if (bind(socketdescriptor, (struct sockaddr *)&bind_ip_port, bind_ip_port_length) < 0)
		die("bind() error");
	printf("bind() ok.\n");

	/*Effettuo la listen in cui specifico massimo 5 connessioni in coda*/
	int listen_res = listen(socketdescriptor, LISTEN);

	/*Controllo eventuali errori nella listen*/
	if (listen_res < 0)
		die("listen() error");
	else
		printf("listen() ok.\n");

	/*While che permette di andare a gestire i client senza interrompere il server ogni volta*/
	while (1)
	{
		/*Accetto la connessione del client*/
		csd = accept(socketdescriptor, (struct sockaddr *)&client_ip_port, (socklen_t *)&client_ip_port_length);

		/*Stampo informazioni per client e server*/
		printf("\n\t--------- INFORMAZIONI -------------\n");
		printf("\tSERVER IP: %s\n", inet_ntoa(bind_ip_port.sin_addr));
		printf("\tSERVER PORT: %d\n", ntohs(bind_ip_port.sin_port));
		printf("\tCLIENT IP: %s\n", inet_ntoa(client_ip_port.sin_addr));
		printf("\tCLIENT PORT: %d\n", ntohs(client_ip_port.sin_port));
		printf("\t------------------------------------\n\n");

		pid=fork();

		if(pid < 0)
		 die("fork() error");
		else if (pid>0)
			close(csd);
		else {
			close(socketdescriptor);
			handleClient(csd);
			exit(0);
		}
	}
	close(socketdescriptor);
	return 0;
}

/*Funzione per la gestione del client*/
void handleClient(int sd)
{
	/*Creo i buffer*/
	char sendbuff[MAXLEN], recvbuff[MAXLEN];

	/*Pulisco i buffer*/
	memset(sendbuff, 0, MAXLEN);
	memset(recvbuff, 0, MAXLEN);

	/*Vado in ricezione dal client*/
	int byterecvd = recv(sd, recvbuff, MAXLEN, 0);

	/*Controllo eventuali errori nella recv*/
	if (byterecvd <= 0)
		die("recv() error");
	printf("%d byte received.\n", byterecvd);
	printf("Recived: %s\n", recvbuff);

	/*Stampo il messaggio ricevuto, quello che ricevo in quello dainviare*/
	strcpy(sendbuff, recvbuff);

	/*Invio il messaggio al client*/
	int bytesent = send(sd, sendbuff, MAXLEN, 0);

	/*Controllo eventuali errori nella send*/
	if (bytesent <= 0)
		die("send() error");
	printf("%d byte sent.\n", bytesent);

	/*Chiudo la socket*/
	close(sd);
}

/*Funzione per la gestione degli errori*/
void die(char *error)
{

	fprintf(stderr, "%s.\n", error);
	exit(1);
}
