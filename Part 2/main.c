///TODO
//Les strcpy peut etre remplacés par strdup
//On doit vérifier la val retourné par malloc
//On n'a pas traité les cas comme 0< et 1> et &
//On a traité "ch"
//On a traité les cas les redirection des STD sont entre...
//Traiter ";", et "cd"(chdir())

//change exit to error msg

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "cmd.h"
#include "shell_fct.h"
 
#define MYSHELL_CMD_OK 0
#define MYSHELL_FCT_EXIT 1
int main(int argc, char** argv)
{
	int ret = MYSHELL_CMD_OK;
	cmd ma_cmd;
	int i;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];
	printf("pid = %d; ppid = %d\n", getpid(), getppid());
	while(ret != MYSHELL_FCT_EXIT)//TODO WHY
	{
		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

		sprintf(str, "{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		while(*readlineptr != '\0' && *readlineptr == ' ')
		{
			readlineptr ++;
		}
		if(*readlineptr == '\0') continue;
		else if( strcmp(readlineptr, "exit")==0)
			break;
		add_history(readlineptr);
		parse_membres(readlineptr, &ma_cmd);
		parse_args(&ma_cmd);
		//aff_membres(&ma_cmd);
		//aff_args(&ma_cmd);

		for(i=0; i<ma_cmd.nb_membres; i++)
		{
			parse_redirect(i, &ma_cmd);
			//aff_redirect(&ma_cmd, i);
		}
		//chdir("..");
		exec_cmd(&ma_cmd);
		alarm(0);
		free_membres(&ma_cmd);
		free_args(&ma_cmd);
		free_redirect(&ma_cmd);

		free(readlineptr);
		//..........
		
	}
	//..........
	return 0;
}
