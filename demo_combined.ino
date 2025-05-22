#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "OneButton.h"
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    26
#define LED_COUNT  4

#define fanPin1 19
#define fanPin2 18
#define ledPin 12  // New LED pin

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C mylcd(0x27, 16, 2);

OneButton button1(16, true);
OneButton button2(27, true);

Servo myservo;
int servoPin = 13;

String password = "";
String correct_p = "-.-";  // Set your password here

void setup() {
  Serial.begin(115200);
  mylcd.init();
  mylcd.backlight();

  button1.attachClick(click1);
  button1.attachLongPressStop(longPressStop1);
  button2.attachClick(click2);
  button2.attachLongPressStop(longPressStop2);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 1000, 2000);

  strip.begin();
  strip.show();
  strip.setBrightness(50);

  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);  // Set LED pin as output

  mylcd.setCursor(0, 0);
  mylcd.print("Enter password");
}

void loop() {
  button1.tick();
  button2.tick();
  delay(10);
}

void click1() {
  Serial.print(".");
  password += '.';
  mylcd.setCursor(0, 1);
  mylcd.print(password);
}

void longPressStop1() {
  Serial.print("-");
  password += '-';
  mylcd.setCursor(0, 1);
  mylcd.print(password);
}

void click2() {
  Serial.println(password);
  if (password == correct_p) {
    myservo.write(180);  // Open the door
    mylcd.clear();
    mylcd.setCursor(0, 0);
    mylcd.print("Tinkacode open");
    lightRGBSuccess();   // Turn on RGB
    turnOnFans();        // Turn on fans
    digitalWrite(ledPin, HIGH);  // Turn on LED
  } else {
    mylcd.clear();
    mylcd.setCursor(0, 0);
    mylcd.print("Tinkacode error");
    delay(2000);
    mylcd.clear();
    mylcd.setCursor(0, 0);
    mylcd.print("input again");
    strip.clear();
    strip.show();  // Turn off LEDs
  }
  password = "";
}

void longPressStop2() {
  myservo.write(0);  // Close the door
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print("Tinkacode close");

  // Turn off RGB
  strip.clear();
  strip.show();

  // Turn off fans and LED
  digitalWrite(fanPin1, LOW);
  digitalWrite(fanPin2, LOW);
  digitalWrite(ledPin, LOW);  // Turn off LED
}

void lightRGBSuccess() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));  // Green
  }
  strip.show();
}

void turnOnFans() {
  digitalWrite(fanPin1, LOW);        // PWM = 0
  analogWrite(fanPin2, 70);
  delay(3000);

  digitalWrite(fanPin1, LOW);
  digitalWrite(fanPin2, LOW);
  delay(1000);

  digitalWrite(fanPin1, HIGH);       // PWM = 255
  analogWrite(fanPin2, 100);
  delay(3000);

  digitalWrite(fanPin1, LOW);
  digitalWrite(fanPin2, LOW);
  delay(1000);
}
