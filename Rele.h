//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE PINES ARDUINO -->
#define sensorRele 10;

class Rele{
  private:
  int estadoRele;
  
  public:
    Rele()//Constructor clase Rele
    {
      estadoDeRele = 0;
      init();
      }
    //GETERS y SETERS
    int getEstadoRele()
    {
      estadoRele = digitalRead(sensorRele);
      return(estadoRele);
      }
    void setEstadoRele(int estado)
    {
      digitalWrite(sensorRele, estado)
      }
    //Metodos
    void init()
    {
      pinMode(sensorRele, OUTPUT);//Configiro pin de salida para el rele
      }   
  };
