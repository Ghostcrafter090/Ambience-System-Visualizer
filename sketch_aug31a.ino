#include <SoftPWM.h>

int latchPin = 4;
int clockPin = 3;
int dataPin = 2;
int lightning = 11;

float f;

unsigned long l;
unsigned long t;
unsigned long tic;
unsigned long ticn;
unsigned long wait = 1;

int counter = 0;
int i = 0;
int vol;
int pin;
int row;
int updateScreen = 0;

byte data = 0;

int fPin[3] = {5, 6, 9};
int values[8][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

int genRand(int low, int high) {
  int n = random(low, high);
  return n;
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(fPin[0], OUTPUT);
  pinMode(fPin[1], OUTPUT);
  pinMode(fPin[2], OUTPUT);
  Serial.begin(2000000);
  // SoftPWMBegin();
  // SoftPWMSet(lightning, 0);
}

void Math() {
  vol = analogRead(0);
  Serial.println(vol);
  vol = (vol * 10) / 30;
  if (vol > 255) {
    vol = 255;
  }
  pin = genRand(0, 3);
  row = genRand(1, 8);
  if (vol < 0) {
    vol = 0;
  }
  if (values[row][pin] > 255) {
    values[row][pin] = 0;
  }
  if (((row == 1 or row == 3 or row == 5 or row == 7) and (pin == 0 or pin == 2)) or ((row == 2 or row == 4 or row == 6) and (pin == 1 or pin == 3))) {
    t = ((values[row][pin] * 6) + (vol)) / 7;
    if (t > values[row][pin]) {
      values[row][pin] = t;
    }
    if (l < millis()) {
      l = millis() + 2000;
      values[row][pin] = ((values[row][pin] * 6) + (vol)) / 7;
    }
  } else {
    values[row][pin] = ((values[row][pin] * 4) + (vol)) / 5;
  }
  if (vol < 0) {
    vol = 0;
  }
}

void mathNoFastLoop() {
  vol = (analogRead(1)) - (vol * vol);
  if (vol < 105) {
    vol = 0;
  }
  vol = vol * 2;
  if (vol > 255) {
    vol = 255;
  }
  pin = genRand(0, 3);
  row = genRand(1, 8);
  //  if (vol < 20) {
  //    vol = 0;
  //  }
  if (values[row][pin] > 255) {
    values[row][pin] = 0;
  }
  analogWrite(lightning, (vol));
}

void loop() {
  for (int j = 0; j < 8; j++) {
    digitalWrite(latchPin, 0);
    lightShiftPinA(7 - j);
    lightShiftPinA(j);
    counter = counter + 1;
    analogWrite(fPin[0], (values[j][0]));
    analogWrite(fPin[1], (values[j][1]));
    analogWrite(fPin[2], (values[j][2]));
    digitalWrite(latchPin, 1);
  }
  for (int j = 0; j < 8; j++) {
    mathNoFastLoop();
    digitalWrite(latchPin, 0);
    lightShiftPinB(j);
    lightShiftPinB(7 - j);
    counter = counter - 1;
    analogWrite(fPin[0], (values[j][0]));
    analogWrite(fPin[1], (values[j][1]));
    analogWrite(fPin[2], (values[j][2]));
    digitalWrite(latchPin, 1);
  }
}

void lightShiftPinA(int p) {
  // updateScreen = 0;
  // while (updateScreen < wait) {
  // updateScreen = updateScreen + 1;
  Math();
  // }
  ticn = tic;
  tic = micros();
  if ((tic - ticn) < 400) {
    wait = wait + 1;
  } else {
    wait = wait - 1;
  }
  if (wait < 1) {
    wait = 1;
  }
  int pin;
  pin = 1 << p;
  shiftOut(dataPin, clockPin, pin);
}

void lightShiftPinB(int p) {
  // updateScreen = 0;
  // while (updateScreen < wait) {
  // updateScreen = updateScreen + 1;
  Math();
  // }
  ticn = tic;
  tic = micros();
  if ((tic - ticn) < 400) {
    wait = wait + 1;
  } else {
    wait = wait - 1;
  }
  if (wait < 1) {
    wait = 1;
  }
  int pin;
  pin = 1;
  for (int x = 0; x < p; x++) {
    pin = pin * 2;
  }
  shiftOut(dataPin, clockPin, pin);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
  }
}
