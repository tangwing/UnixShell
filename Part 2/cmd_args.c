#include "cmd.h"

/**Remplir les champs cmd_args et nb_args_membres
* de c en analysant les membres de c.
*@param c La pointeur de la cmd à analyser et à remplir.
*/
void parse_args(cmd * c)
{
	char buf[1024];
	c->nb_args_membres = (unsigned int*)calloc(sizeof(unsigned int), c->nb_membres);
	if(c->nb_args_membres == NULL){perror("parse_args(); c->nb_args_membres; calloc"); exit(0);}
	
	c->cmd_args = (char***)calloc(sizeof(char**), c->nb_membres);
	if(c->cmd_args == NULL){perror("parse_args(); c->cmd_args; calloc"); exit(0);}

	int i,j,k;
	for(i = 0; i < c->nb_membres; i++)
	{//Pour chaque membre:
		c->nb_args_membres[i] = 0;
		
		for(j = 0, k = 0; ; j++)
		{//Pour chaque char du membre i

			if(c->cmd_membres[i][j] == '<' ||
                c->cmd_membres[i][j] == '>' ||
                c->cmd_membres[i][j] == '\0' ||
                c->cmd_membres[i][j] == ' ' )
			{	
				//Traitement la redirection de STDERR			
				if( k==1 && c->cmd_membres[i][j] == '>' &&
		            c->cmd_membres[i][j-1] == '2')
		            break;
		                
				if(k>0)
				{//Un nouveau arg est trouvé
					buf[k] = '\0';
					c->nb_args_membres[i]++;
					//Réallouer la taille de cmd_args[i] pour
					//ajouter ce nouveau arg.
					c->cmd_args[i] = (char**)realloc( c->cmd_args[i],
		                                 (c->nb_args_membres[i]+1)*sizeof(char*));
		            if(c->cmd_args[i] == NULL){perror("parse_args(); c->cmd_args[i]; realloc"); exit(0);}
		            
		            c->cmd_args[i][c->nb_args_membres[i]] = NULL;//Le dernier arg
					c->cmd_args[i][c->nb_args_membres[i]-1] = (char*)calloc(sizeof(char), k+1);
					if(c->cmd_args[i][c->nb_args_membres[i]-1] == NULL){perror("parse_args();c->cmd_args[i][c->nb_args_membres[i]-1]; calloc"); exit(0);}
					
					//printf("-----%s\n",buf);
					strncpy(c->cmd_args[i][c->nb_args_membres[i]-1], buf, k);
					//printf("-----%s\n",c->cmd_args[i][c->nb_args_membres[i]-1]);
					k=0;
					
				}
				
				if(c->cmd_membres[i][j] != ' ') 
						break;//L'analyse de ce membre est fini.
			}
			else if(c->cmd_membres[i][j] != ' ' )
			{
 				buf[k] = c->cmd_membres[i][j];
 				k++;
 			}

		}
		
	}
}



/**Afﬁcher les informations des args pour le debuggage.
*@param ma_cmd La commande dont les args sont à afficher
*/
void aff_args(cmd* c)
{
	int i,j;
	for(i = 0; i<c->nb_membres; i++)
	{
		for(j = 0; j<c->nb_args_membres[i]; j++)
		{
			printf("cmd_args[%d][%d] = \"%s\"\n", i, j, c->cmd_args[i][j]);
		}
		if(c->cmd_args[i][c->nb_args_membres[i]] == NULL)
			printf("cmd_args[%d][%d] = NULL\n", i, j);
	}
}



/**Libérer les espaces mémoires des args des membres.
*@param ma_cmd La commande dont les args sont à désallouer.
*/
void free_args(cmd* c)
{
	int i,j;
	for(i = 0; i<c->nb_membres; i++)
	{
		for(j = 0; j<c->nb_args_membres[i]; j++)
		{
			if(c->cmd_args[i][j] != NULL)
				free(c->cmd_args[i][j]);
		}
		free(c->cmd_args[i]);
	}
	if(c->cmd_args != NULL)
		free(c->cmd_args);
	if(c->nb_args_membres != NULL)
		free(c->nb_args_membres);
}
