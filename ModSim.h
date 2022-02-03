SoftwareSerial Serial1(7, 8);// (RX - TX) Serial PARA el modulo MODSIM800L en ARDUINO
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
    String enviarMensaje(String dataRTC, String text)
    {
      String msm = getNameP()+" "+dataRTC+" "+text;
      return(msm); 
      }
    
    String enviarAlerta(String dataRTC, String text)
    {
      String msm = "ALARMA! "+getNameP()+" "+dataRTC+" "+text;
      return(msm);
      }
    
    void handShake()//Capturalo que este leyendo el serialdel modSIM
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
      //Serial.println("Inicializando recepcion SMS...");
      delay(1000);//                                            Tiempo para que el SIM800L establezca la señal (5 segundos)
      Serial1.println("AT\n");//                                Handshake con el modulo SIM800L para comprobar su conectividad .
      handShake();
      
      String txt1 = getLectura();
      //Serial.println(txt1.indexOf("OK"));
      int num1 = txt1.indexOf("OK");
      //Serial.println(txt1.substring(num1, num1+2));

      Serial1.println("AT+IPR=19200\n");
      handShake();

      /*Serial1.println("AT+CSCS=\"8859-1\"\n");
      HandShake();*/
      
      Serial1.println("AT+CMGF=1\n");//                         Configurar el módulo en modo de texto . 
      handShake(); 
      String txt2 = getLectura();
      //Serial.println(txt2.indexOf("OK"));
      int num2 = txt2.indexOf("OK");
      //Serial.println(txt2.substring(num2, num2+2)); 

      /*Serial1.println("AT+CSCS=\"UTF-8\"\n");
      HandShake();*/
      
      Serial1.println("AT+CNMI=2,2,0,0,0\n"); //Configuracion de la forma como recibir mensajes.
      handShake();
      String txt3 = getLectura();
      //Serial.println(txt3.indexOf("OK"));
      int num3 = txt3.indexOf("OK");
      //Serial.println(txt3.substring(num3, num3+2));
      /*
      if(num1 == -1 || num2 == -1 || num3 == -1){
          digitalWrite(rst, HIGH);
        }*/
      }
      
    void RecepcionSMS ()  //Funcion para la recepcion y carga de datos de SMS
    {          
        if (Serial1.available())
        {      
          String MensajeRecibido = "";//<--Definicion dela variable como local
          MensajeRecibido = Serial1.readStringUntil("\n");
          //Serial.println(MensajeRecibido);
          int Size = MensajeRecibido.length();
          //Serial.println(Size);
          String msnText = MensajeRecibido.substring(48, Size-2);
          //Serial.println(msnText);
          String number = MensajeRecibido.substring(9, 19);
          //Serial.println(number);
          delay(200);
          //MensajeRecibido = "";
          if(Size != 56 && Size != 58){     //Verificar para numeros desconocidos
          }
          else{ 
            Message(Size, msnText, number);
            //led();
          }
        }
          
    }

    String Message(int Size, String msnText, String number)
    {
        //Serial.print("size: "); 
        //Serial.println(Size);
        //Serial.print("Numero: "); 
        //Serial.println(number);
        //Serial.print("Mensaje: "); 
        //Serial.println(msnText);
        //Imprime las varables mencionadas
        delay(1000);
    
        if(number == numRoot && (msnText == "encender" || msnText== "apagar" || msnText== "estado")){
          //validaEstado(msnText);
          return(msnText);
        }else{
          sendMsn(concatText()+"SMS invalido");
          //Serial.println("SMS invalido");
          //Imprime SMS invalido
          /*if(number == numRoot){
            sendMsn(concatText()+"SMS invalido");
            }
            else{
              Serial.println("Numero desconocido");
              msnText="";
              sendMsn(("ALERTA!!!\n"+concatText()+"\nMensaje de numero desconocido: "+number));
              }*/
        }
          
    }

    void enviarMensaje(String estate) //Funcion para el envio de mensaje
    { 
      //Serial.println("Enviando mensaje...");
      //SerialFlush();
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
        //Serial.println("Mensaje enviado");
      }
      Serial.println(freeMemory());
    }
    
  };
