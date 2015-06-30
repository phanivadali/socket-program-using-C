/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main()

{

	int sock, bytes_recieved, i, connected;
	char user_choice[1];
	char *recv_data;
	struct hostent *host;
	char nl;
	struct sockaddr_in server_addr;

	host = gethostbyname("192.168.2.10");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2030);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	if (connect(sock, (struct sockaddr *)&server_addr,
		sizeof(struct sockaddr)) == -1) {
		perror("Connect");
		exit(1);
	}

	while(1) {
		printf("\t\tMenu\n");
		printf("1. Display a list of available stocks\n");
		printf("4. Exit\n");
		printf("Enter your choice:\n");
		fflush(stdin);
		scanf("%c", &nl);
		user_choice[0]= nl;
		send(sock, user_choice, 1, 0);

		switch(user_choice[0]) {
			case '1':
				recv(sock, recv_data, 200, 0);
				printf("%s\n", recv_data);
				break;
			case '4':
				close(sock);
				exit(1);
			default:
				break;
		}
	}

	return 0;
}