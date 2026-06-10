#include "../include/nexus.h"

char *history[HISTORY_MAX];
int history_count = 0;


int main() {

	char input[1024];

	load_history();

	while(1) {

		printf("nexus> ");
		fflush(stdout);

		if (fgets(input, sizeof(input), stdin) == NULL) {
			break;
			}

		// remove newline
		input[strcspn(input, "\n")] = 0;

		if (strlen(input) > 0) {
			history[history_count++] = strdup(input);
			save_history();
		}

		// split into args
		char *args[100];
		int i = 0;

		char *token = strtok(input, " ");
		while (token != NULL) {
			args[i++] = token;
			token = strtok(NULL, " ");
		}

		args[i] = NULL;

		// ignore empty input
		if (args[0] == NULL) continue;

		if (is_builtin(args[0])) {
			if (!run_builtin(args)) break;
			continue;
		}


		// pipe detection
		int pipe_index = -1;
		for (int j = 0; args[j] != NULL; j++) {
			if (strcmp(args[j], "|") == 0) {
				pipe_index = j;
				args[j] = NULL;
				break;
			}
		}

		// redirection detection
		char *outfile = NULL;
		char *infile = NULL;
		int append = 0;

		for (int j = 0; args[j] != NULL; j++) {
			if (strcmp(args[j], ">>") == 0) {
				outfile = args[j +1];
				append = 1;
				args[j] = NULL;
				break;
			}
			if (strcmp(args[j], ">") == 0) {
				outfile = args[j + 1];
				args[j] = NULL;
				break;
			}
			if (strcmp(args[j], "<") == 0) {
				infile = args[j + 1];
				args[j] = NULL;
				break;
			}
		}

		// pipe execution
		if(pipe_index != -1) {
			char **left = args;
			char **right = args + pipe_index + 1;

			int pipefd[2];
			pipe(pipefd);

			pid_t pid1 = fork();
			if (pid1 == 0) {
				// left command writes to  pipe
				dup2(pipefd[1], 1);
                		close(pipefd[0]);
                		close(pipefd[1]);
                		execvp(left[0], left);
                		perror("nexus");
                		exit(1);
			}

			pid_t pid2 = fork();
			if (pid2 == 0) {
				// right commands reads from pipe
				dup2(pipefd[0], 0);
                		close(pipefd[0]);
                		close(pipefd[1]);
                		execvp(right[0], right);
                		perror("nexus");
                		exit(1);
			}

			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);
			continue;
		}

		// create process
		pid_t pid = fork();

		if (pid == 0) {

		// redirection application
			if (outfile != NULL) {
				int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
				int fd = open(outfile, flags, 0644);
				if (fd < 0) {
					perror("nexus: open");
					exit(1);
				}
				dup2(fd, 1);
				close(fd);
			}

			if (infile != NULL) {
				int fd = open(infile, O_RDONLY);
				if (fd < 0) {
					perror("nexus: open");
					exit(1);
				}
				dup2(fd, 0);
				close(fd);
			}


			// child process runs command
			execvp(args[0], args);

			// if exec fails
			perror("nexus");
			exit(1);
		}
		else {

			// parent waits
			wait(NULL);
		}
	}


	return 0;

}


void save_history() {

	char *home = getenv("HOME");
	char path[1024];

	snprintf(path, sizeof(path), "%s%s", home, HISTORY_FILE);

	FILE *f = fopen(path, "w");
	if (f == NULL) return;

	for (int i = 0; i < history_count; i++) {

		fprintf(f, "%s\n", history[i]);

	}

	fclose(f);
}


void load_history() {

	char *home =getenv("HOME");
	char path[1024];
	snprintf(path, sizeof(path), "%s%s", home, HISTORY_FILE);

	FILE *f = fopen(path, "r");
	if (f == NULL) return;

	char line[1024];

	while (fgets(line, sizeof(line), f) && history_count < HISTORY_MAX) {
		line[strcspn(line, "\n")] = 0;
		history[history_count++] = strdup(line);
	}

	fclose(f);
}

