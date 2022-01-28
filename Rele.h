class Rele{
  private:
  public:
    Rele()//Constructor clase Rele
    {
      init();
      }
    //GETERS y SETERS
    
    //Metodos
    void init()
    {
      pinMode(releP1, OUTPUT);//Configiro pin de salida para el rele
      }   
  };
