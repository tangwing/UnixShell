#include "cmd.h"
#include <string.h>

void parse_membres(char *chaine,cmd * ma_cmd)
{
	const char* pdelim = "|";
	char *token;
	int len = strlen(chaine);
	printf("%d\n", len);
	//trim()
	if(len == 0)
	{
		ma_cmd->nb_membres = 0;
		return;
	}
	
	char * chaine2 = strdup(chaine);
	printf("dfddds\n");
	int i = 0;
	ma_cmd->nb_membres = 1;
	for( ; i<len ; i++)
	{
		printf("%d\n", i);
		if(chaine2[i] == '|') ma_cmd->nb_membres++;
	}	
	//ma_cmd.cmd_initial = 
	printf("%d\n", ma_cmd->nb_membres);
	printf("\n%s\n", chaine2);
	token = strtok( chaine2, "|");// pdelim);
	while( token != NULL)
	{
				
		printf("%s\n", token);
		token = strtok( NULL, pdelim);
	}
	printf("\n%s\n", chaine2);
}

int main()
{
	cmd ma_cmd;
	char chaine[100] = "cat < varlogmessages | grep ACPI| wc -l > truc.txt"; 
	parse_membres(chaine, &ma_cmd);
	return 0;
}
