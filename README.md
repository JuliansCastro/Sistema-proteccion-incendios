# SISTEMA DE PROTECCIÓN CONTRA INCENDIOS


### Integrantes:

- Julian Andres Castro Pardo     <span style="padding-left:62px;"></span> (juacastropa@unal.edu.co)
- Daniela Valentina Amaya Vargas <span style="padding-left:30px;"></span> (dvamayav@unal.edu.co)
- Javier Leonardo Rodriguez <span style="padding-left:64px;"></span> (@unal.edu.co)
- Sebastian <span style="padding-left:64px;"></span> (@unal.edu.co)



## Descripción del proyecto:




## PROGRAMACIÓN DE ATTINY84/44 CON ARDUINO UNO

Utilizando Arduino UNO como programador de ATtiny84/44 y el IDE de Arduino.

### Programar ATtiny con el IDE de Arduino

![ATTiny84 Distribución de pines ATTiny44](https://42bots.com/wp-content/uploads/2014/01/ATTiny84-ATTiny44-pinout.png "ATTiny84 Distribución de pines ATTiny44")

1. **Configurar el software IDE de Arduino[[1]](#1-programación-de-attiny84-42bots)**
   
   En el menu de ***Archivo > Preferencias > Gestor de URLs Adicionales de Tarjetas*** agregar el link:

```
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
```

![URLs Gestor de Tarjetas](https://github.com/JuliansCastro/Sistema-proteccion-incendios/blob/master/imgs/URL_gestor_tarjetas.png?raw=true "URLs Gestor de Tarjetas")

En el menu de ***Herramientas > Placa*** seleccionar ***Gestor  de Tarjetas..*** buscar ***ATTiny*** e instalar el paquete ***ATTiny by David A. Mellis[[1]](#1-programación-de-attiny84-42bots)***

![Instalar ATTiny](https://github.com/JuliansCastro/Sistema-proteccion-incendios/blob/master/imgs/Gestor_Tarjetas_Attyny_David.png?raw=true "Instalar ATTiny")


1. **Configurar el Arduino Uno para que actúe como programador de ATTiny**


- Abra el boceto de ArduinoISP desde los Ejemplos ("Archivo" ->"Ejemplos" -> "Arduino ISP").
- Seleccione la placa y el puerto serie que correspondan a su placa Arduino (por ejemplo, Arduino Uno).
- Sube el boceto al Arduino Uno

3. **Conecte el ATTiny84 al Arduino Uno**
   
   Revisar la hoja de [datos de ATTiny84](https://www.sigmaelectronica.net/manuals/ATTINY84A-PU.pdf). El ATtiny85 tiene 14 pines según el diagrama a continuación. El pin 1 del Attiny está marcado en el chip con un punto pequeño. Conecte el Attiny y el Arduino Uno según los diagramas a continuación:

   ![Diagrama conexión Attiny84/44](https://42bots.com/wp-content/uploads/2014/01/programming-attiny44-attiny84-with-arduino-uno.png "Diagrama conexión Attiny84/44")

   Conecte un condensador de 10 uF entre los pines "*RESET*" y "*GND*" en la placa Arduino Uno como se muestra en el diagrama. La franja blanca del condensador con el signo negativo ("-") va al pin *"GND"* de Arduino. Esto evita que el Arduino Uno se reinicie y asegura que el IDE de Arduino se comunique con el ArduinoISP (y no con el gestor de arranque) durante la carga de los bocetos en el Attiny.

   Se debe tener en cuenta que la numeración de los pines de Attiny84/44 en la hoja de datos no coincide necesariamente con las asignaciones de Arduino. El pin 6 del Attiny84, por ejemplo, es en realidad el pin digital 7 de Arduino. A continuación se muestra el mapeo de pines de Attiny84/44 a Arduino:


   ```
   // ATMEL ATTINY84/44 - ARDUINO
   //
   //                           +-\/-+
   //                     VCC  1|    |14  GND
   //             (D 10)  PB0  2|    |13  AREF (D  0)
   //             (D  9)  PB1  3|    |12  PA1  (D  1) 
   //       RESET         PB3  4|    |11  PA2  (D  2) 
   //  PWM  INT0  (D  8)  PB2  5|    |10  PA3  (D  3) 
   //  PWM        (D  7)  PA7  6|    |9   PA4  (D  4)  SCK
   //  PWM  MOSI  (D  6)  PA6  7|    |8   PA5  (D  5)  MISO  PWM
   //                           +----+
   ```

4. **Prueba con el sketch de Blink y asegúrese de que todo funcione correctamente [[1]](#1-programación-de-attiny84-42bots)**

![Sketch Blink](https://42bots.com/wp-content/uploads/2014/01/ATTiny84-Blink-Example.jpg "Sketch Blink")


Cargar el sketch usando el comando *"Programa" > "Cargar usando programador" (Ctrl + Mayús + U)*:

```arduino
/*
  Blink Attiny84/44
  Turns on an LED on for one second, then off for one second, repeatedly.
 */

int LED = 7; // LED connected to digital pin D7 (Attiny84/44 pin 6)

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

Asegúrese de seleccionar la opción "ATTiny84 (reloj interno de 1 MHz)", o "ATTiny44 (reloj interno de 1 MHz)" en el menú *Herramientas > Placa* (dependiendo de la versión del chip que tenga). Tambien seleccione el programador ***"Arduino as ISP"*** en el menú *Herramientas > Programador*.:

5. **Configura el ATTiny84 para que funcione a 8Mhz para una mejor compatibilidad con las bibliotecas de Arduino.**

Este paso es opcional. De forma predeterminada, los chips Attiny funcionan a 1 MHz. Necesita un paso adicional para configurar el microcontrolador para que funcione a 8 MHz. Este es un requisito para usar algunas bibliotecas populares de Arduino, así como para aprovechar al máximo su chip.

Para hacerlo, debe usar el mismo cableado que usó para cargar el sketch de ***Blink*** en el paso anterior. En el menú *"Herramientas" > "Placa"*, esta vez seleccione la opción "ATtiny84 (reloj interno de 8 Mhz)", o "ATtiny44 (reloj interno de 8 Mhz)", dependiendo del chip que tenga. Es importante elegir la velocidad de reloj correcta en el menú. Elegir la opción de reloj externo de 20 Mhz, requerirá que agregue un cable en un cristal de 20 MHz, o resonador, para programar y usar su ATTiny84/44.

![Blink 8Mhz](https://github.com/JuliansCastro/Sistema-proteccion-incendios/blob/master/imgs/blink_8Mhz.png?raw=true "Sketch Blink 8Mhz")

A continuación, de nuevo en el menú ***"Herramientas"***, elija la opción ***"Quemar bootloader"***. En realidad, no hay bootloader en los chips Attiny, pero este paso establecerá los fusibles en los valores que le permitirán funcionar a 8 Mhz

Ignore cualquier error relacionado con las señales PAGEL y BS2 que pueda obtener. Para asegurarse de que el cambio funcionó, es posible que tenga que volver a cargar el sketch de Blink modificado del paso 4, esta vez utilizando la opción de reloj interno de 8 MHz para su chip desde el menú ***Boards > Clock***.


## COMUNICACIÓN SERIAL ATTINY84/44 USANDO ARDUINO UNO DE INTERFAZ

La biblioteca SoftwareSerial permite la comunicación en serie en otros pines digitales de una placa Arduino, utilizando software para replicar la funcionalidad (de ahí el nombre "SoftwareSerial"). Es posible tener múltiples puertos serie de software con velocidades de hasta 115200 bps (Tener en cuenta que existen [limitaciones](https://docs.arduino.cc/learn/built-in-libraries/software-serial#limitations-of-this-library)). Un parámetro habilita la señalización invertida para los dispositivos que requieren ese protocolo.

La versión de SoftwareSerial incluida en la versión 1.0 y posteriores se basa en la [biblioteca NewSoftSerial](http://arduiniana.org/libraries/newsoftserial/) de 'Mikal Hart'[[3]](#3-softwareserial-library-docsarduinoccsoftware-serial).

Para usar esta biblioteca:
```arduino
#include <SoftwareSerial.h>
```


Pasos:
1. Conectar el Attiny84/44 al Arduino Uno como se muestra en la imagen del [Paso 3](#programar-attiny-con-el-ide-de-arduino) de la programación de Attiny.
2. Cargar el sketch de comunicación al Attiny de ejemplo usando el Arduino Uno como programador([Paso 4](#programación-de-attiny8444-con-arduino-uno)):
   
El sketch de comunicación serial de ejemplo:
```arduino
/*
 *  ATMEL ATTINY84/44 - ARDUINO
 *  
 *                             +-\/-+
 *                       VCC  1|    |14  GND
 *         Rx    (D 10)  PB0  2|    |13  AREF (D  0)
 *         Tx    (D  9)  PB1  3|    |12  PA1  (D  1) 
 *        (RESET)        PB3  4|    |11  PA2  (D  2) 
 *    PWM  INT0  (D  8)  PB2  5|    |10  PA3  (D  3) 
 *    PWM        (D  7)  PA7  6|    |9   PA4  (D  4)   SCK
 *    PWM (MOSI) (D  6)  PA6  7|    |8   PA5  (D  5)  (MISO)  PWM
 *
 */

#include <SoftwareSerial.h>

// Set up a new SoftwareSerial object with RX in digital pin 10 and TX in digital pin 11
SoftwareSerial mySerial(10, 9); // RX, TX

int analogValue = 30;

void setup() {
    // Set the baud rate for the SerialSoftware object
    mySerial.begin(9600);
}

void loop() {
    // Read the analog value on pin A0
    //analogValue = analogRead(A0);

    // Print analogValue in the Serial Monitor in many formats:
    mySerial.print(analogValue);         // Print as an ASCII-encoded decimal
    mySerial.print("\t");                // Print a tab character
    mySerial.print(analogValue, DEC);    // Print as an ASCII-encoded decimal
    mySerial.print("\t");                // Print a tab character
    mySerial.print(analogValue, HEX);    // Print as an ASCII-encoded hexadecimal
    mySerial.print("\t");                // Print a tab character
    mySerial.print(analogValue, OCT);    // Print as an ASCII-encoded octal
    mySerial.print("\t");                // Print a tab character
    mySerial.print(analogValue, BIN);    // Print as an ASCII-encoded binary
    mySerial.print("\t");                // Print a tab character    
    mySerial.println();                  // Print a line feed character

    // Pause for 10 milliseconds before the next reading
    delay(100);
}
```

4. En el Arduino Uno desconecte el capacitor del *RESET* y prográmelo con el sketch *BareMinumun*. Luego abra el ***Monitor Serie*** de esta instancia:
   
  ***Archivo > Ejemplos > 0.1Basics > BareMinumun***:
```arduino
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
```

![Monitor Serie](https://github.com/JuliansCastro/Sistema-proteccion-incendios/blob/master/imgs/BareMinimum.png?raw=true "Bare minimun > Monitor Serie")


5. Conectar los cables de comunicación del el Attiny84/44 ***Rx:2 y Tx: 3 (Pines Arduino D10, D9)*** al Arduino Uno seleccionando los pines del Attiny84/44  para la comunicación serial. En el *Monitor Serie* se debe ver los datos transmitidos en la comunicación del Attiny84/44 al Arduino Uno:

![CommSerialAttny_ArduinoUno]()
![MonitorSerieAttny](https://github.com/JuliansCastro/Sistema-proteccion-incendios/blob/master/imgs/MonitorSerieAttiny84.png?raw=true "Monitor Serie Attiny84")

## COMUNICACIÓN LoRa CON ATTINY84/44

Basado en la modificación de la libreria de LoRa por ***[Geeky Electronics Projects](https://youtu.be/amkT5NtOgWc?si=71hFIrWzsCA1K3ym)*** [[4](#4-libreria-de-lora-attiny85-geeky-electronics-projects)].


### Transmisor [[5](#5-interfacing-sx1278-ra-02-lora-module-with-arduino-howtoelectronics)]

![Conexión Transmisor](https://how2electronics.com/wp-content/uploads/2019/12/Arduino-Lora-Transmitter-Circuit-768x420.jpg "Conexión Transmisor")

```arduino
```


### Emisor [[5](#5-interfacing-sx1278-ra-02-lora-module-with-arduino-howtoelectronics)]

```arduino
```


## REFERENCIAS:

##### [1] Programación de ATtiny84 [(42bots)](https://42bots.com/tutorials/programming-attiny-ics-with-arduino-uno-and-the-arduino-ide-1-6-4-or-above/) 

##### [2] Programming ATtiny ICs with Arduino Uno and the Arduino IDE 1.6.4 or above [(42bots)](https://42bots.com/tutorials/programming-attiny-ics-with-arduino-uno-and-the-arduino-ide-1-6-4-or-above/)

##### [3] SoftwareSerial Library [(docs.arduino.cc/software-serial)](https://docs.arduino.cc/learn/built-in-libraries/software-serial)

##### [4] libreria de LoRa Attiny85 [(Geeky Electronics Projects)](https://youtu.be/amkT5NtOgWc?si=71hFIrWzsCA1K3ym)


##### [5] Interfacing SX1278 (Ra-02) LORA Module with Arduino [(HowToElectronics)](https://how2electronics.com/interfacing-sx1278-lora-module-with-arduino/)
