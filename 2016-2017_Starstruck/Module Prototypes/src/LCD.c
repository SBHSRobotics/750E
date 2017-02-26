#include <main.h>
#include <string.h>

MenuItem *current;
FILE *port;

void pulseMotor(unsigned char port);
void beep();
void boop();
void runSelfTest();
void callAuton();

MenuItem *lcdmInit(FILE *lcdPort) {
  port = lcdPort;
  lcdInit(lcdPort);
  lcdClear(lcdPort);
  lcdSetBacklight(lcdPort, true);
  lcdSetText(lcdPort, 1, "~~750Evolution~~");
  lcdSetText(lcdPort, 2, "Initializing...");
  printf("LCD Initialized. Creating root menu...\n\r");
  delay(300);
  // char title[] = "<    Select    >";
  MenuItem *root = malloc(sizeof(MenuItem *));
  root = lcdmCreateItem("<   Select     >");
  (*root).previous = root;
  (*root).next = root;
  printf("Menu created. Setting active menu...\n\r");
  delay(200);
  current = root;
  printf("LCD Menu Initialized\n\r");
  delay(200);
  return root;
}

void lcdmAddDefaults(MenuItem *root) {
  printf("Adding default LCDM features...\n\r");
  delay(100);
  printf("Creating Manual Test menu...\n\r");
  delay(100);
  MenuItem *manualTest = malloc(sizeof(MenuItem *)),
           *exitManualTest = malloc(sizeof(MenuItem *));
  manualTest = lcdmCreateItem("< Manual Test  >");
  exitManualTest = lcdmCreateItem("<     Back     >");
  delay(300);
  (*exitManualTest).previous = exitManualTest;
  (*exitManualTest).next = exitManualTest;
  (*exitManualTest).select = manualTest;
  printf("Adding Manual Test submenus...\n\r");
  delay(800);
  char *title;
  for (int i = 1; i<=10; i++) {
    title = malloc(sizeof(char)*16);
    sprintf(title, "<   Motor %d  >",i);
    delay(100);
    MenuItem *motor = malloc(sizeof(MenuItem *));
    motor = lcdmCreateItem(title);
    printf("%s", (*motor).name);

    (*motor).action = &pulseMotor;
    (*motor).param = i;

    lcdmAddItem(exitManualTest, motor);
  }
  MenuItem *speaker = malloc(sizeof(MenuItem *));
  speaker = lcdmCreateItem("<   Speaker    >");
  lcdmAddItem(exitManualTest, speaker);
  speaker->action = &beep;
  //TODO: This is getting repetitive, consider abstracting
  MenuItem *selfTest = malloc(sizeof(MenuItem *));
  selfTest = lcdmCreateItem("<     POST     >");
  lcdmAddItem(root, selfTest);
  selfTest->action = &runSelfTest;
  MenuItem *runAuton = malloc(sizeof(MenuItem *));
  runAuton = lcdmCreateItem("<     Auton    >");
  lcdmAddItem(root, runAuton);
  runAuton->action = &callAuton;
  // speaker->param = 0;
  printf("Done.\n\r");
  delay(800);
  (*manualTest).select = exitManualTest;
  printf("Adding Manual Test menu...\n\r");
  delay(800);
  lcdmAddItem(root, manualTest);
}

MenuItem *lcdmCreateItem(char *name) {
  // MenuItem *item = malloc(sizeof(MenuItem *));
  MenuItem item = { .name = name,
                    .previous = malloc(sizeof(MenuItem *)),
                    .next = malloc(sizeof(MenuItem *)),
                    .select = malloc(sizeof(MenuItem *)),
                    .action = malloc(sizeof(void *)),
                    .param = malloc(sizeof(void *))
                  };
  item.select = NULL;
  item.action = NULL;
  item.param = NULL;
  MenuItem *clone = malloc(sizeof(MenuItem *));
  *clone = item;

  return clone;
}

void lcdmAddItem(MenuItem *root, MenuItem *item) {
  MenuItem *last = (*root).previous;
  (*last).next = item;
  (*root).previous = item;
  (*item).next = root;
  (*item).previous = last;
}

void lcdmLoop(MenuItem *root) {
  printf("loop: %p\n\r",current);
  printf("Current: %s\n\r",(*current).name);
  lcdSetText(port, 2, (*current).name);

  if(lcdReadButtons(port) == LCD_BTN_LEFT) {
    current = (*current).previous;
  } else if (lcdReadButtons(port) == LCD_BTN_RIGHT) {
    current = (*current).next;
  } else if (lcdReadButtons(port) == LCD_BTN_CENTER) {
    if((*current).select > 0) {
      current = (*current).select;
    } else if((*current).action > 0) {
      (*current).action((*current).param);
      printf("action %p\tpulseMotor %p",(*current).action, &pulseMotor);
    } else {
      printf("Nothing to do for %s\n\r", (*current).name);
    }
  }
}

/**
 * Action Functions:
 */

void pulseMotor(unsigned char port) {
  motorSet(port, 127);
  delay(500);
  motorSet(port, -127);
  delay(500);
  motorStop(port);
}

void beep() {
  speakerPlayRtttl("Beep:d=4,o=5,b=100:16f5");
  speakerPlayRtttl("Beep:d=4,o=5,b=100:16a#6");
}

void boop() {
  speakerPlayRtttl("Beep:d=4,o=5,b=100:16a#6");
  speakerPlayRtttl("Beep:d=4,o=5,b=100:16f5");
}

void runSelfTest() {
  lcdSetText(port, 2, "POST: DONT TOUCH");
  for(int i = 1; i <= 10; i++) {
    pulseMotor(i);
  }
  beep();
  beep();
}

void callAuton() {
  lcdSetText(port, 2, "AUTON-DONT TOUCH");
  autonomous();
}
