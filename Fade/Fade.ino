// Pin assignments for each LED color
// Blue -> 9, Red -> 10, Green -> 11, Yellow -> 6 (all PWM-capable pins)
const int ledPins[] = {9, 10, 11, 6}; 
const int numLeds = 4;  // Number of LEDs

// Track current brightness of each LED (0–255)
int brightness[] = {0, 0, 0, 0};

// Amount to change brightness each step (positive to fade in, negative to fade out)
int fadeAmount[] = {5, 5, 5, 5};

// Track the last time each LED was updated
unsigned long previousMillis[] = {0, 0, 0, 0};

// Time intervals (in milliseconds) between brightness updates for each LED
// This gives each LED a different fade speed
unsigned int fadeIntervals[] = {30, 50, 70, 90};

// Current LED index to fade (we start with the first LED: Blue)
int currentLedIndex = 0;

void setup() {
  // Set each LED pin as an OUTPUT
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);  // All LEDs are PWM-capable
  }
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time in milliseconds

  // Only update the current LED if its interval has passed
  if (currentMillis - previousMillis[currentLedIndex] >= fadeIntervals[currentLedIndex]) {
    previousMillis[currentLedIndex] = currentMillis;  // Update the last time this LED was updated

    // Fade in the current LED by increasing its brightness
    brightness[currentLedIndex] += fadeAmount[currentLedIndex]; 

    // If brightness reaches either extreme (0 or 255), reverse the fade direction
    if (brightness[currentLedIndex] >= 255) {
      brightness[currentLedIndex] = 255;  // Ensure it doesn't go over 255
      fadeAmount[currentLedIndex] = 0;  // Stop fading once it's fully bright
    }
    
    // Apply the new brightness using PWM (for Blue, Red, Green, Yellow)
    analogWrite(ledPins[currentLedIndex], brightness[currentLedIndex]);

    // If the current LED is fully bright (255), move to the next LED
    if (brightness[currentLedIndex] == 255) {
      // Move to the next LED in the sequence (Blue -> Red -> Green -> Yellow)
      currentLedIndex++;
      
      // If we reach the last LED, stop the fading (or reset to the first LED if desired)
      if (currentLedIndex >= numLeds) {
        currentLedIndex = numLeds;  // Stop at the last LED
      }
    }
  }
}

