
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Inicia a serial por software nos pinos 10 e 11
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#define SENSOR_UMIDADE A5

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  pinMode(SENSOR_UMIDADE, INPUT_PULLUP);

  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    while (true);
  }

    //Definicoes iniciais
  myDFPlayer.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer.volume(15); //Volume 5
  myDFPlayer.EQ(0); //Equalizacao normal
  myDFPlayer.enableLoopAll();
}

bool estaSeco = false;
bool tocando = false;

void loop() {
  int sensorVal = analogRead(SENSOR_UMIDADE);
  Serial.println(sensorVal);
  estaSeco = (sensorVal >= 800);
  Serial.print("estado do sensor: ");
  Serial.println(estaSeco);

  Serial.print("estado do tocando: ");
  Serial.println(tocando);
  if (estaSeco) {
    // socorro
    if (!tocando){
      Serial.println("tocando musica");
      myDFPlayer.play('1'-48);
      myDFPlayer.loop('1'-48);
      tocando = true;
    }
  } else {
    tocando = false;
    myDFPlayer.pause();
  }
}
