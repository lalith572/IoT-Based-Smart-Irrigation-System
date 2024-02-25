#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>  
#include <Wire.h>
#include <SimpleTimer.h>
#include <SPI.h>

int X;
int Y;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
int Flowsensor = D2;

char auth[] = "";//Enter your Auth token
char ssid[] = "";//Enter your WIFI name
char pass[] = "";//Enter your WIFI password

DHT dht(D5, DHT11); //(sensor pin,sensor type)
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3
WidgetLCD lcd(V2); 
WidgetLED LED1Blynk(V5);
#define PIR D4
//D4 PIR Motion Sensor
int PIR_ToggleValue;

void checkPhysicalButton();
int relay1State = LOW;
int pushButton1State = HIGH;
#define RELAY_PIN_1       D3   //D3 Relay
#define PUSH_BUTTON_1     D7   //D7 Button
#define VPIN_BUTTON_1    V12 

BlynkTimer timer;
//dht
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

//Get the button value
BLYNK_WRITE(V12) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
  } else {
    digitalWrite(waterPump, HIGH);
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V3, value);
}

//Get the PIR sensor values
void PIRsensor() {
  bool value = digitalRead(PIR);
  if (value) {
    Blynk.logEvent("pirmotion","WARNNG! Motion Detected!"); //Enter your Event Name
    WidgetLED LED(V5);
    LED.on();
  } else {
    WidgetLED LED(V5);
    LED.off();
  }  
  }

BLYNK_WRITE(V6)
{
 PIR_ToggleValue = param.asInt();  
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE_2(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

void checkPhysicalButton()
{
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (pushButton1State != LOW) {

      // Toggle Relay state
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}

void setup() {

   pinMode(PIR, INPUT);

  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);


  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  pinMode(Flowsensor,INPUT_PULLUP); 

  Wire.begin(D2, D1);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, sendSensor);
  timer.setInterval(500L, checkPhysicalButton);
}

void loop() {

if (PIR_ToggleValue == 1){
      PIRsensor();
}
else{
    WidgetLED LED(V5);
    LED.off();
}
Blynk.run();
X = pulseIn(Flowsensor, HIGH);
Y = pulseIn(Flowsensor, LOW);
TIME = X + Y;
FREQUENCY = 1000000/TIME;
WATER = FREQUENCY/7.5;
LS = WATER/60;
if(FREQUENCY >= 0)
{
if(isinf(FREQUENCY))
{
    Serial.println(TIME);
    Blynk.virtualWrite(V2,TOTAL,  "L" );
    lcd.clear(); 
    lcd.print(5,0," Water"); 
    lcd.print(2,1,"Not Flowing!"); 
    LED1Blynk.off(); 
    Blynk.virtualWrite(V3,TIME/100);
}
else
{
    TOTAL = TOTAL + LS;
    Blynk.virtualWrite(V3,TIME/100);
    Serial.println(FREQUENCY);
    lcd.clear(); 
    lcd.print(4,0," Water"); 
    lcd.print(4,1,"Flowing!");
    LED1Blynk.on(); 
    Serial.println(WATER);
    Serial.println(TOTAL);
    Blynk.virtualWrite(V4,TOTAL,  "L/H" );
}
delay(1000);
  }

  timer.run();
}