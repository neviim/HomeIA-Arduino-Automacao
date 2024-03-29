// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      modulo: homeialib.h 

#ifndef HOMEIALIB_H
#define HOMEIALIB_H

#define DEBUG          0      // Debug ativad
#define BYTEPACK       3      // byte que contem o tamanho do pacote.

#define SYNCCODE       0xfeef // codigo de controle.
#define SERIALSPEED    19200  // Velocidade de transferencia da serial
  
#define ON  1                 // Ligado
#define OFF 0                 // Desligado


// Shield sensor de humidade e temperatura                             
#define DHTTYPE DHT22         // DHT 22 (AM2302)
#define DHTPIN  8             // Padrao pino 2, sensor humidade e temperatura

// Shield ethernet com OSC.
#define PINOUT  0
#define PININP  1

// Funcoes a serem chamadas DEFINICOES
#define LDPORTA      0x48     // Liga e desliga portas digitais.
#define STATUSPORTA  0x49     // pede status das portas digitais.
#define HUMIDATEMPE  0x50     // humudade temperatura.
#define IDVERSAO     0x51     // versao ID modulo.
#define IDDADO       0x5E     // ID (Identificador) se este e um pacote de dado.
#define IDRETORNO    0x5F     // ID (Identificador) se este e um pacote de retorno de dado.
#define KEYCHECKSUM  0xFF     // Chave de operacao xor para o checksum

/*
 Estrutura de um protocolo em um pacote de 10 bytes, para comunicacao entre modulos serial: xBee/RF/WIFI/RS485
 
 
 - 7E FF 00 05 48 5E 44 04 01 10        Ativa    - "rele 1" ou porta 04 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 04 00 11        Desativa - "rele 1" ou porta 04 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 05 01 0F        Ativa    - "rele 2" ou porta 05 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 05 00 10        Desativa - "rele 2" ou porta 05 de todos os Shields modulos.
 
 - 7E FF 01 05 51 5E 00 00 00 50        Retorna a versao e ID de todos os modulo ou especifico.            > Versao
 - 7E FF 00 05 49 5E 00 00 00 58        Solicita envio do status das portas, para todos os modulos.        > Status 
 - 7E 02 01 05 50 5E 44 08 03 02        Solicita envio da temperatura e humidade do sensor.                > Temperatura
 
 
 - 7E 03 00 05 48 5E 44 04 01 10 7E 03 00 05 48 5E 44 05 01 0F 7E 03 00 05 49 5E 00 00 00 58
   Modulo 3 ativa porta 4        Modulo 3 ativa porta 5        Solicita status para modulo 3
    
 - 7E 01 00 05 48 5E 44 04 01 10 7E 02 00 05 48 5E 44 05 01 0F 7E 02 00 05 49 5E 00 00 00 58 7E 01 00 05 49 5E 00 00 00 58
   Modulo 1 ativa porta 4 (RL1)  Modulo 2 ativa porta 5 (RL2)  Solicita status para modulo 2 Solicita status para modulo 1
 
  0x7E: ID de inicio de pacote.
  0xFF: Destino do pacote. 
        - 0xFF => enviado para todos xBees
        - 0x01 => para o xBee I
        - 0x02 => para o xBee II
        - 0x03 => para o xBee III
  0x00: Este pacote requer uma resposta; 
        - 0x00 => nao enviar uma resposta. 
        - 0x01 => sim enviar uma resposta.
  0x05: tamanha do pacote enviado, pacote maximo de 255 bytes.
  0x48: identificador da API, Que funcao esta sendo chamada?
        - 0x48 => Acionamento de Reles 1 e 2, porta 4 e 5.
        - 0x49 => Solicitação do status das portas Output do arduino.
        - 0x50 => leitura da temperatura / humidade
        - 0x51 => Versao e ID do modulo.
  0x5E: identificador do pacote.
        - 0x5E => pacote de pergunta ou comando.
        - 0x5F => pacote de reposta.
  0x44: Porta a ser manipulada; ex:
        - 0x00 => Nenhuma.
        - 0x44 => D = Digital. 
        - 0x65 => A = Analogica.
  0x04: Numero da porta ex; 
        - 0x04 => Porta 4.
        - 0x05 => Porta 5.
  0x01: Valor do status da porta:
        - 0x00 = porta desligada. 
        - 0x01 = porta ligada.
        - 0x03 = sensor dht temperatura/humidade, (retorna uma leitura)  
  0x10: Checksum dos 5 bytes de dados a ser enviado. (KEYCHECKSUM - (Total-Suma-Dados & KEYCHECKSUM))

*/
struct {
  char inic ; // 0x7E: inicio do pacote.
  char radi ; // 0xFF: Destino do pacote. [ 0xFF enviado para todos ].
  char resp ; // 0x00: Este pacote demanda uma resposta; 0x00 = nao, 0x01 = sim.
  char lbat ; // 0x05: L dado byte tamanho, pacote maximo de 255 bytes.
  char dad1 ; // 0x48: identificador da API, Que funcao esta sendo chamada?
  char dad2 ; // 0x5E: identificador do pacote.
  char dad3 ; // 0x44: Porta a ser manipulada; ex: D = Digital, A = Analogica.
   int dad4 ; // 0x34: Numero da porta ex; 4 = D4, 5 = D5.
  char dad5 ; // 0x01: Valor do status da porta 1 = ligado / 0 = desligado.  
  char csum ; // 0x??: Checksum dos 5 bytes de dados a ser enviado. (KEYCHECKSUM - (Total-Suma-Dados & KEYCHECKSUM))
} pckRele;

/*
 Estrutura de pacote de resposta da solicitacao de status das postas.

 - 7E FF 00 16 49 5E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 SS 
 
*/
struct {
  char inic    ; // 0x7E: inicio do pacote.
  char radi    ; // 0x00: Destino do pacote. [ 0x00 envia para o master ].
  char orig    ; // 0x01: A origem do pacote, quem esta o enviando.
  char resp    ; // 0x00: Este pacote demanda uma resposta; 0x00 = nao, 0x01 = sim.
  char lbat    ; // 0x16: L dado byte tamanho, pacote maximo de 255 bytes.
  char idAPI   ; // 0x49: identificador da API, Que funcao esta sendo chamada?
  char idPCK   ; // 0x5E: identificador do pacote.  
  char porta[] ; //
  char csum    ; // 0x??:
} pckPortas;

#endif
