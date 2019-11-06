//Programa: Modulo MP3 DFPlayer Mini com Arduino
//Autor: Arduino e Cia

/***************************************************
  DFPlayer - A Mini MP3 Player For Arduino
 ***************************************************
  Created 2016-12-07
  By [Angelo qiao](Angelo.qiao@dfrobot.com)

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Inicia a serial por software nos pinos 10 e 11
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

char buf;
int pausa = 0;
int equalizacao = 0;

void setup()
{
  //Comunicacao serial com o modulo
  mySoftwareSerial.begin(9600);
  //Inicializa a serial do Arduino
  Serial.begin(115200);

  //Verifica se o modulo esta respondendo e se o
  //cartao SD foi encontrado
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Inicializando modulo DFPlayer... (3~5 segundos)"));
  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(F("Nao inicializado:"));
    Serial.println(F("1.Cheque as conexoes do DFPlayer Mini"));
    Serial.println(F("2.Insira um cartao SD"));
    while (true);
  }
  Serial.println();
  Serial.println(F("Modulo DFPlayer Mini inicializado!"));

  //Definicoes iniciais
  myDFPlayer.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer.volume(5); //Volume 5
  myDFPlayer.EQ(0); //Equalizacao normal

  //Mostra o menu de comandos
  Serial.println();
  Serial.print("Numero de arquivos no cartao SD: ");
  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
  menu_opcoes();
}

void loop()
{
  //Aguarda a entrada de dados pela serial
  while (Serial.available() > 0)
  {
    buf = Serial.read();
    
    //Reproducao
    if ((buf >= '1') && (buf <= '9'))
    {
      Serial.print("Reproduzindo musica: ");
      Serial.println(buf);
      buf = buf - 48;
      myDFPlayer.play(buf);
      menu_opcoes();
    }
    
    //Parada
    if (buf == 's')
    {
      myDFPlayer.stop();
      Serial.println("Musica parada!");
      menu_opcoes();
    }
    
    //Pausa/Continua a musica
    if (buf == 'p')
    {
      pausa = !pausa;
      if (pausa == 0)
      {
        Serial.println("Continua musica...");
        myDFPlayer.start();
      }
      if (pausa == 1)
      {
        Serial.println("Musica pausada...");
        myDFPlayer.pause();
      }
      menu_opcoes();
    }
    
    //Seleciona equalizacao
    if (buf == 'e')
    {
      equalizacao++;
      if (equalizacao == 6)
      {
        equalizacao = 0;
      }
      myDFPlayer.EQ(equalizacao);
      Serial.print("Equalizacao: ");
      Serial.print(equalizacao);
      Serial.println(" (0 = Normal, 1 = Pop, 2 = Rock, 3 = Jazz, 4 = Classic, 5 = Bass)");
      menu_opcoes();
    }
    
    //Aumenta volume
    if (buf == '+')
    {
      myDFPlayer.volumeUp();
      Serial.print("Volume atual:");
      Serial.println(myDFPlayer.readVolume());
      menu_opcoes();
    }
    
    //Diminui volume
    if (buf == '-')
    {
      myDFPlayer.volumeDown();
      Serial.print("Volume atual:");
      Serial.println(myDFPlayer.readVolume());
      menu_opcoes();
    }
  }
}

void menu_opcoes()
{
  Serial.println();
  Serial.println(F("Comandos:"));
  Serial.println(F(" [1-9] Para selecionar o arquivo MP3"));
  Serial.println(F(" [s] parar reproducao"));
  Serial.println(F(" [p] pausa/continua a musica"));
  Serial.println(F(" [e] seleciona equalizacao"));
  Serial.println(F(" [+ or -] aumenta ou diminui o volume"));
  Serial.println();
}
