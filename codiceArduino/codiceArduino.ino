#include <MPU6050_tockn.h>

#include <NewPing.h>
#include <Servo.h>

#include "myInterface.h"


// ***************** //
Servo head_horiz, head_vert;
NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

_headpos Testa;

_motor MotorSx;
_motor MotorDx;

_guida Guida;

_sensori Sensori;

MPU6050 mpu6050(Wire);
float GyroDir;

int mapping = 0;
bool _verbose = true;

long timer = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(SERVO_VERT, OUTPUT);
  pinMode(SERVO_HORIZ, OUTPUT);

  pinMode(MOTSX_A, OUTPUT);
  pinMode(MOTSX_B, OUTPUT);
  pinMode(MOTDX_A, OUTPUT);
  pinMode(MOTDX_B, OUTPUT);

  head_horiz.attach(SERVO_HORIZ);  // pin collegato servo orizzontale
  head_vert.attach(SERVO_VERT);  // pin collegato servo verticale

  pos_testa(SERVO_H_CENTRO, 90);
  orienta_testa();

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  MotorSx.pinFwd = MOTSX_A;
  MotorSx.pinBack = MOTSX_B;
  MotorDx.pinFwd = MOTDX_A;
  MotorDx.pinBack = MOTDX_B;

  Guida.verso = MUOVI_AVANTI;
  Guida.dir = MUOVI_DRITTO;
  Guida.velocita = MUOVI_VELOCITA_SCAN;
  Guida.ruota = false;
  Guida.fermo = true;

  GyroDir = mpu6050.getAngleZ();

  ferma();

  Serial.println("Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:

  mpu6050.update();
  senseDistance(Guida.fermo);
  senseTemperature(Guida.fermo);

  if (semOk(timer, 1000)) {

    if (_verbose) {
      // Lettura distanza ostacoli
      mapDistance(Testa.horiz);
      mapTemperature(Guida.dir);
    }

    // leggi comando seriale
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');

      // COMANDI Servo
      if (cmd.equals("head-h")) {
        Serial.print(">");
        while (!Serial.available()) {
          delay(1);
        }
        pos_testa(Serial.readStringUntil('\n').toInt(), Testa.vert);
      }
      else if (cmd.equals("head-v")) {
        Serial.print(">");
        while (!Serial.available()) {
          delay(1);
        }
        pos_testa(Testa.horiz, Serial.readStringUntil('\n').toInt());
      }

      // COMANDI motori
      else if (cmd.equals("avanti")) {
        Guida.verso = MUOVI_AVANTI;
        Guida.ruota = false;
        Guida.fermo = false;
      }
      else if (cmd.equals("dietro")) {
        Guida.verso = MUOVI_DIETRO;
        Guida.ruota = false;
        Guida.fermo = false;
      }
      else if (cmd.equals("sx")) {
        Guida.dir = MUOVI_SX;
      }
      else if (cmd.equals("dx")) {
        Guida.dir = MUOVI_DX;
      }
      else if (cmd.equals("dritto")) {
        Guida.dir = MUOVI_DRITTO;
        GyroDir = mpu6050.getAngleZ();
      }
      else if (cmd.equals("ruota-sx")) {
        Guida.dir = MUOVI_SX;
        Guida.ruota = true;
        Guida.fermo = false;
      }
      else if (cmd.equals("ruota-dx")) {
        Guida.dir = MUOVI_DX;
        Guida.ruota = true;
        Guida.fermo = false;
      }
      else if (cmd.equals("stop")) {
        Guida.dir = MUOVI_DRITTO;
        Guida.fermo = true;
      }
      else if (cmd.equals("vel")) {
        Serial.print(">");
        while (!Serial.available()) {
          delay(1);
        }
        Guida.velocita = Serial.readStringUntil('\n').toInt();
      }

      // COMANDI mapping
      else if (cmd.equals("stop-mapping")) {
        mapping = 0;
        _verbose = true;
      }
      else if (cmd.equals("blindtest")) {
        mapping = 1;
        _verbose = false;
      }
      else if (cmd.equals("start-mapping")) {
        mapping = 2;
        _verbose = false;
      }

    }




    semUp(timer);
  }


  switch (mapping) {

    // Guida Manuale
    case 0: {
        // MOVIMENTO servo
        orienta_testa();

        // MOVIMENTO motori
        if (!Guida.fermo) {
          if (Guida.ruota) {
            ruota(Guida.dir, Guida.velocita);
          }
          else {
            muovi(Guida.verso, Guida.dir, Guida.velocita);
          }
        }
        else {
          ferma();
        }
        break;
      }

    // Algoritmo Mapping 1
    case 1: {
        blindTest();
        break;
      }

    // Algoritmo Mapping 2
    case 2: {
        escapingAlgo();
        break;
      }
  }


}
