// Autor: Neviim Jads    neviim@yahoo.com    (Twitter: neviim)    
//
// data: 10/08/2012      modulo: homeialib.h 

#ifndef HOMEIALIB_H
#define HOMEIALIB_H

#define IDDORADIO 0x01       // ID do Radio xBee I
#define SYNCCODE 0xfeef      // codigo de controle.
#define BYTEPACK 3           // byte que contem o tamanho do pacote.
#define SERIALSPEED 19200    // Velocidade de transferencia da serial
#define KEYCHECKSUM 0xFF     // Chave de operacao xor para o checksum
#define DEBUG 0              // Debug ativado  
#define IDDADO 0x5E          // ID (Identificador) de dado de pacote.
#define ON 1                 // Ligado
#define OFF 0                // Desligado

/*
 Estrutura de um protocolo em um pacote de 10 bytes, para comunicacao entre modulos serial: xBee/RF/WIFI/RS485
 
 - 7E FF 00 05 48 5E 44 04 01 10        Ativa    - "rele 1" ou porta 04 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 04 00 11        Desativa - "rele 1" ou porta 04 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 05 01 0F        Ativa    - "rele 2" ou porta 05 de todos os Shields modulos.
 - 7E FF 00 05 48 5E 44 05 00 10        Desativa - "rele 2" ou porta 05 de todos os Shields modulos.
 
*/
struct {
  char inic ; // 0x7E: inicio do pacote.
  char radi ; // 0xFF: Destino do pacote. [ 0xFF enviado para todos ].
  char resp ; // 0x00: Este pacote demanda uma resposta; 0x00 = nao, 0x01 = sim.
  char lbat ; // 0x05: L dado byte tamanho, pacote maximo de 255 bytes.
  char dad1 ; // 0x48: identificador da API.
  char dad2 ; // 0x5E: identificador do pacote.
  char dad3 ; // 0x44: Porta a ser manipulada; ex: D = Digital, A = Analogica.
   int dad4 ; // 0x34: Numero da porta ex; 4 = D4, 5 = D5.
  char dad5 ; // 0x01: Valor do status da porta 1 = ligado / 0 = desligado.  
  char csum ; // 0x??: Checksum dos 5 bytes de dados a ser enviado. (KEYCHECKSUM - (Total-Suma-Dados & KEYCHECKSUM))
} pckRele;


#endif
