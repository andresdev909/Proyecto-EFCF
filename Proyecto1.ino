#include <MemoryFree.h>
#include <SoftwareSerial.h>
#include "RTClib.h"

#define pinled 13
#define rst 6


//Planta 1
#define planta1 A0    
#define releP1 10
#define senOutP1 11
#define senRedP1 12

//#define planta2 A1
RTC_DS3231 rtc;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&& VARIABLES GLOBALES &&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&
char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

String  MensajeEnviado,
        MensajeRecibido,
        numRoot,
        nameP,
        lectura;

int     yearMod,
        monthMod,
        dayMod,
        hourMod,
        secondMod,
        timeDelay = 1000;   //Tiempo de encendido de la planta (por ciclo)

boolean var = false;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SoftwareSerial Serial1(7, 8);// (RX - TX) Serial PARA el modulo MODSIM800L en ARDUINO

void setup() {
  //*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
  //*&&&&&&&& CONFIGURACION INICIAL PLANTA 1 &&&&&&&*/
  //*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
  pinMode(planta1, INPUT);//    Sensor Estado -> Disponible / No disponible 
  pinMode(releP1, OUTPUT);//    Sensor Rele
  pinMode(senOutP1, INPUT);//   Sensor Salida planta 1
  pinMode(senRedP1, INPUT);//   Sensor de Red
  pinMode(rst, OUTPUT);//       Pin habilita Reset
  
  digitalWrite(releP1, HIGH);// Inicializa puerto en 1 
  digitalWrite(rst, LOW);//     Inicializa el pin de reset
  
  //*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
  //*&&&&&&&& CONFIGURACION INICIAL USUARIO &&&&&&&&*/
  //*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
  
  numRoot = "3157365561";//     Numero de remitente y destinatario para los SMS
  nameP =   "Planta: Tambo 1";//         Nombre de la planta
    
  //*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
  
  Serial.begin(19200);//        Serial arduino
  Serial1.begin(19200);//       Velocidad de comunicacion con el modulo SIM800L
  delay(30000);//               Tiempo de espera mientras inician los seriales

  #ifndef ESP8266
  while (!Serial); //           wait for serial port to connect. Needed for native USB
  #endif

  if (!rtc.begin()) {//         Verifica que haya un RTC conectado
    //Serial.println("No se pudo encontrar RTC");
    Serial.flush();
    sendMsn("P: "+nameP+" No se pudo encontrar RTC");
    abort();
  }
  if (rtc.lostPower()) {//      Infomra que la bateria del RTC se ha agotado
    //Serial.println("RTC sin energia");
    sendMsn("P: "+nameP+" Cambiar bateria RTC");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  pinMode(pinled, OUTPUT);

  startArrvMsn();//             Se llama a la función que verifica y configura el SIM800L para resivir mensajes
  Serial.println(freeMemory());
  
  sendMsn("Iniciando...\n"+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());//Mensaje de inicio de la planta
  //sendMsn(concatText(nameP, validarRandP()));
  //sendMsn(concatText(nameP, estadoDePlanta()));
  //Serial.print("freeMemory()=");
  led();
}

void loop() {                        

    if((dateModH() == 6 && dateModMn() == 00) || (dateModH() == 18 && dateModMn() == 00)){     //Horas de testeo programadas
      String alarm = "";
      if(estadoDePlanta() == "Indisponible" || (digitalRead(senOutP1) == 1 && digitalRead(senRedP1) == 1) || digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
        }
      delay(10000);
      sendMsn(alarm+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP()); 
      delay(20000);                  
    }else if(dateModMn() == 30){//Testeo cada hora
      
      if(estadoDePlanta() == "Indisponible"){
        //Serial.println("Estado fuera de servicio");
        //Planta Estado fuera de servicio
        delay(10000);
        sendMsn("ALARMA!!!\n"+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
        delay(20000);
      }else{
        //Serial.println("En servicio");
        //Planta Estado En servicio
      }
    }
    else{
      RecepcionSMS();
      int estado = digitalRead(senRedP1);
      String alarm = "";
      if(estado == 1 && var == false){
          delay(4000);
          sendMsn("ALARMA!!!\n"+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
          //sendMsn((estadoDePlanta()+". "+validarRandP()));
          var = true;
      }else if(estado == 0 && var == true){
          delay(4000);
          if(estadoDePlanta() == "Indisponible"){
          alarm = "ALARMA!!!\n";
          }
          sendMsn(alarm+nameP+" "+concatText()+"Estado "+estadoDePlanta()+"\n"+validarRandP());
          
          //sendMsn(nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
          //sendMsn((estadoDePlanta()+". "+validarRandP()));
          var = false;
          }  
    }
    //Serial.println(freeMemory());
    freeMemory();
}

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&&& CONCATENAR FECHA Y HORA &&&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

String concatText(){
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
    for(int i = 0; i < 10; i++){
            textoMsn += arrayTextMsn[i];
          }
    return textoMsn;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& ESTADO SALIDA DE LA PLANTA &&&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

String estadoSalidaPlanta(String var){
      //delay(3000);
      String estadoOutP = "";
      //delay(3000);

      for(int i=0; i < 3; i++){
        delay(timeDelay);
        int estado = "";
        estado = digitalRead(senOutP1);
        //Serial.println(estado);
        //Imprime variable estado
        
        if(var == "encender" && estado == 0){           //Mensaje "encender" y planta Prendio
          //Serial.println("Planta encendida");
          //Imprime Planta prendida
          estadoOutP = "Encendida";
        }
        else if(var == "apagar" && estado == 1){        //Mensaje "apagar" y planta apago
          //Serial.println("Planta apagada");
          //Imprime Planta apagada
          estadoOutP = "Apagada";
        }
        else if(var == "encender" && estado == 1){      //Mensaje "encender" y planta no prendio
          //Serial.println("Planta NO encendio");
          //Imprime Planta no encendió
          estadoOutP = "NO encendio";
        }
        else if(var == "apagar" && estado == 0){        //Mensaje "apagar" y planta no apago
          //Serial.println("Planta NO apago");
          //Imprime Planta no apagó
          //digitalWrite(releP1, HIGH);
          estadoOutP = "NO apago";
        }
      }
      if(estadoOutP == "NO encendio"){
        digitalWrite(releP1, HIGH);
        }else if(estadoOutP == "NO apago"){
          //digitalWrite(releP1, LOW);                    //Validar estado de la red
          //validarRandP();
          }
       
      return estadoOutP;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& DISPONIBILIDAD DE LA PLANTA &&&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

String estadoDePlanta(){
      String estado = "";
      int estadoP = 0;
      
      estadoP = digitalRead(planta1);
      
      if(estadoP == 1){
        estado = "Disponible";
        }
        else{
          estado = "Indisponible";
          }
          
      //Serial.println(estadoP);
      //Serial.println(estado);
      return estado;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& FECHA Y HORA MODULO &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

int dateModY(){     //AÑO
    DateTime now = rtc.now();
    yearMod = now.year();

    return  yearMod;
  }

int dateModM(){     //MES
    DateTime now = rtc.now();
    monthMod = now.month();

    return  monthMod;
  }

int dateModD(){     //DIA
    DateTime now = rtc.now();
    dayMod = now.day();

    return  dayMod;
  }

int dateModH(){     //HORA
    DateTime now = rtc.now();
    hourMod = now.hour();

    return  hourMod;
  }

int dateModMn(){     //MINUTOS
    DateTime now = rtc.now();
    hourMod = now.minute();

    return  hourMod;
  }

int dateModS(){     //SEGUNDOS
    DateTime now = rtc.now();
    secondMod = now.second();

    return  secondMod;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&&&&&&&&&&&& HANDSHAKE &&&&&&&&&&&&&&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void HandShake(){//                                             Funcion para la lectura de comandos del SIM800L luego de relizar un handshake
    delay(500);
    lectura = " ";
    while (Serial1.available())
    {
      String lect = Serial1.readStringUntil("\n");
      lectura = lect;
      Serial.println(lect);    
    }  
}

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&&&&&&&&&&&& SERIALFLUSH &&&&&&&&&&&&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void SerialFlush ()//                                           Limpieza del buffer para omitir datos denominados basura o inservible.
{
  while (Serial1.available())
  {
    Serial1.read();
  }
}

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& CONFIGURACION DEL MOD SIM800L PARA MESAJES &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void startArrvMsn(){
      //Serial.println("Inicializando recepcion SMS...");
      delay(1000);//                                            Tiempo para que el SIM800L establezca la señal (5 segundos)
      Serial1.println("AT\n");//                                Handshake con el modulo SIM800L para comprobar su conectividad .
      HandShake();
      String txt1 = lectura;
      //Serial.println(txt1.indexOf("OK"));
      int num1 = txt1.indexOf("OK");
      //Serial.println(txt1.substring(num1, num1+2));

      Serial1.println("AT+IPR=19200\n");
      HandShake();

      /*Serial1.println("AT+CSCS=\"8859-1\"\n");
      HandShake();*/
      
      Serial1.println("AT+CMGF=1\n");//                         Configurar el módulo en modo de texto . 
      HandShake(); 
      String txt2 = lectura;
      //Serial.println(txt2.indexOf("OK"));
      int num2 = txt2.indexOf("OK");
      //Serial.println(txt2.substring(num2, num2+2)); 

      /*Serial1.println("AT+CSCS=\"UTF-8\"\n");
      HandShake();*/
      
      Serial1.println("AT+CNMI=2,2,0,0,0\n"); //Configuracion de la forma como recibir mensajes.
      HandShake();
      String txt3 = lectura;
      //Serial.println(txt3.indexOf("OK"));
      int num3 = txt3.indexOf("OK");
      //Serial.println(txt3.substring(num3, num3+2));
      if(num1 == -1 || num2 == -1 || num3 == -1){
          digitalWrite(rst, HIGH);
        }
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& RECEPCION DE MSM &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void RecepcionSMS ()  //Funcion para la recepcion y carga de datos de SMS
{          
    if (Serial1.available())
    {      
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
        led();
      }
      //Message(Size, msnText, number);
      //led();
    }
      
}

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& VALIDAR RED Y PLANTA &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

String validarRandP(){//Está durando (9 seg)
  String var = "";
  for(int i=0; i < 3; i++){
  delay(timeDelay);//Tiempo que se demora en enceder la planta
  int planta = digitalRead(senOutP1);// En 0 esta encendido
  int red = digitalRead(senRedP1);// En 0 red en servicio
  //Serial.println(planta);
  //Serial.println(red);
  if(red == 1){
      if(planta == 0){
        var = "Red: sin servicio \nPlanta: encendida";
        }else{
          var = "Red: sin servicio \nPlanta: aapagada";
          }

    }else{//Entra cuando la red entre de nuevo a funcionar
      if(planta == 0){
        var = "Red: en servicio \nPlanta: encendida";
        }else{
          var = "Red: en servicio \nPlanta: apagada";
          }
      }
  }
  //Serial.println(var);
  return var;
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& VALIDACION DEL SMS &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void Message(int Size, String msnText, String number){// Recepcion de datos del MSN y decision on/off
    //Serial.print("size: "); 
    //Serial.println(Size);
    //Serial.print("Numero: "); 
    //Serial.println(number);
    //Serial.print("Mensaje: "); 
    //Serial.println(msnText);
    //Imprime las varables mencionadas
    delay(1000);

    if(number == numRoot && (msnText == "encender" || msnText== "apagar" || msnText== "estado")){
      validaEstado(msnText);
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

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& ESTADO PLANTA &&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void validaEstado(String msn){
    //Serial.println(msn);
    //Serial.println(estadoDePlanta());
    //Serial.print("Estado salida: ");
    //Serial.println(digitalRead(senOutP1));
    //Serial.println(" < -- >");
    //Imprime lo anterior mencionado
    String alarm = "";

    if(msn == "encender" && estadoDePlanta() == "Disponible" &&  digitalRead(senOutP1) == 1){
      //Serial.println("Encendiendo...");
      //Imprime encendiendo...
      digitalWrite(releP1, LOW);
      delay(3000);
      //Serial.println("Verificando estado...");
      //imprime Verificando estado...
      sendMsn(nameP+" "+concatText()+"Validando estado");
      
      if(estadoSalidaPlanta(msn) == "NO encendio" || digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
        }

      sendMsn(alarm+nameP+" "+concatText()+estadoSalidaPlanta(msn));
      //sendMsn((estadoDePlanta()+". "+validarRandP()));
    }
    else if(msn == "encender" && estadoDePlanta() == "Disponible" &&  digitalRead(senOutP1) == 0){
      //msn Planta ya esta prendida
      if(digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
        }
      sendMsn(alarm+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
      //Serial.print(nameP+" "+concatText()+"Está encendida");
      //sendMsn((estadoDePlanta()+". "+validarRandP()));
    }
    /*else if(msn == "encender" && estadoDePlanta() == "Indisponible" &&  digitalRead(senOutP1) == 1){
      //msn Planta indisponible
    }*/
    else if(msn == "apagar" && estadoDePlanta() == "Disponible" &&  digitalRead(senOutP1) == 0){
      //Serial.println("Apagando...");
      //Imprime apagando...
      digitalWrite(releP1, HIGH);
      delay(3000);
      //Serial.println("Verificando estado...");
      //Imprime Verificando estado...
      sendMsn(nameP+" "+concatText()+"Validando estado.");
   
      if(estadoSalidaPlanta(msn) == "NO apago" || digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
        }
      sendMsn(alarm+nameP+" "+concatText()+estadoSalidaPlanta(msn));
      //sendMsn((estadoDePlanta()+". "+validarRandP()));
    }
    else if(msn == "apagar" && estadoDePlanta() == "Disponible" &&  digitalRead(senOutP1) == 1){
      //msn Planta ya esta apagada
      if(digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
        }
      sendMsn(alarm+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
      //sendMsn((estadoDePlanta()+". "+validarRandP())); 
    }
    /*else if(msn == "apagar" && estadoDePlanta() == "Indisponible" &&  digitalRead(senOutP1) == 1){
      //msn Planta indisponible
    }*/
    else if(msn == "estado"){
        if(estadoDePlanta() == "Indisponible" || (digitalRead(senOutP1) == 1 && digitalRead(senRedP1) == 1) || digitalRead(senRedP1) == 1){
          alarm = "ALARMA!!!\n";
          }
        sendMsn(alarm+nameP+" "+concatText()+"Estado "+estadoDePlanta()+"\n"+validarRandP());
    }else{//Todos los casos que no se dan se derivan aqui
      sendMsn("ALARMA!!!\n"+nameP+" "+concatText()+"Estado: "+estadoDePlanta()+"\n"+validarRandP());
      //sendMsn((estadoDePlanta()+". "+validarRandP()));
    }
  }

//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
//*&&&&&&& ENVIO DE SMS &&&&&&&*/
//*&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void sendMsn(String estate) //Funcion para el envio de SMS
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
  led();
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
    
