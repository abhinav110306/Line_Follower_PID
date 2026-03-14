// -------- SENSOR SETTINGS --------
bool isBlackLine = 1;
unsigned int lineThickness = 25;
unsigned int numSensors = 7;

// -------- MOTOR PINS --------
#define AIN1 4
#define AIN2 5
#define BIN1 18
#define BIN2 19
#define PWMA 23
#define PWMB 15
#define STBY 2


// -------- SENSOR PINS --------
int sensorPins[7] = {34,35,32,33,25,26,27};

// -------- PID VARIABLES --------
int P,D,I,previousError,PIDvalue,error;
int lsp,rsp;

int lfSpeed = 140;
int currentSpeed = 40;

float Kp = 0.06;
float Kd = 1.5;
float Ki = 0;

// -------- SENSOR DATA --------
int minValues[7], maxValues[7], sensorValue[7];
int onLine = 1;

// -------- BUTTONS --------
#define CALIB_BUTTON 12
#define START_BUTTON 13

//--------------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(STBY,OUTPUT);

  pinMode(CALIB_BUTTON,INPUT_PULLUP);
  pinMode(START_BUTTON,INPUT_PULLUP);

  digitalWrite(STBY,HIGH);
}

//--------------------------------

void loop()
{
 

  Serial.println("Starting Calibration...");
  delay(1000);

  calibrate();

  Serial.println("Calibration Finished");



  Serial.println("Starting Line Following");

  while(1)
  {
    readLine();

    Serial.print("Sensors: ");
    for(int i=0;i<7;i++)
    {
      Serial.print(sensorValue[i]);
      Serial.print(" ");
    }

    Serial.print(" Error:");
    Serial.println(error);

    if(currentSpeed < lfSpeed)
      currentSpeed++;

    if(onLine)
      linefollow();
    else
      searchLine();
  }
}

//--------------------------------

void linefollow()
{
  error =
  (3*sensorValue[0] +
   2*sensorValue[1] +
   sensorValue[2] -
   sensorValue[4] -
   2*sensorValue[5] -
   3*sensorValue[6]);

  if(lineThickness > 22)
    error *= -1;

  if(isBlackLine)
    error *= -1;

  P = error;
  I += error;
  D = error - previousError;

  PIDvalue = (Kp*P) + (Ki*I) + (Kd*D);

  previousError = error;

  lsp = currentSpeed - PIDvalue;
  rsp = currentSpeed + PIDvalue;

  lsp = constrain(lsp,0,255);
  rsp = constrain(rsp,0,255);

  motorA(lsp);
  motorB(rsp);
}

//--------------------------------

void searchLine()
{
  if(error > 0)
  {
    motorA(-120);
    motorB(150);
  }
  else
  {
    motorA(150);
    motorB(-120);
  }
}

//--------------------------------

void motorA(int speed)
{
  if(speed >= 0)
  {
    digitalWrite(AIN1,HIGH);
    digitalWrite(AIN2,LOW);
    analogWrite(PWMA,speed);
  }
  else
  {
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
    analogWrite(PWMA,-speed);
  }
}

//--------------------------------

void motorB(int speed)
{
  if(speed >= 0)
  {
    digitalWrite(BIN1,HIGH);
    digitalWrite(BIN2,LOW);
    analogWrite(PWMB,speed);
  }
  else
  {
    digitalWrite(BIN1,LOW);
    digitalWrite(BIN2,HIGH);
    analogWrite(PWMB,-speed);
  }
}

//--------------------------------

void calibrate()
{
  for(int i=0;i<7;i++)
  {
    minValues[i] = analogRead(sensorPins[i]);
    maxValues[i] = analogRead(sensorPins[i]);
  }

  for(int i=0;i<8000;i++)
  {
    motorA(80);
    motorB(-80);

    for(int j=0;j<7;j++)
    {
      int val = analogRead(sensorPins[j]);

      if(val < minValues[j])
        minValues[j] = val;

      if(val > maxValues[j])
        maxValues[j] = val;
    }
  }

  motorA(0);
  motorB(0);

  Serial.println("Calibration Done");
}

//--------------------------------

void readLine()
{
  onLine = 0;

  for(int i=0;i<7;i++)
  {
    sensorValue[i] = map(
      analogRead(sensorPins[i]),
      minValues[i],
      maxValues[i],
      0,
      1000);

    sensorValue[i] = constrain(sensorValue[i],0,1000);

    if(sensorValue[i] > 700)
      onLine = 1;
  }
}


