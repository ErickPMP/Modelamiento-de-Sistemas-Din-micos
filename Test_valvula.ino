// CONEXIONES
//BLANCO 7
//AZUL 6
//MARRON GND
//ROJO 5V
//AMARILLO A0 POTEN ARRIBA
//AZUL BLANCO A1 POTEN ABAJO
//****************************
#include <TimerOne.h>
//#include <PWM.h>
int pot;
float frec = 1000;
float ms, s, t, In, Out, Out_1, rk, rk1;
int pot1,pot2;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 500;  //the value is a number of milliseconds
//int32_t frequency = 2000;
void setup(){
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  //InitTimersSafe(); 

  //Establece la frecuencia para el pin especificado
  //bool success = SetPinFrequencySafe(9, frequency);

  pinMode(9, OUTPUT);
  Timer1.initialize(400);  // arg = period in us/2 kHz
  Timer1.pwm(9, 512);

  noInterrupts();
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  TCCR0A |= (1 << WGM01); // CTC mode
  TCCR0B = _BV(CS02)| _BV(CS00); // Clock/1024
  OCR0A = 16000000/1024/frec;
  TIMSK0 |= (1 << OCIE0A);
  interrupts();
}
  
ISR(TIMER0_COMPA_vect){ // timer 0 (cada 1 mS)
  ms = ms+1;
  if(ms == 1000.00){
  ms = 0.0;
  s = s+1;
  }
  t = s + ms/1000.00;
  In = 100;//100.0*sin(2.0*PI*0.5*t);
  //Out = (K*In*(1/frec) + Out_1);
  //Out_1 = Out;
}

void loop(){
  Serial.println(t);
  //pwmWrite(9, sensorValue / 4); 
  pot1 = analogRead(A0); //ARRIBA
  pot2 = analogRead(A1); //ABAJO
  Serial.print("Valor1: ");
  Serial.println(pot1);
  Serial.print("Valor2: ");
  Serial.println(pot2);
  //if (pot1 <= 500){
  //  digitalWrite(6,HIGH);
  //}
  //else{
  //  digitalWrite(6,LOW);
  //}
  if (pot2 <= 500){
    digitalWrite(7,LOW);
    digitalWrite(6,HIGH);
  }
  else{
    digitalWrite(7,HIGH);
    digitalWrite(6,LOW);
  }
}
