/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "wrap.h"
#include "process_message.h"
#include "chatmacro.h"

#define SERV_PORT 8000

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	socklen_t cliaddr_len;
	struct sockaddr_in	cliaddr, servaddr;
    char msg[MAXARG][MAXLINE];
    int msgn;
    order_t cmd;
    char online_client[FD_SETSIZE][MAXLINE];
    loginstat_t loginstat;


	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);

	maxfd = listenfd;		/* initialize */
	maxi = -1;			/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

    memset(online_client, 0, sizeof(online_client));

	for ( ; ; ) 
	{
		rset = allset;	/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");

		if (FD_ISSET(listenfd, &rset)) 
		{ /* new client connection */
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

			printf("received from %s at PORT %d\n",
			       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			       ntohs(cliaddr.sin_port));

			for (i = 0; i < FD_SETSIZE; i++)
			{
				if (client[i] < 0) 
				{
					client[i] = connfd; /* save descriptor */
					break;
				}
			}
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */

			if (--nready == 0)
				continue;	/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) 
		{	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) 
			{
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) 
				{
					/* connection closed by client */
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
                    memset(online_client[i], '\0', sizeof(online_client[i]));
				} else 
				{
                    memset(msg, '\0', sizeof(msg));
                    msgn = split_msg(buf, msg);
                    cmd = process_cmd(msg[0]);

                    if(cmd == REG)
                    {
                        if(reg(msg[1], msg[2]) == REG_SUCCESS)
                        {
                            Write(sockfd, "register success!\n", 18);
                        }else
                        {
                            Write(sockfd, "Username has already exist!\n", 28);
                        }
                    } else if (cmd == LOGIN)
                    {
                        loginstat = login(msg[1], msg[2], online_client[i]);
                        if (loginstat == LOGIN_SUCCESS)
                        {
                            Write(sockfd, "login success!\n", 15);
                        }else if (loginstat == LOGIN_WRONGPW)
                        {
                            Write(sockfd, "wrong password!\n", 16);
                        }else if (loginstat == LOGIN_UNREG)
                        {
                            Write(sockfd, "username doesn't exist!\n", 24);
                        }else
                        {
                            Write(sockfd, "already login!\n", 15);
                        }
                    } else if (cmd == LIST)
                    {
                        int j;
                        int nword;

                        Write(sockfd, "online user:", 12);
                        for(j = 0; j < FD_SETSIZE; j++)
                        {
                            nword = (int)strlen(online_client[j]);
                            if(nword != 0)
                            {
                                Write(sockfd, online_client[j], nword);
                                Write(sockfd, " ", 1);
                            }
                        }
                        Write(sockfd, "\n", 1);
                    }else
                    {
                        //Write(sockfd, "wtf", 3);
                    }
				}

				if (--nready == 0)
					break;	/* no more readable descriptors */
			}
		}
	}
}
