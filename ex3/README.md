# Monitoreo de Temperatura y Humedad con DHT22, Sensor Analógico y OLED (ESP32)
## Descripción del proyecto
Este proyecto implementa un sistema de monitoreo ambiental usando un ESP32, combinando:
* Un sensor DHT22 para temperatura y humedad.
* Un sensor analógico de temperatura (por ejemplo LM35 o similar).
* Un display OLED SSD1306 por I2C para visualización local.
El sistema realiza promediado de múltiples muestras analógicas para reducir ruido y reporta los valores periódicamente tanto por puerto serial como en el display OLED.

## Funcionamiento general
El ESP32 realiza de forma continua:
* Muestreo de una entrada analógica de temperatura.
* Acumulación de muestras para calcular un promedio.
* Lectura periódica del sensor DHT22.
* Visualización de los valores consolidados en un display OLED.
El reporte se realiza cada report_period milisegundos, lo que permite desacoplar la adquisición del despliegue de información.

## Adquisición y procesamiento de datos
### Sensor analógico de temperatura
La lectura analógica se realiza en el pin GPIO 34:
```cpp
val += analogRead(34) * 330.0 / 4095.0;
```
* Se asume un rango de 0-3.3 V
* Se promedian múltiples muestras para reducir el ruido:
```cpp
float avg_val = val / num_samples;
```
Este enfoque mejora la estabilidad de la medición frente a fluctuaciones instantáneas.
### Sensor DHT22
El DHT22 proporciona:
* Temperatura en °C
* Humedad relativa en %
Las lecturas se validan comprobando valores NaN para detectar errores de comunicación.
## Visualización en OLED
El display OLED:
* Muestra temperatura y humedad del DHT22.
* Muestra la temperatura promedio del sensor analógico.
* Actualiza la pantalla de forma periódica.
Configuración principal:
* Resolución: 128×64
* Dirección I2C: 0x3C
## Temporización
El sistema usa millis() para controlar el intervalo de reporte:
```cpp
if (millis() - last_report >= report_period)
```
Esto evita el uso de retardos bloqueantes y permite que el muestreo sea continuo.
## Requisitos
### Hardware
* ESP32
* Sensor DHT22
* Sensor analógico de temperatura (LM35 o equivalente)
* Display OLED SSD1306 (128×64, I2C)
* Resistencias y cableado
### Software
* Arduino IDE
* Soporte para ESP32
* Librería DHT
* Librerías Adafruit_GFX y Adafruit_SSD1306
* Librería Wire
## Conexiones
### DHT22 → ESP32
| Señal | GPIO |
|-------|------|
| DATA | GPIO 4 |

### Sensor analógico → ESP32
| Señal | GPIO |
|-------|------|
| OUT | GPIO 34 |

### OLED I2C → ESP32
| Señal OLED | GPIO |
|------------|------|
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## Instalación y uso
1. Instalar Arduino IDE.
2. Agregar soporte para ESP32.
3. Instalar las librerías necesarias.
4. Conectar el hardware según el esquema.
5. Cargar el código al ESP32.
6. Abrir el monitor serial a 115200 baudios.
7. Observar las mediciones en el OLED y en la salida serial.

## Manejo de errores
* Si el DHT22 no responde, el sistema:
  * Muestra un mensaje de error en el OLED
  * Reporta el error por el puerto serial.
* Las lecturas analógicas se reinician después de cada reporte.

Para más información revisar el reporte que se encuentra en la raíz del repositorio.
