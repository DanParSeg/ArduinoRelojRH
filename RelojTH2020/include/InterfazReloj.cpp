#include <Arduino.h>
#include <EEPROM.h>

#include <Wire.h>
#include "DS3231.h"

#include "Displays.cpp"
#include "sensorTactil.cpp"
#include "rtc.cpp"

void actualizaDisplays();
void leeBotones();




void setupRTC () {
    Wire.begin();
    /*
    Clock.setYear(20);
    Clock.setMonth(10);
    Clock.setDate(17);
    Clock.setDoW(5);
    Clock.setHour(16);
    Clock.setMinute(38);
    Clock.setSecond(0);
    */
}

enum{
  EEPROM_HORARIO_VERANO=19
};

enum{
  E_ENCENDIENDO,

  E_NORMAL,
  E_NORMAL_ALARMA,

  E_AJUSTE_HORA,
  E_AJUSTE_MINUTO,

  E_AJUSTE_DIA,
  E_AJUSTE_MES,
  E_AJUSTE_ANO,

  E_AJUSTE_ALARMA,
  E_AJUSTE_ALARMA_HORA,
  E_AJUSTE_ALARMA_MINUTO,
  E_AJUSTE_ALARMA_DIA
};

RTClib RTC;
DS3231 Clock;
DateTime now;
byte estadoDisplays=E_NORMAL;//TODO arranque
byte estadoAnterior;
sensorTactil botAjuste=sensorTactil(A0);
sensorTactil botMas=sensorTactil(A1);
sensorTactil botMenos=sensorTactil(A2);

void actualizaDiaSemana(){
  int v1=calculaDiaSemana(now.day(),now.month(),now.year());
  if(Clock.getDoW()!=v1);
    Clock.setDoW(v1);
}



void funcionCadaSegundo(){
  //actualiza displays, modificar el punto de la hora
  ajustaBrillo();
  actualizaDisplays();
}
void funcionCada10Segundos(){
  //medir tem hum
}
void funcionCadaMinuto(){
  //comprobar alarmas
}
void funcionCadaHora(){
  //horarioverano/invierno
  now = RTC.now();

  actualizaDiaSemana();

  bool hVerano=horarioVerano(now.hour(), now.day(), now.month(), now.year());
  if(hVerano!=EEPROM.read(EEPROM_HORARIO_VERANO)){
    if(hVerano){
      Clock.setHour(now.hour()+1);
    }
    else{
      Clock.setHour(now.hour()-1);
    }
    Serial.print(hVerano);
    Serial.println(" escribiendo en eeprom");
    EEPROM.write(EEPROM_HORARIO_VERANO, hVerano);
  }
}



void leeBotones(){
  now = RTC.now();
  botAjuste.check();
  botMas.check();
  botMenos.check();
  int v1;
  if(estadoDisplays!=estadoAnterior){
    actualizaDisplays();
  }
  switch(estadoDisplays){
    case E_NORMAL:
      if(botAjuste.pulsacionLarga&&botMas.pulsacionLarga&&botMenos.pulsacionLarga){
        estadoDisplays=E_AJUSTE_HORA;
      }
      break;
    case E_AJUSTE_HORA://TODO pulsacion media para las horas
      if(botMas.flancoSubida){
        v1=(now.hour()+1)%24;
        Clock.setHour(v1);
        actualizaDisplays();
      }
      else if(botMenos.flancoSubida){
        v1=now.hour()-1;
        if(v1<0) v1=23;
        Clock.setHour(v1);
        actualizaDisplays();
      }
      else if(botAjuste.flancoSubida){
        estadoDisplays=E_AJUSTE_MINUTO;
      }

      break;
    case E_AJUSTE_MINUTO:
      if(botMas.flancoSubida||botMas.pulsacionMedia){
        v1=(now.minute()+1)%60;
        Clock.setMinute(v1);
        Clock.setSecond(0);
        actualizaDisplays();
      }
      else if(botMenos.flancoSubida||botMenos.pulsacionMedia){
        v1=now.minute()-1;
        if(v1<0) v1=59;
        Clock.setMinute(v1);
        Clock.setSecond(0);
        actualizaDisplays();
      }
      if(botAjuste.flancoSubida){
        estadoDisplays=E_AJUSTE_DIA;
      }

      break;
    case E_AJUSTE_DIA:
      if(botMas.flancoSubida||botMas.pulsacionMedia){
        v1=now.day()+1;
        if(v1>31)v1=1;
        Clock.setDate(v1);
        actualizaDisplays();
      }
      else if(botMenos.flancoSubida||botMenos.pulsacionMedia){
        v1=(now.day()-1);
        if(v1<1)v1=31;
        Clock.setDate(v1);
        actualizaDisplays();
      }
      if(botAjuste.flancoSubida){
        estadoDisplays=E_AJUSTE_MES;
      }
      break;
    case E_AJUSTE_MES:
      if(botMas.flancoSubida||botMas.pulsacionMedia){
        v1=now.month()+1;
        if(v1>12)v1=1;
        Clock.setMonth(v1);
        actualizaDisplays();
      }
      else if(botMenos.flancoSubida||botMenos.pulsacionMedia){
        v1=(now.month()-1);
        if(v1<1)v1=12;
        Clock.setMonth(v1);
        actualizaDisplays();
      }
      if(botAjuste.flancoSubida){
        estadoDisplays=E_AJUSTE_ANO;
      }
      break;
    case E_AJUSTE_ANO:
      if(botMas.flancoSubida||botMas.pulsacionMedia){
        v1=now.year()+1;
        Clock.setYear(v1-2000);
        actualizaDisplays();
      }
      else if(botMenos.flancoSubida||botMenos.pulsacionMedia){
        v1=now.year()-1;
        Clock.setYear(v1-2000);
        actualizaDisplays();
      }
      if(botAjuste.flancoSubida){
        estadoDisplays=E_NORMAL;
      }
      actualizaDiaSemana();
      break;
  }//switch(estadoDisplays)
}

void actualizaDisplays(){
  now = RTC.now();
  switch(estadoDisplays){
    case E_ENCENDIENDO:
      break;
    case E_NORMAL:
      poneHora(now.hour(),now.minute(),now.second()%2);
      if(now.minute()%2){
        poneFechaDia(Clock.getDoW(),now.day());
      }else{
        poneFechaMes(now.day(),now.month());
      }
      poneTextoDisplay(displayTemperatura,CHAR_V,CHAR_E,CHAR_r,CHAR_A);
      poneTextoDisplay(displayHumedad,CHAR_I,CHAR_n,CHAR_v,CHAR_i);
      //displayTemperatura.clear();//TODO temperatura y humedad
      //displayHumedad.clear();
      break;
    case E_NORMAL_ALARMA:
      break;

    case E_AJUSTE_HORA:
      poneHora(now.hour(),now.minute(),true);
      if(estadoDisplays!=estadoAnterior){
        poneTextoDisplay(displayTemperatura,CHAR_H,CHAR_o,CHAR_r,CHAR_A);
        if(EEPROM.read(EEPROM_HORARIO_VERANO)){
          poneTextoDisplay(displayHumedad,CHAR_V,CHAR_E,CHAR_r,CHAR_A);
        }
        else{
          poneTextoDisplay(displayHumedad,CHAR_I,CHAR_n,CHAR_v,CHAR_i);
        }

      }
      break;
    case E_AJUSTE_MINUTO:
      poneHora(now.minute(),now.second(),true);
      if(estadoDisplays!=estadoAnterior){
        poneTextoDisplay(displayTemperatura,CHAR_M,CHAR_i,CHAR_n,CHAR_u);
      }
      break;
    case E_AJUSTE_DIA:
      poneFechaMes(now.day(),now.month());
      if(estadoDisplays!=estadoAnterior){
        displayHora.clear();
        poneTextoDisplay(displayTemperatura,CHAR_D,CHAR_i,CHAR_A,CHAR_SPACE);
      }
      break;
    case E_AJUSTE_MES:
      poneFechaMes(now.day(),now.month());
      if(estadoDisplays!=estadoAnterior){
        displayHora.clear();
        poneTextoDisplay(displayTemperatura,CHAR_M,CHAR_E,CHAR_S,CHAR_SPACE);
      }
      break;
    case E_AJUSTE_ANO:
      displayAlarma.showNumberDec(now.year());//0x80 >>1: añadir punto decimal pos 1,  true: leading 0s
      if(estadoDisplays!=estadoAnterior){
        displayHora.clear();
        poneTextoDisplay(displayTemperatura,CHAR_A,CHAR_ENE,CHAR_o,CHAR_SPACE);
      }
      break;

    case E_AJUSTE_ALARMA:
      break;
    case E_AJUSTE_ALARMA_HORA:
      break;
    case E_AJUSTE_ALARMA_MINUTO:
      break;
    case E_AJUSTE_ALARMA_DIA:
      break;
  }
  estadoAnterior=estadoDisplays;
}
