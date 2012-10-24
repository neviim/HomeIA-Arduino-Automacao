// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      (modulo: homeia-protocolo shields 2 reles) v 1.1
//

#include "homeialib.h"

int pinRele1 = 4;
int pinRele2 = 5;

//setup
void setup() {
  Serial.begin(SERIALSPEED);
  
  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  
  //char tpack[] = {0x7E, 0x00, 0x00, 0x05, 0x08, 0x01, 0x4E, 0x4A, 0x01, 0x2C}; 
  //for ( int p=0; p <= sizeof(tpack)-1; p++) {
  //  Serial.write( tpack[p] );
  //}
  
  // Codigos de controle 
  //Serial.write(SYNCCODE >> 8 & 0xff); 
  //Serial.write(SYNCCODE & 0xff); 
}

//loop
void loop() {
  
  // Le a porta serial e verifica se tem algum pacote para este modulo.
  if ( idRX_pacote() == 1 ) {
    Serial.println( "Pacote recebido corretamente.");   
  }
  
  delay(100); 
  
}
