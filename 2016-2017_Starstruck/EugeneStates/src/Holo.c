#include <main.h>

#define DISABLED 0
#define HOLO_X4 1
#define HOLO_X6 2
#define HOLO_H5 3
#define HOLO_H6 4
#define HOLO_H56 5

int currentConfig = 0;
unsigned char W_FL, // Front left wheel port
              W_FR, // Front right wheel port
              W_BL, // Back left wheel port
              W_BR, // Back right wheel port
              W_CLHF, // Center left / Horizontal / Horizontal front wheel port
              W_CRHB; // Center right / Horizontal back wheel port



void holoSetUniversal(int FL, int BL, int FR, int BR, int CLHF, int CRHB);


void holoInitX4(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR) {
  W_FL = FL;
  W_BL = BL;
  W_FR = FR;
  W_BR = BR;
  W_CLHF = DISABLED;
  W_CRHB = DISABLED;
  currentConfig = HOLO_X4;
}

void holoInitX6(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char CL, unsigned char CR) {
  W_FL = FL;
  W_BL = BL;
  W_FR = FR;
  W_BR = BR;
  W_CLHF = CL;
  W_CRHB = CR;
  currentConfig = HOLO_X6;
}

void holoInitH5(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char H) {
  W_FL = FL;
  W_BL = BL;
  W_FR = FR;
  W_BR = BR;
  W_CLHF = H;
  W_CRHB = DISABLED;
  currentConfig = HOLO_H5;
}

void holoInitH6(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char HF, unsigned char HB) {
  W_FL = FL;
  W_BL = BL;
  W_FR = FR;
  W_BR = BR;
  W_CLHF = HF;
  W_CRHB = HB;
  currentConfig = HOLO_H6;
}

void holoInitH56(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char HF, unsigned char HB) {
  W_FL = FL;
  W_BL = BL;
  W_FR = FR;
  W_BR = BR;
  W_CLHF = HF;
  W_CRHB = HB;
  currentConfig = HOLO_H56;
}

void holoStop() {
  W_FL = DISABLED;
  W_BL = DISABLED;
  W_FR = DISABLED;
  W_BR = DISABLED;
  W_CLHF = DISABLED;
  W_CRHB = DISABLED;
  currentConfig = DISABLED;
}

bool isHoloInitialized() {
  return currentConfig > 0;
}


void holoSet(int xAxis, int yAxis, int rot) {
  switch (currentConfig) {
    case HOLO_X4:
      //FL BL FR BR TODO: What the daft is this? (Bot might be wired wrong)
      holoSetUniversal(xAxis + yAxis + rot, xAxis + -yAxis + -rot, xAxis + -yAxis + rot, -xAxis + -yAxis + rot, DISABLED, DISABLED);
      break;
    case HOLO_X6:
      holoSetUniversal(-xAxis + -yAxis + -rot, xAxis + -yAxis + -rot, -xAxis + yAxis + -rot, xAxis + yAxis + -rot, -yAxis + -rot, yAxis + -rot);
      break;
    case HOLO_H5:
      holoSetUniversal(-yAxis + -rot, -yAxis + -rot, yAxis + -rot, yAxis + -rot, -xAxis, DISABLED);
      break;
    case HOLO_H6:
      holoSetUniversal(-yAxis + -rot, -yAxis + -rot, yAxis + -rot, yAxis + -rot, -xAxis + -rot, xAxis + -rot);
      break;
    case HOLO_H56:
      holoSetUniversal(-yAxis + -rot, -yAxis + -rot, yAxis + -rot, yAxis + -rot, xAxis, -xAxis);
      break;
    case DISABLED:
    default:
      printf("ERROR: Running holoSet before holo module is initialized");
      break;
  }
}

void holoSetUniversal(int FL, int BL, int FR, int BR, int CLHF, int CRHB) {
  motorSet(W_FL, FL);
  motorSet(W_BL, BL);
  motorSet(W_FR, FR);
  motorSet(W_BR, BR);
  motorSet(W_CLHF, CLHF);
  motorSet(W_CRHB, CRHB);
}
