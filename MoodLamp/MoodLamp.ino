/* 
 * Pin numbers for different colors
 */
int RED = 10;
int GREEN = 11;
int BLUE = 9;
int WHITE = 6;

/*
 * Pin numbers for buttons
 */
int PAUSEBUTTON = 2;
int NEXTBUTTON = 4;

/*
 * Color values
 */

int colors[][4] = {
  {255,   0,    0,    0}, // Red
  {255, 70,    0,    0},  // Orange
  {255, 255,    0,    0}, // Yellow
  {0,   255,    0,    0}, // Green
  {0,   255,  255,    0}, // Cyan
  {0,     0,  255,    0}, // Blue
  {255,   0,  150,    0}, // Pink
};

int colorCount = 7;

int currentColor = 0;
int nextColor = 0;

int mod[3];
int power[4];
int out[] = {RED, GREEN, BLUE, WHITE};

int lastReadPause = LOW;
int lastReadNext = LOW;
int mode = 4;
int curSpeed = 1;
int numModes = 5;
int numSpeeds = 6;
double lowRand = 0.3;
double highRand = 1;

boolean skip = false;

double lastPushTime = 0.0;

void setup() {
  randomSeed(analogRead(0));
  currentColor = random(0, colorCount - 1);
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

long lastChangeTime = -2000;
int index = 0;
const int SECONDS = 1000;

long u = 5000; // Constant color time
long v = 2500; // Interpolation time

boolean isConstant = true;
int newColor = 0;

void loop() {
  if (isConstant) {
    // Write current color
    for (int i = 0; i < 4; i++) {
      analogWrite(out[i], colors[currentColor][i]);
    }
    
    if (millis() - lastChangeTime >= u) {
        lastChangeTime = millis();
        isConstant = false;
        
        do {
          newColor = random(0, colorCount - 1);
        } while (newColor == currentColor);
    }
  } else { // interpolating two colors
    float delta = (float)(millis() - lastChangeTime) / (float)v;
    
    if (delta >= 1.0f) {
      isConstant = true;
      currentColor = newColor;
      lastChangeTime = millis();
      delta = 1.0f;
    }
    
    for (int i = 0; i < 4; i++) {
      int c = ((1.0f - delta) * (float)colors[currentColor][i]) + ((delta) * (float)colors[newColor][i]);
      analogWrite(out[i], c);
    }
  }
}

/*
Reads input from the serial monitor (Ctrl-Shift-M) and changes the color accordingly.
Multiple colors on one line not permitted
Usage:
r0
g128
b255
w100
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
}
*/

/*
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
*/
