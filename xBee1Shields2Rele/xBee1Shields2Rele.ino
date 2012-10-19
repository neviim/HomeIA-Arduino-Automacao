// Author: Neviim JAds <neviim@yahoo.com>   -   Twitter: neviim
// Date: 10/09/2012    versao: 0.8
// 

#include "homeIAlib.h"

String serInString = "";
                        
int serInIndx  = 0;   
int serOutIndx = 0;    // index para a saida de serInString[] array;

setPino rele1(4); // Portas que controla os reles 1
setPino rele2(5); // Portas que controla os reles 2

// Le string da porta serial
boolean readSerialString() {
  
   if(Serial.available()) { 
      while (Serial.available()){ 
         char sb = Serial.read();
              
         serInString += sb;     
         serInIndx++;
         
         // identifica se cabecalho ID valido do pacote.
         //for(serOutIndx=0; serOutIndx < serInIndx; serOutIndx++) {
           //Serial.print( serInString[serOutIndx] );
           //serInString[serOutIndx] = "";
         //}            
      }
      return true;
   }
   else { return false; }
}

/*===============================================================
  inicio     - 0xAF  Idenificador de inicio de pacote
  packSize   - 0x05  Matrix de dados com 5 bytes
  idAPI      - 0x77  ID do pacote, identifica a API
  idPacote   - 0x01  Identificador do pacote (diferente de 0)  
  mComando   - 0x52  Comando "R" = Rele em codigo ASCII
  lComando   - 0x31  Comando "1" = Rele 1 em codigo ASCII          
  Valor      - 0x30  Valor do comando R1, 0x30 = 0, 0x31 = 1
  Checksum   - 0x2C    (0xFF-(0x77+0x01+0x52+0x31+0x30))            
*/
struct hiaPackReles {
  byte inicio;    // determinador de inicio de pacote
  byte packSize;  // defino o tamanho do frame de dados. 
  byte idAPI;     // Id da API
  byte idPacote;  // Id pacote
  byte mComando;  // R - Rele
  byte lComando;  // 1 - Qual rele
  byte Valor;     // Valor comando 0 ou 1, On/Off 
  byte Checksum;  // integridade da mensagem.
};

int montaPackt (void) {
  
  int packFullSize = 8;
  
  hiaPackReles pacote;
    
  pacote.inicio   = 0xAF;       // 0xAF Inicio do pacote.
  pacote.packSize = 5;          // 0x05 Tamanho do pacote de dados
  pacote.idAPI    = 0xC8;       // 0xC8 byte fixo, ID da API desta rotina
  pacote.idPacote = 49;         // 0x01 Quanquer valor, menos Zero, ID Pacote.
  pacote.mComando = 82;         // 0x52 Comando R para funcao Reles 
  pacote.lComando = 1;          // 0x01 Numero de rele - 1 ou 2
  pacote.Valor    = 1;          // 0x01 1 - ligado, 0 - desligado  
    
  // Agrupa os bytes do pacote e faz a somatoria do 
  // checkSum dos bytes de dados a ser enviados. 
  //  
  byte pack[5] = {pacote.idAPI, pacote.idPacote, pacote.mComando, pacote.lComando, pacote.Valor };
  
  for (int i = 0; i < pacote.packSize-1; i++) {
     pacote.Checksum += pack[i];
  }  

  byte packFull[8] = { pacote.inicio,   pacote.packSize, 
                       pacote.idAPI,    pacote.idPacote, 
                       pacote.mComando, pacote.lComando, 
                       pacote.Valor,    pacote.Checksum };
  
  for (int i = 0; i < packFullSize; i++) {
     Serial.print( packFull[i] );
     Serial.print( " " );
  }

  Serial.println( "" );  
 
}

// processa rotinas das atividades deste modulo.
//
void kernelShieldsRele() {
  
   if( serInIndx > 0) {
    
     // teste de resposta do modulo.
     if (serInString.substring(0,serInIndx-UM) == "ping") {
       Serial.write("pong"); 
     }
     
     // Ativa e desativa rele 1
     if (serInString.substring(0,serInIndx-UM) == "rl1on") {
       rele1.onoff(1); // porta e estado 1 = ligado
       Serial.write("Rele 1, modulo 1, ligado.");
     }
     if (serInString.substring(0,serInIndx-UM) == "rl1of") {
       rele1.onoff(0); // porta estado 0 = desligada 
       Serial.write("Rele 1, modulo 1, desligado.");  
     }
     
     // Ativa e desativa rele 2
     if (serInString.substring(0,serInIndx-UM) == "rl2on") {
       rele2.onoff(1); // ligado
       Serial.write("Rele 2, modulo 1, ligado.");
     }
     if (serInString.substring(0,serInIndx-UM) == "rl2of") {
       rele2.onoff(0); // desligado
       Serial.write("Rele 2, modulo 1, desligado.");  
     }
     
     // Gestao de sensores
       
     serOutIndx = 0;
     serInIndx  = 0;
     serInString = "";
     Serial.println();
   }
}

// Seta as variaveis iniciais.
void setup() {

  Serial.begin(19200);
  
  Serial.println("Neviim - Serie-0001: modulo reles shield v1.0");
  Serial.println("Modulo ativado...");
  Serial.println();
  
  montaPackt(); // Monta e imprime na serial um pacote (teste)
 
  // defini estado das portas dos reles do shield como saida.
  rele1.output(); // define a porta 4 do shield, rele1 para saida
  rele2.output(); // define a porta 5 do shield, rele2 para saida
}

// Loop central.
void loop () {
  
  if (readSerialString()) {;  // le a porta serial.
    kernelShieldsRele();
  }  
  // rotinas, que este modulo for desempenhar. 
  delay(1000);          //espera de 1 segungo.
}

