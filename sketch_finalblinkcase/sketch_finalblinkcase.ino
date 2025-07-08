const int led1Pin = 4;   // LED1 blinks at 10Hz
const int led2Pin = 2;   // LED2 blinks at 2Hz

const int sw34 = 34;     // Start LED1 (on press)
const int sw35 = 35;     // Stop LED1 (on short press), Stop LED2 (on long press)
const int sw36 = 36;     // Start LED2 (on release)

bool led1Active = false;
bool led2Active = false;

bool led1State = LOW;
bool led2State = LOW;

unsigned long lastBlink1 = 0;
unsigned long lastBlink2 = 0;

unsigned long sw35PressStart = 0;
bool sw35LongPressed = false;

bool prevSw34 = HIGH;
bool prevSw35 = HIGH;
bool prevSw36 = LOW;

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  pinMode(sw34, INPUT);  // With external pull-up
  pinMode(sw35, INPUT);
  pinMode(sw36, INPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
}

void loop() {
  unsigned long now = millis();

  // Read switch states
  bool currSw34 = digitalRead(sw34);
  bool currSw35 = digitalRead(sw35);
  bool currSw36 = digitalRead(sw36);

  // --- Start LED1 on SW34 press ---
  if (prevSw34 == HIGH && currSw34 == LOW) {
    if (!led2Active) {  // Only turn on LED1 if LED2 is OFF
      led1Active = true;
      led2Active = false;
      led2State = LOW;
      digitalWrite(led2Pin, LOW);
    }
  }

  // --- Stop LED1 on SW35 short press ---
  if (prevSw35 == HIGH && currSw35 == LOW && !sw35LongPressed) {
    led1Active = false;
    led1State = LOW;
    digitalWrite(led1Pin, LOW);
  }

  // --- Start LED2 on SW36 release ---
  if (prevSw36 == LOW && currSw36 == HIGH) {
    if (!led1Active) {  // Only turn on LED2 if LED1 is OFF
      led2Active = true;
      led1Active = false;
      led1State = LOW;
      digitalWrite(led1Pin, LOW);
    }
  }

  // --- Stop LED2 on SW35 long press ---
  if (currSw35 == LOW) {
    if (sw35PressStart == 0) {
      sw35PressStart = now;
    } else if ((now - sw35PressStart >= 2000) && !sw35LongPressed) {
      led2Active = false;
      led2State = LOW;
      digitalWrite(led2Pin, LOW);
      sw35LongPressed = true;
    }
  } else {
    sw35PressStart = 0;
    sw35LongPressed = false;
  }

  // --- Blink LED1 at 10Hz (50ms ON + 50ms OFF) ---
  if (led1Active && now - lastBlink1 >= 50) {
    lastBlink1 = now;
    led1State = !led1State;
    digitalWrite(led1Pin, led1State);
  }

  // --- Blink LED2 at 2Hz (250ms ON + 250ms OFF) ---
  if (led2Active && now - lastBlink2 >= 250) {
    lastBlink2 = now;
    led2State = !led2State;
    digitalWrite(led2Pin, led2State);
  }

  // Save previous states for edge detection
  prevSw34 = currSw34;
  prevSw35 = currSw35;
  prevSw36 = currSw36;

  delay(1);
}
