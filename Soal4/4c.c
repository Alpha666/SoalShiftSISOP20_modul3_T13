#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main() {
  int deskriptor[2];
  pid_t pid;
  pipe(deskriptor);

  pid = fork();
  if (pid == 0) {
    dup2(deskriptor[1], 1);
    close(deskriptor[0]);
    close(deskriptor[1]);
    char *argv[] = {"ls", NULL};
    execv("/bin/ls", argv);
  }
  while(wait(NULL) > 0);

  pid = fork();
  if (pid == 0) {
    dup2(deskriptor[0], 0);
    close(deskriptor[0]);
    close(deskriptor[1]);
    char *argv[] = {"wc", "-l", NULL};
    execv("/usr/bin/wc", argv);
  }
}
