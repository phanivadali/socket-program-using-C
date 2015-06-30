/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define _GNU_SOURCE

void socket_program(char **read);

void socket_program(char **read) {
	struct sockaddr_in server_addr, client_addr;
	int sin_size, sock, connected, bytes_recieved , true = 1,k = 0;
    char recv_data[1];
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)	{
        perror("Socket");
        exit(1);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2030);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
            perror("Unable to bind");
            exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }

	while(1) {
	    sin_size = sizeof(struct sockaddr_in);
	    connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
		while (1)
		{
			bytes_recieved = recv(connected, recv_data, 1, 0);
			printf("\n");
			printf("Recieved from client %c\n", recv_data[0]);
			switch(recv_data[0])
			{
			  case '1':
				  printf("In case 1\n");
				  int i = 0;
				  for (i = 0; i < 20; i++) {
				  	printf("sending %s\n", *(read + i));
				  	send(connected, *(read + i), 200, 0);	
				  }
				  break;
			  case '4':
				  close(sock);
				  exit(1);
			  break;
			  default:
			  break;
			}

		}
	}
	
	close(sock);

    return;
}

int main()
{
	int i = 0, number_of_lines = 20;
	FILE *fp;
	char line[128];
	char **read_line;
	fp = fopen("stocks.txt", "r");
	read_line = (char **)malloc(sizeof(char*) * number_of_lines);
	while (fgets(line, sizeof line, fp) != NULL) {
		read_line[i] = (char *)malloc(sizeof(line));
		strcpy(read_line[i], line);
		i += 1;
	}
	fclose(fp);

	socket_program(read_line);

	return 0;
}
