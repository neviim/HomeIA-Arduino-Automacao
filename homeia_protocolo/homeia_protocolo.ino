// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      (modulo: homeia-protocolo shields 2 reles)
//

#include <Ethernet.h>
#include <ArdOSC.h>

#include "netFuncoes.h"
#include "homeialib.h"
#include "dht.h"

// --- .

#define IDDORADIO   0x01  // ID do Radio xBee I, Router 
#define IDDODESTI   0x00  // ID do Radio xBee 0, Master
#define VERSAO      1.20  // Versao do sistema.

// --- .

byte myMac[]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]   = { 172, 10, 10, 200 };   // Arduino homeIA
byte procIp[] = { 172, 10, 10, 249 };   // Destino Server python: 172, 10, 10, 37

int ledPins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
int flag[]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0 };

int pinRele1 = 4;
int pinRele2 = 5;
int somaflag = 0;
int releLed  = 0; 
int xlp      = 0;

OSCServer server;
OSCClient client;

DHT dht(DHTPIN, DHTTYPE);

// modulo setup.
void setup() {
  // inicializa funcoes do sistema.
  Serial.begin(SERIALSPEED);
  Ethernet.begin(myMac ,myIp);
  
  server.begin(SERVERPORT);
  dht.begin();   // Inicia sensor dht22.
  
  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  
  // Ao reinicializar mostra a versao do sistema.
  versao();
  
  for (int i=0; i<=15; i++) { flag[i]=0; } // Seta todas as portas como desligada.
  
  // set callback function & oscaddress
  server.addCallback("/hia/luz", &ligaDesligaPorta); // ligaDesligaPorta
  server.addCallback("/hia/luzoff", &desligaPortas); // desligaPortas
  server.addCallback("/hia/luzon", &ligaPortas);     // ligaPortas

}

// modulo loop.
void loop() {
  
  if ( DEBUG == 1 ) { Serial.print("Loop: "); Serial.println( xlp++ ); }
  
  if ( idRX_pacote() == 1 ) {
     /* Funcao interpreta comandos enviados por radio xBee.
     */ 
     xlp = 0; } 

  else  {

    /* Rede ethernet protocolo OSC.
    */ 
    server.aviableCheck(); 

  }

  delay(10); 
  
}
