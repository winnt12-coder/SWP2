/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/faq/how-to-control-speed-of-servo-motor
 */

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
unsigned long MOVING_TIME = 1000; // moving time is 3 seconds
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

  // 0이 아니면서 50mm 이내일 때만 감지
  bool detected = (dist_raw > 0 && dist_raw <= 200.0);

  // 감지 상태가 바뀌었을 때만 목표 각도 변경
  if (detected != wasDetected) {
    startAngle = myServo.read();
    targetAngle = detected ? 90 : 0;
    moveStartTime = millis();
    wasDetected = detected;
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

//float sin(){
  
//}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
