 #include <Servo.h>


// Motor Pins
#define speedPinR 9
#define RightMotorDirPin1 22
#define RightMotorDirPin2 24
#define speedPinL 10
#define LeftMotorDirPin1 26
#define LeftMotorDirPin2 28
#define speedPinRB 11
#define RightMotorDirPin1B 5
#define RightMotorDirPin2B 6
#define speedPinLB 12
#define LeftMotorDirPin1B 7
#define LeftMotorDirPin2B 8


// Ultrasonic Pins
#define Trig_PIN 30
#define Echo_PIN 31


// Servo
#define SERVO_PIN 13
Servo head;


// Speed Settings
#define SPEED 100
#define TURN_SPEED 150
#define DISTANCE_LIMIT 30
#define TURN_TIME 400


int frontDistance, leftDistance, rightDistance;


void setup() {
  // Motor Setup
  pinMode(RightMotorDirPin1, OUTPUT); pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(RightMotorDirPin1B, OUTPUT); pinMode(RightMotorDirPin2B, OUTPUT);
  pinMode(LeftMotorDirPin1B, OUTPUT);  pinMode(LeftMotorDirPin2B, OUTPUT);
  pinMode(speedPinL, OUTPUT); pinMode(speedPinR, OUTPUT);
  pinMode(speedPinLB, OUTPUT); pinMode(speedPinRB, OUTPUT);


  // Ultrasonic Setup
  pinMode(Trig_PIN, OUTPUT);
  pinMode(Echo_PIN, INPUT);
  digitalWrite(Trig_PIN, LOW);


  // Servo Setup
  head.attach(SERVO_PIN);
  head.write(90);
  delay(1000);


  // Serial & Random
  Serial.begin(9600);
  randomSeed(analogRead(A0));


  stopMotors();
}


void loop() {
  frontDistance = readDistance();


  if (frontDistance > DISTANCE_LIMIT) {
    goForward();
    setMotorSpeed(SPEED);
  } else {
    stopMotors();
    delay(200);
    scanAndAvoid();
  }


  delay(50);
}


// ---------- MOVEMENT ----------


void goForward() {
  FR_fwd(); FL_fwd(); RR_fwd(); RL_fwd();
}


void goLeft() {
  FR_fwd(); FL_bck(); RR_fwd(); RL_bck();
}


void goRight() {
  FR_bck(); FL_fwd(); RR_bck(); RL_fwd();
}


void stopMotors() {
  digitalWrite(RightMotorDirPin1, LOW); digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);  digitalWrite(LeftMotorDirPin2, LOW);
  digitalWrite(RightMotorDirPin1B, LOW); digitalWrite(RightMotorDirPin2B, LOW);
  digitalWrite(LeftMotorDirPin1B, LOW);  digitalWrite(LeftMotorDirPin2B, LOW);
  setMotorSpeed(0);
}


void setMotorSpeed(int speed) {
  analogWrite(speedPinL, speed);
  analogWrite(speedPinR, speed);
  analogWrite(speedPinLB, speed);
  analogWrite(speedPinRB, speed);
}


// ---------- WHEEL DIRECTIONS ----------


void FR_fwd()  { digitalWrite(RightMotorDirPin1, LOW);  digitalWrite(RightMotorDirPin2, HIGH); }
void FR_bck()  { digitalWrite(RightMotorDirPin1, HIGH); digitalWrite(RightMotorDirPin2, LOW); }


void FL_fwd()  { digitalWrite(LeftMotorDirPin1, LOW);   digitalWrite(LeftMotorDirPin2, HIGH); }
void FL_bck()  { digitalWrite(LeftMotorDirPin1, HIGH);  digitalWrite(LeftMotorDirPin2, LOW); }


void RR_fwd()  { digitalWrite(RightMotorDirPin1B, LOW); digitalWrite(RightMotorDirPin2B, HIGH); }
void RR_bck()  { digitalWrite(RightMotorDirPin1B, HIGH);digitalWrite(RightMotorDirPin2B, LOW); }


void RL_fwd()  { digitalWrite(LeftMotorDirPin1B, LOW);  digitalWrite(LeftMotorDirPin2B, HIGH); }
void RL_bck()  { digitalWrite(LeftMotorDirPin1B, HIGH); digitalWrite(LeftMotorDirPin2B, LOW); }


// ---------- ULTRASONIC SENSOR ----------


int readDistance() {
  digitalWrite(Trig_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_PIN, LOW);


  long duration = pulseIn(Echo_PIN, HIGH, 20000);
  int distance = duration * 0.01657;
  return constrain(distance, 0, 300);  // Cap max distance
}


// ---------- SCANNING & AVOIDANCE ----------


void scanAndAvoid() {
  // Look Left
  head.write(170);
  delay(300);
  leftDistance = readDistance();
  Serial.print("Left: "); Serial.println(leftDistance);


  // Look Right
  head.write(0);
  delay(300);
  rightDistance = readDistance();
  Serial.print("Right: "); Serial.println(rightDistance);


  // Return to Center
  head.write(90);
  delay(200);


  if (leftDistance > DISTANCE_LIMIT && rightDistance <= DISTANCE_LIMIT) {
    Serial.println("Turning LEFT");
    goLeft();
  } else if (rightDistance > DISTANCE_LIMIT && leftDistance <= DISTANCE_LIMIT) {
    Serial.println("Turning RIGHT");
    goRight();
  } else if (rightDistance > DISTANCE_LIMIT && leftDistance > DISTANCE_LIMIT) {
    if (random(0, 2) == 0) {
      Serial.println("Both clear, randomly turning LEFT");
      goLeft();
    } else {
      Serial.println("Both clear, randomly turning RIGHT");
      goRight();
    }
  } else {
    Serial.println("Both sides blocked, turning RIGHT");
    goRight();  // Default fallback
  }


  setMotorSpeed(TURN_SPEED);
  delay(TURN_TIME);
  stopMotors();
  delay(200);
}
