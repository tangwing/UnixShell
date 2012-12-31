#include "cmd.h"


///TODO
//Les strcpy peut etre remplacés par strdup
//On doit vérifier la val retourné par malloc
//On n'a pas traité les cas comme 0< et 1>



int main()
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
	return 0;
}
