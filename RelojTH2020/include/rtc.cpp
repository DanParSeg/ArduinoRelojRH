bool horarioVerano(byte hora, byte dia, byte mes, int ano){
  //mes 1-12
  //ano es 4 digitos
  //hay que llamar a esta función cada hora
  //=31-MOD((INT((5*ano)/4)+4);7) marzo
  //=31-MOD((INT((5*ano)/4)+1);7) octubre

  if(mes<3||mes>10)
    return false;
  if(mes>3&&mes<10)
    return true;
  byte diaCambio;
  if(mes==3){
    diaCambio=31-((5*ano)/4+4)%7;
    if(dia<diaCambio||(dia==diaCambio&&hora<2)){
      return false;
    }
    else{
      return true;
    }
  }
  if(mes==10){
    diaCambio=31-((5*ano)/4+1)%7;
    if(dia<diaCambio||(dia==diaCambio&&hora<3)){
      return true;
    }
    else{
      return false;
    }
  }
}

byte calculaDiaSemana(byte d, byte m, int y)  /* 1 <= m <= 12,  y > 1752 (in the U.K.) */
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return ((y + y/4 - y/100 + y/400 + t[m-1] + d)-1) % 7;
}



/*
void pruebaRTC () {

    delay(1000);

    DateTime now = RTC.now();
    Serial.print('/');

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    /*
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    */
  /*
    // Test of alarm functions
    // set A1 to one minute past the time we just set the clock
    // on current day of week.
    Clock.setA1Time(DoW, Hour, Minute+1, Second, 0x0, true,
      false, false);
    // set A2 to two minutes past, on current day of month.
    Clock.setA2Time(Date, Hour, Minute+2, 0x0, false, false,
      false);
    // Turn on both alarms, with external interrupt
    Clock.turnOnAlarm(1);
    Clock.turnOnAlarm(2);
    *//*
}
*/
