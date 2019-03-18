// ============ RESPUBLISH.C ================

// ================= Include =================

#include "respublish.h"
#if BUILDMODE == 0
#include "respublish-inline.c"
#endif

// ================ Define ====================

// ================ Functions implementation ====================

// Create a new TextOMeter of 'width' columns and 'height' lines and
// 'title' as title of the attached xterm
// May return NULL if the creation of the Xterm failed
TextOMeter* TextOMeterCreate(char* const title, 
  const int width, const int height) {
#if BUILDMODE == 0
  if (title == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'title' is null");
    PBErrCatch(ResPublishErr);
  }
  if (width <= 0) {
    ResPublishErr->_type = PBErrTypeInvalidArg;
    sprintf(ResPublishErr->_msg, "'width' is invalid (0<%d)", width);
    PBErrCatch(ResPublishErr);
  }
  if (height <= 0) {
    ResPublishErr->_type = PBErrTypeInvalidArg;
    sprintf(ResPublishErr->_msg, "'height' is invalid (0<%d)", height);
    PBErrCatch(ResPublishErr);
  }
#endif 
  // Declare the new TextOMeter
  TextOMeter* that = NULL;
  // Ensure the temporary file to get the tty and pid doesn't exists
  remove(TEXTOMETER_TTY_FILENAME);
  // Fork to create the Xterm
  pid_t pid = 0;
  if ((pid = fork()) == 0) {
    // Create the Xterm
    char cmd[] = TEXTOMETER_XTERM_CMD;
    char geometry[100];
    sprintf(geometry, "%dx%d", width, height);
    char* argv[] = {
      "xterm",
      "-xrm",
      "'XTerm.vt100.allowTitleOps: false'",
      "-T",
      title,
      "-geometry",
      geometry,
      "-e",
      "(echo $$ && tty) > " TEXTOMETER_TTY_FILENAME " ; bash",
      NULL
      };
    if (execv(cmd, argv) == -1) {
      fprintf(stderr, "TextOMeter '%s' couldn't create the Xterm, "
        "execv failed (%d)\n", title, errno);
    }
  } else {
    // Wait for the tty and pid from the Xterm
    FILE *fp = NULL;
    sleep(1);
    int wait = 10000;
    do {
      fp = fopen(TEXTOMETER_TTY_FILENAME, "r");
      wait--;
    } while (fp == NULL && wait > 0);
    if (fp == NULL) {
      fprintf(stderr, 
        "TextOMeter '%s' couldn't read the tty and pid\n", title);
    } else {
      // Read the tty and pid from the Xterm
      char tty[100];
      if (fscanf(fp, "%d %s\n", &pid, tty) == EOF) {
        fprintf(stderr, 
          "TextOMeter '%s' couldn't read the tty and pid\n", title);
      } else {
        // Allocate memory for the new TextOMeter
        that = PBErrMalloc(ResPublishErr, sizeof(TextOMeter));
        // Set properties
        that->_width = width;
        that->_height = height;
        that->_title = strdup(title);
        that->_pid = pid;
        strcpy(that->_tty, tty);
        // Open the tty to send message to the Xterm
        that->_fp = fopen(that->_tty, "w");
      }
      // Close the temporary file and delete it
      fclose(fp);
      remove(TEXTOMETER_TTY_FILENAME);
    }
  }
  // Return the new TextOMeter
  return that;
}

// Free the memory used by the TextOMeter 'that'
void TextOMeterFree(TextOMeter** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Close the file pointer to the tty
  if ((*that)->_tty != NULL)
    fclose((*that)->_fp);
  // Kill the terminal
  if (kill((*that)->_pid, SIGTERM) == -1) {
    fprintf(stderr, 
      "Couldn't kill the TextOMeter '%s'\n", (*that)->_title);
  }
  // Free memory
  free((*that)->_title);
  free(*that);
  *that = NULL;
}

