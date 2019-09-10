#include "builtin.h"

// returns true if the 'exit' call
// should be performed
// (It must not be called here)
int exit_shell(char* cmd) {

	// Your code here

	if(strcmp(cmd,"exit")==0)
    {
        return true;
    }

	return 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to HOME)
//  it has to be executed and then return true
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int cd(char* cmd) {

	// Your code here
	char* home = getenv("HOME");
	char* newpromt;
	char* prefix = "~/";
	char aux_cmd[strlen(cmd)];
	strcpy(aux_cmd,cmd);
	char* directory = split_line(aux_cmd,' ');
	bool b;

	if(strcmp(aux_cmd,"cd")==0)
    {
        if(strlen(directory)!=0)
        {
            if(chdir(directory)!=0)
                printf("cd: %s: No existe el archivo o el directorio\n",directory);
            else
            {
                newpromt = (char*)malloc(strlen(directory)+2);
                strcpy(newpromt,prefix);
                strcat(newpromt,directory);
                strcpy(promt,newpromt);
                free(newpromt);
            }
        }
        else
        {
            chdir(home);
            strcpy(promt,home);
        }
        return true;
    }
    //
	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
// (It has to be executed here and then
// 	return true)
int pwd(char* cmd) {

	// Your code here

	char currentDirectory[BUFLEN];

	if(strcmp(cmd,"pwd")==0)
    {
        printf("%s \n",getcwd(currentDirectory,sizeof(currentDirectory)));
        return true;
    }
	return 0;
}

