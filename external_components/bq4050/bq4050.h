#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace bq4050 {

static const uint8_t BQ4050_I2C_ADDRESS = 0x0B;

static const uint8_t REG_VOLTAGE = 0x09;
static const uint8_t REG_CURRENT = 0x0A;
static const uint8_t REG_TEMPERATURE = 0x08;
static const uint8_t REG_STATE_OF_CHARGE = 0x0D;
static const uint8_t REG_REMAINING_CAPACITY = 0x0F;
static const uint8_t REG_FULL_CHARGE_CAPACITY = 0x10;
static const uint8_t REG_CYCLE_COUNT = 0x17;
static const uint8_t REG_BATTERY_STATUS = 0x16;
static const uint8_t REG_CELL1_VOLTAGE = 0x3F;
static const uint8_t REG_CELL2_VOLTAGE = 0x3E;
static const uint8_t REG_CELL3_VOLTAGE = 0x3D;
static const uint8_t REG_CELL4_VOLTAGE = 0x3C;

static const uint8_t MAC_CMD_DEVICE_RESET = 0x41;
static const uint8_t MAC_CMD_FET_CONTROL = 0x46;

class BQ4050Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_state_of_charge_sensor(sensor::Sensor *state_of_charge_sensor) { state_of_charge_sensor_ = state_of_charge_sensor; }
  void set_remaining_capacity_sensor(sensor::Sensor *remaining_capacity_sensor) { remaining_capacity_sensor_ = remaining_capacity_sensor; }
  void set_full_charge_capacity_sensor(sensor::Sensor *full_charge_capacity_sensor) { full_charge_capacity_sensor_ = full_charge_capacity_sensor; }
  void set_cycle_count_sensor(sensor::Sensor *cycle_count_sensor) { cycle_count_sensor_ = cycle_count_sensor; }
  void set_battery_status_sensor(sensor::Sensor *battery_status_sensor) { battery_status_sensor_ = battery_status_sensor; }
  void set_cell1_voltage_sensor(sensor::Sensor *cell1_voltage_sensor) { cell1_voltage_sensor_ = cell1_voltage_sensor; }
  void set_cell2_voltage_sensor(sensor::Sensor *cell2_voltage_sensor) { cell2_voltage_sensor_ = cell2_voltage_sensor; }
  void set_cell3_voltage_sensor(sensor::Sensor *cell3_voltage_sensor) { cell3_voltage_sensor_ = cell3_voltage_sensor; }
  void set_cell4_voltage_sensor(sensor::Sensor *cell4_voltage_sensor) { cell4_voltage_sensor_ = cell4_voltage_sensor; }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;

 protected:
  bool read_register16_(uint8_t reg, uint16_t *value);
  uint8_t calculate_crc8_(const uint8_t *data, size_t len);

  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *state_of_charge_sensor_{nullptr};
  sensor::Sensor *remaining_capacity_sensor_{nullptr};
  sensor::Sensor *full_charge_capacity_sensor_{nullptr};
  sensor::Sensor *cycle_count_sensor_{nullptr};
  sensor::Sensor *battery_status_sensor_{nullptr};
  sensor::Sensor *cell1_voltage_sensor_{nullptr};
  sensor::Sensor *cell2_voltage_sensor_{nullptr};
  sensor::Sensor *cell3_voltage_sensor_{nullptr};
  sensor::Sensor *cell4_voltage_sensor_{nullptr};
};

}  // namespace bq4050
}  // namespace esphome