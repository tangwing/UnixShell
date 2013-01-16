#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_NUM_ARG 10
#define MAX_NUM_CLIENT 10

typedef struct _ClientCmd{ int sockClient; char * cmd;}ClientCmd;
void exeCmd(ClientCmd* cc)
{
	
	if(fork()==0)
	{
		//Set redirection
		dup2(cc->sockClient, 1);
		dup2(cc->sockClient, 2);
		close(cc->sockClient);

		char *argv[MAX_NUM_ARG] = {NULL};
		int i;
		char *arg;
		for(i=0; i<MAX_NUM_ARG; i++)
		{
			if(i>0) cc->cmd = NULL;
			arg = strtok(cc->cmd, " ");
			if(arg != NULL)
			{
				//put this arg into argv[]
				//argv[i] = calloc( sizeof(char), strlen(arg+1)) ;
				//strcpy(argv[i], arg);
				argv[i] = arg;//We can also simply do like this.
				//fprintf(stderr, "cmd[%d] = %s\n",i,arg);
			}
			else
			{//execute this cmd
				if( -1 == execvp(argv[0], argv)
				{
					//TODO
				}
				exit(0);
			}
			
		}
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	int numPort, lenMsg, i;
	int sockServ, sockClient;
	struct sockaddr_in adrServ, adrClient;
	ClientCmd cc;
	pthread_t ptr;
	char message[256];
	//char * overMsg = "over";
	int adrSize = sizeof(adrClient);
	int yes = 1;

	if(argc < 2)
	{
		printf("syntaxe : %s numPort\n", argv[0]);
		return -1;
	}
	

	//set local addr and port
	numPort = atoi(argv[1]);
	adrServ.sin_addr.s_addr = INADDR_ANY;
	adrServ.sin_family = AF_INET;
	adrServ.sin_port = htons(numPort);
	bzero(&(adrServ.sin_zero), 8) ;
	
	//Get a socket
	if((sockServ = socket(PF_INET, SOCK_STREAM, 0)) == -1) { perror("sockServet"); return -1; }
	
	//We can set some param here.// paramètrage de la socket pour réutiliser le N° ce port sans délai
	if(setsockopt(sockServ, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) { perror("setsockopt"); return -1; }

	// bind
	if(bind(sockServ, (struct sockaddr *)(&adrServ), sizeof(adrServ)) == -1) { perror("bind"); return -1; }

	// place la socket en écoute
	if(listen(sockServ, MAX_NUM_CLIENT) == -1) { perror("listen"); return -1; }
	


	do
	{
		// wait for a connection from a client
		if((sockClient = accept(sockServ, (struct sockaddr *)(&adrClient), &adrSize)) == -1) { perror("accept"); return -1; }
		//printf("New socket client : %s\n", inet_ntoa(adrClient.sin_addr));
		// Read a cmd from client
		//fgets(stdin, message);
		//fscanf(sockClient,"%s",message);
		if((lenMsg = recv(sockClient, message, 256, 0)) <= 0)
		{ 
			perror("recv");
			close(sockClient);
			continue;
		}
		message[lenMsg] = '\0';
		printf("New cmd received : %s\n", message);
		//ClientCmd cc;
		cc.sockClient = sockClient;
		cc.cmd = message;
		if(pthread_create(&ptr, NULL, (void*)exeCmd, (void*)(&cc) ) != 0)
		{	
			perror("pthread_create");exit(1);
		}
		//exeCmd(sockClient, message);
		//fflush(sockClient);
		//printf("New msg received : %s, %d\n", message, strcmp(message, overMsg));
		
	}while(1);//strcmp(message, overMsg) != 0);	
	// fermeture de la connexion
	close(sockClient);
	
	return 0;
}

