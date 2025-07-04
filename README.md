# Sistema de Protección contra Incendios con LoRa y Raspberry Pi Pico

## Resumen

Este proyecto desarrolla un sistema integral de monitoreo y protección contra incendios forestales utilizando tecnología IoT. El sistema emplea sensores distribuidos que se comunican mediante protocolo LoRa para detectar condiciones ambientales críticas (temperatura, humedad y presencia de llamas) y transmitir los datos a una estación central basada en Raspberry Pi Pico W que proporciona interfaz web para visualización en tiempo real.

## Características Principales

- **Monitoreo Distribuido**: Red de sensores inalámbricos con comunicación LoRa de largo alcance
- **Detección Multiparamétrica**: Sensores de temperatura (DHT11), humedad relativa y detector PIR para llamas
- **Comunicación Robusta**: Protocolo LoRa a 433MHz con alcance de varios kilómetros
- **Interfaz Web**: Servidor HTTP integrado en Raspberry Pi Pico W para visualización remota
- **Display Local**: Pantalla LCD I2C para monitoreo local de datos
- **Alertas en Tiempo Real**: Detección automática de condiciones de riesgo de incendio
- **Arquitectura Modular**: Diseño escalable para múltiples nodos sensores
- **Bajo Consumo**: Optimizado para aplicaciones de campo con alimentación autónoma

## Arquitectura del Sistema

```text
┌─────────────────┐       LoRa 433MHz      ┌──────────────────────┐
│   Nodo Sensor   │ ◄────────────────────► │  Estación Central    │
│  (Tx - Pico)    │                        │  (Rx - Pico W)       │
├─────────────────┤                        ├──────────────────────┤
│ • DHT11         │                        │ • LoRa Receptor      │
│ • PIR Sensor    │                        │ • WiFi Server        │
│ • LoRa Tx       │                        │ • LCD Display        │
│ • Raspberry     │                        │ • Raspberry Pi       │
│   Pi Pico       │                        │   Pico W             │
└─────────────────┘                        └──────────────────────┘
                                                      │
                                                      │ WiFi
                                                      ▼
                                           ┌──────────────────────┐
                                           │   Cliente Web        │
                                           │ (Navegador/App)      │
                                           └──────────────────────┘
```

### Componentes del Sistema

1. **Nodo Transmisor (Tx)**:
   - Raspberry Pi Pico como microcontrolador principal
   - Sensor DHT11 para temperatura y humedad
   - Sensor PIR para detección de movimiento/llamas
   - Módulo LoRa para transmisión de datos
   - Encapsulado 3D impreso para protección ambiental

2. **Estación Receptora (Rx)**:
   - Raspberry Pi Pico W con conectividad WiFi
   - Módulo LoRa para recepción de datos
   - Display LCD I2C 16x2 para visualización local
   - Servidor web HTTP integrado
   - Procesamiento y almacenamiento temporal de datos

3. **Protocolo de Comunicación**:
   - LoRa a 433MHz para comunicación de largo alcance
   - Estructura de paquetes con ID de nodo, datos de sensores y verificación
   - Interfaz web HTTP para acceso remoto

## Tecnologías Utilizadas

### Hardware

- **Microcontroladores**:
  - Raspberry Pi Pico (RP2040)
  - Raspberry Pi Pico W (RP2040 + WiFi)
- **Sensores**:
  - DHT11 (Temperatura y Humedad)
  - PIR Sensor (Detección de movimiento/llamas)
- **Comunicación**:
  - Módulos LoRa SX1276/SX1278 (433MHz)
  - WiFi integrado (Pico W)
- **Display**: LCD I2C 16x2 con controlador PCF8574
- **PCB**: Diseño personalizado en KiCad
- **Encapsulado**: Carcasas 3D impresas en PLA/PETG

### Software

- **Lenguajes**: C++ (Arduino Framework)
- **Librerías**:
  - `LoRa.h` - Comunicación LoRa
  - `DHT.h` - Sensor de temperatura/humedad
  - `WiFi.h` - Conectividad inalámbrica
  - `LiquidCrystal_I2C.h` - Control de display
  - `TimeLib.h` - Manejo de tiempo
- **Protocolos**: HTTP, LoRa, I2C, SPI
- **IDE**: Arduino IDE con soporte para RP2040
- **Herramientas de Diseño**:
  - KiCad (Diseño de PCB)
  - Fusion 360 (Modelado 3D)

## Instalación y Configuración

### Requisitos de Hardware

- 2x Raspberry Pi Pico (1x estándar, 1x Pico W)
- 2x Módulos LoRa SX1276/SX1278
- 1x Sensor DHT11
- 1x Sensor PIR
- 1x Display LCD I2C 16x2
- Componentes electrónicos (resistencias, capacitores, conectores)
- PCBs personalizadas o protoboard
- Carcasas 3D impresas
- Fuentes de alimentación (baterías o adaptadores)

### Montaje y Conexiones

#### Nodo Transmisor (Pico)

```text
DHT11:
- VCC → 3.3V
- GND → GND  
- DATA → GPIO 28

PIR Sensor:
- VCC → 5V
- GND → GND
- OUT → GPIO 27

LoRa Module:
- VCC → 3.3V
- GND → GND
- NSS → GPIO 8
- RST → GPIO 9
- DIO0 → GPIO 7
- SCK → GPIO 18
- MISO → GPIO 16
- MOSI → GPIO 19
```

#### Estación Receptora (Pico W)

```text
LoRa Module:
- (Mismas conexiones que Tx)

LCD I2C:
- VCC → 3.3V
- GND → GND
- SDA → GPIO 0
- SCL → GPIO 1
```

### Instalación del Entorno de Software

1. **Configurar Arduino IDE**:

```bash
# Instalar Arduino IDE desde: https://www.arduino.cc/en/software

# Agregar soporte para RP2040:
# File → Preferences → Additional Board Manager URLs
# Agregar: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

# Tools → Board → Boards Manager
# Buscar "pico" e instalar "Raspberry Pi Pico/RP2040"
```

2. **Instalar Librerías Requeridas**:

```bash
# En Arduino IDE: Tools → Manage Libraries
# Instalar:
- DHT sensor library by Adafruit
- LoRa by Sandeep Mistry  
- LiquidCrystal I2C by Frank de Brabander
- Time by Michael Margolis
```

3. **Configurar Red WiFi**:

```cpp
// Editar en RPi_Pico_W_Lora_Rx.ino:
#define SSID "TU_RED_WIFI"
#define PASSWORD "TU_CONTRASEÑA"
```

4. **Cargar Firmware**:

```bash
# 1. Conectar Raspberry Pi Pico manteniendo presionado BOOTSEL
# 2. Seleccionar Board: "Raspberry Pi Pico" o "Raspberry Pi Pico W"
# 3. Cargar RPi_Pico_Lora_Tx.ino en el nodo transmisor
# 4. Cargar RPi_Pico_W_Lora_Rx.ino en la estación receptora
```

## Despliegue

### Ejecución Local

1. **Inicializar Sistema**:
   - Encender nodo transmisor (comenzará a enviar datos cada 20 segundos)
   - Encender estación receptora
   - Verificar conexión LoRa en monitor serial
   - Confirmar conexión WiFi y obtener IP local

2. **Acceso a Interface Web**:
   - Conectarse a la misma red WiFi que el Pico W
   - Abrir navegador en `http://[IP_DEL_PICO_W]`
   - La página se actualiza automáticamente cada 5 segundos

3. **Monitoreo Local**:
   - Visualizar datos en tiempo real en LCD
   - Revisar logs en monitor serial (115200 baud)

### Despliegue en Campo

1. **Preparación**:
   - Instalar nodos en ubicaciones estratégicas
   - Configurar alimentación autónoma (paneles solares/baterías)
   - Verificar cobertura LoRa entre nodos

2. **Red WiFi de Campo**:
   - Configurar hotspot móvil o router portable
   - Ajustar configuración WiFi en código según disponibilidad

## Casos de Uso

### Monitoreo Forestal

```text
Escenario: Bosque de 10 km²
- 5 nodos transmisores distribuidos estratégicamente
- 1 estación central con conectividad satelital/celular
- Alertas automáticas cuando temperatura > 35°C y humedad < 30%
- Detección temprana de focos de incendio mediante sensores PIR
```

### Instalaciones Industriales

```text
Escenario: Almacén de materiales inflamables
- Nodos en cada sector crítico
- Integración con sistemas de extinción automática
- Dashboard web para personal de seguridad
- Registro histórico de condiciones ambientales
```

### Agricultura de Precisión

```text
Escenario: Invernaderos automatizados
- Monitoreo continuo de microclima
- Optimización de sistemas de riego
- Prevención de condiciones de estrés térmico
- Alertas de condiciones anómalas
```

## Estructura del Proyecto

```text
Sistema-proteccion-incendios/
├── README.md
├── Carcasa - Fusion360/           # Modelos 3D de encapsulados
│   ├── 3D/                        # Archivos STL para impresión
│   └── Fusion360/                 # Archivos fuente F3D
├── Diseño electrónico - PCB KiCad/ # Diseños de circuitos impresos
│   ├── sys_incendios_rx/          # PCB para receptor
│   ├── sys_incendios_Tx/          # PCB para transmisor
│   └── Fab_PCB_diseno/            # Archivos Gerber para fabricación
├── Software microcontroladores/    # Código fuente
│   ├── RPi_Pico_Lora_Tx/         # Firmware nodo transmisor
│   └── RPi_Pico_W_Lora_Rx/       # Firmware estación receptora
├── Docs/                          # Documentación técnica
│   ├── BOOM Materials.xlsx        # Lista de materiales
│   └── Datasheets/                # Hojas de datos de componentes
└── imgs/                          # Imágenes del proyecto
```

## Desarrollo Futuro

- [ ] Integración con bases de datos (SQLite/MySQL)
- [ ] API REST para integración con terceros
- [ ] Aplicación móvil nativa
- [ ] Protocolo MQTT para IoT
- [ ] Integración con servicios en la nube (AWS IoT, Azure IoT)
- [ ] Algoritmos de machine learning para predicción
- [ ] Soporte para más tipos de sensores
- [ ] Mesh networking con múltiples receptores

## Contribuciones

Las contribuciones son bienvenidas. Para cambios importantes:

1. Fork el proyecto
2. Crear una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abrir un Pull Request

## Licencia

Este proyecto está licenciado bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## Créditos y Autores

**Equipo de Desarrollo:**

- **Julian A. Castro** - *Líder del proyecto* - [juacastropa@unal.edu.co](mailto:juacastropa@unal.edu.co)
- **Daniela V. Amaya** - *Desarrollo de hardware*
- **Sebastian A. Moreno** - *Desarrollo de software*
- **Javier L. Rodríguez** - *Diseño mecánico*

**Institución:** Universidad Nacional de Colombia  
**Materia:** Diseño de Sistemas Electrónicos  
**Año:** 2023

---

## Agradecimientos

- Comunidad Arduino y Raspberry Pi Foundation
- Desarrolladores de las librerías utilizadas
- Universidad Nacional de Colombia por el apoyo académico

---

*Para soporte técnico o consultas, contactar al equipo de desarrollo a través de los emails proporcionados.*