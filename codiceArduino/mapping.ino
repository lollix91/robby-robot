int distSx = 500, distDx = 500, distCx = 500;
#define MIN_DIST 50

void mapObstacles() {
  long timer1;

  // sinistra
  pos_testa(SERVO_H_SX, Testa.vert);
  orienta_testa();

  semUp(timer1);
  while (!semOk(timer1, 500)) {
    mpu6050.update();
  }

  semUp(timer1);
  while (!semOk(timer1, 200)) {
    mpu6050.update();
    senseDistance(Guida.fermo);
    senseTemperature(Guida.fermo);
  }

  mapDistance(Testa.horiz);
  mapTemperature(Guida.dir);
  distSx = Sensori.distance;

  // destra
  pos_testa(SERVO_H_DX, Testa.vert);
  orienta_testa();

  semUp(timer1);
  while (!semOk(timer1, 500)) {
    mpu6050.update();
  }

  semUp(timer1);
  while (!semOk(timer1, 200)) {
    mpu6050.update();
    senseDistance(Guida.fermo);
    senseTemperature(Guida.fermo);
  }

  mapDistance(Testa.horiz);
  mapTemperature(Guida.dir);
  distDx = Sensori.distance;


  // centro
  pos_testa(SERVO_H_CENTRO, Testa.vert);
  orienta_testa();

  semUp(timer1);
  while (!semOk(timer1, 500)) {
    mpu6050.update();
  }

  semUp(timer1);
  while (!semOk(timer1, 200)) {
    mpu6050.update();
    senseDistance(Guida.fermo);
    senseTemperature(Guida.fermo);
  }

  mapDistance(Testa.horiz);
  mapTemperature(Guida.dir);
  distCx = Sensori.distance;


}

void mapObstaclesContinuous() {
  long timer1;

  // centro
  pos_testa(SERVO_H_CENTRO, Testa.vert);
  orienta_testa();

  semUp(timer1);
  while (!semOk(timer1, 500)) {
    mpu6050.update();
  }

  Sensori.distance = DistanceSensor.ping_cm();
  semUp(timer1);
  while (!semOk(timer1, 200)) {
    mpu6050.update();
    senseDistance(Guida.fermo);
    senseTemperature(Guida.fermo);
  }
  mapDistance(Testa.horiz);
  mapTemperature(Guida.dir);
  distCx = Sensori.distance;


  if (distCx < MIN_DIST) {
    ferma();

    // sinistra
    pos_testa(SERVO_H_SX, Testa.vert);
    orienta_testa();

    semUp(timer1);
    while (!semOk(timer1, 500)) {
      mpu6050.update();
    }

    Sensori.distance = DistanceSensor.ping_cm();
    semUp(timer1);
    while (!semOk(timer1, 200)) {
      mpu6050.update();
      senseDistance(Guida.fermo);
      senseTemperature(Guida.fermo);
    }

    mapDistance(Testa.horiz);
    mapTemperature(Guida.dir);
    distSx = Sensori.distance;

    // destra
    pos_testa(SERVO_H_DX, Testa.vert);
    orienta_testa();

    semUp(timer1);
    while (!semOk(timer1, 500)) {
      mpu6050.update();
    }

    Sensori.distance = DistanceSensor.ping_cm();
    semUp(timer1);
    while (!semOk(timer1, 200)) {
      mpu6050.update();
      senseDistance(Guida.fermo);
      senseTemperature(Guida.fermo);
    }

    mapDistance(Testa.horiz);
    mapTemperature(Guida.dir);
    distDx = Sensori.distance;

    // raddrizza testa
    pos_testa(SERVO_H_CENTRO, Testa.vert);
    orienta_testa();

  }



}


void blindTest() {
  long timer1;

  muovi(MUOVI_AVANTI, MUOVI_DRITTO, MUOVI_VELOCITA_SCAN);
  semUp(timer1);
  while (!semOk(timer1, 500)) {
    mpu6050.update();
  }
  ferma();
  semUp(timer1);
  while (!semOk(timer1, 125)) {
    mpu6050.update();
  }
  mapObstacles();
}


void escapingAlgo() {
  long timer1;

  // identifica le direzioni libere (avanti, sx, dx)
  //mapObstacles();
  mapObstaclesContinuous();

  // scegli <criterio> la direzione da seguire nel prossimo passo
  // <criterio>
  // 1. prova ad andare dritto
  // 2. Scegli casualmente fra destra e sinistra
  // 3. Segui l'unica direzione libera

  if (distCx >= MIN_DIST) {
    GyroDir = mpu6050.getAngleZ();
    semUp(timer1);
    while (!semOk(timer1, 1000)) {
      mpu6050.update();
      muovi(MUOVI_AVANTI, MUOVI_DRITTO, MUOVI_VELOCITA_SCAN);
    }
  }
  else if (distSx >= MIN_DIST) { //vado a sx
    semUp(timer1);
    while (!semOk(timer1, 500)) {
      mpu6050.update();
      muovi(MUOVI_AVANTI, MUOVI_SX, MUOVI_VELOCITA_SCAN);
    }
  }
  else if (distDx >= MIN_DIST) { //vado a dx
    semUp(timer1);
    while (!semOk(timer1, 500)) {
      mpu6050.update();
      muovi(MUOVI_AVANTI, MUOVI_DX, MUOVI_VELOCITA_SCAN);
    }
  }
  else { //ruota
    semUp(timer1);
    while (!semOk(timer1, 150)) {
      mpu6050.update();
      ruota(MUOVI_DX, MUOVI_VELOCITA_MAX);
    }
  }

  // muovi nella direzione un certo numero di passi
  /*
    ferma();
    semUp(timer1);
    while (!semOk(timer1, 125)) {
      mpu6050.update();
    }
  */

}
