// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      modulo: homeialib.cpp

#include <Streaming.h>
#include "homeialib.h" 
#include "dht.h"

//
//  Funsao para gerar checksum
//
byte getChecksum(void) {
  
  byte kSum;
  
  //kSum = (KEYCHECKSUM - (tSum & KEYCHECKSUM));
  //Serial.println( kSum, HEX );

}

// Nome.....: DB_mostra_parametro
//
// Descricao: Funcao Debug mostra parametro
//
// Syntax...: DB_mostra_parametro(int _i, byte _pkey, byte _pSum, long _tSum);
//              
// Exemplo..: if ( DEBUG == 1 ) {     // Debug
//               DB_mostra_parametro( i, pkey[i], pSum, tSum);
//            } 
           
// Retorna..:
// --
void DB_mostra_parametro(int _i, byte _pkey, byte _pSum, long _tSum) {
    Serial.print( "i .......: " ); Serial.println( _i );
    Serial.print( "pkey[i]..: " ); Serial.println( _pkey, HEX );
    Serial.print( "pSum.....: " ); Serial.println( _pSum, HEX );
    Serial.print( "tSum.....: " ); Serial.println( _tSum, HEX );
    Serial.print( "tSum+pSum: " ); Serial.println( _tSum + _pSum, HEX );
    Serial.println( "-------------" ); 
}

// Funcao procura ID inicio de pacote
//
// Nome...: id_pacote
//
// Syntax.: id_pacote();
//
// Retorna: 0 - operacao mal sucedida  
//          1 - operacao bem sucedida
// --
int idRX_pacote(void) {
  
int entrega_correta = 0; 
  
if (Serial.available() > 0) {
     
  pckRele.inic = Serial.read();
  //pckRele.radi = Serial.read();
     
  // Identificador de pacote.
  if (pckRele.inic == 0x7E) { 
       
    // Identificado de ID do radio, cada radio xBee tem seu ID.  
    //if( pckRele.radi == IDDORADIO || pckRele.radi == 0xFF ) {
       
       // declara variaveis local
       int tamanho = BYTEPACK+1;
       byte pkey[] = {};
       byte checkSum = 0x00;
       byte somaDado = 0x00;
       byte cSum = 0x00;
       byte pSum = 0x00;
       long tSum = 0x00;
       
       // arquiva pacotes de dados recebibo.
       for ( int i=1; i<=tamanho; i++) {
         pkey[i] = Serial.read();
         cSum = pkey[i];
         
         // identificador do ID do modulo xBee
         if ( i == 1) {
              // verifica se o ID Destino do pacote e deste modulo.
              pckRele.radi =  pkey[i];
              
              // valida se o pacote recebido e deste modulo.
              if ( byte(pckRele.radi) != IDDORADIO ) {
                if ( byte(pckRele.radi) != 0xFF ) {
                  // Este pacote nao e' deste modulo, sai da funcao.
                  return entrega_correta;
                }
              }
         }

          // identifica o tamanho do frame do pacote
          if ( i == BYTEPACK ) { 
               // reajusta para o tamanho dos dados 
               tamanho += pkey[i]; 
               pckRele.lbat =  pkey[i];
          }
         
          // inicio da captura dos dados do pacote dados.
          if (i == 4) {
               pckRele.dad1 = pkey[i]; 
               pSum = pkey[i];
               tSum += pkey[i];
          }   
          if (i == 5) {
               pckRele.dad2 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;
          }  
          if (i == 6) {
               pckRele.dad3 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;    
          }          
          if (i == 7) {
               pckRele.dad4 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;
          }               
          if (i == 8) { 
               pckRele.dad5 = pkey[i];
               pSum = pkey[i];
               tSum += pSum;
          }
          // fim da captura dos dados do pacote de dados.               
             
       }
       // CheckSum, efetua certificacao dos dados enviados.
       pckRele.csum = cSum;
       checkSum = (KEYCHECKSUM - (tSum & KEYCHECKSUM));
       
       // caso queira conferir a ckecksum de um pacote enviado, uso de referencia. 
       //Serial.print( "Conferencia ckSum: " ); Serial.println( checkSum, HEX );
       //Serial.print( "Enviada no pacote: " ); Serial.println( cSum,     HEX );

       /* Efetua as chamadas de funcoes requerida no pacote enviado 
          pos certificacao do CheckSum. 
       */


       // Se checkSum esta correto executa o comando recebido.  
       if ( byte(pckRele.csum) == byte(checkSum) ) { 
           
          /* Sertifica se o ID "0x5E identificador" de pacote de dados esta correto.
          */
          if ( pckRele.dad2 == IDDADO ) {  

              /* Liga ou desliga uma porta determinada, se pckRele.dad1 for 0x48
              */
              if ( byte(pckRele.dad1) == LDPORTA ) {
                  
                 entrega_correta = 1;  // valida'ao ok.
                 //Serial.println( "Liga desliga portas " ); 
                 
                 // Verifica se e'porta digital
                 if ( pckRele.dad3 == 'D' ) { 

                     // Se Porta for = 4 ou 5
                     if ( pckRele.dad4 == 4 || pckRele.dad4 == 5) {

                        // Ativa a porta especificada.
                        if ( pckRele.dad5 == ON ) {
                           digitalWrite( pckRele.dad4, HIGH);     
                           status_porta[pckRele.dad4] = 1; // Liga o status da porta
                        }
                        
                        // Ativa a porta especificada.   
                        if ( pckRele.dad5 == OFF ) {
                           digitalWrite(char(pckRele.dad4), LOW); 
                           status_porta[pckRele.dad4] = 0; // Desliga o status da porta
                        }

                     }
                 }

              } // end-if-LDPORTA
              
              /* Retorna status das portas digitais, se pckRele.dad1 for 0x49
              */  
              if ( byte(pckRele.dad1) == STATUSPORTA ) {
                
                entrega_correta = 1;  // validacao ok.
                //Serial.println( "Status das portas " ); 
                
                byte kSum = 0x00;
                long tSum = 0x00;
                
                // inicio da montagem e envio do pacote.
                Serial.print( 0x7E,      HEX ); // Cabecalho pacote
                Serial.print( 0x00,      HEX ); // Destino, quem o recebera.
                Serial.print( IDDORADIO, HEX ); // Origem, quem o envio.
                Serial.print( 0x16,      HEX ); // Tamanho do pacote de dados.
                Serial.print( 0x49,      HEX ); // ID da API, identificador do comando.
                Serial.print( 0x5E,      HEX ); // ID do pacote dados, isso o identifica se e um pacote.
                
                // transmite os dados do status da porta
                for ( int x=0; x<=15; x++ ) {
                    Serial.print( status_porta[x] );
                    tSum += status_porta[x];
                }    
                
                // efetua soma do checksum dos dados a serem enviados.
                kSum = (KEYCHECKSUM - (tSum & KEYCHECKSUM));
                Serial.println( kSum, HEX );
                // fim do envio do pacote, Status das portas digitais
                //



                /* Sensor de temperatura/humidade, se pckRele.dad1 for 0x50
                */
                
              } // end-if-STATUSPORTA

              /* Sensor de temperatura/humidade, se pckRele.dad1 for 0x50
              */
              if ( byte(pckRele.dad1) == HUMIDATEMPE ) {

                 entrega_correta = 1;  // validacao ok.   

                 float h = dht.readHumidity();      // Humidade.
                 float t = dht.readTemperature();   // Temperatura.

                 if (isnan(t) || isnan(h)) {
                    Serial.println("Falha na leitura do DHT.");

                 }  else {

                    byte kSum = 0x00;
                    long tSum = 0x00;

                    // inicio da montagem e envio do pacote.
                    Serial.print( 0x7E,      HEX ); // Cabecalho pacote
                    Serial.print( 0x00,      HEX ); // Destino, quem o recebera.
                    Serial.print( IDDORADIO, HEX ); // Origem, quem o envio.
                    Serial.print( 0x10,      HEX ); // Tamanho do pacote de dados.
                    Serial.print( 0x50,      HEX ); // ID da API, identificador do comando.
                    Serial.print( 0x5F,      HEX ); // ID do pacote dados, isso o identifica se e um pacote.
                    Serial.print( h );              // float humidade.
                    Serial.print( t );              // float temperatura.

                    tSum += (0x50 + 0x5F + h + t );

                    // efetua soma do checksum dos dados a serem enviados.
                    kSum = (KEYCHECKSUM - (tSum & KEYCHECKSUM));
                    Serial.println( kSum, HEX );
                    // fim do envio do pacote, Status das portas digitais
                 } 

              } // end-if-HUMIDATEMPE
              
          } // end-if-IDDADO 

       } // end-if-checkSum 
         
       Serial.flush();  
        
       if ( DEBUG == 1 ) {
           // imprime resultado parcial do pacote montado, modulo Debug.
           Serial.print( "============= " ); Serial.println( );
           Serial.print( "pckRele.inic: " ); Serial.println( pckRele.inic ); 
           Serial.print( "pckRele.radi: " ); Serial.println( pckRele.radi ); 
           Serial.print( "pckRele.lbat: " ); Serial.println( pckRele.lbat );
           Serial.print( "pckRele.dad1: " ); Serial.println( pckRele.dad1 );
           Serial.print( "pckRele.dad2: " ); Serial.println( pckRele.dad2 );
           Serial.print( "pckRele.dad3: " ); Serial.println( pckRele.dad3 );
           Serial.print( "pckRele.dad4: " ); Serial.println( pckRele.dad4 );
           Serial.print( "pckRele.dad5: " ); Serial.println( pckRele.dad5 );
           Serial.print( "pckRele.csum: " ); Serial.println( pckRele.csum );
           Serial.print( "============= " ); Serial.println( );
           Serial.print( "pSum........: " ); Serial.println( pSum, HEX );
           Serial.print( "tSum........: " ); Serial.println( tSum, HEX );
           Serial.print( "tSum + pSum.: " ); Serial.println( tSum + pSum, HEX );
           Serial.print( "cSum Valida.: " ); Serial.println( cSum, HEX );
           Serial.print( "CheckSum....: " ); Serial.println( checkSum, HEX );
           Serial.println(byte(pckRele.csum) == byte(checkSum) ? " -- Ok." : " -- Erro.");
       } // end-if-DEBUG

     }
    
  }
   
 return entrega_correta;
}

