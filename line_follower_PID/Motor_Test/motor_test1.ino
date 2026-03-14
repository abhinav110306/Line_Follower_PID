#define PWMA 25
#define AIN1 26
#define AIN2 27

#define PWMB 14
#define BIN1 12
#define BIN2 13

#define STBY 33

int pwmFreq = 1000;
int pwmResolution = 8;

int pwmChannelA = 0;
int pwmChannelB = 1;

void setup() {

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);   // Enable motor driver

  // New ESP32 PWM API
  ledcAttach(PWMA, pwmFreq, pwmResolution);
  ledcAttach(PWMB, pwmFreq, pwmResolution);
}

void loop() {
  delay(3000);

  // MOVE FORWARD
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(PWMA, 240);
  ledcWrite(PWMB, 240);

  delay(3000);

  // STOP
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);
  delay(2000);

  // MOVE BACKWARD
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(PWMA, 200);
  ledcWrite(PWMB, 200);

  delay(3000);

  // STOP
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);
  delay(2000);

  // TURN RIGHT
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(PWMA, 200);
  ledcWrite(PWMB, 200);

  delay(3000);

  // TURN LEFT
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(PWMA, 200);
  ledcWrite(PWMB, 200);

  delay(3000);
}
