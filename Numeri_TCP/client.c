#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLEN 255
#define PORT 3333
void die(char *);

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Formato di avvio: './client <par1> <par2>'");
		return -1;
	}
	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[2]);
	printf("num1: %d\n", num1);
	printf("num2: %d\n\n", num2);
	/*Creo il sd*/
	int socketdescriptor;
	/*Creo i due buffer invio e ricezione*/
	int send = 0, recv = 0;
	/*Creo la struttura della socket internet*/
	struct sockaddr_in server_ip_port;
	/*La lunghezza della struttura*/
	int server_ip_port_length = sizeof(server_ip_port);

	if (argc < 3)
	{
		printf("Formato: client <numero 1> <numero 2>\n");
		return -1;
	}
	/*Creo la scoket a cui passo dominio, tipo STREAM è TCP e il protocollo*/
	socketdescriptor = socket(AF_INET, SOCK_STREAM, 0);

	/*Controllo se la socket è stata creata*/
	if (socketdescriptor < 0)
		die("socket() error");
	printf("socket() ok.\n");

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

	/*-----------INVIO PRIMO NUMERO-----------*/

	int netlong1 = htonl(num1);
	/*Invio la stringa al server, passo il sd, il buffer (conetiene il mio testo), la dimesione del testo da inviare e le opzioni*/
	int bytesent = write(socketdescriptor, &netlong1, sizeof(netlong1));

	/*Controllo eventuali errori nella send*/
	if (bytesent <= 0)
		die("send() error");

	/*Stampo a schermo il nuermo di byte inviati*/
	printf("Sent: %d\n", num1);
	printf("Sent-htonl: %d\n", netlong1);
	printf("%d byte sent.\n\n", bytesent);
	/*---------FINE INVIO PRIMO NUMERO---------*/

	/*-----------INVIO SECONDO NUMERO----------*/
	int netlong2 = htonl(num2);
	/*Invio la stringa al server, passo il sd, il buffer (conetiene il mio testo), la dimesione del testo da inviare e le opzioni*/
	int bytesent2 = write(socketdescriptor, &netlong2, sizeof(netlong2));

	/*Controllo eventuali errori nella send*/
	if (bytesent2 <= 0)
		die("send() error");

	/*Stampo a schermo il nuermo di byte inviati*/
	printf("Sent: %d\n", num2);
	printf("Sent-htonl: %d\n", netlong2);
	printf("%d byte sent.\n\n", bytesent2);
	/*---------FINE INVIO SECONDO NUMERO--------*/

	/*IL SERVER HA FATTO UNA RECIVE PER RICEVERE I MIEI DATI E ORA STA FACENDO UNA SEND*/

	int ricevuto = 0;

	/*Ricevo la stringa dal server, passo il sd, il buffer (vuoto), la dimesione del testo da inviare e le opzioni*/
	int byterecvd = read(socketdescriptor, &ricevuto, sizeof(ricevuto));

	/*Controllo eventuali errori nella recv*/
	if (byterecvd <= 0)
		die("recv() error");

	/*Stampo a schermo il nuermo di byte ricevuti*/
	printf("%d byte received.\n", byterecvd);
	int locallong = ntohl(ricevuto);
	/*Stampo a schermo la stringa ricevuta*/
	printf("Received: %d\n", locallong);

	/*Chiudo la socket*/
	close(socketdescriptor);

	return 0;
}

void die(char *error)
{

	fprintf(stderr, "%s.\n", error);
	exit(1);
}
