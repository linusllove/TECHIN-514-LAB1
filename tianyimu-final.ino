#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET_PIN -1

// Sensor and display objects
Adafruit_BME280 temperatureHumidityPressureSensor;
Adafruit_VEML7700 lightSensor;
Adafruit_SSD1306 OLEDdisplay(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET_PIN);

// Pin and thresholds
const int lightControlPin = A1;
const float temperatureThreshold = 30.0;
const float lightLowThresholdValue = 50.0;
const float lightHighThresholdValue = 100.0;

void setup() {
  Serial.begin(9600);

  if (!temperatureHumidityPressureSensor.begin(0x76) || !lightSensor.begin() || !OLEDdisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Failed to initialize sensors. Please check the wiring!");
    while (1);
  }

  pinMode(lightControlPin, OUTPUT);
  OLEDdisplay.clearDisplay();
  OLEDdisplay.display();
  delay(2000);
}

void loop() {
  float currentTemperature = temperatureHumidityPressureSensor.readTemperature();
  float currentHumidity = temperatureHumidityPressureSensor.readHumidity();
  float currentPressure = temperatureHumidityPressureSensor.readPressure() / 100.0F;
  float currentLightIntensity = lightSensor.readLux();

  displayDataOnOLED(currentTemperature, currentHumidity, currentPressure, currentLightIntensity);
  controlLightBasedOnIntensity(currentLightIntensity);

  delay(2000);
}

void displayDataOnOLED(float temp, float hum, float press, float lightIntensity) {
  OLEDdisplay.clearDisplay();
  OLEDdisplay.setTextSize(1);
  OLEDdisplay.setTextColor(SSD1306_WHITE);
  OLEDdisplay.setCursor(0, 0);
  OLEDdisplay.print("Temperature: "); OLEDdisplay.print(temp); OLEDdisplay.println(" C");
  OLEDdisplay.print("Humidity: "); OLEDdisplay.print(hum); OLEDdisplay.println(" %");
  OLEDdisplay.print("Pressure: "); OLEDdisplay.print(press); OLEDdisplay.println(" hPa");
  OLEDdisplay.print("Light: "); OLEDdisplay.print(lightIntensity); OLEDdisplay.println(" Lux");
  OLEDdisplay.display();
}

void controlLightBasedOnIntensity(float lightIntensity) {
  if (lightIntensity < lightLowThresholdValue) {
    digitalWrite(lightControlPin, HIGH);
  } else if (lightIntensity > lightHighThresholdValue) {
    digitalWrite(lightControlPin, LOW);
  }
}
