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
  for (int i = 0; i < 5; ++i) {
    printf("%s\n", ETCGet(&etc, (float)i / 5.0));
    sleep(1);
  }
  ETCReset(&etc);
  for (int i = 0; i < 5; ++i) {
    printf("%s\n", ETCGet(&etc, (float)(i * i) / 50000000.0));
    sleep(1);
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
  
  // Emails received:
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

void UnitTestProgBarTxt() {

  ProgBarTxt progBar = ProgBarTxtCreateStatic();
  if (fabs(progBar._status) > PROGBARTXT_EPSILON ||
    strcmp(progBar._bar, "|-----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtCreateStatic NOK %s",progBar._bar);
    PBErrCatch(ResPublishErr);
  }
  if (ProgBarTxtGet(&progBar) != progBar._bar) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtGet NOK");
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.0);
  if (strcmp(ProgBarTxtGet(&progBar), "|-----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.0 %s", 
      ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.001);
  if (strcmp(ProgBarTxtGet(&progBar), "|0----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.0 %s", 
      ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.01);
  if (strcmp(ProgBarTxtGet(&progBar), "|1----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.01 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.09);
  if (strcmp(ProgBarTxtGet(&progBar), "|9----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.099 %s", 
      ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.099);
  if (strcmp(ProgBarTxtGet(&progBar), "|9----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.099 %s", 
      ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.1);
  if (strcmp(ProgBarTxtGet(&progBar), "|X----------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.1 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.11);
  if (strcmp(ProgBarTxtGet(&progBar), "|X1---------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.11 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.5);
  if (strcmp(ProgBarTxtGet(&progBar), "|XXXXX------|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.5 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.9);
  if (strcmp(ProgBarTxtGet(&progBar), "|XXXXXXXXX--|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.9 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 0.99);
  if (strcmp(ProgBarTxtGet(&progBar), "|XXXXXXXXX9-|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 0.99 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  ProgBarTxtSet(&progBar, 1.0);
  if (strcmp(ProgBarTxtGet(&progBar), "|XXXXXXXXXXX|")) {
    ResPublishErr->_type = PBErrTypeUnitTestFailed;
    sprintf(ResPublishErr->_msg, "ProgBarTxtSet NOK 1.0 %s", ProgBarTxtGet(&progBar));
    PBErrCatch(ResPublishErr);
  }
  
  ProgBarTxtFreeStatic(&progBar);
  printf("UnitTestProgBarTxt OK\n");
}

void UnitTestAll() {
  UnitTestTextOMeter();
  UnitTestEstimTimeToComp();
  UnitTestPBMailer();
  UnitTestProgBarTxt();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

