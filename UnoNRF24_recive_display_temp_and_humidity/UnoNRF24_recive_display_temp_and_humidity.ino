#include <SPI.h>
#include <RadioNRF24.h> //https://github.com/nettigo/RadioNRF24

#define BUFF_SIZE 40
#define LED   13 // build in waveshar uno plus led
#define CSN_PIN 8 //plug here CSN ping from NRF24
#define CE_PIN 7 //plug here CE pin from NRF24

typedef unsigned char uchar;

byte serverAddr[3] = {0, 0, 1}; // uno address - we sent here data from tinyBrd
byte received[BUFF_SIZE];

void lightOn(unsigned tme = 100) {
  digitalWrite(LED, HIGH);
  delay(tme);
}

void lightOff(unsigned tme = 100) {
  delay(tme);
  digitalWrite(LED, LOW);
}

void blinkReadiness() {
  lightOn(2000);
  lightOff();
  delay(300);
  lightOn();
  delay(500);
  lightOff();
}


void setup() {
  Serial.begin(19200);
  pinMode(LED, OUTPUT);
  Serial.println("Begin");
  RadioNRF24.begin(serverAddr, 100, CSN_PIN, CE_PIN); // set address, channel = 100,
  blinkReadiness();
}

void loop() {
  Serial.print("Loop: ");
  int cnt = RadioNRF24.available();
  Serial.print("read bytes ");
  Serial.print(cnt);
  //if something was recived we want to read it.
  if (cnt) { 
    Serial.print(F("avail: "));
    Serial.println(cnt, DEC);// should read 9 bytes - 1 int, 2 float numbers.
    RadioNRF24.read(received);
    Serial.print("ID: ");
    Serial.print(received[0]); // just counter
    float temp;
    uchar tBuff[] = {received[1],received[2],received[3],received[4]}; //first float number - temperature
    float humi;
    uchar hBuff[] = {received[5],received[6],received[7],received[8]}; //second float number - humidity
    memcpy(&temp, &tBuff, sizeof(temp));
    memcpy(&humi, &hBuff, sizeof(humi));
    //print it :)
    Serial.print("TEMPERATURE: ");
    Serial.print(temp);
    Serial.print("C, HUMIDITY: ");
    Serial.print(humi);
    Serial.print("%");
  }
  Serial.println(); //move carrier to next line
  delay(500); //tinyBrd sent data each second, simple delay for SerialMonitor, we don't want spam on it.
}
