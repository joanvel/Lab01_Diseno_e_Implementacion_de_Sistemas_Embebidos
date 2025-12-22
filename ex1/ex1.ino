#define LED0 19
#define LED1 18
#define LED2 5
#define LED3 17
#define BUTTON 34
#define BOUNCE_WAIT 50
#define LED_WAIT 500


enum state {
  INIT,
  FIRST_PRSS_WAIT,
  RUN,
  SECOND_PRSS_WAIT,
  PAUSE,
};

bool led0,led1,led2,led3 = 0;

state current_state = INIT;
unsigned long last_time = millis();

void setup() {
  Serial.begin(9600);
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(BUTTON, INPUT);

  digitalWrite(LED0, led0);
  digitalWrite(LED1, led0);
  digitalWrite(LED2, led0);
  digitalWrite(LED3, led0);
}

void loop() {
  bool aux = led0;
  switch(current_state)
  {
    case INIT:
      led0 = true;
      if (digitalRead(BUTTON)) current_state = FIRST_PRSS_WAIT;
      Serial.println("INIT");
      
      break;
    case FIRST_PRSS_WAIT:
      current_state = RUN;
      delay(BOUNCE_WAIT);
      Serial.println("FIRST_PRSS_WAIT");
      while(digitalRead(BUTTON));
      delay(BOUNCE_WAIT);
      break;
    case RUN:
      digitalWrite(LED0, led0);
      digitalWrite(LED1, led1);
      digitalWrite(LED2, led2);
      digitalWrite(LED3, led3);

      if(millis() - last_time > LED_WAIT)
      {
        led0 = led1;
        led1 = led2;
        led2 = led3;
        led3 = aux;
        last_time = millis();
      }

      if (digitalRead(BUTTON)) current_state = SECOND_PRSS_WAIT;
      Serial.println("RUN");
      break;
    case SECOND_PRSS_WAIT:
      current_state = PAUSE;
      delay(BOUNCE_WAIT);
      Serial.println("SECOND_PRSS_WAIT");
      while(digitalRead(BUTTON));
      delay(BOUNCE_WAIT);
      break;
    case PAUSE:
      if (digitalRead(BUTTON)) current_state = FIRST_PRSS_WAIT;
      Serial.println("PAUSE");
      break;
  }
}
