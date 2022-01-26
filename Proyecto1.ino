#include <MemoryFree.h>
#include <SoftwareSerial.h>
#include "RTClib.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE PINES ARDUINO -->
#define pinled 13
#define rst 6
//              <-- PLANTA -->
#define planta1 A0
#define senOutP1 11
//              <-- RELE -->
#define releP1 10
//              <-- RED -->
#define senRedP1 12
//              <-- RTC -->
RTC_DS3231 rtc;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

class Planta{
  private:
    
  public:
  Planta(){
    init();
  }
  void init(){
    pinMode (planta1, INPUT);
    pinMode (senOutP1,INPUT);
    }
  }

class ModSim{
  private:
  String numRoot;
  String nameP;
  
  public:
  ModSim(String numRoot, String nameP){
    this->numRoot = numRoot;
    this->nameP = nameP;
    }
  void getMensaje(){}
  void sendMensaje(){}
  void sendAlerta(){}
  }
