#define CHA   27
#define CHB   26
#define PWM   25
#define DIR1  33
#define DIR2  32
#define STBY  12

#include "INA226.h"

INA226 INA(0x40);

/*
  MOTOR 37D     0.0333
	MOTOR 25D     0.0470
*/

int DIR;
double Ts = 0.01;
const unsigned long Ts2 = 10000;
double enAcc = 0;
double theta_n = 0, theta_n1 = 0;
double omega_n = 0;
double crrnt_n = 0;
float t = 0;
float value;
hw_timer_t * timer = NULL;
int timer_flag = 0;

typedef union {
  float number;
  uint8_t bytes[4];
} valor;

valor theta_s_ML, omega_s_ML, crrnt_s_ML,Tiempo_ML;
float theta_s, omega_s, crrnt_s;
unsigned long timeold,timeOld2 = 0;
unsigned long timeNow;
/*
void IRAM_ATTR timer_ISR(){
  //t = t + Ts;
  if(t>=0 && t<10) {analogWrite(PWM,0);}
  else if(t>=10 && t<20) {analogWrite(PWM,64);}
  else if(t>=20 && t<30) {analogWrite(PWM,0);}  
  else if(t>=30 && t<40) {analogWrite(PWM,128); }   
  else if(t>=40 && t<50) {analogWrite(PWM,0); }     
  else if(t>=50 && t<60) {analogWrite(PWM,192); }   
  else if(t>=60 && t<70) {analogWrite(PWM,0); }     
  else if(t>=70 && t<80) {analogWrite(PWM,255); }    
  else if(t>=80 && t<90) {analogWrite(PWM,0); }      
  else if(t>=90) {t = 0;}
  timer_flag = 1;
}
*/
void IRAM_ATTR CHA_ISR(){
  DIR = (digitalRead(CHA)==digitalRead(CHB))?-1:1;
  enAcc += DIR;
}

void IRAM_ATTR CHB_ISR(){
  DIR = (digitalRead(CHA)==digitalRead(CHB))?1:-1;
  enAcc += DIR; 
}

void setup(){
  Serial.begin(115200);
  pinMode(CHA, INPUT_PULLUP);
  pinMode(CHB, INPUT_PULLUP);
  pinMode(STBY,OUTPUT);
  pinMode(PWM,OUTPUT);
  pinMode(DIR1,OUTPUT);
  pinMode(DIR2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), CHA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(CHB), CHB_ISR, RISING);

  digitalWrite(STBY,HIGH);
  digitalWrite(DIR1,LOW);
  digitalWrite(DIR2,HIGH); 
  //analogWrite(PWM,50); 
  delay(5000);

  INA.begin();
  INA.setMaxCurrentShunt(0.8, 0.01);
  //timer =timerBegin(1000000);
  //timerAttachInterrupt(timer, &timer_ISR);
  //timerAlarm(timer, 10000, true, 0);
  timeOld2 = micros();
} 
float u_n ;
void loop(){
  if(Serial.available() > 0){value = recepcion();}
  if (value == 1){/*
  if(t>=0 && t<10) {analogWrite(PWM,0);}
  else if(t>=10 && t<20) {analogWrite(PWM,64);}
  else if(t>=20 && t<30) {analogWrite(PWM,0);}  
  else if(t>=30 && t<40) {analogWrite(PWM,128); }   
  else if(t>=40 && t<50) {analogWrite(PWM,0); }     
  else if(t>=50 && t<60) {analogWrite(PWM,192); }   
  else if(t>=60 && t<70) {analogWrite(PWM,0); }     
  else if(t>=70 && t<80) {analogWrite(PWM,255); }    
  else if(t>=80 && t<90) {analogWrite(PWM,0); }      
  else if(t>=90) {t = 0;}*/
  u_n = 255*sin(2*pi*0.03*t);
  if(u_n >= 0)
  {
    digitalWrite(DIR1,LOW);
    digitalWrite(DIR2,HIGH); 
  }
  else
  {
    digitalWrite(DIR1,HIGH);
    digitalWrite(DIR2,LOW); 
  }
  analogWrite(PWM,fabs(u_n))
  }
  else{t=0.0;}

  timeNow = micros();
  if (timeNow - timeOld2 >= Ts2) {
    timeOld2 = timeNow;
    t = t + (Ts2 / 1e6);  // Suma Ts en segundos (puedes usar Ts/1000.0 si lo quieres en milisegundos)
  }
  //if(timer_flag == 1){0.0330 Motores gordos theta_n
  if (millis()-timeold >= 10){
		theta_n = ((enAcc/22.0) * 0.0470)*2*3.1416; // Una revolución del encoder 0.22  --- 0.0470 revolución del eje (Unidades en revolución) * 2pi en radianes
		omega_n = (theta_n - theta_n1)/0.01;
    crrnt_n = INA.getCurrent_mA() / 1000.0;
    theta_n1 = theta_n;
    theta_s_ML.number = theta_n;    
    omega_s_ML.number = omega_n;    
    crrnt_s_ML.number = crrnt_n;
    Tiempo_ML.number = t;
    Serial.write('V');
    for (int i = 0; i < 4; i++) Serial.write(theta_s_ML.bytes[i]);
    for (int i = 0; i < 4; i++) Serial.write(omega_s_ML.bytes[i]);
    for (int i = 0; i < 4; i++) Serial.write(crrnt_s_ML.bytes[i]);
    for (int i = 0; i < 4; i++) Serial.write(Tiempo_ML.bytes[i]);
    Serial.write('\n');
    timer_flag = 0;
    timeold = millis();
  }
  //}
}

//Recibir Flotante
float recepcion(){
  int i;
  valor buf;
  for(i=0; i<4; i++)
    buf.bytes[i] = Serial.read();  
  return buf.number;
}
//2 eXPERIMENTOS, PRIMERO ESCALONES Y GUARDAR LA DATA, SEGUNDO UNA SEÑAL SENO VER LA FRICCIÓN ESTÁTICA Y SUS EFECTOS
