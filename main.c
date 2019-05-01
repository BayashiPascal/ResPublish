#include <stdlib.h>
#include <stdio.h>
#include "respublish.h"

void UnitTestTextOMeter() {
  TextOMeter* meterA = TextOMeterCreate("UnitTestA", 40, 20);
  TextOMeter* meterB = TextOMeterCreate("UnitTestB", 40, 20);
  if (meterA == NULL || meterB == NULL) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "TextOMeterCreate NOK");
    PBErrCatch(ResPublishErr);
  }
  if (!TextOMeterClear(meterA) || !TextOMeterClear(meterB)) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "TextOMeterClear NOK");
    PBErrCatch(ResPublishErr);
  }
  sleep(2);
  if (!TextOMeterPrint(meterA, "Message from UnitTestTextOMeterA") ||
    !TextOMeterPrint(meterB, "Message from UnitTestTextOMeterB")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "TextOMeterClear NOK");
    PBErrCatch(ResPublishErr);
  }
  TextOMeterFlush(meterA);
  TextOMeterFlush(meterB);
  sleep(2);
  TextOMeterFree(&meterA);
  TextOMeterFree(&meterB);
  if (meterA != NULL || meterB != NULL) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "TextOMeterFree NOK");
    PBErrCatch(ResPublishErr);
  }
  printf("UnitTestTextOMeter OK\n");
}

void UnitTestEstimTimeToComp() {
  EstimTimeToComp etc = EstimTimeToCompCreateStatic();
  char* checkA[5] = {
    "???d:??h:??m:??s",
    "000d:00h:00m:03s",
    "000d:00h:00m:02s",
    "000d:00h:00m:01s",
    "000d:00h:00m:00s"
  };
  for (int i = 0; i < 5; ++i) {
    printf("%s\n", ETCGet(&etc, (float)i / 5.0));
    sleep(1);
    if (strcmp(etc._etc, checkA[i]) != 0) {
      ResPublishErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ResPublishErr->_msg, "ETCGet NOK");
      PBErrCatch(ResPublishErr);
    }
  }
  ETCReset(&etc);
  char* checkB[5] = {
    "???d:??h:??m:??s",
    "576d:16h:53m:19s",
    "286d:08h:26m:38s",
    "191d:21h:37m:43s",
    "144d:16h:13m:16s"
  };
  for (int i = 0; i < 5; ++i) {
    printf("%s\n", ETCGet(&etc, (float)(i * i) / 50000000.0));
    sleep(1);
    if (strcmp(etc._etc, checkB[i]) != 0) {
      ResPublishErr->_type = PBErrTypeUnitTestFailed;
      sprintf(ResPublishErr->_msg, "ETCGet NOK");
      PBErrCatch(ResPublishErr);
    }
  }
  printf("UnitTestEstimTimeToComp OK\n");
}

void UnitTestPBMailer() {
  char* email = "Your@Email.net";
  PBMailer mailer = PBMailerCreateStatic(email);
  char* lineA = "UnitTestPBMailer, line A\n";
  char* lineB = "UnitTestPBMailer, line B\n";
  char* lineC = "UnitTestPBMailer, line C\n";
  PBMailerSend(&mailer, "");
  PBMailerAddStr(&mailer, lineA);
  PBMailerAddStr(&mailer, lineB);
  PBMailerSend(&mailer, "UnitTestPBMailer, subject 1");
  PBMailerAddStr(&mailer, lineC);
  PBMailerSend(&mailer, "UnitTestPBMailer, subject 1");
  PBMailerSetDelayBetweenEmails(&mailer, 10);
  if (mailer._delayBetweenEmails != 10) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "PBMailerSetDelayBetweenEmails NOK");
    PBErrCatch(ResPublishErr);
  }
  if (PBMailerGetDelayBetweenEmails(&mailer) != 10) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "PBMailerGetDelayBetweenEmails NOK");
    PBErrCatch(ResPublishErr);
  }
  sleep(11);
  PBMailerSend(&mailer, "UnitTestPBMailer, subject 2");
  PBMailerAddStr(&mailer, lineA);
  PBMailerFreeStatic(&mailer);
  
  // Emails reveived:
/*
UnitTestPBMailer, subject 1
> UnitTestPBMailer, line A
> UnitTestPBMailer, line B
  
UnitTestPBMailer, subject 2
> UnitTestPBMailer, line C

PBMailerFreeStatic flushing remaining messages
> UnitTestPBMailer, line A
*/
  
  printf("UnitTestPBMailer OK\n");
}

void UnitTestAll() {
  UnitTestTextOMeter();
  UnitTestEstimTimeToComp();
  UnitTestPBMailer();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

