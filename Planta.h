class Planta{
  private:
    
  public:
    Planta()//Constructor Planta
    {
      init();
    }
    //GETERS y SETERS

    //Metodos
    void init()//Metodo que inicializa laplanta y sensor deplanta
    {
      pinMode (planta1, INPUT);//declaracion como entrada
      pinMode (senOutP1,INPUT);//declaracion como entrada
    }
  };
