// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      modulo: homeialib.cpp

#include <Streaming.h>
#include "homeialib.h" 


byte hiaChecksum(void) {
  
  byte cksum;

}

// Funcao Debug mostra parametro
//
// Nome...: DB_mostra_parametro
//
// Syntax.: DB_mostra_parametro(int _i, byte _pkey, byte _pSum, long _tSum);
//              
// Retorna:
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
              // verifica se o ID Destino do pacote e' deste modulo.
              pckRele.radi =  pkey[i];
              
              // valida se o pacote recebido e'deste modulo.
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
         
         // arquiva os dados do pacote dados.
         if (i == 4) {
               pckRele.dad1 = pkey[i]; 
               pSum = pkey[i];
               tSum += pkey[i];
               
               if ( DEBUG == 1 ) { // Debug
                 DB_mostra_parametro( i, pkey[i], pSum, tSum);
               }
          }   
          if (i == 5) {
               pckRele.dad2 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;

               if ( DEBUG == 1 ) { // Debug
                 DB_mostra_parametro( i, pkey[i], pSum, tSum);
               }
          }  
          if (i == 6) {
               pckRele.dad3 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;
               
               if ( DEBUG == 1 ) { // Debug
                 DB_mostra_parametro( i, pkey[i], pSum, tSum);
               }     
          }          
          if (i == 7) {
               pckRele.dad4 = pkey[i]; 
               pSum = pkey[i];
               tSum += pSum;
               
               if ( DEBUG == 1 ) { // Debug
                 DB_mostra_parametro( i, pkey[i], pSum, tSum);
               }
          }               
          if (i == 8) { 
               pckRele.dad5 = pkey[i];
               pSum = pkey[i];
               tSum += pSum;
               
               if ( DEBUG == 1 ) { // Debug
                 DB_mostra_parametro( i, pkey[i], pSum, tSum);
               }
           }               
             
       }
       // CheckSum, efetua certificacao dos dados enviados.
       pckRele.csum = cSum;
       checkSum = (KEYCHECKSUM - (tSum & KEYCHECKSUM));
       
       // Se checkSum esta correto executa o comando recebido.  
       if ( byte(pckRele.csum) == byte(checkSum) ) { 
         entrega_correta = 1; // valida'ao ok.
         
         // Executa comando enviado no pacote.
         //
         
         // E' um pacote de dado.
         if ( pckRele.dad2 == IDDADO ) {
           if ( DEBUG == 1 ) { Serial.println( "Passo 1" ); }   
           
           // Verifica se e'porta digital
           if ( pckRele.dad3 == 'D' ) {
             if ( DEBUG == 1 ) { Serial.println( "Passo 2" ); }
             
             // Se Porta for = 4 ou 5
             if ( pckRele.dad4 == 4 || pckRele.dad4 == 5) { 
               if ( DEBUG == 1 ) { Serial.println( "Passo 3" ); }
               
               // Ativa ou desativa porta 
               if ( pckRele.dad5 == ON ) {

                 // Liga o status da porta
                 digitalWrite( pckRele.dad4, HIGH); 
               }
                 
               if ( pckRele.dad5 == OFF ) {
                 
                 // Desliga o status da porta
                 digitalWrite(char(pckRele.dad4),  LOW); 
               }
             }
           }
         }  
          
       }
         
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
       }
     } 
   //}
 }
   
 return entrega_correta;
}

