#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 4950
#define BUFSIZE 1024
#define MAX_CLIENTS 10

void send_to_all(int j, int i, int sockfd, int nbytes_recvd, char *recv_buf, fd_set *master)
{

}

void send_recv(int i, fd_set *master, int sockfd, int fdmax)
{

}


int main(int argc, char ** argv)
{
	//Creation des listes de sockets
	fd_set master;
	fd_set read_fds;

	//Declaration des variables
	int fdmax, i;
	int sockfd= 0;
	struct sockaddr_in my_addr, client_addr;

	//Initiation des listes aux ensembles vides
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//Creation du socket serveur
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
	//Affectation des parametres ( IP et port etc.)
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);


	if (bind(sockfd, (struct sockaddr *) & my_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(1);
	}

	//Definir la taille de la file de connexions en attente (MAX_CLIENTS)
	if (listen(sockfd, MAX_CLIENTS) == -1) {
		perror("listen");
		exit(1);
	}

	//Affichage du message d'attente sur le terminal ( d'ou le stdout ).
	printf("\nTCP Server Waiting for clients on port 4950\n");
	fflush(stdout);

	//ajout du socket serveur a l'ensemble des sockets "master"
	FD_SET(sockfd, &master);

	//Initialisation de la valeur du plus grand du plus grand descripteur
	//( pour le moment le descripteur du socket serveur)
	fdmax = sockfd;

	//Boucle infinie pour ecouter sur le port
	//et accepter/refuser les demandes des clients
	while(1){
		read_fds = master;
		//Verfication du changement d'état des sockets contenus dans l'ensemble read_fds
		//verifie seulement s'il existe des donnees en lecture disponible
		//Attente illimité (NULL)
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(4);
		}

		for (i = 0; i <= fdmax; i++){
			//FD_ISSET verifie si le socket i est present dans l'ensemble read_fds
			if (FD_ISSET(i, &read_fds)){
				if (i == sockfd){  //Si c'est le socket serveur c'est une demande de connection
					socklen_t addrlen;
					int newsockfd;
					addrlen = sizeof(struct sockaddr_in);

					if((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) == -1) {
						perror("accept");
						exit(1);
					}
					else {
						FD_SET(newsockfd, &master);
						if(newsockfd > fdmax){
							fdmax = newsockfd;
						}
						printf("new connection from %s on port %d \n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					}
				}
				else{ //Sinon c'est une donnée en lecture d'un socket client
					int nbytes_recvd, j;
					char recv_buf[BUFSIZE];

					//Recuperation du message envoyé par le socket client i
					if ((nbytes_recvd = recv(i, recv_buf, BUFSIZE, 0)) <= 0) {
						if (nbytes_recvd == 0) {
							printf("socket %d a quitte \n", i);
						}else {
							perror("recv");
						}
						close(i);
						FD_CLR(i, &master);
					}

					else {
						//Envoi à tous les autres clients sauf au socket server et a lui meme.
						for(j = 0; j <= fdmax; j++){
							if (FD_ISSET(j, &master)){
								if (j != sockfd && j != i) {
									if (send(j, recv_buf, nbytes_recvd, 0) == -1) {
										perror("send");
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
