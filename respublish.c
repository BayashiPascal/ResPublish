// ============ RESPUBLISH.C ================

// ================= Include =================

#include "respublish.h"
#if BUILDMODE == 0
#include "respublish-inline.c"
#endif

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
        // Clear the content of the TextOMeter
        TextOMeterClear(that);
        TextOMeterFlush(that);
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

// ---- EstimTimeToComp

// ================ Functions implementation ====================

// Create a new EstimTimeToComp
EstimTimeToComp EstimTimeToCompCreateStatic() {
  // Declare the new EstimTimeToComp
  EstimTimeToComp that;
  // Set properties
  ETCReset(&that);
  that._etc[0] = '\0';
  // Return the new EstimTimeToComp
  return that;
}

// Free the memory used by the EstimTimeToComp 'that'
void EstimTimeToCompFreeStatic(EstimTimeToComp* that) {
  // Nothing to do
  (void)that;
}

// Reset the start time of the EstimTimeToComp 'that' to current time
void ETCReset(EstimTimeToComp* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // Reset the start time to the current time
  that->_start = time(NULL);
}

// Estimate the ETC of the EstimTimeToComp 'that' given the percentage
// of completion 'comp'
// time(0) is expected to returned Thu Jan  1 00:00:00 1970
const char* ETCGet(EstimTimeToComp* that, float comp) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // Get the current time
  time_t cur = time(NULL);
  // If the percentage of completino is valid
  if (comp > 0.0 && comp <= 1.0) {
    // Calculate the estimated time to completion and store the result
    // in a string format
    time_t elapsed = cur - that->_start;
    time_t remain = (time_t)((float)elapsed / comp) - elapsed;
    struct tm* rtm = gmtime(&remain);
    sprintf(that->_etc, "%03dd:%02dh:%02dm:%02ds", 
      (rtm->tm_year - 70) * 365 + rtm->tm_mon * 30 + rtm->tm_mday - 1, 
      rtm->tm_hour, rtm->tm_min, rtm->tm_sec);
  } else {
    sprintf(that->_etc, "???d:??h:??m:??s"); 
  }
  // Return the etc
  return that->_etc;
}

