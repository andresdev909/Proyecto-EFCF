//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE PINES ARDUINO -->
#define pinled 13
#define rst 6
//              <-- PLANTA -->
/*#define planta1 A0
#define senOutP1 11*/
//              <-- RELE -->
/*#define releP1 10*/
//              <-- RED -->
/*#define senRedP1 12*/
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//&&&& VARIABLES GLOBALES &&&&
boolean var = false;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- INCLUCION DE CLASES -->
#include "Planta.h"
#include "ModSim.h"
#include "RTC.h"
#include "Rele.h"
#include "Red.h"
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&  

SoftwareSerial Serial1(7, 8);// (RX - TX) Serial PARA el modulo MODSIM800L en ARDUINO

void setup(){
  Serial.begin(19200);//        Serial arduino
  Serial1.begin(19200);//       Velocidad de comunicacion con el modulo SIM800L
  delay(30000);//               Tiempo de espera mientras inician los seriales
  
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  //              <-- INSTANCIA DE OBJETOS -->
  ModSim moduloSim("3157232406", "Planta: Tambo1");//<--Definicion objeto Modulo SIM
  RTC moduloRTC();//<--Definicion objeto Modulo RTC
  Planta planta();
  Red red();
  Rele rele();
  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&FIN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
  
  moduloSim.prepararRecepcionSMS();//Se llama a la función que verifica y configura el SIM800L para resivir mensajes
  moduloSim.enviarMensaje("Iniciando...\n"+moduloSim.getNameP+" "+moduloRTC.infoRTC()+"Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta());//Mensaje de inicio de la planta
  led()
  }
  
void loop(){
  if((moduloRTC.dateModH() == 6 && moduloRTC.dateModMn() == 00) || (moduloRTC.dateModH() == 18 && moduloRTC.dateModMn() == 00))
  {
    if((planta.disponibilidadDePlanta() == "Indisponible") || (planta.getSalidaPlanta() == 1 && red.getEstadoRed() == 1) || (red.getEstadoRed() == 1))
    {
      moduloSim.enviarMensaje(moduloSim.alerta(moduloRTC.infoRTC(), ""));
      }
      else
      {
        moduloSim.enviarMensaje(moduloSim.mensaje(moduloRTC.infoRTC(), ""));
        }
    }
    else if(moduloRTC.dateModMn() == 30)
    {
      if(planta.disponibilidadDePlanta() == "Indisponible")
      {
        moduloSim.enviarMensaje(moduloSim.alerta(moduloRTC.infoRTC(), ""));
        }
      }
      else
      {
        moduloSim.esperandoMensaje();
        if(red.getEstadoRed() == 1 && var == false)
        {
          delay(4000);
          moduloSim.enviarMensaje(moduloSim.alerta(moduloRTC.infoRTC(), ""));
          var = true;
          }
          else if(red.getEstadoRed() == 0 && var == true)
          {
            delay(4000);
            if()
            {
              moduloSim.enviarMensaje(moduloSim.alerta(moduloRTC.infoRTC(), ""));
              }
              else
              {
                moduloSim.enviarMensaje(moduloSim.mensaje(moduloRTC.infoRTC(), ""));
                }
            var = false;
            }
        }
    
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& VALIDAR RED Y PLANTA &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

String validarRedYPlanta(){//Está durando (9 seg)
  String var = "";
  for(int i=0; i < 3; i++){
    
    delay(1000);//Tiempo que se demora en enceder la planta
    int planta = planta.getSalidaPlanta();// En 0 esta encendido
    int red = red.getEstadoRed();// En 0 red en servicio
    
    if(red == 1){
        if(planta == 0){
          var = "Red: sin servicio \nPlanta: encendida";
          }
          else
          {
            var = "Red: sin servicio \nPlanta: aapagada";
            }

      }
      else//Entra cuando la red entre de nuevo a funcionar
      {
        if(planta == 0)
        {
          var = "Red: en servicio \nPlanta: encendida";
          }
          else
          {
            var = "Red: en servicio \nPlanta: apagada";
            }
      }
    }
    
  return var;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& ESTADO PLANTA &&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

public static void validaEstado(String mensaje){

    if(mensaje == "encender" && planta.disponibilidadDePlanta() == "Disponible" &&  planta.getSalidaPlanta() == 1){

      rele.setEstadoRele(LOW)
      delay(3000);
      moduloSim.enviarMensaje(moduloSim.getNameP()+" "+rtc.infoRTC()+"Validando estado");
      
      if(planta.validarSalidaPlanta(mensaje) == "NO encendio" || red.getEstadoRed() == 1)
      {
          moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), planta.validarSalidaPlanta(mensaje)));
        }
        else
        {
          moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), planta.validarSalidaPlanta(mensaje)));
          }
      }
      else if(mensaje == "encender" && planta.disponibilidadDePlanta() == "Disponible" &&  planta.getSalidaPlanta() == 0)
      {
        if(rele.getEstadoRele() == 1)
        {
          moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
          }
          else
          {
            moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
            }

        }
        else if(mensaje == "apagar" && planta.disponibilidadDePlanta() == "Disponible" &&  planta.getSalidaPlanta() == 0)
        {
          digitalWrite(releP1, HIGH);
          delay(3000);
          moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), "Validando estado"));
   
          if(planta.validarSalidaPlanta(mensaje) == "NO apago" || red.getEstadoRed() == 1)
          {
              moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), planta.validarSalidaPlanta(mensaje)));
            }
            else
            {
              moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), planta.validarSalidaPlanta(mensaje)));
              }

          }
          else if(mensaje == "apagar" && planta.disponibilidadDePlanta() == "Disponible" &&  planta.getSalidaPlanta() == 1)
          {
            if(red.getEstadoRed() == 1)
            {
              moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
              }
              else
              {
                moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
                }

            }
            else if(mensaje == "estado")
            {
              if(planta.disponibilidadDePlanta() == "Indisponible" || (planta.getSalidaPlanta() == 1 && red.getEstadoRed() == 1) || red.getEstadoRed() == 1)
              {
                moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
                }
                else
                {
                  moduloSim.enviarMensaje(moduloSim.mensaje(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
                  }
              }
              else//Todos los casos que no se dan se derivan aqui
              {
                moduloSim.enviarMensaje(moduloSim.alerta(rtc.infoRTC(), "Estado: "+planta.disponibilidadDePlanta()+"\n"+validarRedYPlanta()));
                }
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& ENCENDIDO DE LED &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void led(){     //Funcion led intermitente
    digitalWrite(pinled, HIGH);
    delay(100);
    digitalWrite(pinled, LOW);
    delay(100);
    digitalWrite(pinled, HIGH);
    delay(100);
    digitalWrite(pinled, LOW);
}
