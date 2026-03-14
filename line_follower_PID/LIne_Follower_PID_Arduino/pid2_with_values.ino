#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// ===== PID CONSTANTS =====
float Kp = 0.04;
float Ki = 0.0000;
float Kd = 0.20;

// ===== PID VARIABLES =====
float filteredError = 0;
float lastError = 0;
float integral = 0;

// LEDs
const int leftLED = 10;
const int rightLED = 11;

// Smoothed LED outputs
float smoothLeft = 0;
float smoothRight = 0;

// Print timer
unsigned long lastPrint = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){2,3,4,5,6,7,8,9}, SensorCount);
  // LEDON tied to 5V

  delay(500);

  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  Serial.println("Done.");
}

void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);

  // ===== ERROR =====
  int rawError = position - 3500;

  // ===== LOW PASS FILTER =====
  filteredError = 0.7 * filteredError + 0.3 * rawError;

  // ===== INTEGRAL =====
  integral += filteredError;
  integral = constrain(integral, -10000, 10000);

  // ===== DERIVATIVE =====
  float derivative = filteredError - lastError;

  // ===== PID =====
  float correction = Kp * filteredError +
                     Ki * integral +
                     Kd * derivative;

  lastError = filteredError;

  int baseBrightness = 100;

  float leftBrightness  = baseBrightness + correction;
  float rightBrightness = baseBrightness - correction;

  leftBrightness  = constrain(leftBrightness, 0, 255);
  rightBrightness = constrain(rightBrightness, 0, 255);

  // ===== OUTPUT SMOOTHING =====
  smoothLeft  = 0.8 * smoothLeft  + 0.2 * leftBrightness;
  smoothRight = 0.8 * smoothRight + 0.2 * rightBrightness;

  analogWrite(leftLED, smoothLeft);
  analogWrite(rightLED, smoothRight);

  // ===== PRINT EVERY 50ms =====
  if (millis() - lastPrint > 50)
  {
    lastPrint = millis();

    Serial.print("Sensors: ");
    for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print("\t");
    }

    Serial.print(" | Pos: ");
    Serial.print(position);

    Serial.print(" | Err: ");
    Serial.print(filteredError);

    Serial.print(" | L: ");
    Serial.print((int)smoothLeft);

    Serial.print(" | R: ");
    Serial.println((int)smoothRight);
  }
}
