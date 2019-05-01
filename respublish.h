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
const char* ETCGet(EstimTimeToComp* that, float comp);

// ================ Inliner ====================

#if BUILDMODE != 0
#include "respublish-inline.c"
#endif


#endif
