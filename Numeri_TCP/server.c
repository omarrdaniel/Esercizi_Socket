#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLEN 255
#define PORT 3333
#define LISTEN 5

void die(char *);
void handleClient(int);

int main()
{

	/*Creo i due sd*/
	int socketdescriptor, clientdescriptor;
	/*Creo le due strutture per i due indirizzi*/
	struct sockaddr_in bind_ip_port, client_ip_port;
	/*Trovo le due lunghezze*/
	int bind_ip_port_length = sizeof(bind_ip_port);
	int client_ip_port_length = sizeof(client_ip_port);

	/*Creo la scoket a cui passo dominio, tipo STREAM è TCP e il protocollo. 0 sta per i flag aggiuntivi*/
	socketdescriptor = socket(AF_INET, SOCK_STREAM, 0);

	/*Controllo se la socket è stata creata. Se va in errore, ritorna -1 e quindi capiamo che c'è stato un errore*/
	if (socketdescriptor < 0)
		die("socket() error");
	printf("socket() ok.\n");

	//applichiamo le opzioni reuse address e reuse port così se ci sono errori, possiamo subito riutilizzare quella porta e quell'IP.
	//entrambe le opzioni sono opzionali e non richieste
	/*if(setsockopt(socketdescriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");
	if(setsockopt(socketdescriptor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEPORT) failed");*/

	/*Valorizzo indirizzo porta del server*/
	//usiamo 0.0.0.0 per accettare qualsiasi connessione e non solo quelle che arrivano da local host (bisogna mettere 127.0.0.1)
	bind_ip_port.sin_family = AF_INET;
	bind_ip_port.sin_addr.s_addr = inet_addr("0.0.0.0");
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
		//questa volta gli passiamo tutti i parametri così riusciamo a stampare le info del client connesso con i metodi usati sotto
		clientdescriptor = accept(socketdescriptor, (struct sockaddr *)&client_ip_port, (socklen_t *)&client_ip_port_length);

		/*Stampo informazioni per client e server*/
		printf("\n\t--------- INFORMAZIONI -------------\n");
		printf("\tSERVER IP: %s\n", inet_ntoa(bind_ip_port.sin_addr));
		printf("\tSERVER PORT: %d\n", ntohs(bind_ip_port.sin_port));
		printf("\tCLIENT IP: %s\n", inet_ntoa(client_ip_port.sin_addr));
		printf("\tCLIENT PORT: %d\n", ntohs(client_ip_port.sin_port));
		printf("\t------------------------------------\n\n");

		/*Eseguo la funzione per andare a gestire il client*/
		handleClient(clientdescriptor);
	}
	/*Chiudo la socket, non ci arrivo mai tanto*/
	close(socketdescriptor);
	return 0;
}

/*Funzione per la gestione del client*/
void handleClient(int sd)
{

	int ricevuto1 = 0;
	int ricevuto2 = 0;
	/*--------RICEZIONE PRIMO NUMERO--------*/
	/*Vado in ricezione dal client*/
	int byterecvd = read(sd, &ricevuto1, sizeof(ricevuto1));

	/*Controllo eventuali errori nella recv*/
	if (byterecvd <= 0)
		die("read() error");
	printf("%d byte received.\n", byterecvd);

	int num1local = ntohl(ricevuto1);
	printf("Ricevuto %d\n", num1local);
	printf("Ricevuto formato network %d\n", ricevuto1);
	/*------FINE RICEZIONE PRIMO NUMERO------*/

	/*-------RICEZIONE SECONDO NUMERO--------*/
	/*Vado in ricezione dal client*/
	int byterecvd2 = read(sd, &ricevuto2, sizeof(ricevuto2));

	/*Controllo eventuali errori nella recv*/
	if (byterecvd2 <= 0)
		die("read() error");
	printf("%d byte received.\n", byterecvd2);

	int num2local = ntohl(ricevuto2);
	printf("Ricevuto formato host %d\n", num2local);
	printf("Ricevuto formato network %d\n", ricevuto2);
	/*-----FINE RICEZIONE SECONDO NUMERO------*/

	/*------CALCOLO SOMMA LATO SERVER-------*/
	printf("Calculting %d + %d\n",num1local, num2local);
	int sum = num1local + num2local;
	printf("\nSomma: %d\n", sum);

	int netlong = htonl(sum);
	/*Invio il messaggio al client*/
	int bytesent = write(sd, &netlong, sizeof(netlong));

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
