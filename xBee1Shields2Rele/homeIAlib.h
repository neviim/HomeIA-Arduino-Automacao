// Author: Neviim JAds <neviim@yahoo.com>   -   Twitter: neviim
// Date: 10/09/2012
//
#ifndef homeIAlib_h
#define homeIAlib_h

#include "Arduino.h"

#define SYNC_CODE 0xfeef
#define SERIAL_TIMEOUT 1000
#define UM 0
// #pragma pack(1)

// Classe para setar as portas do arduino como entrada ou saida e o controle do estado
// de cada porta como On = 1 ou Off = 0 (Ligado/Desligado).
//
// Use: setPino porta4(4); //defini um objeto de trabalho porta4.
//
//              porta4.output(); //defini porta como saida.
//              porta4.input();  //defini porta como entrada.
//              porta4.onoff(1); //seta porta para ligada.
//              porta4.onoff(0); //seta porta para desligada.
// ----------
class setPino
{
  public:
    setPino(int pin);
    void output();
    void input();
    void onoff(int onoff);
  private:
    int _pin;
};






#endif
