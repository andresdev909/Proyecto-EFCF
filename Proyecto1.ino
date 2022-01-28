#include <SoftwareSerial.h>

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
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- INCLUCION DE CLASES -->
//#include "Planta.h"
#include "ModSim.h"
//#include "RTC.h"
//#include "Rele.h"
//#include "Red.h"
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE OBJETOS -->
ModSim Modsim("3157232406", "Tambo1");
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void setup(){
  Serial.begin(19200);//        Serial arduino
  Modsim.getNumRoot();
  Serial.print(Modsim.getNumRoot());  
  }

void loop(){}
