#include <Servo.h>

// Arduino pin assignment
#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

// configurable parameters for sonar
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

Servo myServo;
unsigned long MOVING_TIME = 1000; // moving time is 1 seconds
unsigned long moveStartTime;
int startAngle = 0; 
int stopAngle  = 90;
int targetAngle = 0;
bool wasDetected = false;

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  myServo.attach(PIN_SERVO);
  moveStartTime = millis(); // start moving

  myServo.write(startAngle); // Set position
  delay(500);
}

void loop() {
  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  bool detect = (dist_raw > 0 && dist_raw <= 200.0);
  
  if (detect != wasDetected) {
    startAngle = myServo.read();
    targetAngle = detect ? 90 : 0;
    moveStartTime = millis();
    wasDetected = detect;
  }

  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    float t = sigmoid(progress);
    int angle = startAngle + (targetAngle - startAngle) * t;
    myServo.write(angle);
  } else {
    myServo.write(targetAngle);
  }

  delay(INTERVAL);
}

float sigmoid(float x){
  x = x / 1000.0 * 12.0 - 6.0;
  return 1.0 / (1.0 + exp(-x));
}

/*float sinF(float x){
  x = x / 1000.0;
  float s = sin(PI * x / 2.0);
  return s * s;
}*/

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
