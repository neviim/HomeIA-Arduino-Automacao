/*
Delimite      Lenght       Frame Data (5 bytes)      Checksum
--------  -------------  -------------------------  ----------
| 0x7E |  | MSB | LSB |  | API-specific Strutura |  | 1 Byte |
--------  -------------  -------------------------  ----------

Exemplo:

0x7E     - Start Delimiter. (delimitador de inicio)
0x0005   - Lenght (5 bytes do Frame de Dados).
0x08     - ID do pacote (API Identifier).
0x01     - Identificador do pacote (diferente de zero). 
0x4E4A   - Comando (“NJ” escrito em código ASCII). 
0xFF     - Valor do comando NJ
0x5F     - Checksum (0xFF – (0x08 + 0x01 + 0x4E + 0x4A + 0xFF) )
*/
/*
          Tabela 2.4 - Tipos de Pacotes API
------------------------------------------------------------------
Nome do pacote API 				       Valor
------------------------------------------------------------------
Modem Status 						0x8A
AT Command 						0x08 
AT Command – Queue Parameter Value 			0x09 
AT Command Response 					0x88 
Remote Command Request 					0x17 
Remote Command Response 				0x97 
ZigBee Transmit Request 				0x10 
Explicit Addressing ZigBee Command Frame 		0x11 
ZigBee Transmit Status 					0x8B 
ZigBee Receive Packet (AO=0) 				0x90 
ZigBee Explicit Rx Indicator (AO=1) 			0x91 
ZigBee IO Data Sample 					0x92 
XBee Sensor Read Indicator (AO=0) 			0x94
Node Identification Indicator 				0x95
------------------------------------------------------------------
Fonte: Manual XBee ZNet 2.5/XBee-PRO ZNet 2.5 OEM RF Modules, 2008
------------------------------------------------------------------
*/

/* ===============================================================
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
    
  pacote.inicio   = 0xAF;       // byte fixo
  pacote.packSize = 5;          // pode sofrer variacao
  pacote.idAPI    = 0x77;       // byte fixo, vario se mudar API
  pacote.idPacote = 0x01;       // Quanquer valor menos Zero.
  pacote.mComando = atoi("R");  // Comando R de Rele 
  pacote.lComando = 1;          // Numero de rele - 1 ou 2
  pacote.Valor    = 1;          // 1 - ligado, 0 - desligado  
    
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
     Serial.println( packFull[i], HEX );
  }  
  
}

void setup() {
  montaPackt();
}

void loop() {
}
