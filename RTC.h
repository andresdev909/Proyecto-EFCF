class RTC{
  private:
  public:
    RTC()//Constructor RTC
    {
      init();
    }
    //GETERS y SETERS

    //METODOS
    void init()
    {
      RTC_DS3231 rtc;//Defino el objeto rtc
      }

    String validaRTC()
    {
      #ifndef ESP8266
      while (!Serial); //wait for serial port to connect. Needed for native USB
      #endif
    
      if (!rtc.begin())//Verifica que haya un RTC conectado
      {
        //Serial.println("No se pudo encontrar RTC");
        Serial.flush();
        return("No se pudo encontrar RTC");
        abort();
        }
      if (rtc.lostPower())
      {//      Infomra que la bateria del RTC se ha agotado
        //Serial.println("RTC sin energia");
        return("Cambiar bateria RTC");
        //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        }
      return("RTC listo"); 
      }
    
    int dateModY()//AÑO
    {
      DateTime now = rtc.now();
      yearMod = now.year();
      return  yearMod;
      }

    int dateModM()//MES
    {
      DateTime now = rtc.now();
      monthMod = now.month();
      return  monthMod;
      }

    int dateModD()//DIA
    {     
      DateTime now = rtc.now();
      dayMod = now.day();
      return  dayMod;
      }
    
    int dateModH()//HORA
    {
      DateTime now = rtc.now();
      hourMod = now.hour();
      return  hourMod;
      }
    
    int dateModMn()//MINUTOS
    {
      DateTime now = rtc.now();
      hourMod = now.minute();
      return  hourMod;
      }
    
    int dateModS()//SEGUNDOS
    {
      DateTime now = rtc.now();
      secondMod = now.second();
      return  secondMod;
      }

    String infoRTC()
    {
      String textoMsn = "";
      String  dateY = (String)dateModY(), 
              dateM = (String)dateModM(),
              dateD = (String)dateModD(),
              timeh = (String)dateModH(),
              timemn =(String)dateModMn(),
              timeH,
              timeMn;

              if(timeh.length() == 1){
                  timeH = "0"+timeh;
                }else{
                  timeH = timeh;
                  }
              if(timemn.length() == 1){
                  timeMn = "0"+timemn;
                }else{
                  timeMn = timemn;
                  }
                
      String arrayTextMsn[] ={dateD, "/", dateM, "/", dateY, " ", timeH, ":", timeMn,"\n"};    
      for(int i = 0; i < 10; i++)
      {
        textoMsn += arrayTextMsn[i];
        }
      return textoMsn;
      }
    
  };
