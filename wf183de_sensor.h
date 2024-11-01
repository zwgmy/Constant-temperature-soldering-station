#include "esphome.h"
using namespace esphome;

class WF183DESensor : public PollingComponent {
 public:
  Sensor *temperature_sensor = new Sensor();
  Sensor *pressure_sensor = new Sensor();

  WF183DESensor() : PollingComponent(5000) {}

  Sensor *get_temperature_sensor() { return temperature_sensor; }
  Sensor *get_pressure_sensor() { return pressure_sensor; }

  void setup() override {
    // Setup code, if needed
  }

  void update() override {
    // Trigger temperature reading
    Wire.beginTransmission(0xDA); // I2C write address
    Wire.write(0x0A);  // Command register
    Wire.write(0x04);  // Temperature command
    Wire.endTransmission();
    delay(10);

    // Read temperature
    Wire.beginTransmission(0xDA);
    Wire.write(0x0F);  // Temperature high byte register
    Wire.endTransmission();
    Wire.requestFrom(0xDB, 2); // Request 2 bytes
    int16_t temp_raw = (Wire.read() << 8) | Wire.read();
    float temperature = temp_raw / 10.0;
    temperature_sensor->publish_state(temperature);

    // Trigger pressure reading
    Wire.beginTransmission(0xDA);
    Wire.write(0x0A);
    Wire.write(0x06);  // Pressure command
    Wire.endTransmission();
    delay(10);

    // Read pressure
    Wire.beginTransmission(0xDA);
    Wire.write(0x0B);
    Wire.endTransmission();
    Wire.requestFrom(0xDB, 4); // Request 4 bytes
    uint32_t pressure_raw = (Wire.read() << 24) | (Wire.read() << 16) | (Wire.read() << 8) | Wire.read();
    float pressure = pressure_raw / 1000.0;
    pressure_sensor->publish_state(pressure);
  }
};
