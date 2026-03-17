/*

░█████╗░██████╗░██╗░░░██╗██████╗░███╗░░██╗
██╔══██╗██╔══██╗██║░░░██║██╔══██╗████╗░██║
███████║██████╔╝██║░░░██║██████╔╝██╔██╗██║
██╔══██║██╔═══╝░██║░░░██║██╔══██╗██║╚████║
██║░░██║██║░░░░░╚██████╔╝██║░░██║██║░╚███║
╚═╝░░╚═╝╚═╝░░░░░░╚═════╝░╚═╝░░╚═╝╚═╝░░╚══╝

*/

// This Program was written by Ved Tripathi

// APURN's purpose is to detect food spoilage before consumption, leading to less food waste and food sickness

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Libraries
#include <string.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>  // Bluetooth support

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Bluetooth module on pins 10 (RX), 11 (TX)

SoftwareSerial bluetooth(2, 3);  // RX, TX

const int MQ9_PIN = A0;
const int MQ3_PIN = A1;
const int ALERT_LED = 4;
const int CALIBRATE_BUTTON = 7;

bool spoilageDetected = false;
bool moderateDetected = false;
String detectionText = "⚠️⚠️ Spoilage Detected!⚠️⚠️";
String moderateText = "❌ Moderately spoiled, not completely safe";
char data = 0;
int scanMultiplier = 3;
int recalibrateAverageMultiplier = 10;
int warmupDelay = 10000;

int mq3Total = 0;
int mq9Total = 0;

int mq9Val = 0;
int mq3Val = 0;

int mq9LastValue = 180;
int mq3LastValue = 340;

int mq3ThresholdAddend = 90;
int mq9ThresholdAddend = 60;

int MQ9_THRESHOLD = 0;
int MQ3_THRESHOLD = 0;

// Function to blink buzzer or LED
void blinkBuzzer(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ALERT_LED, HIGH);
    delay(delayMs);
    digitalWrite(ALERT_LED, LOW);
    delay(delayMs);
  }
}

// Read sensor values and act
void readSensors() {

  spoilageDetected = false;  // RESET spoilageDetected
  moderateDetected = false;  // RESET moderateDetected


  mq9Val = analogRead(MQ9_PIN);
  mq3Val = analogRead(MQ3_PIN);

  Serial.print(" MQ-9: ");
  Serial.print(mq9Val);
  Serial.print(" | MQ-3: ");
  Serial.println(mq3Val);

  // Send sensor data via Bluetooth
  //bluetooth.print("MQ9: ");
  //bluetooth.print(mq9Val);
  //bluetooth.print(" MQ3: ");
  //bluetooth.println(mq3Val);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ9:");
  lcd.print(mq9Val);
  lcd.setCursor(0, 1);
  lcd.print("MQ3:");
  lcd.print(mq3Val);

  if ((mq3Val - mq3LastValue) > 75) {
    Serial.println("  Outlier detected 🔴🔴🔴 (ignoring this value)");
  } else {

    if (mq9Val > MQ9_THRESHOLD || mq3Val > MQ3_THRESHOLD) {
      spoilageDetected = true;
      moderateDetected = false;
    } /*else if ((mq9Val > MQ9_THRESHOLD - mq9ThresholdAddend + 30) || (mq3Val > MQ3_THRESHOLD - mq3ThresholdAddend + 30)) {
      moderateDetected = true;
      spoilageDetected = false;
    }*/ else {
      digitalWrite(ALERT_LED, LOW);
      bluetooth.println("1");  // Send clear alert
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Growth");
      lcd.setCursor(0, 1);
      lcd.println("Detected               ");
    }
    if (spoilageDetected) {
      digitalWrite(ALERT_LED, HIGH);
      Serial.println(detectionText);
      bluetooth.println("3");  // Send spoilage alert
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SPOILAGE ");
      delay(500);
      digitalWrite(ALERT_LED, LOW);
      delay(500);
      digitalWrite(ALERT_LED, HIGH);
      delay(4000);
      digitalWrite(ALERT_LED, LOW);



    } else if (moderateDetected) {
      digitalWrite(ALERT_LED, HIGH);
      Serial.println(moderateText);
      bluetooth.println("2");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Moderate");
      lcd.setCursor(0, 1);
      lcd.print("Spoilage");
      delay(4000);
      digitalWrite(ALERT_LED, LOW);
    }
  }

  mq9LastValue = mq9Val;
  mq3LastValue = mq3Val;

  delay(1000);
}

// Recalibrate sensor thresholds
void Recalibrate() {
  //bluetooth.println("CALIBRATING");  // Notify app
  mq3Total = 0;
  mq9Total = 0;

  for (int i = 0; i < recalibrateAverageMultiplier; i++) {
    mq3Total += analogRead(MQ3_PIN);
    mq9Total += analogRead(MQ9_PIN);
  }

  MQ3_THRESHOLD = (mq3Total / recalibrateAverageMultiplier) + mq3ThresholdAddend;
  MQ9_THRESHOLD = (mq9Total / recalibrateAverageMultiplier) + mq9ThresholdAddend;

  Serial.print("NEW MQ-9 Threshold: ");
  Serial.println(MQ9_THRESHOLD);
  Serial.print("NEW MQ-3 Threshold: ");
  Serial.println(MQ3_THRESHOLD);

  //bluetooth.println("CALIBRATION DONE");  // Notify app

  blinkBuzzer(3, 200);
  delay(500);
}

void setup() {
  pinMode(ALERT_LED, OUTPUT);
  digitalWrite(ALERT_LED, LOW);
  pinMode(CALIBRATE_BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
  bluetooth.begin(9600);  // Start Bluetooth

  Serial.println("Warming and calibrating sensors... (warmupDelay milliseconds)");
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  delay(warmupDelay);

  Recalibrate();
  readSensors();
  readSensors();
  readSensors();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("APURN is ready!");

  Serial.println("Apurn - Auto Spoilage Detection Ready");
  bluetooth.println("READY");
}

void loop() {
  
  if (bluetooth.available() > 0) {
    data = bluetooth.read();
  }
  
  if ((digitalRead(CALIBRATE_BUTTON) == LOW) || (data == 'A')) {
    Serial.println("---------------------------------------------------------------------------------------------------------------");
    Serial.println("Manual recalibration triggered.");
    delay(200);
    Serial.println("Recalibrating in 8 seconds. Please move the device at least a foot away from the produce item.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recalibrating...");
    delay(3000);
    lcd.clear();
    lcd.print("Move device");
    lcd.setCursor(0, 1);
    lcd.println("away            ");
    delay(8000);
    Recalibrate();

    Serial.println("Scanning in 5 seconds. Please move the device close to the surface of the produce.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Move device");
    lcd.setCursor(0, 1);
    lcd.println("to produce             ");
    delay(5000);

    for (int i = 0; i < scanMultiplier; i++) {
      readSensors();
      if (spoilageDetected || moderateDetected) {
        delay(500);
        digitalWrite(ALERT_LED, LOW);
        break;
        
      }
      if ((digitalRead(CALIBRATE_BUTTON) == LOW) || (data == "95")) {
        Serial.println("Manual recalibration button detected mid-scan.");
        break;
      }
    }


    data = 0;  // Reset after handling command

  } else {
    // Optional: Keep scanning in idle if needed
  }
}
