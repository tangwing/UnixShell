#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "cmd.h"
#include "shell_fct.h"
 

int main(int argc, char** argv)
{
	cmd ma_cmd;
	int i;
	char chaine[100] = "cat acpi0< var/log/messages | grep acpi| wc -l 2>> truc.txt";
	parse_membres(chaine, &ma_cmd);
	aff_membres(&ma_cmd);
	parse_args(&ma_cmd);
	aff_args(&ma_cmd);
	
	for(i=0; i<ma_cmd.nb_membres; i++)
	{
		parse_redirect(i, &ma_cmd);
		aff_redirect(&ma_cmd, i);
	}
	free_membres(&ma_cmd);
	free_args(&ma_cmd);
	free_redirect(&ma_cmd);
	return 0;
	
	/*/..........
	int ret = MYSHELL_CMD_OK;
	cmd mycmd;
	char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	//..........
	while(ret != MYSHELL_FCT_EXIT)
	{
		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		//..........
		//..........parse_membres(readlineptr, &mycmd); 
		//..........parse_args(&mycmd);
		//..........parse_redirect(&mycmd);
		//..........exec_cmd(&mycmd);
		//.......... 

		free(readlineptr);
		//..........
		
	}
	//..........*/
	//return 0;
}
