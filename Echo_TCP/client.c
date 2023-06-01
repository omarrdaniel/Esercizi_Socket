User
modifica il seguente codice di un client in C il quale utilizza le socket per permettergli di effettuare una richiesta al server in cui chiede un file di testo, una volta ricevuto visualizza il contenuto

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 255
#define PORT 3333

void die(char *);

int main()
{
	/*Creo il sd*/
	int socketdescriptor;
	/*Creo i due buffer invio e ricezione*/
	char sendbuff[MAXLEN], recvbuff[MAXLEN];
	/*Creo la struttura della socket internet*/
	struct sockaddr_in server_ip_port;
	/*La lunghezza della struttura*/
	int server_ip_port_length = sizeof(server_ip_port);

	/*Pulisco i buffer*/
	memset(sendbuff, 0, MAXLEN);
	memset(recvbuff, 0, MAXLEN);

	/*Creo la scoket a cui passo dominio, tipo STREAM è TCP e il protocollo*/
	socketdescriptor = socket(AF_INET, SOCK_STREAM, 0);

	/*Controllo se la socket è stata creata*/
	if (socketdescriptor < 0)
		die("socket() error");
	printf("socket() ok.\n");

	/*Leggo la stringa da inviare*/
	printf("Inserisci stringa: ");
	fgets(sendbuff, MAXLEN, stdin);

	/*Valorizzo indirizzo porta del server*/
	server_ip_port.sin_family = AF_INET;
	server_ip_port.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_ip_port.sin_port = htons(PORT);

	/*Faccio la connect a cui passo sd, l'indirizzo del server e la lunghezza dell'indirizzo */
	if (connect(socketdescriptor, (struct sockaddr *)&server_ip_port, server_ip_port_length) < 0)
	{
		/*Controllo eventuali errori nella connect*/
		perror("connect");
		exit(1);
	}

	/*Invio la stringa al server, passo il sd, il buffer (conetiene il mio testo), la dimesione del testo da inviare e le opzioni*/
	int bytesent = send(socketdescriptor, sendbuff, MAXLEN, 0);

	/*Controllo eventuali errori nella send*/
	if (bytesent <= 0)
		die("send() error");

	/*Stampo a schermo il nuermo di byte inviati*/
	printf("%d byte sent.\n", bytesent);

	/*IL SERVER HA FATTO UNA RECIVE PER RICEVERE I MIEI DATI E ORA STA FACENDO UNA SEND*/

	/*Ricevo la stringa dal server, passo il sd, il buffer (vuoto), la dimesione del testo da inviare e le opzioni*/
	int byterecvd = recv(socketdescriptor, recvbuff, MAXLEN, 0);

	/*Controllo eventuali errori nella recv*/
	if (byterecvd <= 0)
		die("recv() error");

	/*Stampo a schermo il nuermo di byte ricevuti*/
	printf("%d byte received.\n", byterecvd);

	/*Stampo a schermo la stringa ricevuta*/
	printf("Received: %s\n", recvbuff);

	/*Chiudo la socket*/
	close(socketdescriptor);

	return 0;
}


void die(char *error)
{

	fprintf(stderr, "%s.\n", error);
	exit(1);
}
