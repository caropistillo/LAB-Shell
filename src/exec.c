#include "exec.h"


// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char* arg, char* key) {

	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char* arg, char* value, int idx) {

	int i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void set_environ_vars(char** eargv, int eargc) {

	// Your code here
	int idx;
	char *key, *value;
	for(int i=0;i<eargc;i++)
    {
        idx = block_contains(eargv[i],'=');
        key = (char*)malloc(i);
        value = (char*)malloc(strlen(eargv[i])-i-1);
        get_environ_key(eargv[i],key);
        get_environ_value(eargv[i],value,idx);
        setenv(key,value,0);
        free(key);
        free(value);
    }
    //
}

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int open_redir_fd(char* file, int flags) {

	// Your code here

	int fd = open(file,flags,S_IWUSR|S_IRUSR);

	return fd;
	//return -1;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void exec_cmd(struct cmd* cmd) {

	// To be used in the different cases
	struct execcmd* e;
	struct backcmd* b;
	struct execcmd* r;
	struct pipecmd* p;
	int fd[3];

	switch (cmd->type) {


		case EXEC:

			// spawns a command
			//
			// Your code here
			e = (struct execcmd*)cmd;
			if(e->eargc > 0)
                set_environ_vars(e->eargv,e->eargc);

            execvp(*(e->argv),e->argv);
            //
			printf("Commands are not yet implemented\n");
			_exit(-1);

			break;

		case BACK: {
			// runs a command in background
			//
			// Your code here
			b = (struct backcmd*)cmd;
			exec_cmd(b->c);
			//

			printf("Background process are not yet implemented\n");
			_exit(-1);
			break;
		}

		case REDIR: {
			// changes the input/output/stderr flow
			//
			// To check if a redirection has to be performed
			// verify if file name's length (in the execcmd struct)
			// is greater than zero
			//
			// Your code here
			r = (struct execcmd*)cmd;

			if(strlen(r->out_file) > 0)
			{
				fd[0] = open_redir_fd(r->out_file,O_CREAT|O_RDWR|O_APPEND);
				dup2(fd[0],STDOUT); //lo que iba a imprimirse en pantalla se imprime en un archivo de out
			}
			if(strlen(r->in_file) > 0)
			{
				fd[1] = open_redir_fd(r->in_file,O_CREAT|O_RDWR|O_APPEND);
				dup2(fd[1],STDIN); //uso el archivo como entrada
			}
			if(strlen(r->err_file) > 0)
			{
				if(block_contains(r->err_file,'&')==0 && block_contains(r->err_file,'1')==1)
					dup2(fd[0],STDERR); //lo que iba a imprimirse en pantalla se imprime en el mismo archivo que el out

				else
				{
					fd[2] = open_redir_fd(r->err_file,O_CREAT|O_RDWR|O_APPEND);
					dup2(fd[2],STDERR); //lo que iba a imprimirse en pantalla se imprime en un archivo de err
				}

			}
			execvp(*(r->argv),r->argv);

			for(int i=0;i<3;i++)
			{
				close(fd[i]);
			}

			//
			printf("Redirections are not yet implemented\n");
			_exit(-1);
			break;
		}

		case PIPE: {
			// pipes two commands
			//
			// Your code here

			int pipefd[2]; //={readEnd,writeEnd}

			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}

			int pidLeft;
			int pidRight;

			p = (struct pipecmd*) cmd;


		    if((pidLeft=fork())==0)
		    {
		    	close(pipefd[0]);          // Close unused read end
			    dup2(pipefd[1],STDOUT);//STDOUT para escribir
			    exec_cmd(p->leftcmd);
			    _exit(0);
		    }
		    else
		    {
		    	if((pidRight=fork())==0)
		    	{
				    close(pipefd[1]);          // Close unused write end
				    dup2(pipefd[0],STDIN); //STDIN para leer
				    exec_cmd(p->rightcmd);
				    _exit(0);
		    	}

		    	else
		    	{
		    		waitpid(-1,NULL,0);
		    		close(pipefd[0]);
		    		close(pipefd[1]);
		    		// free the memory allocated
		    		// for the pipe tree structure
		    		free_command(parsed_pipe);
		    		_exit(0);

		    	}
		    }

			printf("Pipes are not yet implemented\n");

			break;
		}
	}
}


