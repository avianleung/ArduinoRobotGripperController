#include <AccelStepper.h>
 
long receivedMMdistance = 0; 
long receivedDelay = 0;
long receivedAcceleration = 0; 
long demoStage = 1;
long demoStages = 11;
char receivedCommand; 
 
bool newData, runDemo, runAngular, runRadial, runVertical, magnetState = false; 
 
 
AccelStepper angular(AccelStepper::FULL4WIRE, 10, 11, 12, 13);
AccelStepper radial(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
AccelStepper vertical(AccelStepper::FULL4WIRE, 2, 3, 4, 5);


const int RELAY_PIN = 22;
 
 
void setup()
{
  Serial.begin(9600); 
  Serial.println("Testing Accelstepper"); 

  pinMode(RELAY_PIN, OUTPUT);
 

  angular.setMaxSpeed(1000); 
  radial.setMaxSpeed(1000); 
  vertical.setMaxSpeed(1000);

  angular.setAcceleration(500); 
  radial.setAcceleration(500); 
  vertical.setAcceleration(500); 

  angular.disableOutputs(); 
  radial.disableOutputs(); 
  vertical.disableOutputs(); 
}
 
void loop()
{
  commandService(); 
  commandController();

  if (runDemo == true) {
    magnet(true);
    moveAngularStepper(-96, 50);
    delay(500);
    moveRadialStepper(550, 500);
    delay(500);
    moveVerticalStepper(3500, 300);
    delay(500);
    moveVerticalStepper(-2500, 300);
    delay(500);
    moveAngularStepper(200, 50);
    delay(500);
    moveVerticalStepper(2500, 300);
    delay(500);
    magnet(false);
    moveVerticalStepper(-3500, 300);
    delay(500);
    moveRadialStepper(-550, 500);
    delay(500);
    moveAngularStepper(-105, 50);
    delay(500);
    stopDemo();
  }
}

void moveAngularStepper(long steps, long speed) {
  angular.move(steps);
  angular.setMaxSpeed(speed); 

  while(abs(angular.currentPosition()) < abs(steps)) {
    angular.enableOutputs();
    angular.run(); // Step the motor by one step
  }
  angular.disableOutputs(); 
  angular.setCurrentPosition(0);
}

void moveRadialStepper(long steps, long speed) {
  radial.move(steps);
  radial.setMaxSpeed(speed); 

  while(abs(radial.currentPosition()) < abs(steps)) {
    radial.enableOutputs();
    radial.run(); // Step the motor by one step
  }

  radial.disableOutputs();
  radial.setCurrentPosition(0);
}

void moveVerticalStepper(long steps, long speed) {
  vertical.move(steps);
  vertical.setMaxSpeed(speed); 

  while(abs(vertical.currentPosition()) < abs(steps)) {
    vertical.enableOutputs(); 
    vertical.run(); // Step the motor by one step
  }

  vertical.disableOutputs();
  vertical.setCurrentPosition(0);
}

void magnet(bool magnet) {
  if (magnet == false) {
    digitalWrite(RELAY_PIN, LOW);
  } else {
        digitalWrite(RELAY_PIN, HIGH);
  }
}

void stopDemo() {
  runDemo = false;
}
 
 
void commandController() 
{
  if (runAngular == true)
  {
    if (abs(angular.currentPosition()) < receivedMMdistance) 
    {
      angular.enableOutputs(); 
      angular.run(); 
    }
    else 
    {
      runAngular = false; 
      angular.disableOutputs(); 
      angular.setCurrentPosition(0);
    }
  }
  else if (runRadial == true) {
    if (abs(radial.currentPosition()) < receivedMMdistance) 
    {
      radial.enableOutputs(); 
      radial.run(); 
    }
    else
    {
      runRadial = false; 
      radial.disableOutputs(); 
      radial.setCurrentPosition(0); 
    }

  }
  else if (runVertical == true) {
    if (abs(vertical.currentPosition()) < receivedMMdistance) 
    {
      vertical.enableOutputs(); 
      vertical.run(); 
    }
    else {
      runVertical = false; 
      vertical.disableOutputs(); 
      vertical.setCurrentPosition(0); 
    }

  }
  else if (magnetState == false) {
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (magnetState == true) {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else
  {
    return;
 
  }
}
 
void commandService()
{  
  if (Serial.available() > 0) 
  {
    receivedCommand = Serial.read(); 
    newData = true; 
  }
 
  if (newData == true) 
  {
    if (receivedCommand == 'm')
    {
      Serial.println("Magnet On");
      magnetState = true;
 
    }
    if (receivedCommand == 'd')
    {
      Serial.println("Magnet Off");
      magnetState = false;
 
    }
    if (receivedCommand == 'o') 
    {
      runRadial = true;
     
 
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat();
 
      radial.setMaxSpeed(receivedDelay);
      radial.move(receivedMMdistance);
 
    }
    if (receivedCommand == 'i')
    {
      runRadial = true; 
 
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat(); 
 
      radial.setMaxSpeed(receivedDelay); 
      radial.move(-1 * receivedMMdistance); 
 
    }
    if (receivedCommand == 'a')
    {
      runAngular = true; 
     
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat(); 

      angular.setMaxSpeed(receivedDelay); 
      angular.move(receivedMMdistance); 
 
    }
    if (receivedCommand == 'c') 
    {
      runAngular = true; 
 
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat(); 

      angular.setMaxSpeed(receivedDelay); 
      angular.move(-1 * receivedMMdistance); 
 
    }
    if (receivedCommand == 'l')
    {
      runVertical = true; 
     
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat(); 

      vertical.setMaxSpeed(receivedDelay); 
      vertical.move(receivedMMdistance); 
 
    }
    if (receivedCommand == 'r') 
    {
      runVertical = true; 
 
      receivedMMdistance = Serial.parseFloat(); 
      receivedDelay = Serial.parseFloat(); 

      vertical.setMaxSpeed(receivedDelay); 
      vertical.move(-1 * receivedMMdistance); 
 
    }
    if (receivedCommand == 'e') 
    {
      Serial.print("run demo ");
      runDemo = true;
    }
    if (receivedCommand == 'n') 
    {
      runDemo = false;
      runAngular = false; 
      runRadial = false;
      runVertical = false;
       
      angular.setCurrentPosition(0); 
      radial.setCurrentPosition(0); 
      vertical.setCurrentPosition(0); 

      angular.stop(); 
      radial.stop(); 
      vertical.stop();

      angular.disableOutputs(); 
      radial.disableOutputs();
      vertical.disableOutputs();
    }
  }
  newData = false;
}

