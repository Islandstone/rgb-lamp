int RED = 10;
int GREEN = 11;
int BLUE = 9;
int PAUSEBUTTON = 2;
int NEXTBUTTON = 4;
int mod[3];
int power[3];
int out[] = {
  RED, GREEN, BLUE};
int lastReadPause = LOW;
int lastReadNext = LOW;
int mode = 0;

boolean skip = false;

double offset = 0.0;
double lastPushTime = 0.0;

void setup() {

  for (int i = 0; i < 3; i++) {
    mod[i] = i+1;
    power[i] = 0;
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(PAUSEBUTTON, INPUT);
  pinMode(NEXTBUTTON, INPUT);
}

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


