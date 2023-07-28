#include <DabbleESP32.h>

// Right Motor
int ena = 14;
int rm1 = 27;
int rm2 = 26;


// Left Motor
int enb = 32;
int lm1 = 25;
int lm2 = 19;


// PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

int totalLeft = 0;
int totalRight = 0;

S
void setup() {
  Dabble.begin("PPG");
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(ena, OUTPUT);

 
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(enb, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);

  ledcAttachPin(ena, pwmChannel);
  ledcAttachPin(enb, pwmChannel2);

}

void loop() {
  Dabble.processInput();
  
  ppg();
  
  ledcWrite(pwmChannel, totalLeft);
  ledcWrite(pwmChannel2, totalRight);
}

void ppg() {
  float xval = GamePad.getXaxisData();
  float yval = GamePad.getYaxisData();

  
  if (xval < -6) {
    xval = -6;
  } else if (xval > 6) {
    xval = 6;
  }
  if (yval < -6) {
    yval = -6;
  } else if (yval > 6) {
    yval = 6;
  }

  int ycon = 0;
  if (yval < 0 )
  {
    backward();
    ycon = 250 - ((yval + 6) * (250.0 / 6.0));
  }
  else
  {
    forward();
    ycon = yval * (250.0 / 6.0);
  }

  int xcon;
  if (xval < 0 ){
    xcon = 250 - (xval + 6) * (250.0 / 6.0);
  }
  else {
    xcon = xval * (250.0 / 6.0);
  }

  if (xval < 0 ) {
    totalLeft = ycon + xcon;
    if (totalLeft > 250) {
      totalLeft = 250;
    }

    totalRight = ycon - xcon;
    if (totalRight < 0) {
      totalRight = 0;
    }

  } else {
    totalLeft = ycon - xcon;
    if (totalLeft < 0) {
      totalLeft = 0;
    }

    totalRight = ycon + xcon;
    if (totalRight > 250) {
      totalRight = 250;
    }
  }
}

void forward() {
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
}

void backward() {
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
}