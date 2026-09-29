int led = 7;
int d = 0;
int p;
bool inc = true;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  p = set_period(p);
  d = set_duty(d);
  

  int t = map(d,0,100,0,p);

  if(5000/p > 0){
    for(int i=0; i<5000/p; i++){
      digitalWrite(led, HIGH);
      delayMicroseconds(t);

      digitalWrite(led, LOW);
      delayMicroseconds(p-t);
    }
  }
  else{
    for(int i=0; i<=5000/p; i++){
      digitalWrite(led, HIGH);
      delayMicroseconds(t);

      digitalWrite(led, LOW);
      delayMicroseconds(p-t);
    }
  }
}

int set_period(int period){         //주기
  //period = 100;
  //period = 1000;
   cperiod = 10000;
  return period;
}

int set_duty(int duty){             //단계
  if(5000/p > 0){
    if(inc == true){
      if(duty < 100){
        duty += 1;
      }
      else{
        inc = false;
      }
    }
    else{
      if(duty > 0){
        duty -= 1;
      }
      else{
      inc = true;
      }
    }
  }
  else{
    if(inc == true){
      if(duty < 100){
        duty += 2;
      }
      else{
        inc = false;
      }
    }
    else{
      if(duty > 0){
        duty -= 2;
      }
      else{
      inc = true;
      }
    }
  }
  return duty;
}
