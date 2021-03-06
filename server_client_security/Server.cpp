#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <stddef.h>
#include <stdio.h>
#include <cctype>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "support.h"
#include "Server.h"

void help(char *progname)
{
	printf("Usage: %s [OPTIONS]\n", progname);
	printf("Initiate a network file server\n");
	printf("  -m    enable multithreading mode\n");
	printf("  -l    number of entries in the LRU cache\n");
	printf("  -p    port on which to listen for connections\n");
}

void die(const char *msg1, char *msg2)
{
	fprintf(stderr, "%s, %s\n", msg1, msg2);
	exit(0);
}

/*
	* open_server_socket() - Open a listening socket and return its file
	*                        descriptor, or terminate the program
	*/
int open_server_socket(int port)
{
	int listenfd;			  /* the server's listening file descriptor */
	struct sockaddr_in addrs; /* describes which clients we'll accept */
	int optval = 1;			  /* for configuring the socket */

	/* Create a socket descriptor */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		die("Error creating socket: ", strerror(errno));
	}

	/* Eliminates "Address already in use" error from bind. */
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0)
	{
		die("Error configuring socket: ", strerror(errno));
	}

	/* Listenfd will be an endpoint for all requests to the port from any IP
		address */
	bzero((char *)&addrs, sizeof(addrs));
	addrs.sin_family = AF_INET;
	addrs.sin_addr.s_addr = htonl(INADDR_ANY);
	addrs.sin_port = htons((unsigned short)port);
	if (bind(listenfd, (struct sockaddr *)&addrs, sizeof(addrs)) < 0)
	{
		die("Error in bind(): ", strerror(errno));
	}

	/* Make it a listening socket ready to accept connection requests */
	if (listen(listenfd, 1024) < 0) // backlog of 1024
	{
		die("Error in listen(): ", strerror(errno));
	}

	return listenfd;
}

/*
	* handle_requests() - given a listening file descriptor, continually wait
	*                     for a request to come in, and when it arrives, pass it
	*                     to service_function.  Note that this is not a
	*                     multi-threaded server.
	*/
void handle_requests(int listenfd, void (*service_function)(int, int), int param, bool multithread)
{
	while (1)
	{
		/* block until we get a connection */
		struct sockaddr_in clientaddr;
		memset(&clientaddr, 0, sizeof(sockaddr_in));
		socklen_t clientlen = sizeof(clientaddr);
		int connfd;
		if ((connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen)) < 0)
		{
			die("Error in accept(): ", strerror(errno));
		}

		/* print some info about the connection */
		struct hostent *hp;
		hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		if (hp == NULL)
		{
			fprintf(stderr, "DNS error in gethostbyaddr() %d\n", h_errno);
			exit(0);
		}
		char *haddrp = inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n", hp->h_name, haddrp);

		/* serve requests */
		service_function(connfd, param);

		/* clean up, await new connection */
		if (close(connfd) < 0)
		{
			die("Error in close(): ", strerror(errno));
		}
	}
}

/*
	* file_server() - Read a request from a socket, satisfy the request, and
	*                 then close the connection.
	*/
void file_server(int connfd, int lru_size)
{
	while (1)
	{
		const int MAXLINE = 8192;
		char buf[MAXLINE]; /* a place to store text from the client */
		bzero(buf, MAXLINE);

		/* read from socket, recognizing that we may get short counts */
		char *bufp = buf;		   /* current pointer into buffer */
		ssize_t nremain = MAXLINE; /* max characters we can still read */
		size_t nsofar;			   /* characters read so far */

		while (1)
		{
			// read some data; swallow EINTRs
			if ((nsofar = read(connfd, bufp, nremain)) < 0)
			{
				if (errno != EINTR)
				{
					die("read error: ", strerror(errno));
				}
				//PUT request
				if (strstr(bufp, "PUT"))
				{
					char *pfilename = bufp + 4;
					for (; *pfilename != '\n'; ++pfilename)
					{
						if (!isprint(*pfilename))
						{
							fprintf(stderr, "invalid filename\n");
							break;
						}
					}
					++pfilename;
					char sizebuff[64];
					int bytesread = read(connfd, sizebuff, 64);
					for (char *pstr = sizebuff; *pstr != '\n'; ++pstr)
					{
						if (!isdigit(*pstr))
						{
							fprintf(stderr, "size expected base 10 only\n");
							break;
						}
					}
					char rawdata[1024];
					int bytes2write = read(connfd, rawdata, sizeof(rawdata));
					//could be wb instead of a+???
					FILE *fp = fopen(pfilename, "a+");
					if (!fp)
					{
						fprintf(stderr, "failed to open file %s\n", pfilename);
						break;
					}
					printf("Successfully opened \"%s\"\n", pfilename);
					fputs(rawdata, fp);
					fclose(fp);
					fprintf(stdout, "OK\n");
					continue;
				}
				//GET request
				else if (strstr(bufp, "GET"))
				{
					char *pfilename = bufp + 4;
					for (; *pfilename != '\n'; ++pfilename)
					{
						if (!isprint(*pfilename))
						{
							fprintf(stderr, "invalid filename\n");
							break;
						}
					}
					FILE *fp = fopen(pfilename, "r");
					if (!fp)
					{
						fprintf(stderr, "file %s does not exist\n", pfilename);
						break;
					}
					char preply[1024];
					fgets(preply, sizeof(preply), fp);
					size_t bytes = strlen(preply);
					fprintf(stdout, "OK %s\n", pfilename);
					fprintf(stdout, "%d\n", bytes);
					fprintf(stdout, "%s\n", preply);
					fclose(fp);
					break;
				}
				continue;
			}
			else
			{
				fprintf(stderr, "Request must begin with PUT or GET\n");
			}
			// end service to this client on EOF
			if (nsofar == 0)
			{
				fprintf(stderr, "received EOF\n");
				return;
			}
			// update pointer for next bit of reading
			bufp += nsofar;
			nremain -= nsofar;
			if (*(bufp - 1) == '\n')
			{
				*bufp = 0;
				break;
			}
		}
	}
}

/*
	* main() - parse command line, create a socket, handle requests
	*/
int main(int argc, char **argv)
{
	/* for getopt */
	long opt;
	int lru_size = 10;
	int port = 9000;
	bool multithread = false;

	check_team(argv[0]);

	/* parse the command-line options.  They are 'p' for port number,  */
	/* and 'l' for lru cache size, 'm' for multi-threaded.  'h' is also supported. */
	while ((opt = getopt(argc, argv, "hml:p:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			help(argv[0]);
			break;
		case 'l':
			lru_size = atoi(argv[0]);
			break;
		case 'm':
			multithread = true;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		}
	}

	/* open a socket, and start handling requests */
	int fd = open_server_socket(port);
	handle_requests(fd, file_server, lru_size, multithread);

	exit(0);
}
