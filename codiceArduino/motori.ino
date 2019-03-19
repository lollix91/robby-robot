void _mot_avanti(_motor mot) {
  analogWrite(mot.pinFwd, mot.vel);
  analogWrite(mot.pinBack, 0);
}

void _mot_dietro(_motor mot) {
  analogWrite(mot.pinBack, mot.vel);
  analogWrite(mot.pinFwd, 0);
}

void _mot_ferma(_motor mot) {
  analogWrite(mot.pinFwd, 0);
  analogWrite(mot.pinBack, 0);
}


void ruota(int dir, int vel) {
  MotorSx.vel = vel;
  MotorDx.vel = vel;

  if (dir == MUOVI_SX) {
    _mot_dietro(MotorSx);
    _mot_avanti(MotorDx);

  }
  else if (dir == MUOVI_DX) {
    _mot_avanti(MotorSx);
    _mot_dietro(MotorDx);
  }
}

#define GYRO_ANGLE_ERR 5

void muovi(int verso, int dir, int vel) {
  MotorSx.vel = vel;
  MotorDx.vel = vel;

  // aggiustamento direzione robot
  // direzione invertita perché il Gyro sta sottosopra
  if (dir == MUOVI_DRITTO) {
    if (mpu6050.getAngleZ() < GyroDir - GYRO_ANGLE_ERR) {
      //dir = MUOVI_SX;
      dir = MUOVI_DX;
    }
    else if (mpu6050.getAngleZ() > GyroDir + GYRO_ANGLE_ERR) {
      //dir = MUOVI_DX;
      dir = MUOVI_SX;
    }
  }

  // procedi
  if (dir == MUOVI_DRITTO) {
    if (verso == MUOVI_AVANTI) {
      _mot_avanti(MotorSx);
      _mot_avanti(MotorDx);
    }
    else {
      _mot_dietro(MotorSx);
      _mot_dietro(MotorDx);
    }
  }
  else if (dir == MUOVI_SX) {
    _mot_ferma(MotorSx);
    if (verso == MUOVI_AVANTI) {
      _mot_avanti(MotorDx);
    }
    else {
      _mot_dietro(MotorDx);
    }
  }
  else if (dir == MUOVI_DX) {
    _mot_ferma(MotorDx);
    if (verso == MUOVI_AVANTI) {
      _mot_avanti(MotorSx);
    }
    else {
      _mot_dietro(MotorSx);
    }
  }
}

void ferma() {
  MotorSx.vel = 0;
  MotorDx.vel = 0;
  _mot_ferma(MotorSx);
  _mot_ferma(MotorDx);
}
