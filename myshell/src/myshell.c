/*
Operating Systems
Assignment 2018-2019
Giokotos Konstantinos
AEM: 8791
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int command_handler(char *input, FILE *fp) {
	//Array definitions.	
	char *independent_commands[512];
	char *dependent_commands[512];
	char *output_redirections[10];
	char *input_redirections[10];
	char *command[100];
	int counter,status,fdout,fdin;

	//Remove \n from the end of the input.
	char *pos;
	if ((pos = strchr(input, '\n')) != NULL) {
		*pos = '\0';
	}
	
	//Break the input into independent commands and store them in an array.
	counter = 0;
	char *token = strtok(input, ";");
	while(token) {
		independent_commands[counter] = token;
		counter++;
		token = strtok(NULL, ";");
	}
	independent_commands[counter] = NULL;

	int i = 0;
	//Break each independent command into dependent commands and store them in an array.
	while(independent_commands[i] != NULL) {			
		counter = 0;
		char *token = strtok(independent_commands[i], "&&");
		while(token) {
			dependent_commands[counter] = token;
			counter++;
			token = strtok(NULL, "&&");
		}
		dependent_commands[counter] = NULL;
			
		int j = 0;
		//The first dependent command has to be executed.
		int succeded = 1;
		//Check if there is redirection in the command and then break it.
		while(dependent_commands[j] != NULL) {
			int output_redirection = 0;
			int input_redirection = 0;

			counter = 0;
			//Check for output redirection and store the file name in an array.
			char *token2 = strtok(dependent_commands[j], ">");
			while(token2) {
				output_redirections[counter] = token2;
				counter++;
				token2 = strtok(NULL,">");
			}

			//The part before the ">" is the command to be executed.
			dependent_commands[j] = output_redirections[0];

			//If there is output redirection raise the flag, and remove the spaces from the file where the output will be redirected.
			if (counter == 2) {
				output_redirection = 1;
				int k = 0;
				char *token3 = strtok(output_redirections[1], " ");
				while(token3) {
					output_redirections[1] = token3;
					k++;
					token3 = strtok(NULL, " ");
				}
			}

			//Check for input redirection and store the file name in an array.
			counter = 0;
			char *token4 = strtok(dependent_commands[j], "<");
			while(token4) {
				input_redirections[counter] = token4;
				counter++;
				token4 = strtok(NULL," ");
			}

			//The part before the "<" is the command to be executed.
			dependent_commands[j] = output_redirections[0];

			//If there is input redirection raise the flag, and remove the spaces from the file from where the input will be redirected.
			if (counter == 2) {
				input_redirection = 1;
				int k = 0;
				char *token5 = strtok(input_redirections[1], " ");
				while(token5) {
					input_redirections[1] = token5;
					k++;
					token5 = strtok(NULL, " ");
				}
			}				

			//Break the executable command into its arguments and store them in an array.
			counter = 0;
			char *token6 = strtok(dependent_commands[j], " ");
			while(token6) {
				command[counter] = token6;
				counter++;
				token6 = strtok(NULL, " ");
			}
			command[counter] = NULL;

			//If the previous dependent command has been successfully executed, then execute the current command.
			if (succeded == 1) {
				//If the command is "cd" handle it with the chdir() function.
				if (strstr(command[0],"cd") != NULL) {
					if (command[1] != NULL) {
						//Change directory and print if there is an error.				
						if (chdir(command[1]) == -1) {
							perror("Error changing directory.");
							//Raise the flag so the next dependent command won't be executed.
							succeded = 0;
						}
					} else {
						//If there is no argument go to the home directory.
						if (chdir("/home") == -1) {
							perror("Error changing directory.");
							succeded = 0;
						}
					}

				//If the command is quit, exit the program.
				} else if (strstr(command[0],"quit") != NULL) {
					if (command[1] == NULL) {
						//Return 1 so I know to exit the program.
						return 1;
					} else {
						//If it has any arguments, show the user how to input the command to exit.
						printf("Type 'quit' to exit. \n");
						succeded = 0;
					}

				//If it is any other command, use the execvp() command	.				
				} else {
					//Create a child process.
					pid_t pid;
					pid = fork();
					//Print if there is an error.
					if (pid < 0) {
						perror("Error forking child process.");
						succeded = 0;
						exit(EXIT_FAILURE);
					}
					//The child will execute the command.
					else if (pid == 0) {
						//If the file pointer isn't NULL we are in batch mode.
						if(fp) {
							//Close the file in the child as it won't use it, and it may cause problems.						
							fclose(fp);
						}

						//If there is output redirection, open the file and redirect standard output to the file.
						if (output_redirection == 1) {
							fdout = open(output_redirections[1], O_RDWR | O_CREAT, 0666);
							//If an error is encountered.
							if (fdout == -1) {
								perror("Failed to open specified file. \n");
							} else {
								dup2(fdout,STDOUT_FILENO);
								close(fdout);
							}
						}
						//If there is input redirection, open the file and redirect its contents to the standard input.
						if (input_redirection == 1) {
							fdin = open(input_redirections[1], O_RDONLY);
							if (fdin == -1) {
								perror("Failed to open specified file. \n");
							} else {
								dup2(fdin, STDIN_FILENO);
								close(fdin);
							}
						}
						//Execute the command.
						if (execvp(command[0], command) == -1) {
							//Print if the command wasn't executed.
							perror("Error executing command");
							exit(EXIT_FAILURE);
						}
					}
					//The father waits for the child to finish, and then continues with the next command.
					else {
						waitpid(pid,&status,0);
						//Check the exit status of the child, and if the previous command wasn't executed, raise the flag.
						if (WIFEXITED(status)) {
							int exit_status = WEXITSTATUS(status);
							if (exit_status != 0) {
								succeded = 0;
							}
						}
					}
				}
			}
			j++;
		}
		i++;
	}

	//Return when all the commands have been handled.
	return 0;
}

int main(int argc, char **argv){ 
	//If there are no arguments, use interactive mode.
	if (argc == 1) {		
		char input[512];
		
		//Read the input from the user.
		printf("giokotos_8791>");
		fgets(input,512,stdin);
		
		while (1) {
			//Handle the command, and if the return value is 1, exit the program.
			if(command_handler(input, NULL) == 1) {
				return 0;
			}

			//Read the next input from the user.
			printf("giokotos_8791>");
			fgets(input,512,stdin);
		}
	// If there is an argument, use batch mode.
	} else if (argc == 2) {
		char input[512];
		FILE *fp;

		//Open the batch file.
		fp = fopen(argv[1],"r");

		//Read the line, until we reach the end of file.
		while(fgets(input, 512, fp) != NULL){
			//Handle the command, and if the return value is 1, break from the loop.
			if(command_handler(input, fp) == 1) {
				break;
			}
		}
		//Close the file.
		fclose(fp);
	//Invalid use of the program.	
	} else {
		printf("Use no arguments for interactive mode and one argument for batch mode.\n");
	}
	//Exit the program.
	return 0;
}
