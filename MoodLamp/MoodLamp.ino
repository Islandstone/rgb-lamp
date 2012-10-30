int RED = 10;
int GREEN = 11;
int BLUE = 9;
int WHITE = 6;
int SPEEDBUTTON = 2;
int NEXTBUTTON = 4;
double mod[4];
double power[4];
int speeds[] = {0, 1, 2, 4, 8, 16};
int out[] = {RED, GREEN, BLUE, WHITE};
int outHigh[] = {255, 255, 255, 100};
int outLow[] = {15, 15, 15, 15};

int lastReadSpeed = LOW;
int lastReadNext = LOW;
int mode = 4;
int curSpeed = 1;
int numModes = 5;
int numSpeeds = 6;
double lowRand = 0.3;
double highRand = 1;

boolean skip = false;

double lastPushTime = 0.0;

double getRandom() {
  return lowRand + random(0,1)*(highRand-lowRand);
}

void setup() {
  randomSeed(analogRead(1));
  for (int i = 0; i < 4; i++) {
    power[i] = 15;
    mod[i] = getRandom();
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(SPEEDBUTTON, INPUT);
  pinMode(NEXTBUTTON, INPUT);
  
  Serial.begin(9600);
}

int getColorForChar(char c) {
  switch(c) {
    case 'r': 
      return 0;
    case 'g':
      return 1;
    case 'b':
      return 2;
    case 'w':
      return 3;
    default:
      return -1;
  }
}

int clamp(int val, int min, int max) {
  if (val < min) return min;
  if (val > max) return max;  
  return val;
}

/*
Reads input from the serial monitor (Ctrl-Shift-M) and changes the color accordingly.
Multiple colors on one line not permitted
Usage:
r0
g128
b255
w 100
*/
/*
void loop() {
  // read 2 bytes at a time
  if (Serial.available() == 2) {
    // read the incoming byte:
    char color = Serial.read();
    int value = clamp(Serial.parseInt(), 0, 255);
    
    Serial.print("Color: "); Serial.println(color);
    Serial.print("Value: "); Serial.println(value);
    
    int c = getColorForChar(color);
    if (c < 0) return;
    power[c] = value;
  }
   
  for (int i = 0; i < 4; i++) {
      analogWrite(out[i], power[i]);
  }
}*/


void loop() {
  int valSpeed = digitalRead(SPEEDBUTTON);
  int valNextMode = digitalRead(NEXTBUTTON);
  double time = millis() / 1000.0;

  if (valSpeed == LOW && lastReadSpeed == HIGH) {
    curSpeed += 1;
    curSpeed %= numSpeeds;
  }

  if (valNextMode == LOW && lastReadNext== HIGH) {
    mode++;
    mode%=numModes;
  }

  lastReadSpeed = valSpeed;
  lastReadNext = valNextMode;

  if (skip) {
    return;
  }  


  for (int i = 0; i < 4; i++) {
    if (mode != 4 && mode != i) {
      analogWrite(out[i], 0);
      continue;
    }
    if (random(0,100) == 0) {
      mod[i] = -mod[i];
    }
    if (power[i] < outLow[i]) {
      power[i] = outLow[i];
      mod[i] = getRandom();
    }
    if (outHigh[i] < power[i]) {
      power[i] = outHigh[i];
      mod[i] = -getRandom();
    }
    Serial.println(power[i]);
    power[i] += mod[i] * speeds[curSpeed];

    analogWrite(out[i], power[i]);
  }
  delay(1);

}


