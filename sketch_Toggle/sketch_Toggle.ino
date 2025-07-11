const int ledPin = 13;     // GPIO2 connected to LED
const int switchPin = 34; // GPIO34 connected to push-button (with external pull-up)

bool wasPressed = false;   // Tracks if the button was previously pressed
bool ledState = false;     // Tracks current LED state

void setup() {
  pinMode(ledPin, OUTPUT);       // Set LED pin as output
  pinMode(switchPin, INPUT);     // Set switch pin as input
  digitalWrite(ledPin, LOW);     // Start with LED OFF
}

void loop() {
  int switchState = digitalRead(switchPin); // Read the state of the switch

  if (switchState == LOW) {
    // Button is being pressed
    wasPressed = true;
  }

  if (switchState == HIGH && wasPressed) {
    // Button was just released after being pressed
    ledState = !ledState;                     // Toggle LED state
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    wasPressed = false;                       // Reset for next press
  }

  // Small delay for debouncing (optional, depends on switch)
  delay(10);
}


