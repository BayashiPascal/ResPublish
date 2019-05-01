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
const char* ETCGet(EstimTimeToComp* const that, float comp) {
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

// ---- PBMailer

// ================ Functions implementation ====================

// Create a new PBMailer toward the email adress 'to'
// _delayBetweenEmails is initialiwed to 60s
PBMailer PBMailerCreateStatic(const char* const to) {
#if BUILDMODE == 0
  if (to == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'to' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // Declare the new PBMailer
  PBMailer that;
  // Set properties
  that._to = strdup(to);
  that._messages = GSetStrCreateStatic();
  that._delayBetweenEmails = 60;
  that._lastEmailTime = 0;
  // Return the new PBMailer
  return that;
}

// Free the memory used by the PBMailer 'that'
// Flush the remaining strings if any 
void PBMailerFreeStatic(PBMailer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // Flush the remaing messages
  that->_delayBetweenEmails = 0;
  PBMailerSend(that, "PBMailerFreeStatic flushing remaining messages");
  // Free memory
  free(that->_to);
}

// Send the strings of the PBMailer 'that' if the last PBMailerSend
// call is at least _delayBetweenEmails seconds old, else do nothing
// with the subject 'subject' 
// Uses the 'mail' command which is supposed to configure, up and 
// running by the user
void PBMailerSend(PBMailer* const that, const char* const subject) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
  if (subject == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'subject' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // Get the current time
  time_t curTime = time(NULL);
  // If the delay since the last email is above the threshold and there
  // are messages
  if (curTime - that->_lastEmailTime > that->_delayBetweenEmails &&
    GSetNbElem(&(that->_messages)) > 0) {
    // Calculate the length of the body
    int bodyLength = 0;
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(&(that->_messages));
    do {
      char* str = GSetIterGet(&iter);
      bodyLength += strlen(str);
    } while (GSetIterStep(&iter));
    // Create the body of the email
    char* body = malloc(bodyLength + 1);
    int insertPos = 0;
    while (GSetNbElem(&(that->_messages)) > 0) {
      char* str = GSetPop(&(that->_messages));
      sprintf(body + insertPos, "%s", str);
      insertPos += strlen(str);
      free(str);
    }
    // Save the body to a temporary file
    FILE* fp = fopen("./pbmailer.temp", "w");
    if (fp != NULL) {
      fprintf(fp, "%s", body);
      fclose(fp);
      // Create the command to send the email
      char* cmd = malloc(strlen(that->_to) + strlen(subject) + 50);
      sprintf(cmd, "mail -s \"%s\" %s < ./pbmailer.temp", subject, 
        that->_to);
      // Send the email
      int ret = system(cmd);
      // Erase the temporary file
      ret = system("rm ./pbmailer.temp");
      // TODO should process the returned value
      (void)ret;
      // Free memory
      free(cmd);
    }
    // Update the last email time
    that->_lastEmailTime = time(NULL);
    // Free memory
    free(body);
  }
}

// Add a copy of the string 'str' to the PBMailer 'that' to be sent
// later with PBMailerSend() 
void PBMailerAddStr(PBMailer* const that, const char* const str) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif  
  // If the string is null do nothing
  if (str == NULL || strlen(str) == 0)
    return;
  // Add a copy of the string at the tail of the set of string
  // to send
  GSetAppend(&(that->_messages), strdup(str));
}

