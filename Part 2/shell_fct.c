#include "shell_fct.h"

#define _GNU_SOURCE
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
/**
*Executer une seule commande à distance.
*@param cmd La cmd à executer, de forme "s:10.0.0.1:1234 ls"
*/
int exec_cmd_distant(const char* cmdMembre);

/**
*A chaque exécution d’une commande, le shell met fin aux processus générés
*au bout de 10 seconde si celle-ci est sans réponse. Cette function sera appellée
*quand le processus reçois le signal SIGALRM
*/
void killchildren(int signum, siginfo_t *siginfo, void* context)
{
	int pid = getpid();
	char buf[20];
	sprintf( buf, "%d", pid);
	printf("Myshell n'a pas de réponse pendant 10s... Exit...\n");
	execlp("pkill", "pkill", "-P", buf, NULL);
}

/**
*Executer une commande duement initialisée, effectuer la création des pipes, 
*les fork et les execs correspondants.
*@param ma_cmd La cmd à executer
*/
int exec_cmd(cmd* ma_cmd)
{
	int iBoucle, jBoucle;
	int val_fork, val_exec;
	//Creer les tubes
	int ** tube = (int **)calloc(sizeof(int*), ma_cmd->nb_membres-1);
	if(tube == NULL){perror("exec_cmd(); tube; calloc"); exit(0);}
	for( iBoucle=0; iBoucle < ma_cmd->nb_membres-1; iBoucle++)
	{
		tube[iBoucle] = (int *)calloc(sizeof(int), 2);
		if(tube[iBoucle] == NULL){perror("exec_cmd(); tube[i]; calloc"); exit(0);}
		pipe(tube[iBoucle]);
	}
	
	for( iBoucle = 0; iBoucle < ma_cmd->nb_membres; iBoucle++)
	{
		val_fork = fork();
		if( val_fork == -1){perror("fork();"); exit(0);}
		else if( val_fork == 0)
		{
			
			//Fermer les tubes inutiles
			for( jBoucle=0; jBoucle < ma_cmd->nb_membres-1; jBoucle++)
			{
				if(jBoucle != iBoucle && jBoucle != iBoucle-1)
				{
					close(tube[jBoucle][0]);
					close(tube[jBoucle][1]);
				}
			}
		
			
			if(iBoucle != 0)
			{//Rediriger le STDIN vers le pipe[iBoucle-1]
				close(tube[iBoucle-1][1]);
				dup2(tube[iBoucle-1][0], 0);
				close(tube[iBoucle-1][0]);
			}
			if(iBoucle != ma_cmd->nb_membres-1)
			{//Rediriger le STDOUT vers le pipe[iBoucle]
				close(tube[iBoucle][0]);
				dup2(tube[iBoucle][1], 1);
				close(tube[iBoucle][1]);
			}
			if( ma_cmd->redirect != NULL)
			{
				int fd;
				//fprintf (stderr, "iBoucle = %d\n",iBoucle);
				if( ma_cmd->redirect[iBoucle][STDIN] != NULL)
				{//Redirection de STDIN
					fd = open( ma_cmd->redirect[iBoucle][STDIN], O_RDONLY);
					if(fd == -1){perror("open(),redi_STDIN");exit(0);}
					dup2(fd, 0);
					close(fd);
				}
				if( ma_cmd->redirect[iBoucle][STDOUT] != NULL)
				{//Redirection de STDOUT
					if( ma_cmd->type_redirect[iBoucle][STDOUT] == RAPPEND)
						fd = open( ma_cmd->redirect[iBoucle][STDOUT], 
										O_CREAT | O_WRONLY |O_APPEND, S_IRUSR | S_IWUSR);
					else
						fd = open( ma_cmd->redirect[iBoucle][STDOUT], 
										O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
					if(fd == -1){perror("open(),redi_STDOUT).");exit(0);}
					dup2(fd, 1);
					close(fd);
				}
				
				if( ma_cmd->redirect[iBoucle][STDERR] != NULL)
				{//Redirection de STDERR
					if( ma_cmd->type_redirect[iBoucle][STDERR] == RAPPEND)
						fd = open( ma_cmd->redirect[iBoucle][STDERR], 
										O_CREAT | O_WRONLY |O_APPEND, S_IRUSR | S_IWUSR);
					else
						fd = open( ma_cmd->redirect[iBoucle][STDERR], 
										O_CREAT | O_WRONLY , S_IRUSR | S_IWUSR);
					if(fd == -1){perror("open(),redi_STDERR.");exit(0);}
					dup2(fd, 2);
					close(fd);
				}
			}
			//Executer la commande
			if(ma_cmd->cmd_args[iBoucle][0][0] == 's' &&
				ma_cmd->cmd_args[iBoucle][0][1] == ':')
			{//Traiter les cmds à distance
				val_exec = exec_cmd_distant(ma_cmd->cmd_membres[iBoucle]);
				if( val_exec == -1);
					//TODO fprintf(stderr, "%s : commande introuvable\n", ma_cmd->cmd_membres[iBoucle]);
			}
			else
			{
				val_exec = execvp(ma_cmd->cmd_args[iBoucle][0], ma_cmd->cmd_args[iBoucle]);
				if( val_exec == -1)
					fprintf(stderr, "%s : commande introuvable\n", ma_cmd->cmd_membres[iBoucle]);
			}
			exit(0);//The water is so deep!
		}
	}
	
	struct	sigaction action;
	action.sa_sigaction = killchildren;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_SIGINFO;
	//Tuer les processus fils aprés 10 secondes
	alarm(10);
	sigaction(SIGALRM, &action, NULL);
	//sleep(1);
//wait(0);
	//Fermer tous les tubes (important!!)
	for( jBoucle=0; jBoucle < ma_cmd->nb_membres-1; jBoucle++)
	{
		close(tube[jBoucle][0]);
		close(tube[jBoucle][1]);
	}
	for( jBoucle=0; jBoucle < ma_cmd->nb_membres; jBoucle++)
	{
		wait(NULL);
	}
	for( jBoucle=0; jBoucle < ma_cmd->nb_membres-1; jBoucle++)
	{
		free(tube[jBoucle]);
	}
	free(tube);
	
	return 0;
}

/**
*Executer une seule commande à distance.
*@param cmd La cmd à executer, de forme "s:10.0.0.1:1234 ls"
*/
int exec_cmd_distant(const char* cmdMembre)
{
	int fdSock, res;
	char buf[2048];
	char *addr, *port, *cmd;
	char *cmdMsg = strdup(cmdMembre);
	strtok(cmdMsg, ":");
	//get a socket
	if( (fdSock=socket(AF_INET, SOCK_STREAM, 0)) == -1){perror("socket"); exit(1);}

	//config the address of the server to connect
	struct sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;//Address family
	
	addr = strtok(NULL, ":");
	inet_pton(AF_INET, addr, &(addrServer.sin_addr));//Convert the addr from "presentation" to "numeric"
	port = strtok(NULL, ": ");
	addrServer.sin_port=htons(atoi(port));//set the port number
	
	//
	cmd = port + strlen(port)+1;
	while(*cmd!='\0' && *cmd==' ') cmd++;
	
	
	//Connect to server
	if( connect( fdSock, (struct sockaddr *)(&addrServer), sizeof(addrServer))== -1){perror("connect"); exit(1);}
	//printf("Gets gets : %s\n", buf);
	//Send the msg to server. See also "sendto" and "sendmsg"
	if( send(fdSock, cmd, strlen(cmd), 0) == -1){perror("Send"); exit(1);}
	if( (res = recv(fdSock, buf, 2048, 0)) == -1){perror("recv"); exit(1);}
	else if(res == 0) {close(fdSock); exit(0);}
	else
	{
		buf[res] = '\0';
		printf("%s",buf);
	}
	free(cmdMsg);
	return 0;
}
