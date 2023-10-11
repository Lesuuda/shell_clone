#include "main.h"
int main(int argc, char **argv)
{
	int execmd(char **argv);
	char *buf, *buf_copy;
	ssize_t chars_read;
	size_t n;
	char *delimeter;
	int number_tokens;
	int i;
	char *token;
	char *prompt;
	int status;
	pid_t child_pid;
	bool exit_shell;
	(void)argc;
	
	buf = NULL;
	number_tokens = 0;
	prompt = "\n:)";
	delimeter = " \n";
	i = 0;
	exit_shell = false;


	while (!exit_shell)
	{
		printf("%s", prompt);
		chars_read = getline(&buf, &n, stdin);
		if(chars_read == -1)
		{
			printf("Exiiting Shell...");
			exit_shell = true;
			continue;
		}
		if(buf[chars_read - 1] == '\n')
			buf[chars_read - 1] = '\0';
		buf_copy = malloc(sizeof(char) * chars_read);
		if (buf_copy == NULL)
		{
			perror("memory allocation failed!");
			return (-1);
		}
		memcpy(buf_copy, buf, chars_read);

		token = strtok(buf_copy, delimeter);
		while(token != NULL)
		{
			number_tokens++;
			token = strtok(NULL, delimeter);
		}
		number_tokens++;

		argv = malloc(sizeof(char *) * number_tokens);
		token = strtok(buf_copy, delimeter);
		while(token != NULL)
		{
			argv[i] = malloc(sizeof(char) * (strlen(token) + 1));
			if(argv[i] == NULL)
			{
				perror("argv memory allocation failed");
				return (-1);
			}
			strcpy(argv[i], token);
			token = strtok(NULL, delimeter);
			i++;
		}
		argv[i] = NULL;

		child_pid = fork();
		if(child_pid < 0)
		{
			perror("Error in creating child process");
			return (1);
		}
		
		else if (child_pid == 0)
		{
			if(execmd(argv) == -1)
			{
				perror("command execution failed");
				exit (1);
			}
		}
		else
		{
			waitpid(child_pid, &status, 0);
			if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
				printf("No file or directory");
		}
		for (i = 0; i < number_tokens; i++)
			free(argv[i]);
	}
	free(argv);
	free(buf_copy);

	return (0);
}
