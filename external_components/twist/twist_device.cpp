#include "twist_device.h"
#include "esphome/core/log.h"

namespace esphome {
namespace twist {

static const char *const TAG = "twist_device";

bool TwistDevice::begin(i2c::I2CDevice *i2c_device) {
  i2c_device_ = i2c_device;
  
  if (!is_connected()) {
    return false;
  }
  
  return true;
}

bool TwistDevice::is_connected() {
  if (i2c_device_ == nullptr) {
    return false;
  }
  
  uint8_t data;
  auto err = i2c_device_->read_register(TWIST_ID, &data, 1);
  return err == i2c::ERROR_OK;
}

int16_t TwistDevice::get_count() {
  return read_register_16(TWIST_COUNT);
}

bool TwistDevice::set_count(int16_t amount) {
  return write_register_16(TWIST_COUNT, amount);
}

int16_t TwistDevice::get_diff(bool clear_value) {
  int16_t diff = read_register_16(TWIST_DIFFERENCE);
  if (clear_value) {
    write_register_16(TWIST_DIFFERENCE, 0);
  }
  return diff;
}

bool TwistDevice::is_moved() {
  uint8_t status = read_register(TWIST_STATUS);
  return (status & (1 << STATUS_ENCODER_MOVED_BIT)) != 0;
}

bool TwistDevice::is_pressed() {
  uint8_t status = read_register(TWIST_STATUS);
  return (status & (1 << STATUS_BUTTON_PRESSED_BIT)) != 0;
}

bool TwistDevice::is_clicked() {
  uint8_t status = read_register(TWIST_STATUS);
  bool clicked = (status & (1 << STATUS_BUTTON_CLICKED_BIT)) != 0;
  if (clicked) {
    // Clear the clicked bit by writing back
    write_register(TWIST_STATUS, status & ~(1 << STATUS_BUTTON_CLICKED_BIT));
  }
  return clicked;
}

uint16_t TwistDevice::time_since_last_movement(bool clear_value) {
  uint16_t time = read_register_16(TWIST_LAST_ENCODER_EVENT);
  if (clear_value) {
    write_register_16(TWIST_LAST_ENCODER_EVENT, 0);
  }
  return time;
}

uint16_t TwistDevice::time_since_last_press(bool clear_value) {
  uint16_t time = read_register_16(TWIST_LAST_BUTTON_EVENT);
  if (clear_value) {
    write_register_16(TWIST_LAST_BUTTON_EVENT, 0);
  }
  return time;
}

void TwistDevice::clear_interrupts() {
  uint8_t status = read_register(TWIST_STATUS);
  // Clear moved and clicked bits
  status &= ~((1 << STATUS_ENCODER_MOVED_BIT) | (1 << STATUS_BUTTON_CLICKED_BIT));
  write_register(TWIST_STATUS, status);
}

bool TwistDevice::set_color(uint8_t red, uint8_t green, uint8_t blue) {
  bool success = true;
  success &= set_red(red);
  success &= set_green(green);
  success &= set_blue(blue);
  return success;
}

bool TwistDevice::set_red(uint8_t red) {
  return write_register(TWIST_RED, red);
}

bool TwistDevice::set_green(uint8_t green) {
  return write_register(TWIST_GREEN, green);
}

bool TwistDevice::set_blue(uint8_t blue) {
  return write_register(TWIST_BLUE, blue);
}

uint8_t TwistDevice::get_red() {
  return read_register(TWIST_RED);
}

uint8_t TwistDevice::get_green() {
  return read_register(TWIST_GREEN);
}

uint8_t TwistDevice::get_blue() {
  return read_register(TWIST_BLUE);
}

bool TwistDevice::connect_color(int16_t red, int16_t green, int16_t blue) {
  bool success = true;
  success &= connect_red(red);
  success &= connect_green(green);
  success &= connect_blue(blue);
  return success;
}

bool TwistDevice::connect_red(int16_t red) {
  return write_register_16(TWIST_CONNECT_RED, red);
}

bool TwistDevice::connect_green(int16_t green) {
  return write_register_16(TWIST_CONNECT_GREEN, green);
}

bool TwistDevice::connect_blue(int16_t blue) {
  return write_register_16(TWIST_CONNECT_BLUE, blue);
}

int16_t TwistDevice::get_red_connect() {
  return read_register_16(TWIST_CONNECT_RED);
}

int16_t TwistDevice::get_green_connect() {
  return read_register_16(TWIST_CONNECT_GREEN);
}

int16_t TwistDevice::get_blue_connect() {
  return read_register_16(TWIST_CONNECT_BLUE);
}

bool TwistDevice::set_limit(uint16_t limit) {
  return write_register_16(TWIST_LIMIT, limit);
}

uint16_t TwistDevice::get_limit() {
  return read_register_16(TWIST_LIMIT);
}

uint16_t TwistDevice::get_int_timeout() {
  return read_register_16(TWIST_TURN_INT_TIMEOUT);
}

bool TwistDevice::set_int_timeout(uint16_t timeout) {
  return write_register_16(TWIST_TURN_INT_TIMEOUT, timeout);
}

uint16_t TwistDevice::get_version() {
  return read_register_16(TWIST_VERSION);
}

void TwistDevice::change_address(uint8_t new_address) {
  write_register(TWIST_CHANGE_ADDRESS, new_address);
}

// Private helper methods
uint8_t TwistDevice::read_register(uint8_t addr) {
  if (i2c_device_ == nullptr) return 0;
  
  uint8_t data = 0;
  auto err = i2c_device_->read_register(addr, &data, 1);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read register 0x%02X", addr);
    return 0;
  }
  return data;
}

int16_t TwistDevice::read_register_16(uint8_t addr) {
  if (i2c_device_ == nullptr) return 0;
  
  uint8_t data[2];
  auto err = i2c_device_->read_register(addr, data, 2);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read 16-bit register 0x%02X", addr);
    return 0;
  }
  return (data[1] << 8) | data[0];  // Little endian
}

bool TwistDevice::write_register(uint8_t addr, uint8_t val) {
  if (i2c_device_ == nullptr) return false;
  
  auto err = i2c_device_->write_register(addr, &val, 1);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write register 0x%02X", addr);
    return false;
  }
  return true;
}

bool TwistDevice::write_register_16(uint8_t addr, uint16_t val) {
  if (i2c_device_ == nullptr) return false;
  
  uint8_t data[2];
  data[0] = val & 0xFF;        // Low byte
  data[1] = (val >> 8) & 0xFF; // High byte
  
  auto err = i2c_device_->write_register(addr, data, 2);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write 16-bit register 0x%02X", addr);
    return false;
  }
  return true;
}

bool TwistDevice::write_register_24(uint8_t addr, uint32_t val) {
  if (i2c_device_ == nullptr) return false;
  
  uint8_t data[3];
  data[0] = val & 0xFF;         // Low byte
  data[1] = (val >> 8) & 0xFF;  // Mid byte
  data[2] = (val >> 16) & 0xFF; // High byte
  
  auto err = i2c_device_->write_register(addr, data, 3);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write 24-bit register 0x%02X", addr);
    return false;
  }
  return true;
}

}  // namespace twist
}  // namespace esphome