#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- DHT ----------
#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// ---------- OTROS ----------
#define report_period 5000

unsigned long last_report = 0;

void setup() {
  Wire.begin(21, 22);
  Serial.begin(115200);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Error al iniciar OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  static float val = 0;
  static unsigned int num_samples = 0;

  // Lectura analógica (ej. LM35 u otro)
  val += analogRead(34) * 330.0 / 4095.0;
  num_samples++;

  if (millis() - last_report >= report_period) {
    last_report = millis();

    float avg_val = val / num_samples;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Celsius

    display.clearDisplay();
    display.setCursor(0, 0);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Error leyendo DHT22");
      display.println("Error DHT22");
    } else {
      Serial.println("Temp: " + String(temperature, 1) + " °C");
      Serial.println("Humidity: " + String(humidity, 1) + " %");

      display.println("Temp: " + String(temperature, 1) + " C");
      display.println("Humidity: " + String(humidity, 1) + " %");
    }

    Serial.println("---");
    Serial.print("Temp LM75:");
    Serial.println(String(avg_val, 2) + " C");
    display.println("---");
    display.print("Temp LM75:");
    display.println(String(avg_val, 2) + " C");

    display.display();

    val = 0;
    num_samples = 0;
  }
}
