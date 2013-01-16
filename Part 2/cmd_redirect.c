#include "cmd.h"



/**Remplir les champs redirect et type_redir du membre i
*@param i L'index du membre à traiter
*@param c La pointeur de la cmd à analyser et à remplir.
*/
void parse_redirect(unsigned int i,cmd *c)
{
	if( i >= c->nb_membres){perror("parse_redirect; membre invalide.");exit(0);}
	char buf[1024];
	char term = ' ';
	int j,k=0;
	int iBoucle;

	if(c->redirect == NULL)
	{
		c->redirect = (char***)calloc(sizeof(char**), c->nb_membres);
		if(c->redirect == NULL){perror("parse_redirect(); c->cmd_redirect; calloc"); exit(0);}
		
		for(iBoucle=0; iBoucle < c->nb_membres; iBoucle++)
		{
			c->redirect[iBoucle] = (char**)calloc(sizeof(char*), 3);
            if(c->redirect[iBoucle] == NULL){perror("parse_redirect(); c->cmd_redirect[i]; calloc"); exit(0);}
		}
	}
	if(c->type_redirect == NULL)
	{
		c->type_redirect = (int**)calloc(sizeof(int*), c->nb_membres);
		if(c->type_redirect == NULL){perror("parse_redirect(); c->type_redirect ; calloc"); exit(0);}
		
		for(iBoucle=0; iBoucle < c->nb_membres; iBoucle++)
		{
			c->type_redirect[iBoucle] = (int*)calloc(sizeof(int), 3);
            if(c->type_redirect[iBoucle] == NULL){perror("parse_redirect(); c->type_redirect[i]; calloc"); exit(0);}
		}
	}
	
	for(j=0; c->cmd_membres[i][j] != '\0'; j++)
	{
		if(c->cmd_membres[i][j] == '<')
		{
			j++;
			
			//Enlever les espaces au debut
			while( c->cmd_membres[i][j] == ' ' && c->cmd_membres[i][j] != '\0')
				j++;
			
			//Traitement des cas comme  cat <"dir\"dir/fichier.txt"
			if(c->cmd_membres[i][j]=='"' || c->cmd_membres[i][j]=='\'')
				term = c->cmd_membres[i][j];
				
			for( k=0; c->cmd_membres[i][j] != '\0'; j++)
			{
				//Si term='"', recopier le rest du membre jusqu'a '"' 
				if( term == '"' && c->cmd_membres[i][j]=='"')
				{
					buf[k++] = c->cmd_membres[i][j];
					//buf[k] = '\0';
					if(c->cmd_membres[i][j-1]!='\\')
						break; 
				}
				//Si term!='"', recopier le rest du membre jusqu'a ' ' ou '<' ou '>' 
				else if( term != '"' && (c->cmd_membres[i][j] == term ||
						c->cmd_membres[i][j] == '>' ||
						c->cmd_membres[i][j] == '<'))
						break;
				buf[k++] = c->cmd_membres[i][j];
			}
			buf[k]='\0';
			
			//Remplir c->redirect et c->type_redirect avec buf
			c->redirect[i][STDIN] = strdup(buf);
			if(c->redirect[i][STDIN] == NULL)
			{
				perror("parse_redirect(); c->redirect[i][STDIN]; strdup"); 
				exit(0);
			}
			c->type_redirect[i][STDIN] = DEFAULT;
		}
		else if(c->cmd_membres[i][j] == '>')
		{
			int redi = STDOUT;
			int type_redi = DEFAULT;
			
			if(c->cmd_membres[i][j-1] == '2' && 
				(c->cmd_membres[i][j-2] == ' ' ||
				c->cmd_membres[i][j-2] == '"' ||
				c->cmd_membres[i][j-2] == '\'')
			 )
			 {//STDERR
			 	redi = STDERR;
			 }
			 //Mode de redirection
		 	 if(c->cmd_membres[i][j+1] == '>')
		 	 {
		 		type_redi = RAPPEND;
		 		j++;
		     }
			 
			 j++;
			
			//Enlever les espaces au debut
			while( c->cmd_membres[i][j] == ' ' && c->cmd_membres[i][j] != '\0')
				j++;
			
			//Traitement des cas comme  cat <"dir\"dir/fichier.txt"
			if(c->cmd_membres[i][j]=='"' || c->cmd_membres[i][j]=='\'')
				term = c->cmd_membres[i][j];
				
			for( k=0; c->cmd_membres[i][j] != '\0'; j++)
			{
				//Si term='"', recopier le rest du membre jusqu'a '"' 
				if( term == '"' && c->cmd_membres[i][j]=='"')
				{
					buf[k++] = c->cmd_membres[i][j];
					//buf[k] = '\0';
					if(c->cmd_membres[i][j-1]!='\\')
						break; 
				}
				//Si term!='"', recopier le rest du membre jusqu'a ' ' ou '<' ou '>' 
				else if( term != '"' && (c->cmd_membres[i][j] == term ||
						c->cmd_membres[i][j] == '>' ||
						c->cmd_membres[i][j] == '<'))
						break;
				buf[k++] = c->cmd_membres[i][j];
			}
			buf[k]='\0';
			
			//Remplir c->redirect et c->type_redirect avec buf
			c->redirect[i][redi] = strdup(buf);
			if(c->redirect[i][redi] == NULL)
			{
				perror("parse_redirect(); c->redirect[i][redi]; strdup"); 
				exit(0);
			}
			c->type_redirect[i][redi] = type_redi;
			 
		}

	}
		

}

/**Afﬁcher les informations des redirections pour le debuggage.
*@param c La commande à afficher
*@param i L'index du membre
*/
void aff_redirect(cmd *c, int i)
{
	int j;
	for (j=0; j<3; j++)
	{
		if(c->redirect[i][j] != NULL)
		{
			printf ("redirect[%d][%d] = %s, type_redirect = %d\n",i,j,c->redirect[i][j], c->type_redirect[i][j]);
		}else
		printf ("redirect[%d][%d] = NULL\n",i,j);
		
	}
}


/**Libérer les espaces mémoires alloués concernant les redirections.
*@param ma_cmd La commande à désallouer.
*/
void free_redirect(cmd* ma_cmd)
{
	int i,j;
	if(ma_cmd->redirect != NULL && ma_cmd->type_redirect != NULL)
	{
		for(i = 0; i<ma_cmd->nb_membres; i++)
		{
			for(j = 0; j<3; j++)
			{
				if(ma_cmd->redirect[i][j] != NULL)
					free(ma_cmd->redirect[i][j]);
				//if(c->type_redirect[i][j] != NULL)
					//free(c->type_redirect[i][j]);			
			}
			free(ma_cmd->redirect[i]);
			free(ma_cmd->type_redirect[i]);
		}
		free(ma_cmd->redirect);
		free(ma_cmd->type_redirect);
	}
}

