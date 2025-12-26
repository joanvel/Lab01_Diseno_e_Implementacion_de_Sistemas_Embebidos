# Control de Velocidad de Motor DC con Encoder, Control PI y OLED (ESP32)
## Descripción del proyecto
Este proyecto implementa un control de velocidad (RPM) de un motor DC usando un ESP32, un encoder incremental, y un controlador PI.

El sistema permite:
* Medir la velocidad del motor mediante el periférico PCNT del ESP32.
* Medir la velocidad del motor mediante el periférico PCNT del ESP32.
* Ajustar el setpoint de RPM por puerto serial.
* Visualizar el estado del sistema en un display OLED SSD1306.
El diseño está organizado en tres archivos, separando claramente:
* La lógica del motor y control (clase Motor)
* La configuración y control del sistema (FSM en el main)
## Estructura del proyecto
```text
/ex4
│
├── Motor.h        // Definición de la clase Motor
├── Motor.ino      // Implementación de la clase Motor
└── ex4.ino        // Archivo principal (setup, loop y FSM)
```
## Funcionamiento general
El sistema funciona como un controlador de velocidad en lazo cerrado:
1. El encoder genera pulsos proporcionales a la velocidad del motor.
2. El ESP32 cuenta los pulsos usando el periférico PCNT.
3. Se calcula la velocidad en RPM.
4. Un controlador PI calcula la señal de control.
5. La señal controla:
   * El sentido de giro (pines POS / NEG).
   * El duty cycle PWM.
6. El estado del sistema se reporta por Serial y OLED.
Todo el comportamiento está coordinado por una máquina de estados finitos (FSM).
## Máquina de estados (FSM)
Definida en ex4.ino:
```cpp
enum state_t {
    IDLE,
    SAMPLE,
    REPORT,
    SETPOINT,
    STOP
};
```
### Estados
* IDLE
  * Estado principal de espera.
  * Decide cuándo:
    * Muestrear el motor.
    * Reportar datos.
    * Leer comandos por Serial.
* SAMPLE
  * Calcula la velocidad actual del motor (RPM).
  * Ejecuta el controlador PI.
  * Actualiza la señal de control.
* REPORT
  * Muestra:
    * RPM actual
    * Setpoint
    * Error
  * Reporta información por:
    * Monitor serial
    * Display OLED
* SETPOINT
  * Lee un nuevo setpoint de RPM desde el puerto serial.
  * Actualiza el valor deseado del controlador.
* STOP
  * Detiene el motor.
  * Limpia el estado del controlador (integral).
  * Regresa al estado IDLE.
## Clase Motor
### Responsabilidades
La clase Motor encapsula:
* Medición de RPM usando PCNT.
* Cálculo del error.
* Control PI.
* Aplicación de PWM y dirección.
## Medición de RPM
```cpp
RPM = (counts / (COUNTS_PER_REVOLUTION * 2)) * (60.0f / timeInterval);
```
* counts: pulsos del encoder.
* COUNTS_PER_REVOLUTION: resolución del encoder.
* Se calcula RPM a partir del tiempo entre muestras.
## Control PI
```cpp
float P = Kp * error;
integral += error;
float I = Ki * integral;
controlSignal = P + I;
```
* Controlador PI (sin término derivativo).
* El término integeral se mantiene a nivel de archivo para persistencia.

## Control de dirección y PWM
* El signo del control define el sentido de giro.
* El valor absoluto define el duty cycle.
* La señal PWM se limita a 0–255.

## Configuración del encoder (PCNT)
Se utiliza el periférico PCNT del ESP32:
* Modo incremental con señales A y B.
* Filtro habilitado para eliminar rebotes.
* Conteo ascendente/descendente según dirección.
Esto permite una medición precisa y eficiente de velocidad.

## Visualización en OLED
El display OLED muestra:
* RPM actual
* Setpoint
* Error del controlador
Configuración:
* Resolución: 128×64
* Dirección I2C: 0x3C

## Requisitos
* Hardware
  * ESP32
  * Motor DC
  * Encoder incremental
  * Puente H (se utilizó L298)
  * Display OLED SSD1306 (I2C)
  * Fuente de alimentación adecuada para el motor
* Software
  * Arduino IDE
  * Soporte para ESP32
  * Librería Adafruit_GFX
  * Librería Adafruit_SSD1306
  * Librería Wire (incluida)

## Conexiones principales
### Encoder → ESP32
| Señal | GPIO |
|-------|------|
| ENC A | GPIO 32 |
| ENC B | GPIO 33 |

### Puente H → ESP32
| Puente H | ESP32 |
|----------|-------|
| ENA | GPIO 5 |
| IN1 | GPIO 19 |
| IN2 | GPIO 18 |

### OLED → ESP32
| Señal | GPIO |
|-------|------|
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## Uso del sistemna
### Comandos por serial
* s → Entrar al modo de setpoint:
  * Enviar número (float) → Nuevo setpoint de RPM.
* x → Detener el motor.
* Configurar la terminal a 115200 baudios.

Para más información revisar el informe técnico en la raíz del repositorio.
