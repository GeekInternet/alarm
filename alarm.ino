/*
 * Alarm
 * Kjetil Svalestuen
 */

// Initialize inputs
const int switch1 = 2;
const int switch2 = 3;
const int switch3 = 4;
const int sensor = 8;

// Initialize outputs
const int speaker = 7;
const int red = 12;
const int green = 13;

// Other variables
const int noteC = 523;
const int noteE = 784;

int current = 0;
int pressed = 0;
boolean locked = true;
int code[4] = {1, 3, 2, 3};

long timer = 0;
int timerDelay = 1000;
int initialLight = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(switch3, INPUT);
  
  pinMode(speaker, OUTPUT);
  pinMode(sensor, OUTPUT);
  
  // Find the light level of the room.
  initialLight = analogRead(A0);
  for (int i = 0; i < 10; i++) {
    initialLight = ((initialLight + analogRead(A0)) / 2);
    tone(speaker, noteE, 500);
  }
  
  // Start scanning for movement.
  int light;
  do {
    delay(1000);
    digitalWrite(red, HIGH);
    light = analogRead(A0);
  } while (light > (initialLight * 0.9) && light < (initialLight * 1.1));
  digitalWrite(red, HIGH);
}

void loop() {
  pressed = getPressed();
  
  if (locked)
    beep();
  
  if (pressed != 0) {
    locked = true;
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    tone(speaker, noteC, 50);
    
    // Serial monitor
    printToSerial(pressed, current, code[current]);
    
    // Checks if the next digit of the key code matches the pressed button.
    if (pressed == code[current])
      current++;
    else if (pressed == 1)
      current = 1;
    else current = 0;
    
    // If at end of code, unlock
    if (current == (sizeof(code) / sizeof(int))) {
      waitUntilReleased(pressed);
      unlock();
      current = 0;
    }
   
    waitUntilReleased(pressed);
  }
  
  delay(1);
}

/* Returns a pressed key or zero if there is none. */
int getPressed() {
  if (digitalRead(switch1) == HIGH)      return 1;
  else if (digitalRead(switch2) == HIGH) return 2;
  else if (digitalRead(switch3) == HIGH) return 3;
  else return 0;
}

/* Delays the program flow until the button is released. */
void waitUntilReleased(int pressed) {
  int switchInput = 0;
  switch (pressed) {
  	case 1: switchInput = switch1; break;
  	case 2: switchInput = switch2; break;
  	case 3: switchInput = switch3; break;
  }

  while (digitalRead(switchInput) == HIGH) {
  	delay(20);
        beep();
  }
}

/* Make a sound and switch LEDs */
void unlock() {
  locked = false;
  tone(speaker, noteE, 400);
  
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
}

/* Make a sound every second */
void beep() {
  Serial.print(millis());
  Serial.print("\t");
  Serial.println(timer);
  if (millis() > (timer + timerDelay)) {
    tone(speaker, noteC, 50);
    timer = millis();
    timerDelay -= 20;
  } 
}

void printToSerial(int pressed, int current, int code) {
  Serial.print(pressed);
  Serial.print(" - Iteration ");
  Serial.print(current);
  Serial.print(", match code ");
  Serial.println(code);
}
