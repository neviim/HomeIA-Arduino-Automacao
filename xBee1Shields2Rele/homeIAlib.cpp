// Author: Neviim JAds <neviim@yahoo.com>   -   Twitter: neviim
// Date: 18/10/2012

#include "Arduino.h"
#include "homeIAlib.h"

// Classe: setPino
//
// para setar as portas do arduino como entrada ou saida e o controle do estado
// de cada porta como On = 1 ou Off = 0 (Ligado/Desligado).
//
// Use: setPino porta4(4);       //defini um objeto de trabalho porta4.
//
//              porta4.output(); //defini porta como saida.
//              porta4.input();  //defini porta como entrada.
//              porta4.onoff(1); //seta porta para ligada.
//              porta4.onoff(0); //seta porta para desligada.
//
// ----------------------
setPino::setPino(int pin)
{
  _pin = pin;
}

// defini pino como saida
void setPino::output()
{
  pinMode(_pin, OUTPUT); 
}

// defini pino como entrada
void setPino::input()
{
  pinMode(_pin, INPUT);
}

// ativa pino como ligado = 1 ou desligado = 0
void setPino::onoff(int onoff)
{
  if ( onoff == 1 ) {
    digitalWrite(_pin, HIGH); } //liga.
  if ( onoff == 0 ) {
    digitalWrite(_pin, LOW); }  //desliga.
}


