//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE PINES ARDUINO -->
#define disponibilidadDePlanta A0;
#define estadoSalidaDePlanta 11;

class Planta{
  private:
    String nombre;
    int salidaPlanta;
    int disponibilidad;
    
  public:
    Planta()//Constructor Planta
    {
      salidaPlanta = 0;
      disponibilidad = 0;
      init();
      }
    //GETERS y SETERS
    int getSalidaPlanta()
    {
      salidaPlanta = digitalRead(estadoSalidaDePlanta)
      return(salidaPlanta);
      }
    int getDisponibilidad()
    {
      disponibilidad = digitalRead(disponibilidadDePlanta);
      return(disponibilidad);
      }
      
    //METODOS
    void init()
    {
      /*Metodo que inicializa laplanta y sensor deplanta*/
      pinMode (disponibilidadDePlanta, INPUT);//Estado de disponibilidad 
      pinMode (estadoSalidaDePlanta,INPUT);//declaracion como entrada
      }
      
    String disponibilidadDePlanta(){//<-- Antes: estadoDePlanta
      /*Metodo que captura la disponibilidad de la planta*/
      String estado = "";
      if(getDisponibilidad() == 1)
      {
        estado = "Disponible";
        }else
        {
          estado = "Indisponible";
          }     
      //Serial.println(estadoP);
      //Serial.println(estado);
      return estado;
      }
      
    String validarSalidaPlanta(String var){//Antes: <- EstadoSalidaPlanta
      String estadoOutP = "";
      for(int i=0; i < 3; i++)
      {
        delay(1000);
        int estado = getSalidaPlanta();
        
        if(var == "encender" && estado == 0)
        {
          estadoOutP = "Encendida";
          }
          else if(var == "apagar" && estado == 1)
          {
            estadoOutP = "Apagada";
            }
            else if(var == "encender" && estado == 1)
            {
              estadoOutP = "NO encendio";
              }
              else if(var == "apagar" && estado == 0)
              {
                estadoOutP = "NO apago";
                }
        }
      if(estadoOutP == "NO encendio")//<-- Revisar como evaluar RELE
        digitalWrite(releP1, HIGH);
        }else if(estadoOutP == "NO apago"){
          }
       
      return estadoOutP;
  }
  };
