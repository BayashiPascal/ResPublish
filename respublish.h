// ============ RESPUBLISH.H ================

#ifndef RESPUBLISH_H
#define RESPUBLISH_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "gset.h"
#include "pberr.h"

// ---- TextOMeter

// ================= Define ===================

#define TEXTOMETER_TTY_FILENAME "/tmp/textometertty.tmp"
#define TEXTOMETER_TTY_MAXLENGTH 100
#define TEXTOMETER_XTERM_CMD "/usr/bin/xterm"

// ================= Data structure ===================

typedef struct TextOMeter {
  // Title
  char* _title;
  // Width
  int _width;
  // Height
  int _height;
  // PID of the associated xterm
  pid_t _pid;
  // File pointer to the tty of the associated xterm
  char _tty[TEXTOMETER_TTY_MAXLENGTH];
  FILE* _fp;
} TextOMeter;

// ================ Functions declaration ====================

// Create a new TextOMeter of 'width' columns and 'height' lines and
// 'title' as title of the attached xterm
// May return NULL if the creation of the Xterm failed
TextOMeter* TextOMeterCreate(char* const title, 
  const int width, const int height);

// Free the memory used by the TextOMeter 'that'
void TextOMeterFree(TextOMeter** that);

// Clear the content of the TextOMeter 'that'
// Return true if the content could be cleared, false else
#if BUILDMODE != 0
inline
#endif
bool TextOMeterClear(TextOMeter* const that);

// Print the string 'str' on the TextOMeter 'that'
// Return true if the content could be printed, false else
#if BUILDMODE != 0
inline
#endif
bool TextOMeterPrint(TextOMeter* const that, const char* const str);

// Flush the stream of the TextOMeter 'that'
#if BUILDMODE != 0
inline
#endif
void TextOMeterFlush(TextOMeter* const that);

// ---- EstimTimeToComp

// ================= Define ===================

// ================= Data structure ===================

typedef struct EstimTimeToComp {
  // Start time
  time_t _start;
  // ETC
  char _etc[100];
} EstimTimeToComp;

// ================ Functions declaration ====================

// Create a new EstimTimeToComp
EstimTimeToComp EstimTimeToCompCreateStatic();

// Free the memory used by the EstimTimeToComp 'that'
void EstimTimeToCompFreeStatic(EstimTimeToComp* that);

// Reset the start time of the EstimTimeToComp 'that' to current time
void ETCReset(EstimTimeToComp* that);

// Estimate the ETC of the EstimTimeToComp 'that' given the percentage
// of completion 'comp'
// time(0) is expected to returned Thu Jan  1 00:00:00 1970
const char* ETCGet(EstimTimeToComp* const that, float comp);

// ---- PBMailer

// ================= Define ===================

// ================= Data structure ===================

typedef struct PBMailer {
  // Set of strings to send
  GSetStr _messages;
  // Target email address
  char* _to;
  // Minimum delay in seconds between two actual emails
  // Used to avoid flooding the target address
  time_t _delayBetweenEmails;
  // Time of last sent email
  time_t _lastEmailTime;
} PBMailer;

// ================ Functions declaration ====================

// Create a new PBMailer toward the email adress 'to'
// _delayBetweenEmails is initialiwed to 60s
PBMailer PBMailerCreateStatic(const char* const to);

// Free the memory used by the PBMailer 'that'
// Flush the remaining strings if any 
void PBMailerFreeStatic(PBMailer* that);

// Send the strings of the PBMailer 'that' if the last PBMailerSend
// call is at least _delayBetweenEmails seconds old, else do nothing
// with the subject 'subject' 
// Uses the 'mail' command which is supposed to configure, up and 
// running by the user
void PBMailerSend(PBMailer* const that, const char* const subject);

// Add a copy of the string 'str' to the PBMailer 'that' to be sent
// later with PBMailerSend() 
void PBMailerAddStr(PBMailer* const that, const char* const str);

// Set the minimum delay between emails of the PBMailer 'that' to 'delay'
#if BUILDMODE != 0
inline
#endif 
void PBMailerSetDelayBetweenEmails(PBMailer* const that, const time_t delay);

// Get the minimum delay between emails of the PBMailer 'that'
#if BUILDMODE != 0
inline
#endif 
time_t PBMailerGetDelayBetweenEmails(PBMailer* const that);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "respublish-inline.c"
#endif


#endif
