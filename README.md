# Retos de Sistemas Embebidos – ESP32

Este repositorio contiene la implementación de **cuatro retos de sistemas embebidos** desarrollados utilizando un **ESP32**, como parte de un proyecto académico.  
Cada reto aborda un problema distinto e incluye el código fuente, simulación (cuando aplica) y documentación asociada.

Adicionalmente, el repositorio incluye un **informe técnico** donde se presenta el análisis de requerimientos, la arquitectura del sistema, el desarrollo del firmware y los resultados obtenidos.

---

## Estructura del Repositorio

```text
.
├── ex1/
│   ├── ex1.ino
│   └── README.md
│
├── ex2/
│   ├── ex2.ino
│   └── README.md
│
├── ex3/
│   ├── ex3.ino
|   ├── Demostration/
|       └── video.mp4
│   └── README.md
│
├── ex4/
│   ├── Motor.h
│   ├── Motor.ino
│   ├── ex4.ino
│   └── README.md
│
├── Informe_lab.pdf
│
└── README.md
```

Cada carpeta corresponde a un reto independiente y contiene su respectivo código y documentación específica.

---

## Descripción de los Retos

### 🔹 Reto 1 – Secuencia de LEDs
Sistema embebido que controla una secuencia de cuatro LEDs usando un único botón pulsador.  
El botón permite iniciar, pausar y reanudar la secuencia de forma cíclica mediante una máquina de estados finitos.

**Conceptos principales:**
- GPIO
- Máquinas de estados finitos (FSM)
- Temporización no bloqueante con `millis()`
- Antirrebote por software

---

### 🔹 Reto 2 – Medición de Peso (Simulación)
Sistema simulado que mide peso usando:
- Celda de carga
- Amplificador HX711
- Pantalla LCD 16x2 por I2C

El sistema muestra el peso en tiempo real y lo envía al Serial Monitor.

**Conceptos principales:**
- Interfaz de sensores
- Comunicación I2C
- Firmware basado en estados
- Simulación en Wokwi

---

### 🔹 Reto 3 – Estación Meteorológica
Sistema que mide:
- Temperatura y humedad con un sensor DHT22
- Temperatura analógica con un LM35

Las lecturas se realizan cada 5 segundos usando un enfoque **no bloqueante**, mostrando los datos en una pantalla OLED y en el Serial Monitor.

**Conceptos principales:**
- Adquisición de múltiples sensores
- Temporización no bloqueante
- Pantalla OLED I2C
- Promediado de señales analógicas

---

### 🔹 Reto 4 – Control de Velocidad de Motor con Encoder
Sistema embebido para el control de velocidad y dirección de un motor DC con encoder, utilizando:
- H-bridge
- Controlador PI
- Entrada de set point por consola Serial
- Retroalimentación mediante encoder

**Conceptos principales:**
- Control en lazo cerrado
- Uso del periférico PCNT del ESP32
- Control PI
- Diseño modular de firmware
- Máquina de estados para control y reporte

---

## Instrucciones de Compilación y Ejecución

### Requisitos de Hardware
- ESP32
- Cable USB
- Componentes según el reto:
  - LEDs y botón
  - HX711 y celda de carga
  - DHT22, LM35, OLED
  - Motor DC, encoder, H-bridge, OLED

---

### Requisitos de Software
- **Arduino IDE**
- Soporte para ESP32 instalado  
  - Board Manager → *ESP32 by Espressif Systems*
- Librerías necesarias (dependiendo del reto):
  - `HX711`
  - `LiquidCrystal_I2C`
  - `DHT`
  - `Adafruit_SSD1306`
  - `Adafruit_GFX`

---

### Pasos para Compilar y Ejecutar
1. Clonar el repositorio:
   ```bash
   git clone https://github.com/tu-usuario/retos-sistemas-embebidos.git
   ```
2. Abrir la carpeta del resto deseado.
3. Abrir el archivo .ino con el mismo nombre que la carpeta.
4. Seleccionar: Placa ESP32 Dev Module y el puerto COM correspondiente.
5. Instalar las librerías requeridas si el IDE lo solicita.
6. Compilar y cargar el programa al esp32.
7. Abrir el monitor serial con velocidad de 115200 baudios.

---

### Informe Técnico
En el repositorio también se encuentra el informe técnico del proyecto, el cual incluye:
* Análisis de requerimientos.
* Análisis y diseño del sistema
* Diseño e integración de Hardware.
* Desarrollo del firmware.
* Pruebas y verificación de resultados.

---

### Autor: Joan Esteban Velasco Larrea.
