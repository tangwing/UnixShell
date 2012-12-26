#include "cmd.h"
#include <string.h>


void parse_membres(char *chaine,cmd * ma_cmd)
{
	const char* pdelim = "|";
	char *token;
	int len = strlen(chaine);
	//printf("%d\n", len);
	//trim()
	if(len == 0)
	{
		ma_cmd->nb_membres = 0;
		return;
	}
	
	char * chaine2 = strdup(chaine);
	int i = 0;
	ma_cmd->nb_membres = 1;
	for( ; i<len ; i++)
	{
		if(chaine2[i] == '|') ma_cmd->nb_membres++;
	}

	
	//ma_cmd.cmd_initial = 
	printf("%d\n", ma_cmd->nb_membres);
	ma_cmd->cmd_membres = (char**) calloc(sizeof(char*), ma_cmd->nb_membres);


	printf("\n%s\n", chaine2);
	token = strtok( chaine2, "|");// pdelim);
	i=0;
	while( token != NULL)
	{
				
		printf("%s\n", token);
		
		while(*token == ' ')
			token++;
		len = strlen(token);
		while(token[len-1] == ' ')
			len--;

		ma_cmd->cmd_membres[i] = (char*)calloc(sizeof(char), len+1);
		
		strncpy(ma_cmd->cmd_membres[i], token, len);
		i++;
		token = strtok( NULL, pdelim);
	}
	printf("\n%s\n", chaine2);
}

void parse_args(cmd * c)
{
	char buf[1024];
	c->nb_args_membres = (int*)calloc(sizeof(char), c->nb_membres);
	c->cmd_args = (char***)calloc(sizeof(char**), c->nb_membres);

	int i,j,k;
	for(i = 0; i<c->nb_membres; i++)
	{
		c->nb_args_membres[i] = 0;
		for(j = 0, k = 0; c->cmd_membres[i][j] != '\0'; j++)
		{
		
			if((c->cmd_membres[i][j] == '<' ||
				c->cmd_membres[i][j] == '>') && k>0)
			{
				buf[k+1] = '\0';
				c->nb_args_membres[i]++;
				c->cmd_args[i] = (char**)realloc( c->cmd_args[i], (c->nb_args_membres[i]+1)*sizeof(char*));
				c->cmd_args[i][c->nb_args_membres[i]-1] = (char*)calloc(sizeof(char), k+1);
				strncpy(c->cmd_args[i][c->nb_args_membres[i]-1], buf, k);
				break;
			}
			else if(c->cmd_membres[i][j] != ' ' )
			{
 				buf[k] = c->cmd_membres[i][j];
 				k++;
 			}
			else if( k > 0)
			{
				buf[k+1] = '\0';
				c->nb_args_membres[i]++;
				c->cmd_args[i] = (char**)realloc( c->cmd_args[i], (c->nb_args_membres[i]+1)*sizeof(char*));
				c->cmd_args[i][c->nb_args_membres[i]-1] = (char*)calloc(sizeof(char), k);
				strncpy(c->cmd_args[i][c->nb_args_membres[i]-1], buf, k+1);
				k = 0;
			}
		}
	}
}



void aff_membres(cmd* ma_cmd)
{
	int i = 0;
	printf("Nombre des membres : %d\n", ma_cmd->nb_membres);
	for( ; i<ma_cmd->nb_membres; i++)
	{
		printf("Membre[%d] = \"%s\"\n", i, ma_cmd->cmd_membres[i]);
	}
}

void aff_args(cmd* c)
{
	int i,j;
	for(i = 0; i<c->nb_membres; i++)
	{
		for(j = 0; j<c->nb_args_membres[i]; j++)
		{
			printf("cmd_args[%d][%d] = \"%s\"\n", i, j, c->cmd_args[i][j]);
		}
		printf("cmd_args[%d][%d] = NULL\n", i, j);
	}
}

void free_membres(cmd* ma_cmd)
{
	//free others
}

int main()
{
	cmd ma_cmd;
	char chaine[100] = "cat < varlogmessages | grep ACPI| wc -l > truc.txt"; 
	parse_membres(chaine, &ma_cmd);
	aff_membres(&ma_cmd);
	parse_args(&ma_cmd);
	aff_args(&ma_cmd);
	return 0;
}
