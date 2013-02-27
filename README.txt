This project aims to implement a basic unix-style shell with language C under Ubuntu.

This shell needs to deal with commands with pipe, with redirections of IO and needs to have the functionality of auto-completion and history of commands.

Typically, this little application should correctly handle a command like:
cat < /var/log/messages | grep ACPI| wc -l > truc.txt

In part2, this application can now execute remote commands like: s:127.0.0.1 ls | grep *.o > test.txt

which means to execute 'ls' in server end, and passe the result to local 'grep'. All sub-command(part between two '|') started by s:<addr> will be executed in server end.
