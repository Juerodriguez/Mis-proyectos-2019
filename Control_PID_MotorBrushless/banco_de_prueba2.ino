//Librerias I2C para controlar el mpu6050
//la librería MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include"I2Cdev.h"
#include"MPU6050.h"
#include"Wire.h"
#include"Servo.h"


Servo right_prop;
Servo left_prop;

int ax, ay, az;
float accel_ang_x;
float Kp=0.2, Ki=0.0005, Kd=0.008;
float Setpoint=2, Output, Input;
float elapsedTime, time, timePrev;
int i;
float rad_to_deg = 180/3.141592654;

float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p=0;
float pid_i=0;
float pid_d=0;

double throttle=1100;


// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implícito

MPU6050 sensor; // creamos el objeto sensor

void lectura();




void setup() {
  Serial.begin(57600);
  Wire.begin();
  sensor.initialize();    //Iniciando el sensor
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
 right_prop.attach(10); //attatch the right motor to pin 3
  left_prop.attach(9);  //attatch the left motor to pin 5

  time = millis(); //Start counting time in milliseconds
  //Para iniciar el controlador electronico de Velocidad necesitamos enviarle un pulso constante durante unos 7s hay que recordar que los valores minimos y maximos son 1000 y 2000us
  left_prop.writeMicroseconds(1000); 
  right_prop.writeMicroseconds(1000);
  delay(7000); 
}

void loop() {

timePrev = time;  // tiempo previo de ejecucion antes de guardar el tiempo actual
    time = millis();  // lectura de tiempo actual
    elapsedTime = (time - timePrev) / 1000; // lapso de tiempo

  
  lectura();



error = Input - Setpoint; // se calcula el error entre la lectura y el valor de referencia que queremos configurar.
    
//El valor proporcional es igual a la constante proporcional multiplicado por el error

pid_p = Kp*error;

// La valor integral se lo calcula multiplicando la constante integral por error y sumando los errores anteriores teniendo asi una sumatoria de los errores.
// Existira un rango de valores en los que el banco estara a nivel y por lo tanto en estos no calcularemos el error

if(-1 <error <5)
{
  pid_i = pid_i+(Ki*error);  
}

// El valor derivativo se lo calcula con la diferencial de los errores en el tiempo de cambio multiplicandolo por la constante derivativa

pid_d = Kd*((error - previous_error)/elapsedTime);

// Finalmente se suman los tres valores de PIDs
PID = pid_p + pid_i + pid_d;

//Devido a que los motores que hemos elegido en nuestro caso son de 2kg de empuje estos tienen mucha potencia, por esta razon hemos limitado la acelaracion en 1050 y 1200us y el valor medio
// que hemos elegido para sumar o restar el valor PID es de 1100us.

  if(PID < -300)
{
  PID=-300;
}
if(PID > 300)
{
  PID=300;
}

pwmLeft = throttle + PID;
pwmRight = throttle - PID;



/
//Right
if(pwmRight < 1050)
{
  pwmRight= 1050;
}
if(pwmRight > 1200)
{
  pwmRight=1200;
}
//Left
if(pwmLeft < 1050)
{
  pwmLeft= 1050;
}
if(pwmLeft > 1200)
{
  pwmLeft=1200;
}

Serial.print("Inclinacion izquierda: ");

Serial.println(pwmLeft);

Serial.print("Inclinacion derecha: ");

Serial.println(pwmRight);

// Por ultimo para enviar los valores hacemos uso de lo funcion servo y enviamos los valores al motor izquierdo y el derecho.

left_prop.writeMicroseconds(pwmLeft);
right_prop.writeMicroseconds(pwmRight);
previous_error = error; //Recordar guardar el error previo para el calculo derivativo

  
}

void lectura(){
  sensor.getAcceleration(&ax,&ay,&az);

  //Calcular los angulos de inclinacion con la formula de Euler:
  Input=atan((ay/16384.0)/sqrt(pow((ax/16384.0),2) + pow((az/16384.0),2)))*rad_to_deg;

  //Mostrar los angulos separadas por un [tab]
  
  Serial.print("Inclinacion en X: ");
  Serial.println(Input); 
  
  delay(10);
  
  }
