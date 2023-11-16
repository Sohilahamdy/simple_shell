#include "shell.h"

void display_prompt()
{
	printf(":) ");
}

void print_environment()
{
	extern char **environ;
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

int is_command_valid(char *command)
{
	char *path;
	char *token;
	char *path_env = getenv("PATH");

	if (path_env == NULL)
	{
		fprintf(stderr, "Error: PATH environment variable not set.\n");
		return 0;
	}

	path = strdup(path_env);

	token = strtok(path, ":");
	while (token != NULL)
	{
		char *path_command = malloc(strlen(token) + strlen(command) + 2);
		if (path_command == NULL)
		{
			perror("malloc");
			free(path);
			return 0;
		}

		strcpy(path_command, token);
		strcat(path_command, "/");
		strcat(path_command, command);

		if (access(path_command, X_OK) == 0)
		{
			free(path_command);
			free(path);
			return 1; // Command is valid
		}

		free(path_command);
		token = strtok(NULL, ":");
	}

	free(path);
	return 0; // Command not found in any PATH directory
}

void execute_command(char *command, char **args)
{
	if (strcmp(command, "exit") == 0)
	{
		printf("Exiting shell.\n");
		exit(EXIT_SUCCESS);
	}
	else if (strcmp(command, "env") == 0)
	{
		print_environment();
	}
	else
	{
		if (!is_command_valid(command))
		{
			fprintf(stderr, "Error: Command not found: %s\n", command);
			return;
		}

		pid_t pid, wpid;
		int status;

		pid = fork();
		if (pid == 0)
		{
			// Child process
			if (execve(command, args, NULL) == -1)
			{
				perror("./shell");
				exit(EXIT_FAILURE);
			}
			else if (pid < 0)
			{
				perror("fork");
			}
			else
			{
				// Parent process/
				do
				{
					wpid = waitpid(pid, &status, WUNTRACED);
				}
				while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		}
	}

	int parse_arguments(char *input, char **args)
	{
		int argc = 0;
		char *token;

		token = strtok(input, DELIM);
																
																                                                                                                                                                                 while (token != NULL) {
																                                                                                                                                                                         args[argc++] = token;
																                                                                                                                                                                                 token = strtok(NULL, DELIM);
																                                                                                                                                                                                     }
																
																                                                                                                                                                                                         args[argc] = NULL; // Null-terminate the argument list
																
																                                                                                                                                                                                             return argc;
																                                                                                                                                                                                             }
																                                                                                                                                                                                             
