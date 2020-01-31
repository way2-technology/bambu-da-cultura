
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Inicia a serial por software nos pinos 10 e 11
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#define SENSOR_UMIDADE A5
#define ESTADO_TOCANDO 513
#define LED_VERDE 3
#define LED_VERMELHO 4

void setup() {
  
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  pinMode(SENSOR_UMIDADE, INPUT_PULLUP);

  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.print(".");
    while (true);
  }
  Serial.println("ready");
  myDFPlayer.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer.volume(15); //Volume 15
  myDFPlayer.EQ(0); //Equalizacao normal
  myDFPlayer.enableLoopAll();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
}

bool estaSeco = false;
bool muitoUmido = false;
bool tocando;
void loop() {
  int sensorVal = analogRead(SENSOR_UMIDADE);
  Serial.println(sensorVal);
  estaSeco = (sensorVal >= 800);
  muitoUmido = (sensorVal < 550);

  if (muitoUmido){
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);
  } else {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_VERDE, HIGH);
  }
  
  Serial.print("estaSeco: "); 
  Serial.println(estaSeco);

  Serial.print("estado do tocando: ");
  Serial.println(tocando);
  Serial.print("Status: ");
  Serial.println(myDFPlayer.readState());
  tocando = myDFPlayer.readState() == ESTADO_TOCANDO;
  if (estaSeco) {
    // socorro
    if (!tocando){
      Serial.println("tocando musica");
      myDFPlayer.play('1'-48);
    }
  } else {
    tocando = false;
    myDFPlayer.pause();
  }
  delay(1000);
}
