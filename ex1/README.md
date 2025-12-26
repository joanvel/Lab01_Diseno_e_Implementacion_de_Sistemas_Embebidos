# Control de Secuencia de LEDs
## Descripción del Proyecto
Este proyecto implementa el control de una secuencia de LEDs en un ESP32 utilizando una máquina de estados finitos (FSM).
El sistema permite iniciar, pausar y reanudar la secuencia de encendido de los LEDs mediante un único botón, gestionando además el rebote mecánico del pulsador.

## Funcionamiento general
El sistema controla cuatro LEDs conectados a pines GPIO del ESP32.
La secuencia consiste en un desplazamiento circular del estado de encendido entre los LEDs, con un período fijo.

El usuario interactúa únicamente mediante un botón, que permite:
* Iniciar la secuencia
* Pausar la ejecución
* Reanudar la secuencia
Todo el comportamiento está organizado mediante una máquina de estados, lo que hace el diseño más claro, mantenible y escalable.

### Máquina de Estados
La máquina de estados definida es la siguiente:

```cpp
enum state {
  INIT,
  FIRST_PRSS_WAIT,
  RUN,
  SECOND_PRSS_WAIT,
  PAUSE,
};
```
### Estados y comportamiento
* INIT
  * Estado inicial del sistema.
  * Se enciende el primer LED.
  * Espera a que el usuario presione el botón para comenzar la secuencia.
* FIRST_PRSS_WAIT
  * Estado intermedio para manejar el antirrebote del botón.
  * Espera a que el botón sea liberado antes de iniciar la ejecución.
* RUN
  * Estado principal de ejecución.
  * Los LEDs cambian de estado cada cierto tiempo (LED_WAIT) usando millis().
  * La secuencia se desplaza de forma circular.
  * Si el usuario presiona el botón, se pasa al estado de pausa.
* SECOND_PRSS_WAIT
  * Estado intermedio para manejar el antirrebote antes de pausar.
  * Espera a que el botón sea liberado.
* PAUSE
  * La secuencia queda detenida.
  * Los LEDs conservan su último estado.
  * Al presionar el botón nuevamente, el sistema vuelve a ejecutar la secuencia.

### Control de la secuencia de LEDs
La secuencia se implementa desplazando el estado lógico de los LEDs:
```cpp
led0 = led1;
led1 = led2;
led2 = led3;
led3 = aux;
```
Esto genera un corrimiento circular, donde el estado del último LED vuelve al primero.

El temporizado se controla con:
```cpp
if (millis() - last_time > LED_WAIT)
```
lo que evita el uso de retardos bloqueantes y permite que el sistema siga respondiendo al botón.

## Requisitos
### Hardware
* ESP32
* 4 LEDs
* 4 Resistencias limitadoras (220 Ω – 330 Ω)
* 1 pulsador
* Cables deconexión
### Software
* Arduino IDE
* ESP32 Board Support Package instalado en Arduino IDE

## Conexiones
| Elemento | GPIO ESP32 |
|----------|------------|
| LED 0    | GPIO 19    |
| LED 1    | GPIO 18    |
| LED 2    | GPIO 5     |
| LED 3    | GPIO 17    |
| Botón    | GPIO 34    |

## Instalación y uso
1. Instalar Arduino IDE.
2. Agregar soporte para ESP32 desde el gestor de placas.
3. Conectar el ESP32 al computador.
4. Cargar el código en el ESP32.
5. Abrir monitor serial (9600 baudios) para observar el estado del sistema.
6. Presionar el botón para iniciar, pausar y reanudar la secuencia.

## Notas técnicas
* El antirebote se implementa mediante retardos cortos (BOUNCE_WAIT) y espera activa de liberación del botón.
* La estructura basada en FSM facilita agregar nuevos estados o comportamientos.
* El uso de millis() permite un diseño más robusto que el uso de delay() para temporización principal.

PAra más información al respecto, mirar el reporte en la raiz del repositorio.
