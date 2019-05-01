// ============ RESPUBLISH-INLINE.C ================

// ---- TextOMeter

// ================ Functions implementation ====================

// Clear the content of the TextOMeter 'that'
// Return true if the content could be cleared, false else
#if BUILDMODE != 0
inline
#endif
bool TextOMeterClear(TextOMeter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif 
  // Print on the tty as many line as the height of the Xterm
  for (int i = that->_height; i--;) {
    if (fprintf(that->_fp, "\n") < 0)
      return false;
    fflush(that->_fp);
  }
  return true;
}

// Print the string 'str' on the TextOMeter 'that'
// Return true if the content could be printed, false else
#if BUILDMODE != 0
inline
#endif
bool TextOMeterPrint(TextOMeter* const that, const char* const str) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
  if (str == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'str' is null");
    PBErrCatch(ResPublishErr);
  }
#endif 
  // Print the string on the tty
  if (fprintf(that->_fp, "%s", str) < 0)
    return false;
  return true;
}

// Flush the stream of the TextOMeter 'that'
#if BUILDMODE != 0
inline
#endif
void TextOMeterFlush(TextOMeter* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif 
  fflush(that->_fp);
}

// ---- PBMailer

// Set the minimum delay between emails of the PBMailer 'that' to 'delay'
#if BUILDMODE != 0
inline
#endif 
void PBMailerSetDelayBetweenEmails(PBMailer* const that, 
  const time_t delay) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif 
  that->_delayBetweenEmails = delay;  
}

// Get the minimum delay between emails of the PBMailer 'that'
#if BUILDMODE != 0
inline
#endif 
time_t PBMailerGetDelayBetweenEmails(PBMailer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    ResPublishErr->_type = PBErrTypeNullPointer;
    sprintf(ResPublishErr->_msg, "'that' is null");
    PBErrCatch(ResPublishErr);
  }
#endif 
  return that->_delayBetweenEmails;
}

