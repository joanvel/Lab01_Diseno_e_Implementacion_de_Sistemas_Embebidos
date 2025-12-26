# Medición de Peso con Célula de Carga HX711 y LCD I2C (ESP32)
## Descripción del proyecto
Este proyecto implementa un sistema básico de medición de peso utilizando un ESP32, un módulo HX711 para la lectura de una célula de carga, y un display LCD 16x2 con interfaz I2C para mostrar el resultado.

El diseño está estructurado mediante una máquina de estados finitos (FSM) que separa claramente la adquisición de datos y el reporte de resultados, facilitando la comprensión del flujo del sistema y su posible ampliación.

###  ⚠️⚠️⚠️ Este proyecto fue probado en simulación, por lo que los valores de sensibilidad deben calibrarse para uso real con hardware físico. ⚠️⚠️⚠️

## Funcionamiento general
El sistema realiza lecturas continuas desde el módulo HX711 y, cada cierto intervalo de tiempo, muestra el peso calculado tanto en el monitor serial como en el LCD.

El flujo general es:
1. Leer el valor crudo del HX711.
2. Convertir la lectura a una magnitud física mediante un factor de sensibilidad.
3. Reportar el valor de peso cada intervalo de tiempo fijo.
4. Repetir el proceso indefinidamente.

## Máquina de estados
La máquina de estados definida es:
```cpp
enum state {
  measure,
  report
};
```
### Estados y comportamiento
* measure
  * Se realiza la lectura del ADC del HX711.
  * El valor leído se escala mediante una constante de sensibilidad.
  * Se verifica si ha transcurrido el tiempo de reporte (TIME_REPORT).
* report
  * Se envía el valor de peso al monitor serial.
  * Se muestra el valor en el LCD.
  * Se actualiza el temporizador y se regresa al estado de medición.
Este enfoque desacopla claramente la adquisición de datos del reporte, lo cual es una buena práctica en sistemas embebidos.
## Lectura y escalado de la señal
La lectura cruda del HX711 se obtiene mediante:
```cpp
scale.read()
```
y se convierte a una magnitud física usando:
```cpp
reading = scale.read() * SENSITIVITY;
```
Donde SENSITIVITY es un factor de calibración que depende de:
* La célula de carga utilizada
* La ganancia del HX711
* La configuración mecánica
## Temporización
El reporte se realiza cada TIME_REPORT milisegundos utilizando millis():
```cpp
if (millis() - last_report >= TIME_REPORT)
```
Esto evita el uso de retardos bloqueantes y permite que el sistema sea fácilmente extensible.
## Requisitos
* Hardware
  * ESP32
  * Módulo HX711
  * Célula de carga
  * Display LCD 16x2 con interfaz I2C
* Software
  * Arduino IDE
  * Soporte para placas ESP32
  * Librería HX711
  * Librería LiquidCrystal_I2C
  * Librería Wire (incluida por defecto)
## Conexiones
**HX711 → ESP32**
| Señal HX711 | GPIO ESP32 |
|-------------|------------|
| DOUT        | GPIO 27    |
| SCK         | GPIO 26    |

**LCD I2C → ESP32**
| Señal LCD   | ESP32      |
|-------------|------------|
| SDA         | SDA (I2C)  |
| SCL         | SCL (I2C)  |
## Instalación y uso
1. Instalar Arduino IDE.
2. Agregar soporte para ESP32.
3. Instalar las librerías HX711 y LiquidCrystal_I2C.
4. Conectar los periféricos (o configurar la simulación).
5. Cargar el código al ESP32.
6. Abrir el monitor serial a 115200 baudios.
7. Observar el peso mostrado en el LCD y en el monitor serial.

Para mayor información revisar el reporte en la raiz del repositorio.
