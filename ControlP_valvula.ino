#include <TimerOne.h>
int pot;
float frec = 1000;
float ms, s, t, In, Out, Out_1, rk, rk1, w;
int Pot1,Pot2;
float Kp_Vel = 0.1, Ki_Vel = 0.0, Kd_Vel = 0.0; // MODELO MEJORADO PARAMETROS A 2080 RPM
float u_Vel = 0.0, e_Vel = 0.0, upV, uiV, udV, e_1V, ui_1V, u_abs, DutyCicle;

void setup(){
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  Timer1.initialize(400);  // arg = period in us/2 kHz
  

  noInterrupts();
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  TCCR0A |= (1 << WGM01); // CTC mode
  TCCR0B = _BV(CS02)| _BV(CS00); // Clock/1024
  OCR0A = 16000000/1024/frec;
  TIMSK0 |= (1 << OCIE0A);
  interrupts();
  Serial.println("ProcessSignal Reference ControlSignal Error");
}
  
ISR(TIMER0_COMPA_vect){ // timer 0 (cada 1 mS)
  ms = ms+1;
  w = w+1;
  if(ms == 1000.00){
  ms = 0.0;
  s = s+1;
  }
  t = s + ms/1000.00;
  if(w == 500){
    w = 0.0;
    Serial.print(Pot1);
    Serial.print(" ");
    Serial.print(Pot2);
    Serial.print(" ");
    Serial.print(DutyCicle);
    Serial.print(" ");
    Serial.println(e_Vel);
  }
  In = 100;//100.0*sin(2.0*PI*0.5*t);
  //Out = (K*In*(1/frec) + Out_1);
  //Out_1 = Out;

  e_Vel = Pot1 - Pot2; // POT 2 REFERENCIA POT 1 VC 
    //Control Proporcional Velocidad
    upV = Kp_Vel * e_Vel;
    if (upV > 512.0) {upV = 512.0;}
    else if (upV < -512.0) {upV = -512.0;}
    else {upV = upV;}
    //Control Integral Velocidad
    uiV = e_Vel * 0.001 + ui_1V;
    ui_1V = uiV;
    uiV = Ki_Vel * uiV;
    if (uiV > 512.0) {uiV = 512.0;}
    else if (uiV < -512.0) {uiV = -512.0;}
    else {uiV = uiV;}
    //Control Derivativo Velocidad
    udV = (e_Vel - e_1V) / 0.001;
    e_1V = e_Vel;
    udV = Kd_Vel * udV;
    if (udV > 512.0) {udV = 512.0;}
    else if (udV < -512.0) {udV = -512.0;}
    else {udV = udV;}
    u_Vel = upV + uiV + udV + 512;
    if (u_Vel > 1024.0) {u_Vel = 1024.0;}
    else if (u_Vel < 0.0) {u_Vel = 0.0;}
    else {u_Vel = u_Vel;}
    DutyCicle = u_Vel;
}

void loop(){
  //Serial.println(t);
  Pot1 = analogRead(A0); //ARRIBA
  Pot2 = analogRead(A1); //ABAJO
  Timer1.pwm(9, DutyCicle);
  //**************************
  //Serial.print("Valor1: ");
  //Serial.println(Pot1);
  //Serial.print("Valor2: ");
  //Serial.println(Pot2);
  //Serial.print("DutyCicle: ");
  //Serial.println(DutyCicle);
  //**************************
}
