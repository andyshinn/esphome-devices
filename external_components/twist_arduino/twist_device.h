#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace twist {

#define QWIIC_TWIST_ADDR 0x3F

// Status register bits
static const uint8_t STATUS_BUTTON_CLICKED_BIT = 2;
static const uint8_t STATUS_BUTTON_PRESSED_BIT = 1;
static const uint8_t STATUS_ENCODER_MOVED_BIT = 0;

// Interrupt enable bits
static const uint8_t ENABLE_INTERRUPT_BUTTON_BIT = 1;
static const uint8_t ENABLE_INTERRUPT_ENCODER_BIT = 0;

// Register map
enum TwistRegisters {
  TWIST_ID = 0x00,
  TWIST_STATUS = 0x01,           // 2 - button clicked, 1 - button pressed, 0 - encoder moved
  TWIST_VERSION = 0x02,
  TWIST_ENABLE_INTS = 0x04,      // 1 - button interrupt, 0 - encoder interrupt
  TWIST_COUNT = 0x05,
  TWIST_DIFFERENCE = 0x07,
  TWIST_LAST_ENCODER_EVENT = 0x09, // Millis since last movement of knob
  TWIST_LAST_BUTTON_EVENT = 0x0B,  // Millis since last press/release
  TWIST_RED = 0x0D,
  TWIST_GREEN = 0x0E,
  TWIST_BLUE = 0x0F,
  TWIST_CONNECT_RED = 0x10,      // Amount to change red LED for each encoder tick
  TWIST_CONNECT_GREEN = 0x12,
  TWIST_CONNECT_BLUE = 0x14,
  TWIST_TURN_INT_TIMEOUT = 0x16,
  TWIST_CHANGE_ADDRESS = 0x18,
  TWIST_LIMIT = 0x19,
};

class TwistDevice {
 public:
  TwistDevice() = default;
  
  bool begin(i2c::I2CDevice *i2c_device);
  bool is_connected();
  
  // Encoder functions
  int16_t get_count();
  bool set_count(int16_t amount);
  int16_t get_diff(bool clear_value = true);
  bool is_moved();
  bool is_pressed();
  bool is_clicked();
  uint16_t time_since_last_movement(bool clear_value = true);
  uint16_t time_since_last_press(bool clear_value = true);
  void clear_interrupts();
  
  // LED functions
  bool set_color(uint8_t red, uint8_t green, uint8_t blue);
  bool set_red(uint8_t red);
  bool set_green(uint8_t green);
  bool set_blue(uint8_t blue);
  uint8_t get_red();
  uint8_t get_green();
  uint8_t get_blue();
  
  // Connect functions
  bool connect_color(int16_t red, int16_t green, int16_t blue);
  bool connect_red(int16_t red);
  bool connect_green(int16_t green);
  bool connect_blue(int16_t blue);
  int16_t get_red_connect();
  int16_t get_green_connect();
  int16_t get_blue_connect();
  
  // Configuration
  bool set_limit(uint16_t limit);
  uint16_t get_limit();
  uint16_t get_int_timeout();
  bool set_int_timeout(uint16_t timeout);
  uint16_t get_version();
  void change_address(uint8_t new_address);
  
 protected:
  i2c::I2CDevice *i2c_device_{nullptr};
  
  uint8_t read_register(uint8_t addr);
  int16_t read_register_16(uint8_t addr);
  bool write_register(uint8_t addr, uint8_t val);
  bool write_register_16(uint8_t addr, uint16_t val);
  bool write_register_24(uint8_t addr, uint32_t val);
};

}  // namespace twist
}  // namespace esphome