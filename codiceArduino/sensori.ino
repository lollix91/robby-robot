#define FILTER_MOVING_NEW 0.8
#define FILTER_MOVING_OLD (1 - FILTER_MOVING_NEW)
#define FILTER_STAYING_NEW 0.2
#define FILTER_STAYING_OLD (1 - FILTER_STAYING_NEW)

void senseDistance(bool staying) {
  // legge misura distanza
  int actual = Sensori.distance;

  if (!staying) {
    Sensori.distance = (DistanceSensor.ping_cm() * FILTER_MOVING_NEW) + (actual * FILTER_MOVING_OLD);
  }
  else {
    Sensori.distance = (DistanceSensor.ping_cm() * FILTER_STAYING_NEW) + (actual * FILTER_STAYING_OLD);
  }

}

void senseTemperature(bool staying) {
  // leggi misura temperatura
  float actual = Sensori.temp;

  if (!staying) {
    Sensori.temp = (mpu6050.getTemp() * FILTER_MOVING_NEW) + (actual * FILTER_MOVING_OLD);
  }
  else {
    Sensori.temp = (mpu6050.getTemp() * FILTER_STAYING_NEW) + (actual * FILTER_STAYING_OLD);
  }
}


void mapDistance(int angle) {

  // invia distanza su seriale
  // dist=<valore>
  Serial.print("dist=");
  Serial.println(Sensori.distance);

  // invia angolo direzione
  // headdeg=<valore>
  Serial.print("headdeg=");
  Serial.println(angle);

}

void mapTemperature(int angle) {

  // scala le ultime <numero> misurazioni

  // aggiungi in testa la nuova misura

  // invia la misura su seriale
  // temp=<valore>
  Serial.print("temp=");
  Serial.println(Sensori.temp);

  // invia l'angolo della direzione seguita
  // dirdeg=<valore>
  Serial.print("dirdeg=");
  Serial.println(angle);
}
