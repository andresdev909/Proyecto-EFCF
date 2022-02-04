//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//              <-- DEFINICION DE PINES ARDUINO -->
#define sensorDeRed 12;

class Red{
  private:
  int estadoDeRed;
  
  public:
    Red()
    {
      estadoDeRed = 0;
      init();
      }
      
    //GETERSy SETERS
    int getEstadoRed()
    {
      estadoDeRed = digitalRead(sensorDeRed)
      return(estadoDeRed);
      }
      
    //Metodos
    void init()
    {
      pinMode(sensorDeRed, INPUT);
      }
  }
