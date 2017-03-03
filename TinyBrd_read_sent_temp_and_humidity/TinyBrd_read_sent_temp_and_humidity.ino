//http://static.nettigo.pl/tinybrd/package_nettigo.pl_index.json - URL for BoardManager
#include <SPI.h>
#include <Radio.h> //lib is available after install tinyBrd throw BoardManager
#include <DHT.h> //DHT library from nettigo - https://github.com/nettigo/DHT 

#define LED 10  //tinyBrd onboard LED is available under 10 pin
#define DHTPIN 2 //where we plug in DHT22 data pin
#define DHTTYPE DHT22 //we use DHT22 - if you use DHT11 just replace it

int counter = 0; //count how many packets we sent
byte serverAddr[3] = {0, 0, 1}; //uno address
byte clientAddr[3] = {1, 0, 1}; //tinyBrd address

DHT dht(DHTPIN, DHTTYPE); //initialize DHT22 sensor

struct Payload {
  byte id;
  float temp;
  float humidity;
} payload; //data struct, counter, temparature and humidity from DHT22


void lightOn(unsigned tme = 100) {
  digitalWrite(LED, HIGH);
  delay(tme);
}

void lightOff(unsigned tme = 100) {
  delay(tme);
  digitalWrite(LED, LOW);
}

void blinkReadines() {
  lightOn(2000);
  lightOff();
  delay(300);
  lightOn();
  delay(500);
  lightOff();
}

float readTemp() {
  float temp = dht.readTemperature();
  if(isnan(temp)){ //if temperature can't be read, return 0. 
    return 0;
  }
  return temp;
}

float readHumid() {
  float humi = dht.readHumidity();
  if(isnan(humi)) { //if humidity can't be read, return 0.
    return 0;  
  }
  return humi;
}

void setup() {
  pinMode(LED, OUTPUT);
  Radio.begin(clientAddr, 100); //initilize radio on channel 100
  dht.begin(); //initialize DHT sensor
  blinkReadines(); // just blink, that we know everything is ok
}

void loop() {
  lightOn(); //show that we read data from sensor and sent throw NRF24
  if(counter < 100) {
    counter++;
  } else {
    counter = 1;
  }
  payload.id = counter; //put data in struct
  payload.temp = readTemp();
  payload.humidity = readHumid();
  
  Radio.write(serverAddr, payload); // sent data to uno
    int ret = Radio.flush(); //get RADIO status
    if(ret == RADIO_SENT) { //if everything was sent
      lightOff(); //show us - just turn off LED
    } //if something go wrong - LED will be still ON
  delay(1000); //wait a moment
}
