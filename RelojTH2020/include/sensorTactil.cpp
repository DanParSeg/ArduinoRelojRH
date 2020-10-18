//1M 33nF tr=100
enum{
  PERIODO=50//ms periodo de la llamada a check (lecturas de los pulsadores)
};

class sensorTactil{
  public:
    sensorTactil(byte in){
      input=in;
    }
    void check(){
      if(analogRead(input)>100){
        flancoSubida=false;
        if(!pulsado){
          flancoSubida=true;
        }
        pulsado=true;
        if(count<255)count++;
      }
      else{
        count=0;
        pulsado=false;
        flancoSubida=false;
        pulsacionMedia=false;
        pulsacionLarga=false;
      }
      if(count*PERIODO>=500){//200ms de pulsacion media
        pulsacionMedia=true;
      }
      if(count*PERIODO>=1000){
        pulsacionLarga=true;
      }
    }
    bool pulsado=false;
    bool flancoSubida=false;
    bool pulsacionMedia=false;
    bool pulsacionLarga=false;
    byte input;
    byte count=0;
};
/*
sensorTactil but1=sensorTactil(A0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  /*
  Serial.println(count);
  if(analogRead(A0)){
    digitalWrite(LED_BUILTIN,HIGH);
    if(!lastState)count++;
    lastState=true;
  }
  else{
    digitalWrite(LED_BUILTIN,LOW);
    lastState=false;
  }
  delay(10);
  */
  /*
  bool lastState=false;
  int count;
  but1.check();
  delay(100);
  if(but1.pulsacionLarga) digitalWrite(LED_BUILTIN,HIGH);
  else digitalWrite(LED_BUILTIN,LOW);

}
*/
