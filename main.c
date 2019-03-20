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

void UnitTestAll() {
  UnitTestTextOMeter();
  printf("UnitTestAll OK\n");
}

int main() {
  UnitTestAll();
  // Return success code
  return 0;
}

