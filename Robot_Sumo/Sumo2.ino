//Declaracion de variables para puertos
                          //Sensor Infrarrojo A
int sensor1 = 4;          //Sensor B
int sensor2 = 5;
int sensor3t= 2;
int sensor3e= 3;
//int sensor4t= 11;
//int sensor4e= 7;

int SI1 = 1;      //Declaracion de variables para algoritmo
int SI2 = 1;
int SU1,SU2;      


                    //Motor A
int dir1 = 6;
int dir2 = 7;
                      //Motor B
int dir3= 8;
int dir4 = 9;


int Bandera=0;
int periodo=250;
int tiempoahora=0;
long tiem;
int dis;

void Pelea();
void Busqueda();
int distancia1();
int distancia2();

void setup() 
{
  Serial.begin(9600);     
                            //Sensor infrarrojo A
  pinMode(sensor1, INPUT);  
   
                            //Sensor infrarrojo B
  pinMode(sensor2, INPUT);  

  pinMode(sensor3t,OUTPUT);
  pinMode(sensor3e, INPUT);
  //pinMode(sensor4t,OUTPUT);
  //pinMode(sensor4e, INPUT);
                                      //Motor A
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
                                       //Motor B
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT);

  
}

void loop() 
{
  SU1=distancia1();
  Busqueda();
  if(SU1<30){
              
            Pelea();
                
                     
              }
}

void Pelea(){
Serial.print("Pelea ");
  do{
  analogWrite(dir1,0); //Avanzar
  analogWrite(dir2,150);
  analogWrite(dir3,0);
  analogWrite(dir4,90);
  
  
  
      SU1=distancia1();
  
      
  }while(SU1<30);
  
  }

void Busqueda(){
  
  //PATRON DE MOTOR "GIRAR EN BUSQUEDA"
  Serial.print("Busqueda \n");
  do{
    
    if(millis()>tiempoahora+periodo){ //Lectura de sensores Infrarrojos cada 0,25segundos
     
    SI1=1; //digitalRead(sensor1);
    SI2=1; //digitalRead(sensor2);
    Serial.print("Lectura infrarojo \n");
    }
    if(millis()>tiempoahora+periodo){ //Empezar lectura de distancia cada 0,25s y solo si los sensores Infrarrojos estan en 0. Esto nos ahorra tiempo en el procesador
      tiempoahora=millis();
      
      Serial.println(SU1);
      Serial.print("sensor ultrasonico en busqueda \n");
      }
    
                    analogWrite(dir1,0);
                    analogWrite(dir2,30);
                    analogWrite(dir3,0);
                    analogWrite(dir4,110);
  

  if(SI2==0){
    //Retroceder
                    analogWrite(dir1,150);
                    analogWrite(dir2,0);
                    analogWrite(dir3,150);
                    analogWrite(dir4,0);
 
  delay(500);
  Serial.print("Retroceder \n");
    }
   
    
   SU1=distancia1();
   
  }while(SU1>30);
  }
  

  int distancia1(){
    digitalWrite(2,LOW);//recibimiento del pulso.
  delayMicroseconds(5);
  digitalWrite(2, HIGH);//envió del pulso.
  delayMicroseconds(10);
  tiem=pulseIn(3, HIGH);//fórmula para medir el pulso entrante.
  dis= long(0.017*tiem);//fórmula para calcular la distancia del sensor ultrasónico.

  return dis;
    }

 /* int distancia2(){
    digitalWrite(11,LOW);//recibimiento del pulso.
    delayMicroseconds(5);
    digitalWrite(11, HIGH);//envió del pulso.
    delayMicroseconds(10);
    tiem=pulseIn(7, HIGH);//fórmula para medir el pulso entrante.
    dis= long(0.017*tiem);//fórmula para calcular la distancia del sensor ultrasónico.

    return dis;
    }  */
