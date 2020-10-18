#include "include/InterfazReloj.cpp"

//unsigned long timerHora=0;
//unsigned long timerMinuto=0;
bool cambioHora=false;
bool cambioMinuto=false;
unsigned long timer10Segundos=0;
unsigned long timerSegundo=0;
unsigned long timer50mseg=0;

/*
timerHora=millis();
timerMinuto=millis();
timer10Segundo=millis();
timerSegundo=millis();
*/
byte count=0;

void setup() {

  Serial.begin(9600);
  setupDisplays();
  setupRTC();
  funcionCadaMinuto();
  funcionCadaHora();
}

void loop() {
  
  if(millis()>timer50mseg+50){
    leeBotones();
    timer50mseg=millis();
  }
  if(millis()>timerSegundo+1000){
    funcionCadaSegundo();
    timerSegundo=millis();
  }
  if(millis()>timer10Segundos+10000){
    //funcion que se ejecuta cada 10 segundos como máximo
    funcionCada10Segundos();
    timer10Segundos=millis();
  }
  if(now.second()<30){
    if(cambioMinuto){
      //funcion de cada minuto
      funcionCadaMinuto();
      cambioMinuto=false;
    }
  }else{
    cambioMinuto=true;
  }
  if(now.minute()==0){
    if(cambioHora){
      funcionCadaHora();
      cambioHora=false;
    }
  }else{
    cambioHora=true;
  }
}
