
#ifndef _MY_INTERFACE
#define _MY_INTERFACE




// Servo testa robot
#define SERVO_VERT 12
#define SERVO_HORIZ 13

#define SERVO_H_SX 90
#define SERVO_H_CENTRO 45
#define SERVO_H_DX 0

// Sensori UltraSuoni
#define TRIGGER_PIN  2
#define ECHO_PIN     4
#define MAX_DISTANCE 200

// Driver motori ruote
#define MOTSX_A 3
#define MOTSX_B 9
#define MOTDX_A 11
#define MOTDX_B 10

#define MUOVI_DRITTO 0
#define MUOVI_SX 1
#define MUOVI_DX 2

#define MUOVI_AVANTI true
#define MUOVI_DIETRO false

#define MUOVI_VELOCITA_SCAN 90
#define MUOVI_VELOCITA_MAX 255
#define MUOVI_VELOCITA_MED 125


typedef struct {
  int horiz;
  int vert;
  bool changed;
} _headpos;

typedef struct {
  int vel;
  int pinFwd;
  int pinBack;
} _motor;

typedef struct {
  int verso;
  int dir;
  int velocita;
  bool ruota;
  bool fermo;
} _guida;

typedef struct {
  int distance;
  float temp;
} _sensori;




// General purpose
bool semOk(long tref, long t) {
  return (millis() - tref) > t;
}

void semUp(long& tref) {
  tref = millis();
}


// MOTORI
void ruota(int dir, int vel);
void muovi(int verso, int dir, int vel);
void ferma();

// SENSORI
void senseDistance(bool staying);
void senseTemperature(bool staying);
void mapDistance(int angle);
void mapTemperature(int angle);

// TESTA
void orienta_testa();
void pos_testa(int horiz, int vert);

// MAPPING
void mapObstacles();
void blindTest();

#endif
