#include "DHT.h" //cargamos la librería DHT
#define DHTPIN 9 //Seleccionamos el pin en el que se conectará al sensor
#define DHTTYPE DHT11 //Se selecciona el DHT11
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor
void setup() {
  Serial.begin(9600); //Se inicia la comunicación serial
  dht.begin(); //Se inicia el sensor
}
void loop() {
  int h = dht.readHumidity(); //Se lee la humedad
  int t = dht.readTemperature(); //Se lee la temperatura
  //Se imprimen las variables
  Serial.print("H: ");
  Serial.println(h);
  Serial.print("T: ");
  Serial.println(t);
  delay(1000); //Se espera X segundos para seguir leyendo datos
}
