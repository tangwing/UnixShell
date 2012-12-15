This project aims to implement a basic unix-style shell with language C under Ubuntu.

This shell needs to deal with commands with pipe, with redirections of IO and needs to have the functionality of auto-completion and history of commands.

Typically, this little application should correctly handle a command like:
cat < /var/log/messages | grep ACPI| wc -l > truc.txt