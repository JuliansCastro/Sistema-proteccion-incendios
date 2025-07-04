/*
  Raspberry pi pico w LoRa receiver with server WiFi
  juacastropa@unal.edu.co
*/

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define PCF8574 0x27  // LCD Addres
#define PCF8574A 0x3F
#define dio0 7  // Pinboard 10
#define nss 8   // Pinboard 11
#define rst 9   // Pinboard 12
#define SDA 0   // Pinboard 1
#define SCL 1   // Pinboard 2

#define SSID "Maxwell"       // Put your WifiSSID here
#define PASSWORD "a1b2c3d7"  // Put your wifi password here
#define t_Delay 500

//LCD
TwoWire Wire_00(i2c0, SDA, SCL);
LiquidCrystal_I2C lcd(&Wire_00, PCF8574, 16, 2);

WiFiServer server(80);  // Set web server port number to 80
String header;          // Variable to store the HTTP request

String picoLEDState = "off";  // Variable to store onboard LED state

unsigned long currentTime = millis();  // Current time
unsigned long previousTime = 0;        // Previous time
const long timeoutTime = 2000;         // Define timeout time in milliseconds (example: 2000ms = 2s)

// outgoing message
byte msgCount = 0;  // count of outgoing messages
byte MasterNode = 0xFF;
byte Node2 = 0xCC;

String node = "";
String hum = "";
String temp = "";
String pir = "";
String flame = "";

void setup() {
  Serial.begin(115200);
  Wire_00.begin();
  Wire_00.setClock(100000);  // 100kHz
  lcd.begin();
  lcd.backlight();
  lcd.print("LCD beging!");

  pinMode(LED_BUILTIN, OUTPUT);    // Initialize the LED as an output
  digitalWrite(LED_BUILTIN, LOW);  // Set LED off

  LoRa.setPins(nss, rst, dio0);
  if (!LoRa.begin(433E6)) {  // initialize ratio at 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ;  // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
  lcd.clear();
  lcd.print("LoRa init succeeded.");
  delay(t_Delay);

  WiFi.begin(SSID, PASSWORD);  // Connect to Wi-Fi network with SSID and password

  // Display progress on Serial monitor
  Serial.print("Wait WiFi ...");
  lcd.clear();
  lcd.print("Wait WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print(".");
  }

  // Print local IP address and start web server. Default: http://192.168.33.141/
  Serial.println("");
  Serial.print("WiFi connected at IP Address ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print("WiFi connected at IP Address ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(5000);

  // Start Server
  server.begin();
}

void loop() {
  LoRaOnReceive(LoRa.parsePacket());  // parse for a packet, and call onReceive with the result
  WiFiServer(server.available());     // Listen for incoming clients
}

void LoRaOnReceive(int packetSize) {
  if (packetSize == 0) return;  // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();           // recipient address
  byte sender = LoRa.read();             // sender address
  byte incomingMsgId = LoRa.read();      // incoming msg ID
  byte incomingMsgLength = LoRa.read();  // incoming msg length

  String incomingMsg = "";

  while (LoRa.available()) {
    incomingMsg += (char)LoRa.read();
  }

  if (incomingMsgLength != incomingMsg.length()) {  // check length for error
    // Serial.println("error: message length does not match length");
    return;  // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node2 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    return;  // skip rest of function
  }

  Serial.println("Incoming Msg: " + incomingMsg);
  //lcd.print(incomingMsg);

  node = getValue(incomingMsg, ',', 0);
  hum = getValue(incomingMsg, ',', 1);
  temp = getValue(incomingMsg, ',', 2);
  pir = getValue(incomingMsg, ',', 3);

  lcd.clear();
  lcd.print("Nodo: ");
  lcd.setCursor(0, 1);
  lcd.print(node);
  delay(t_Delay);

  /*
  float tempValue = temp.toFloat();
  int humValue = hum.toInt();
  int pirValue = pir.toInt();

  Serial.println(tempValue);
  Serial.println(humValue);
  Serial.println(pirValue);
  incomingMsg = ""; 
  */

  lcd.clear();
  lcd.print("Humedad: ");
  lcd.setCursor(0, 1);
  lcd.print(hum);
  delay(t_Delay);

  lcd.clear();
  lcd.print("Temperatura: ");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  delay(t_Delay);

  if (pir == "0") {  //Flame sensor detected == 0
    flame = "Flame detected";
    lcd.clear();
    lcd.print("PIR: ");
    lcd.setCursor(0, 1);
    lcd.print(flame);
    Serial.println(flame);
    delay(t_Delay);
  }
  else{
    flame = "No flames";
  }
}


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void WiFiServer(WiFiClient client) {  // Listen for incoming clients
  if (client) {                       // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                                             // print a message out in the serial port
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then
        Serial.write(c);         // print it out the serial monitor
        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta http-equiv=\"refresh\" content=\"5\" charset=\"utf-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">");
            client.println("<title>Diseño Sistemas Electrónicos</title>");

            // CSS to style the paragraph
            client.println("<style>html {");
            client.println("font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("h1 { font-size: 2.0rem; }");
            client.println("p { font-size: 2.0rem; }");
            client.println(".units { font-size: 1.8rem; }");
            client.println(".names { font-size: 1.0rem; }");
            client.println(".bme-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px; }");
            client.println("</style>");
            client.println("</head>");

            // Web Page Heading
            client.println("<body>");
            client.println("<h1>Monitoring station: " + node + "</h1>");

            //client.println("<br>");
            client.println("<p>");
            client.println("<span class=\"names\">Julian A. Castro, </span>");
            client.println("<span class=\"names\">Daniela V. Amaya, </span>");
            client.println("<span class=\"names\">Sebastian A. Moreno, </span>");
            client.println("<span class=\"names\">Javier L. Rodríguez</span>");
            client.println("</p>");
            client.println("<br>");

            // Display current state of sensors
            client.println("<p>");
            client.println("<i class=\"fas fa-thermometer-half\" style=\"color:#059e8a;\"></i>");
            client.println("<span class=\"bme-labels\">Temperature:</span>");
            client.println("<span class=\"units\">" + temp + "°C </span>");
            client.println("</p>");

            client.println("<p>");
            client.println("<i class=\"fas fa-tint\" style=\"color: #00add6;\"></i>");
            client.println("<span class=\"bme-labels\">R. Humidity:</span>");
            client.println("<span class=\"units\">" + hum + "% </span>");
            client.println("</p>");

            client.println("<p>");
            client.println("<i class=\"fas fa-fire-alt\" style=\"color:#ffa500;\"></i>");  
            client.println("<span class=\"bme-labels\"></span>");
            client.println("<span>" + flame + "</span>");
            client.println("</p>");

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            break;  // Break out of the while loop
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
