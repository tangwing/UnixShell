#ifndef CMD_H
#define CMD_H

#include <stdio.h>
//#include <readline/readline.h> 
#include <stdlib.h>
#include <string.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define DEFAULT 0
#define RAPPEND 1

typedef struct commande {
	char* cmd_initial;				/* La chaine initiale tapee par l'utilisateur */
	char **cmd_membres;				/* a chaque case i chaine du membre i */
	unsigned int nb_membres;		/* Nombre de membres */
	char ***cmd_args;				/* cmd_args[i][j] contient le jieme mot du ieme membre */
	unsigned int *nb_args_membres;	/* Nombre d'arguments par membres */
	char ***redirect;				/* Pour stocker le chemin vers le fichier de redirection */
	int **type_redirect;			/* Pour stocker le type de rediction */
} cmd;

void aff_args(cmd *c);
void free_args(cmd *c);
void parse_args(cmd *c);  
void parse_membres(char *chaine,cmd *ma_cmd); 
void aff_membres(cmd *ma_cmd);
void free_membres(cmd *ma_cmd);
//int -> void
void parse_redirect(unsigned int i,cmd *c);  
void free_redirect(cmd *c);
void aff_redirect(cmd *c, int i);

#endif
