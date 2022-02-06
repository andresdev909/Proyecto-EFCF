#include <SoftwareSerial.h>
class ModSim{
  private:
    String numRoot;//variable que contiene el numero root
    String nameP;//Variable que contiene el nombre delaplanta
    String lectura;//Varibale que captura lo leido en el serian modSIM
  
  public:
    ModSim(String numRoot, String nameP)//Constructor ModSim
    {
      numRoot = numRoot;
      nameP = nameP;
      lectura = "";
      SoftwareSerial Serial1(7, 8);// (RX - TX) Serial PARA el modulo MODSIM800L en ARDUINO
      }
    //GETERS y SETERS
    String getNumRoot(){return(numRoot);}//Captura el numero rootdel modSIM
    
    String getNameP(){return(nameP);}//Captura el nombre de la planta
    
    String getLectura(){return(lectura);}//Captura el valor dela variable lectrura
    void setLectura(String val)//Carga un valor a la variable lectura
    {
      lectura = val;
      }
      
    //METODOS
    String mensaje(String dataRTC, String text)
    {
      String msm = getNameP()+" "+dataRTC+" "+text;
      return(msm); 
      }
    
    String alerta(String dataRTC, String text)
    {
      String msm = "ALARMA! "+getNameP()+" "+dataRTC+" "+text;
      return(msm);
      }
    
    void capturarMensaje()//Capturalo que este leyendo el serialdel modSIM <-- Antes: Handshake
    {
      delay(500);
      setLectura("");
      while (Serial1.available())
      {
        String lect = Serial1.readStringUntil("\n");
        setLectura(lect);
        Serial.println(lect);    
        }
      }
      
    void prepararRecepcionSMS()//Metodo que prepara almodulopara resivirmensajes
    {
      delay(1000);
      Serial1.println("AT\n");
      Serial1.println("AT+IPR=19200\n");
      /*Serial1.println("AT+CSCS=\"8859-1\"\n");*/
      Serial1.println("AT+CMGF=1\n"); 
      /*Serial1.println("AT+CSCS=\"UTF-8\"\n");*/
      Serial1.println("AT+CNMI=2,2,0,0,0\n");
      }
      
    void esperandoMensaje()  //Funcion para la recepcion y carga de datos de SMS <-- Antes: ReceocionSMS
    {          
      if (Serial1.available())
      {      
        String MensajeRecibido = Serial1.readStringUntil("\n");
          
        int Size = MensajeRecibido.length();
        String msnText = MensajeRecibido.substring(48, Size-2);
        String number = MensajeRecibido.substring(9, 19);
        delay(200);
          
        if(Size != 56 && Size != 58){     //Verificar para numeros desconocidos
        }
        else
        { 
          validarMensaje(Size, msnText, number);
          }
        }
          
    }

    void validarMensaje(int Size, String msnText, String number)//<-- Antes: Message
    {
        delay(1000);
    
        if(number == numRoot && (msnText == "encender" || msnText== "apagar" || msnText== "estado"))
        {
          validaEstado(msnText);
          }
          else
          {
            enviarMensaje(infoRTC()+"SMS invalido");
            }
          
    }

    void enviarMensaje(String estate) //Funcion para el envio de mensaje <-- Antes: sendMsn
    { 
      delay(5000);  
      Serial1.println("AT+CMGF=1");
      delay(200);
      
      Serial1.println("AT+CMGS=\"+57" + numRoot + "\""); //"+57" es el codigo de area que se asigna previo al numero de telefono en Colombia. Este codigo cambia segun tu pais.
      delay(200);
      String MensajeEnviado = estate;
      int MensajeLength = MensajeEnviado.length();
      Serial.println(MensajeLength);
      if (MensajeLength <= 120) //Mensaje con un maximo de 120 caracteres
      {
        Serial1.println(MensajeEnviado);      
        delay(200);
        Serial1.print((char)26);       
        delay(5000);       
        SerialFlush();
        delay(5000);       
        }
    }

    void SerialFlush ()//Limpieza del buffer para omitir datos denominados basura o inservible.
    {
      while (Serial1.available())
      {
        Serial1.read();
      }
    }
    
  };
