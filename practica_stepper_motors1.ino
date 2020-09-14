#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 50

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 16

// All the wires needed for full functionality
#define DIR 8
#define STEP 9
#define home_switch 7
//Uncomment line to use enable/disable functionality
//#define SLEEP 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

int steps;
int angulo = 13 ;
int posicion;
int x = 0;

void setup() {
  Serial.begin (9600);
  Serial.println("Introduzca los grados");

  stepper.begin(RPM, MICROSTEPS);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(home_switch, INPUT_PULLUP);


  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  // stepper.setEnableActiveState(LOW);

  // Start Homing procedure of Stepper Motor at startup

  while (digitalRead(home_switch)) {  // Do this until the switch is activated
    digitalWrite(DIR, HIGH);      // (HIGH = anti-clockwise / LOW = clockwise)
    digitalWrite(STEP, HIGH);
    delay(5);                       // Delay to slow down speed of Stepper
    digitalWrite(STEP, LOW);
    delay(5);
  }

  while (!digitalRead(home_switch)) { // Do this until the switch is not activated
    digitalWrite(DIR, LOW);
    digitalWrite(STEP, HIGH);
    delay(10);                       // More delay to slow even more while moving away from switch
    digitalWrite(STEP, LOW);
    delay(10);
  }
  posicion = 13;  // Reset position variable to zero
}

void loop() {
  delay(1000);

  if (Serial.available() > 0) {
    angulo = Serial.parseInt();
  }
  //Serial.println(angulo);


  if (angulo > 0 && angulo <= 360) {
    x = posicion - angulo; 
    stepper.rotate(x);
    posicion = angulo;
  }

  if ( angulo < 0 || angulo > 360 ){
    Serial.println("GRADO INGRESADO NO PERMITIDO");
  }

  Serial.println(posicion);
}
