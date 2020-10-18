#include <Arduino.h>
#include <TM1637Display.h>
#include "caracteres.h"

// Module connection pins (Digital Pins)
#define CLK_DISPLAY 2
#define DIO_HORA 4
#define DIO_ALAR 7
#define DIO_TEMP 8
#define DIO_HUME 12
#define PIN_LDR A3

TM1637Display displayHora(CLK_DISPLAY, DIO_HORA);
TM1637Display displayAlarma(CLK_DISPLAY, DIO_ALAR);
TM1637Display displayTemperatura(CLK_DISPLAY, DIO_TEMP);
TM1637Display displayHumedad(CLK_DISPLAY, DIO_HUME);
uint8_t * caracter =new uint8_t[1];

void ajustaBrillo(){
  int luz=analogRead(PIN_LDR);
  byte brillo;

  if(luz>1000){
    brillo=3;
  }
  else if(luz>800){
    brillo=2;
  }
  else if(luz>600){
    brillo=1;
  }
  else{
    brillo=0;
  }
  Serial.print("luz ");
  Serial.println(luz);
  Serial.print(" brillo ");
  Serial.println(brillo);
  displayHora.setBrightness(brillo);
  displayAlarma.setBrightness(brillo+1);
  displayTemperatura.setBrightness(brillo);
  displayHumedad.setBrightness(brillo+1);

}

void poneHora(int h, int m, bool p){
  byte punto=5;//para que no represente el punto
  if(p) punto=1;
  if(h<10){
    caracter[0]=CHAR_SPACE;
    displayHora.setSegments(caracter,1,0);//1: longitud 3: posicion
    displayHora.showNumberDecEx(h*100+m, (0x80 >>(punto-1)), true, 3, 1);//0x80 >>1: añadir punto decimal pos 1 (a partir de donde escribe),  true: leading 0s
  }
  else{
    displayHora.showNumberDecEx(h*100+m, (0x80 >>punto), true);//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s
  }
}

void poneAlarma(int h, int m){
  if(h<10){
    caracter[0]=CHAR_SPACE;
    displayAlarma.setSegments(caracter,1,0);//1: longitud 3: posicion
    displayAlarma.showNumberDecEx(h*100+m, (0x80 >>0), true, 3, 1);//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s
  }
  else{
    displayAlarma.showNumberDecEx(h*100+m, (0x80 >>1), true);//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s
  }
}

void poneTemperatura(float t){
  displayTemperatura.showNumberDecEx(round(t*10), (0x80 >>1), true,3);//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s, 3:solo manda 3 digitos
  caracter[0]=CHAR_GRADO;
  displayTemperatura.setSegments(caracter,1,3);//1: longitud 3: posicion
}

void poneHumedad(float h){
  displayHumedad.showNumberDecEx(round(h*10), (0x80 >>1), true,3);//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s,  3:solo manda 3 digitos
  caracter[0]=CHAR_h;
  displayHumedad.setSegments(caracter,1,3);//1: longitud 3: posicion
}

void poneFechaDia(int ds, int d){
  uint8_t * c =new uint8_t[4];
  c[2]=displayAlarma.encodeDigit(d/10);
  c[3]=displayAlarma.encodeDigit(d%10);
  switch(ds){
    case 0:
      c[0]=CHAR_L;
      c[1]=CHAR_u;
      break;
    case 1:
      c[0]=CHAR_M;
      c[1]=CHAR_A;
      break;
    case 2:
      c[0]=CHAR_M;
      c[1]=CHAR_i;
      break;
    case 3:
      c[0]=CHAR_J;
      c[1]=CHAR_u;
      break;
    case 4:
      c[0]=CHAR_V;
      c[1]=CHAR_i;
      break;
    case 5:
      c[0]=CHAR_S;
      c[1]=CHAR_A;
      break;
    case 6:
      c[0]=CHAR_D;
      c[1]=CHAR_o;
      break;
  }
  displayAlarma.setSegments(c,4,0);
  delete[] c;
}

void poneTextoDisplay(TM1637Display& display, byte c0, byte c1, byte c2, byte c3){
  uint8_t * c =new uint8_t[4];
  c[0]=c0;
  c[1]=c1;
  c[2]=c2;
  c[3]=c3;
  display.setSegments(c,4,0);
  delete[] c;
}

void poneFechaMes(int d, int m){
  uint8_t * c =new uint8_t[4];
  c[0]=displayAlarma.encodeDigit(d/10);
  c[1]=displayAlarma.encodeDigit(d%10);
  switch(m){
    case 1:
      c[2]=CHAR_E;
      c[3]=CHAR_n;
      break;
    case 2:
      c[2]=CHAR_F;
      c[3]=CHAR_E;
      break;
    case 3:
      c[2]=CHAR_M;
      c[3]=CHAR_r;
      break;
    case 4:
      c[2]=CHAR_A;
      c[3]=CHAR_b;
      break;
    case 5:
      c[2]=CHAR_M;
      c[3]=CHAR_y;
      break;
    case 6:
      c[2]=CHAR_J;
      c[3]=CHAR_u;
      break;
    case 7:
      c[2]=CHAR_J;
      c[3]=CHAR_L;
      break;
    case 8:
      c[2]=CHAR_A;
      c[3]=CHAR_g;
      break;
    case 9:
      c[2]=CHAR_S;
      c[3]=CHAR_E;
      break;
    case 10:
      c[2]=CHAR_O;
      c[3]=CHAR_c;
      break;
    case 11:
      c[2]=CHAR_N;
      c[3]=CHAR_o;
      break;
    case 12:
      c[2]=CHAR_D;
      c[3]=CHAR_i;
      break;
  }
  displayAlarma.setSegments(c,4,0);
  delete[] c;
}

void setupDisplays() {
  byte lum=2;
  // put your setup code here, to run once:
  displayHora.setBrightness(lum);
  displayHora.clear();
  displayAlarma.setBrightness(lum+1);
  displayAlarma.clear();
  displayTemperatura.setBrightness(lum);
  displayTemperatura.clear();
  displayHumedad.setBrightness(lum+1);
  displayHumedad.clear();
}

void pruebaDisplays() {
  // put your main code here, to run repeatedly:


  for(int i=0;true;i++){
    //poneAlarma(i%24,i%60);
    poneHora(i%24,i%60,i%2);
    poneFechaMes(i%32,i%11+1);
    poneTemperatura((i%1000)/10.0);
    poneHumedad((i%1000)/10.0);
    //delay(1);
  }

  /*
  for(int i=0; i<7; i++){
    poneFechaDia(i,14);
    delay(200);
  }
  for(int i=1; i<13; i++){
    poneFechaMes(14,i);
    delay(2000);
  }
  */




}
