#include <Keypad.h>
#include <Servo.h>

#define SERVO_PIN 10  // Servo motor connected to pin 10
Servo gateServo;

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};  
byte colPins[COLS] = {6, 7, 8, 9};  
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void openGateForDuration(int minutes) {
  gateServo.write(90); // Open gate
  Serial.print("Gate Open for ");
  Serial.print(minutes);
  Serial.println(" minutes");

  unsigned long endTime = millis() + (minutes * 60000); // Convert minutes to milliseconds

  while (millis() < endTime) {
    char key = keypad.getKey();
    if (key == 'A') {
      Serial.println("Emergency Close Activated!");
      break;  // Exit loop and close gate immediately
    }
  }

  gateServo.write(0); // Close gate
  Serial.println("Gate Closed");
}

void setup() {
  Serial.begin(9600);
  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Ensure the gate starts closed
}

void loop() {
  char key = keypad.getKey();
  
  if (key >= '1' && key <= '9') {
    int minutes = key - '0';  // Convert char to int (e.g., '3' -> 3)
    openGateForDuration(minutes);
  }
  
  if (key == 'A') {
    Serial.println("Emergency Close Triggered");
    gateServo.write(0); // Immediately close gate
  }
}
