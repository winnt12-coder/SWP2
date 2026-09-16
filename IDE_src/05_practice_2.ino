#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial){
    ;
  }
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}

void loop() {
    for(int i = 0; i < 10; i++){
        toggle = toggle_state(toggle);
        digitalWrite(PIN_LED, toggle);
        delay(200);
    }   
    while(1){
      toggle = 1;
      digitalWrite(PIN_LED, toggle);
    }
}

int toggle_state(int toggle){
    if (toggle == 0) {
       return 1;
    }
    else {
       return 0;
    }
}
