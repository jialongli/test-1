/*
*	Client_Demo of OpenSSL
*	Date:Dec.31th,2014
*	Author:Langley Chang
*/

#include <stdio.h>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define PORT 22087
#define BUFFER_SIZE 1024

extern SSL_CTX* init_SSL();

/*
 *	SSL is a layer between transmission layer and application layer.
 *	For client,whether being verificated is optional.So,you didn't load certificate and everything goes OK.
 *	Just like server,the procedure is a little more than normal socket.
 * */


int main(int argc,char *argv[])
{	

	struct sockaddr_in addr;
	// SSL_CTX describes the environment of SSL connection. 
	// SSL stands for the connection and generated when connected
	SSL *ssl = NULL;
	SSL_CTX *ctx = init_SSL();
	char *server_IP = SERVERIP;
	int port = PORT;

	if(argc >= 2)
	{
		server_IP = argv[1];
		//TODO check input here
	}
	if(argc >= 3)
	{
		port = atoi(argv[2]);
		if(port > 0xffff || port < 0x4ff)
		{
			fprintf(stderr,"port input invalid");
			exit(2);
		}

	}
		
	// If connected,ssl is generated and able to be used in SSL_write() or SSL_read()
	int client_fd = SSL_conn(&ssl,&ctx,server_IP,port);

	char buffer[BUFFER_SIZE];

	sprintf(buffer,"Hello RTOAX");
	SSL_write(ssl, buffer, strlen(buffer));
	SSL_read(ssl, buffer, BUFFER_SIZE);
	fprintf(stdout, "CLIENT>>%s\n", buffer);

	//close
	SSL_release(ssl,ctx,client_fd);
}
