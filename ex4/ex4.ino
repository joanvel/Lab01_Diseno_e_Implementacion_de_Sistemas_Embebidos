#include "Motor.h"
#include <Adafruit_SSD1306.h>

#define SAMPLE_PERIOD 50
#define REPORT_PERIOD 500
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3c

enum state_t {
    IDLE,
    SAMPLE,
    REPORT,
    SETPOINT,
    STOP
};

state_t state = IDLE;

Motor motor(1.0f, 0.1f); // Ajustar Kp y Ki según sea necesario
unsigned long last_report = 0;
unsigned long last_sample = 0;
float currentRPM;   // Valor de RPM actual


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Wire.begin(21, 22);
    Serial.begin(115200);

    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(POS_PIN, OUTPUT);
    pinMode(NEG_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);

    pcnt_config_t pcnt_config{};
    pcnt_config.pulse_gpio_num = ENC_A;
    pcnt_config.ctrl_gpio_num  = ENC_B;
    pcnt_config.channel        = PCNT_CHANNEL_0;
    pcnt_config.unit           = PCNT_UNIT;
    pcnt_config.pos_mode       = PCNT_COUNT_INC;
    pcnt_config.neg_mode       = PCNT_COUNT_DEC;
    pcnt_config.lctrl_mode     = PCNT_MODE_REVERSE;
    pcnt_config.hctrl_mode     = PCNT_MODE_KEEP;
    pcnt_config.counter_h_lim  = 32767;
    pcnt_config.counter_l_lim  = -32768;

    pcnt_unit_config(&pcnt_config);

    pcnt_set_filter_value(PCNT_UNIT, 200);  // filtra rebotes
    pcnt_filter_enable(PCNT_UNIT);

    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);

    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void loop() {
    float controlSignal;
    float sp;
    // Finite state machine for motor control
    switch (state) {
        case IDLE:
            // Esperar comando para iniciar muestreo
            if (Serial.available() > 0) {
                char c = Serial.read();
                if (c == 's') { // Comando para iniciar muestreo
                    state = SETPOINT;
                } else if (c == 'x'){
                    state = STOP;
                }
            } else if(millis() - last_sample > SAMPLE_PERIOD) {
                state = SAMPLE;
            } else if(millis() - last_report > REPORT_PERIOD) {
                state = REPORT;
            }
            break;
        case SAMPLE:
            // Muestrear RPM y calcular señal de control
            currentRPM = motor.getRPM(last_sample);
            motor.getError();

            motor.computeControlSignal();
            state = IDLE;
            last_sample = millis();
            break;
        case REPORT:
            // Reportar datos por Serial y pantalla OLED
            controlSignal = motor.getControlSignal();
            display.clearDisplay();
            display.setCursor(0, 0);
            Serial.print("RPM: ");
            Serial.print(currentRPM);
            Serial.print(" | Set point: ");
            Serial.print(motor.getSetpoint());
            Serial.print(" | Error: ");
            Serial.println(motor.getError());
            Serial.print("Control Signal: ");
            Serial.println(controlSignal);
            display.print("RPM: ");
            display.println(currentRPM);
            display.print("Set point: ");
            display.println(motor.getSetpoint());
            display.print("Err: ");
            display.println(motor.getError());
            display.display();
            state = IDLE;
            last_report = millis();
            break;
        case SETPOINT:
            // Esperar comando para cambiar setpoint o detener
            while (Serial.available() == 0); // Esperar hasta que haya datos
            sp = Serial.parseFloat();
            motor.setSetpoint(sp);
            state = IDLE;
            break;

        case STOP:
            // Detener motor y limpiar estados
            motor.setRPMDirect(0);
            motor.setSetpoint(0);
            motor.clearIntegral();
            state = IDLE;
            break;
    }
}
