#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>

byte myMac[]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]   = { 172, 10, 10, 200 };  // Ard homeIA
byte procIp[] = { 172, 10, 10, 249 };  // Server python 172, 10, 10, 37

int serverPort = 10000;    // Server homeIA
int destPort   = 12000;    // Server python
int ledPins[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
int flag[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

int somaflag = 0;
int releLed  = 0; 

OSCServer server;
OSCClient client;

// Setup define e prepara as chamadas do sistema.
void setup() { 
  
 Serial.begin(19200);
 Ethernet.begin(myMac ,myIp); 
 server.begin(serverPort);
 
 for (int i=0; i <= 11; i++) { flag[i]=0; } // Seta todas as luzes como apagada.
 
 // set callback function & oscaddress
 server.addCallback("/hia/luz",    &func1);
 server.addCallback("/hia/luzoff", &func2);
 server.addCallback("/hia/luzon",  &func3);
 
 // pinMode(ledPin, OUTPUT);
 //Serial.println(ledPins[releLed]);
}
  
// Loop central
void loop(){ 
  if( server.aviableCheck() > 0 ) {
    // Serial.println("Neviim sistema ok! "); 
  }
}

// 
// Funcao que defini o esdado do ledpin ligado ou desligado
//
void func1(OSCMessage *_mes) {
  int releLed = _mes->getArgInt32(0);    // Le o parametro enviado.
  
  // DEBUG qual conteudo esta vindo de parametro?
  //println(value); 
  //Serial.println("Luz porta: ");
  //Serial.println(releLed);
  
  pinMode(ledPins[releLed], OUTPUT);
  
  if( releLed<0 or releLed>9 ) return;  // Valida se parametro esta entre 0 a 11 (Numero da porta disponivel no arduino UNO Ethernet disponivel)
  
  // Cria uma nova osc message
  OSCMessage newMes;
  
  // Seta destinacao endereco IP & port
  newMes.setAddress(procIp, destPort);   // newMes.setAddress(_mes->getIpAddress(),destPort);
  newMes.beginMessage("/hia/luz");
  
  // Seta o estado da porta rele 1 = on, 0 = off
  if(flag[releLed] == 1) {
    flag[releLed] = 0;
    digitalWrite(ledPins[releLed], LOW);   // Seta como desligado
    Serial.println("desligado"); }  
  else {
    flag[releLed] = 1;
    digitalWrite(ledPins[releLed], HIGH);  // Seta como ligado
    Serial.println("ligado"); } 
    
  Serial.println("");
  
  String sporta = "";
  for (int i=0; i <= 11; i++) {
    sporta = sporta + String(flag[i]);
  }
  
  char cporta[15];
  sporta.toCharArray(cporta,15);
  
  newMes.addArgInt32(releLed);
  newMes.addArgInt32(flag[releLed]);
  newMes.addArgString( cporta );
  client.send(&newMes);                // Envia uma osc message para server osc (python)
  
}

//
// Funcao que desliga todas as luzes.
//
void func2(OSCMessage *_mes) {
  int parametro = _mes->getArgInt32(0);    // Le o parametro recebido.
  
  somaflag = 0;   // Seta varialvel somaflag como zero.
  
  for(int i=7; i <= 9; i++) {
    somaflag += flag[i];     // Se resultado maior que zero; tem luz acessa.
  }
  
  String buffporta = "";
  char charBuf[15];      // Numero de portas controlada pelo sistema.
  
  if( parametro == 0 & somaflag > 0) { 
    
      // Apaga todas as luzes de pino de 7 a 9
      for(int x=7; x <= 9; x++) { 
        
        // (Ajustar pinagem das luzes quando estiverem definidas.)
        pinMode(ledPins[x], OUTPUT);     // Seta como saida.
        digitalWrite(ledPins[x], LOW);   // Seta como desligado
        
        // pegar a porta que esta sendo processada.
        if(flag[x] > 0) { 
           buffporta = buffporta + String(x);
           
           flag[x] = 0;  // Seta flag como off (luz apagada)
        }
      }
      
      // Se apago alguna luz envia para registro de log.
      if( buffporta != "" ) {
      
        String sporta = "";
        for (int i=0; i <= 11; i++) {
          sporta = sporta + String(flag[i]);
        }
  
        char cporta[15];
        sporta.toCharArray(cporta,15);
        
        buffporta.toCharArray(charBuf,15);
      
        // Cria uma nova osc message
        OSCMessage newMes;
        newMes.setAddress(procIp, destPort); // newMes.setAddress(_mes->getIpAddress(),destPort);
        newMes.beginMessage("/hia/luzoff");
        newMes.addArgString( charBuf );
        newMes.addArgInt32(0);
        newMes.addArgString( cporta );
        client.send(&newMes);                // Envia uma osc message para server osc (python)
      }
  }
}

//
// Funcao que liga todas as luzes.
//
void func3(OSCMessage *_mes) {
  int parametro = _mes->getArgInt32(0);    // Le o parametro recebido.
  
  somaflag = 0;   // Seta varialvel somaflag como zero.
  
  for(int i=7; i <= 9; i++) {
    somaflag += flag[i];     // Se resultado maior que zero; tem luz acessa.
  }
  
  String buffporta = "";
  char charBuf[15];      // Numero de portas controlada pelo sistema.
  
  // DEBUG
  //Serial.println("DEBUG - inicio");
  //Serial.println(parametro);
  //Serial.println(somaflag);
  //Serial.println(buffporta);
  //Serial.println("DEBUG - Fim");
  
  if( parametro == 1 ) { 
    
      // Apaga todas as luzes de pino de 7 a 9
      for(int x=7; x <= 9; x++) { 
        
        // pegar a porta que esta sendo processada.
        if(flag[x] == 0) { 
           buffporta = buffporta + String(x);
           
          // (Ajustar pinagem das luzes quando estiverem definidas.)
          pinMode(ledPins[x], OUTPUT);      // Seta como saida.
          digitalWrite(ledPins[x], HIGH);   // acessa
          
          flag[x] = 1;  // Seta flag como on (luz acessa)   
        }      
      }
      
      // Se alguma luz foi acessa ele malda dados para ser gravado no log.
      if( buffporta != "" ) {
        
        String sporta = "";
        for (int i=0; i <= 11; i++) {
          sporta = sporta + String(flag[i]);
        }
  
        char cporta[15];
        sporta.toCharArray(cporta,15);
      
        // converte variavel string buffporta para chat.
        buffporta.toCharArray(charBuf,15); 
      
        // Cria uma nova osc message
        OSCMessage newMes;
        newMes.setAddress(procIp, destPort); // newMes.setAddress(_mes->getIpAddress(),destPort);
        newMes.beginMessage("/hia/luzon");
        newMes.addArgString( charBuf );
        newMes.addArgInt32( 1 );
        newMes.addArgString( cporta );
        client.send(&newMes);                // Envia uma osc message para server osc (python)
      }  
  }   
}


