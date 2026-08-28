#include "bq4050.h"
#include "esphome/core/log.h"

namespace esphome {
namespace bq4050 {

static const char *const TAG = "bq4050";

void BQ4050Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up BQ4050...");
}

void BQ4050Component::dump_config() {
  ESP_LOGCONFIG(TAG, "BQ4050:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  
  LOG_SENSOR("  ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("  ", "Current", this->current_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "State of Charge", this->state_of_charge_sensor_);
  LOG_SENSOR("  ", "Remaining Capacity", this->remaining_capacity_sensor_);
  LOG_SENSOR("  ", "Full Charge Capacity", this->full_charge_capacity_sensor_);
  LOG_SENSOR("  ", "Cycle Count", this->cycle_count_sensor_);
  LOG_SENSOR("  ", "Battery Status", this->battery_status_sensor_);
  LOG_SENSOR("  ", "Cell 1 Voltage", this->cell1_voltage_sensor_);
  LOG_SENSOR("  ", "Cell 2 Voltage", this->cell2_voltage_sensor_);
  LOG_SENSOR("  ", "Cell 3 Voltage", this->cell3_voltage_sensor_);
  LOG_SENSOR("  ", "Cell 4 Voltage", this->cell4_voltage_sensor_);
}

float BQ4050Component::get_setup_priority() const { 
  return setup_priority::DATA; 
}

void BQ4050Component::update() {
  uint16_t raw_value;

  if (this->voltage_sensor_ != nullptr && this->read_register16_(REG_VOLTAGE, &raw_value)) {
    float voltage = raw_value / 1000.0f;
    this->voltage_sensor_->publish_state(voltage);
  }

  if (this->current_sensor_ != nullptr && this->read_register16_(REG_CURRENT, &raw_value)) {
    int16_t signed_value = static_cast<int16_t>(raw_value);
    float current = signed_value / 1000.0f;
    this->current_sensor_->publish_state(current);
  }

  if (this->temperature_sensor_ != nullptr && this->read_register16_(REG_TEMPERATURE, &raw_value)) {
    float temperature = (raw_value / 10.0f) - 273.15f;
    this->temperature_sensor_->publish_state(temperature);
  }

  if (this->state_of_charge_sensor_ != nullptr && this->read_register16_(REG_STATE_OF_CHARGE, &raw_value)) {
    float soc = raw_value;
    this->state_of_charge_sensor_->publish_state(soc);
  }

  if (this->remaining_capacity_sensor_ != nullptr && this->read_register16_(REG_REMAINING_CAPACITY, &raw_value)) {
    float capacity = raw_value;
    this->remaining_capacity_sensor_->publish_state(capacity);
  }

  if (this->full_charge_capacity_sensor_ != nullptr && this->read_register16_(REG_FULL_CHARGE_CAPACITY, &raw_value)) {
    float capacity = raw_value;
    this->full_charge_capacity_sensor_->publish_state(capacity);
  }

  if (this->cycle_count_sensor_ != nullptr && this->read_register16_(REG_CYCLE_COUNT, &raw_value)) {
    float cycles = raw_value;
    this->cycle_count_sensor_->publish_state(cycles);
  }

  if (this->battery_status_sensor_ != nullptr && this->read_register16_(REG_BATTERY_STATUS, &raw_value)) {
    float status = raw_value;
    this->battery_status_sensor_->publish_state(status);
  }

  if (this->cell1_voltage_sensor_ != nullptr && this->read_register16_(REG_CELL1_VOLTAGE, &raw_value)) {
    float voltage = raw_value / 1000.0f;
    this->cell1_voltage_sensor_->publish_state(voltage);
  }

  if (this->cell2_voltage_sensor_ != nullptr && this->read_register16_(REG_CELL2_VOLTAGE, &raw_value)) {
    float voltage = raw_value / 1000.0f;
    this->cell2_voltage_sensor_->publish_state(voltage);
  }

  if (this->cell3_voltage_sensor_ != nullptr && this->read_register16_(REG_CELL3_VOLTAGE, &raw_value)) {
    float voltage = raw_value / 1000.0f;
    this->cell3_voltage_sensor_->publish_state(voltage);
  }

  if (this->cell4_voltage_sensor_ != nullptr && this->read_register16_(REG_CELL4_VOLTAGE, &raw_value)) {
    float voltage = raw_value / 1000.0f;
    this->cell4_voltage_sensor_->publish_state(voltage);
  }
}

bool BQ4050Component::read_register16_(uint8_t reg, uint16_t *value) {
  uint8_t data[2];
  if (!this->read_bytes(reg, data, 2)) {
    ESP_LOGW(TAG, "Failed to read register 0x%02X", reg);
    return false;
  }
  *value = (data[1] << 8) | data[0];
  return true;
}

uint8_t BQ4050Component::calculate_crc8_(const uint8_t *data, size_t len) {
  static const uint8_t crc_table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
    0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
    0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
    0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
    0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
    0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
    0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
    0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
    0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
    0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
    0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
    0xFA, 0xFD, 0xF4, 0xF3
  };

  uint8_t crc = 0;
  for (size_t i = 0; i < len; i++) {
    crc = crc_table[crc ^ data[i]];
  }
  return crc;
}

}  // namespace bq4050
}  // namespace esphome