#include "cmd.h"
/**Initialiser un cmd
*/
void init_cmd(cmd * c)
{
	c->cmd_initial = NULL;				/* La chaine initiale tapee par l'utilisateur */
	c->cmd_membres = NULL;				/* a chaque case i chaine du membre i */
	c->nb_membres = 0;		/* Nombre de membres */
	c->cmd_args = NULL;				/* cmd_args[i][j] contient le jieme mot du ieme membre */
	c->nb_args_membres = NULL;	/* Nombre d'arguments par membres */
	c->redirect = NULL;				/* Pour stocker le chemin vers le fichier de redirection */
	c->type_redirect = NULL;
}

/**Analyser la chaine de commande passés en parametre
* en remplisant les champs initial_cmd, membres_cmd et
* nb_membres de ma_cmd qui est structure cmd.
*
*@param chaine La chaine de commande saisie par l'utilisateur
*@param ma_cmd La pointeur de la cmd à compléter
*/
void parse_membres(char *chaine,cmd * ma_cmd)
{
	init_cmd(ma_cmd);//Initialisation
	
	const char* pdelim = "|";
	char *token;
	int len = strlen(chaine);
	if(len == 0)
	{
		ma_cmd->nb_membres = 0;
		return;
	}

	//Dupliquer la chaine
	char * chaine2 = strdup(chaine);
	if( chaine2 == NULL){perror("strdup"); exit(0);}
	
	int i = 0;

	//Compter le nombre des membres
	ma_cmd->nb_membres = 1;
	for( ; i<len ; i++)
	{
		if(chaine2[i] == '|') ma_cmd->nb_membres++;
	}

	token = strtok( chaine2, "|");
	ma_cmd->cmd_membres = (char**) calloc(sizeof(char*), ma_cmd->nb_membres);
	if(ma_cmd->cmd_membres == NULL){perror("calloc"); exit(0);}
	
	i=0;
	while( token != NULL)
	{

		//Enlever les espaces au debut et au bout
		while(*token == ' ')
			token++;
		len = strlen(token);
		while(token[len-1] == ' ')
			len--;

        //Remplir les champs
		ma_cmd->cmd_membres[i] = (char*)calloc(sizeof(char), len+1);
		if(ma_cmd->cmd_membres[i] == NULL){perror("calloc"); exit(0);}
		
		strncpy(ma_cmd->cmd_membres[i], token, len);
		token = strtok( NULL, pdelim);
		i++;
	}
	free(chaine2);
}



/**Afﬁcher les informations des membres pour le debuggage.
*@param ma_cmd La commande dont les membres sont à afficher
*/
void aff_membres(cmd* ma_cmd)
{
	int i = 0;
	printf("Nombre des membres : %d\n", ma_cmd->nb_membres);
	for( ; i<ma_cmd->nb_membres; i++)
	{
		printf("Membre[%d] = \"%s\"\n", i, ma_cmd->cmd_membres[i]);
	}
}

/**Libérer les espaces mémoire alloués pour des membres pour le debuggage.
*@param ma_cmd La commande dont les membres sont à désallouer.
*/
void free_membres(cmd* ma_cmd)
{
	int i = 0;
	for(; i<ma_cmd->nb_membres; i++)
	{
		if(ma_cmd->cmd_membres[i] != NULL)
			free(ma_cmd->cmd_membres[i]);
	}
	if(ma_cmd->cmd_membres != NULL)
		free(ma_cmd->cmd_membres);
}

