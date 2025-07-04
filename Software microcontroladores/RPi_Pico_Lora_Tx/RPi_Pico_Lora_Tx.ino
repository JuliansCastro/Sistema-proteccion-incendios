/*
  Raspberry pi pico transmitter
  juacastropa@unal.edu.co
*/

#include <DHT.h>
#include <SPI.h>
#include <LoRa.h>
#include <TimeLib.h>

#define dio0 7              // Pinboard 10
#define nss 8               // Pinboard 11
#define rst 9               // Pinboard 12
#define pinPIR 27           // Pinboard 32
#define DHTPIN 28           // Pinboard 34 -> GPIO 28
#define DHTTYPE DHT11       // Select DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

byte MasterNode = 0xFF;
byte Node2 = 0xCC;
String SenderNode = "N2";
String message;
byte msgCount = 0;          // count of outgoing messages
long lastSendTime = 0;      // last send time
int interval = 20;          // interval between sends

void setup() {
  Serial.begin(9600);
  dht.begin();              // initialize serial
  pinMode(pinPIR, INPUT);
  //setTime(00, 37, 50, 18, 11, 2023);

  LoRa.setPins(nss, rst, dio0);

  if (!LoRa.begin(433E6)) { // initialize ratio at 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);           // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  if ((millis() - lastSendTime) > interval) {
    int h = dht.readHumidity();
    float t = dht.readTemperature();
    int pir = digitalRead(27);

    message = SenderNode + "," + h + "," + t + "," + pir;
    sendMessage(message, MasterNode, Node2);
    delay(50);

    Serial.println("Sending: " + message);
    lastSendTime = millis();  // timestamp the message
    message = "";
  }
}


void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();             // start packet
  LoRa.write(otherNode);          // add destination address
  LoRa.write(MasterNode);         // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}

/*
String date(){
  
  char fecha[20];
  time_t t = now();     //Init date

  int dia = day(t);
  int mes = month(t);
  int anio = year(t);
  int hora = hour(t);
  int minuto = minute(t);
  int segundo = second(t);

  sprintf( fecha, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d", dia, mes, anio, hora, minuto, segundo);
  return String( fecha );
}
*/
