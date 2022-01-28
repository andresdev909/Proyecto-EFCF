class ModSim{
  private:
    String numRoot;//variable que contiene el numero root
    String nameP;//Variable que contiene el nombre delaplanta
  
  public:
    ModSim(String numRoot, String nameP)//Constructor ModSim
    {
      this->numRoot = numRoot;
      this->nameP = nameP;
    }
    //GETERS y SETERS
    String getNumRoot()
    {
      return numRoot;
      }
    //Metodos
    void sendMensaje()
    {}
    void sendAlerta()
    {}
  };
