int RED = 10;
int GREEN = 11;
int BLUE = 9;
int WHITE = 6;
int PAUSEBUTTON = 2;
int NEXTBUTTON = 4;
int mod[3];
int power[4];
int out[] = {RED, GREEN, BLUE, WHITE};
int lastReadPause = LOW;
int lastReadNext = LOW;
int mode = 0;

boolean skip = false;

double offset = 0.0;
double lastPushTime = 0.0;

void setup() {

  for (int i = 0; i < 4; i++) {
    power[i] = 0;
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(PAUSEBUTTON, INPUT);
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
  if (val < min)  return min;
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

/*
void loop() {
  analogWrite(RED, 255);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 255);
  analogWrite(WHITE, 255);
}
*/

/*
void loop() {
  analogWrite(RED, 255);
  delay(1000);
  analogWrite(RED, 0);
  analogWrite(GREEN, 255);
  delay(1000);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 255);
  delay(1000);
  analogWrite(BLUE, 0);
  analogWrite(WHITE, 255);
  delay(1000);
  analogWrite(WHITE, 0);
}
*/

/*
void loop() {
  int valPause = digitalRead(PAUSEBUTTON);
  int valNextMode = digitalRead(NEXTBUTTON);
  double time = millis() / 1000.0;

  if (valPause == LOW && lastReadPause == HIGH) {
    skip = !skip;

    if (skip) {
      lastPushTime = time;
    } 
    else {
      offset = time - lastPushTime;
    }
  }

  if (valNextMode == LOW && lastReadNext== HIGH) {
    mode++;
    mode%=2;
  }

  lastReadPause = valPause;
  lastReadNext = valNextMode;

  if (skip) {
    return;
  }  

  double red = 0.5 * (sin(time - offset) + 1);
  
  if (mode == 0) {
    analogWrite(RED, red * 255);
    analogWrite(BLUE, 0);    
    analogWrite(GREEN, 0);
  } 
  else {
    for (int i = 0; i < 3; i++) {
      if (power[i] < 0 || 255 < power[i]) {
        mod[i] *= -1;  
      }
      power[i] += mod[i];

      analogWrite(out[i], power[i]);
    }
  }

  delay(2);
}
*/

