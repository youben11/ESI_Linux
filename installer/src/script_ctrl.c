#include "script_ctrl.h"

ps_info script_ctrl(char *script_name,char **script_argv){
	// script is the script name
	// script_argv is the arguments array
	// the last one in script_argv must be a NULL pointer
	// the first argument of script_argv is the the script_name

	ps_info ps_err;
	ps_err.pid = ps_err.stdout_fd = ps_err.stderr_fd=-1;

	int linker_stdout[2];
	if ( pipe(linker_stdout) == -1 ){
		perror("can't create the 1st pipe");
		return ps_err;
	}

	int linker_stderr[2];
	if ( pipe(linker_stderr) == -1 ){
		perror("can't create the 2nd pipe");
		return ps_err;
	}

	int pid = fork();
	if (pid == -1){
		perror("can't fork the process");
		return ps_err;
	}

	if (!pid){
		// this part of code run in the child process

		// closing the pipes read ends
		close(linker_stdout[0]);
		close(linker_stderr[0]);

		if ( dup2(linker_stdout[1], STDOUT_FILENO) == -1){
			perror("child: can't redirect stdout");
			return ps_err;
		}

		if ( dup2(linker_stderr[1], STDERR_FILENO) == -1){
			perror("child: can't redirect stderr");
			return ps_err;
		}


		// closing the pipes write ends
		close(linker_stdout[1]);
		close(linker_stderr[1]);

		// we will run our script using execv()
		execv(script_name, script_argv);
		perror("execl failled");
		return ps_err;
  }
  else{
		// this part of code run in the parent process

		// closing the pipes write ends
		close(linker_stdout[1]);
		close(linker_stderr[1]);

		ps_info psinfo;
		psinfo.pid = pid;
		psinfo.stdout_fd = linker_stdout[0];
		psinfo.stderr_fd = linker_stderr[0];

    return psinfo;
	}
}
