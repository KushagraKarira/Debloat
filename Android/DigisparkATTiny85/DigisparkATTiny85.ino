#include "DigiKeyboard.h"
void setup() {
  // void

}

void loop() {
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStrtoke(MOD_CONTROL_LEFT, MOD_ALT_LEFT,T);
  DigiKeyboard.delay(500);
  DigiKeyboard.print("adb devices");
  DigiKeyboard.DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
    DigiKeyboard.print("adb pm uninstall --user 0 com.google");
  DigiKeyboard.DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
    DigiKeyboard.print("adb pm uninstall --user 0 com.google");
  DigiKeyboard.DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
    DigiKeyboard.print("adb pm uninstall --user 0 com.google");
  DigiKeyboard.DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
    DigiKeyboard.print("adb pm uninstall --user 0 com.google");
  DigiKeyboard.DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(500);
}
