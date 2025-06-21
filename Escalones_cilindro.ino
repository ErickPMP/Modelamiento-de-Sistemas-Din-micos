#include <TimerOne.h>
int pot;
float frec = 1000;
float ms, s, t, In, Out, Out_1, rk, rk1, w;
float SPo=512.0;;
int Pot1,Pot2;
float Kp_Vel = 0.1, Ki_Vel = 0.0, Kd_Vel = 0.0; // MODELO MEJORADO PARAMETROS A 2080 RPM
float u_Vel = 0.0, e_Vel = 0.0, upV, uiV, udV, e_1V, ui_1V, u_abs, DutyCicle;
unsigned long timeold,timeOld2 = 0;
unsigned long timeNow;
const unsigned long Ts = 10000;

typedef union {
  float number;
  uint8_t bytes[4];
} valor;
valor SPo_ML, Pot1_ML;

void setup(){
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  Timer1.initialize(400);  // arg = period in us/2 kHz
  timeOld2 = micros();
}

void loop(){
  Pot1 = analogRead(A0); //ARRIBA
  Pot2 = analogRead(A1); //ABAJO
  Timer1.pwm(9, SPo);
  /*
  if (t<=10.0){SPo=512.0;}
  else if (t>10.0 && t<=20.0){SPo=640.0;}
  else if (t>20.0 && t<=30.0){SPo=384.0;}
  else if (t>30.0 && t<=40.0){SPo=768.0;}
  else if (t>40.0 && t<=50.0){SPo=256.0;}
  else if (t>50.0 && t<=60.0){SPo=896.0;}
  else if (t>60.0 && t<=70.0){SPo=128.0;}
  else if (t>70.0 && t<=80.0){SPo=1024.0;}
  else if (t>80.0 && t<=90.0){SPo=0.0;}
  else if (t>90.0){SPo=640.0;}
  */
/*
  if (t<=10.0){SPo=512.0;}
  else if (t>10.0 && t<=20.0){SPo=640.0;}
  else if (t>20.0 && t<=30.0){SPo=512.0;}
  else if (t>30.0 && t<=40.0){SPo=384.0;}
  else if (t>40.0 && t<=50.0){SPo=512.0;}
  else if (t>50.0 && t<=60.0){SPo=768.0;}
  else if (t>60.0 && t<=70.0){SPo=512.0;}
  else if (t>70.0 && t<=80.0){SPo=256.0;}
  else if (t>80.0 && t<=90.0){SPo=512.0;}
  else if (t>90.0 && t<=100.0){SPo=896.0;}
  else if (t>100.0 && t<=110.0){SPo=512.0;}
  else if (t>110.0 && t<=120.0){SPo=128.0;}
  else if (t>120.0 && t<=130.0){SPo=512.0;}
  else if (t>130.0 && t<=140.0){SPo=1024.0;}
  else if (t>140.0 && t<=150.0){SPo=512.0;}
  else if (t>150.0 && t<=160.0){SPo=0.0;}
  else if (t>160.0 && t<=170.0){SPo=512.0;}
  else if (t>170.0){SPo=640.0;}
*/
  /*
  if (t<=10.0){SPo=512.0;}
  else if (t>10.0 && t<=20.0){SPo=544.0;}
  else if (t>20.0 && t<=30.0){SPo=480.0;}
  else if (t>30.0 && t<=40.0){SPo=576.0;}
  else if (t>40.0 && t<=50.0){SPo=448.0;}
  else if (t>50.0 && t<=60.0){SPo=608.0;}
  else if (t>60.0 && t<=70.0){SPo=416.0;}
  else if (t>70.0 && t<=80.0){SPo=640.0;}
  else if (t>80.0 && t<=90.0){SPo=384.0;}
  else if (t>90.0){SPo=640.0;}
*/
    if (t<=10.0){SPo=512.0;}
  else if (t>10.0 && t<=40.0){SPo=480.0;}
  else if (t>40.0 && t<=50.0){SPo=512.0;}
  else if (t>50.0 && t<=80.0){SPo=528.0;} //Retraccción
  else if (t>80.0 && t<=90.0){SPo=512.0;}
  else if (t>90.0 && t<=110.0){SPo=448.0;} //Sale
  else if (t>110.0 && t<=120.0){SPo=512.0;}
  else if (t>120.0 && t<=130.0){SPo=544.0;}
  else if (t>130.0 && t<=140.0){SPo=512.0;}
  else if (t>140.0 && t<=150.0){SPo=416.0;}
  else if (t>150.0 && t<=160.0){SPo=512.0;}
  else if (t>160.0 && t<=170.0){SPo=560.0;}
  else if (t>170.0 && t<=180.0){SPo=512.0;}
  else if (t>180.0 && t<=190.0){SPo=384.0;}
  else if (t>190.0 && t<=200.0){SPo=512.0;}
  else if (t>200.0 && t<=210.0){SPo=576.0;}
  else if (t>210.0 && t<=220.0){SPo=512.0;}
  else if (t>220.0){SPo=576.0;}
  
  timeNow = micros();
    if (timeNow - timeOld2 >= Ts){
        timeOld2 = timeNow;
        t = t + (Ts / 1e6);  // Suma Ts en segundos (puedes usar Ts/1000.0 si lo quieres en milisegundos)
    }
    
    
   if (millis()-timeold >= 10){
      SPo_ML.number = SPo;
      Pot1_ML.number = Pot1;
      Serial.write('V');
      for (int i = 0; i < 4; i++) Serial.write(SPo_ML.bytes[i]);
      for (int i = 0; i < 4; i++) Serial.write(Pot1_ML.bytes[i]);
      Serial.write('\n');
      timeold = millis();
    }

}
