#include <stdio.h>
#include <unistd.h>
#include <signal.h>
// Main function
// Example code to display text in an independant Xterm from a running
// parent process.
// Compile with gcc -o main main.c
int main() {
  // Temporary file to transmit the tty and pid from the Xterm to the 
  // parent process
  #define TMPFILENAME "./tmptty"
  // Ensure the temporary file doesn't exist
  remove(TMPFILENAME);
  // Fork between the parent process executing the main algorithm and
  // the child process running the Xterm
  pid_t pid = fork();
  // If we are in the child process
  if (pid == 0) {
    // Open a new Xterm, executing '(echo $$ && tty) > ./tmptty'
    // to save its pid and tty into the temporary file, followed
    // by a bash to avoid the window closing right after the command
    char cmd[] = "/usr/bin/xterm";
    char* argv[] = {
      "xterm",
      "-e",
      "(echo $$ && tty) > " TMPFILENAME " ; bash",
      NULL
      };
    execv(cmd, argv);
    // Never reach here as the child get replaced by the Xterm
  // Else, we are in the parent process
  } else {
    // Wait one second to give time to the Xterm to write its pid and 
    // tty into the temporary file
    sleep(1);
    // Open the temporary file
    FILE* fp = fopen(TMPFILENAME, "r");
    // Variable to memorize the tty of the Xterm
    char tty[100] = {'\0'};
    // Read the pid and tty of the Xterm
    fscanf(fp, "%d %s\n", &pid, tty);
    // Close and remove the temporary file
    fclose(fp);
    remove(TMPFILENAME);
    // Display the information about the Xterm
    printf("Xterm attached to tty %s and has pid %d\n", tty, pid);
    // Open the tty
    FILE* ftty = fopen(tty, "w");
    // Simulate a process sending info toward the Xterm and its own 
    // console
    for (int i = 0; i < 10; ++i) {
      fprintf(ftty, "Toward Xterm: %d\n", i);
      printf("Toward parent's terminal: %d\n", i);
      sleep(1);
    }
    // Close the tty
    fclose(ftty);
    // Kill the Xterm
    kill(pid, SIGKILL);
  }
  // Return success code
  return 0;
}
