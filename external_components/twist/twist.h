#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/light/light_output.h"
#include "twist_device.h"

namespace esphome {
namespace twist {

enum class SensorType {
  POSITION,
  TURN_DIRECTION
};

enum class BinarySensorType {
  CLICK,
  TURN_CLOCKWISE,
  TURN_COUNTER_CLOCKWISE
};

// Forward declaration
class TwistLight;

// Trigger classes for automation
class TwistClockwiseTrigger : public Trigger<> {};
class TwistCounterClockwiseTrigger : public Trigger<> {};
class TwistPressTrigger : public Trigger<> {};
class TwistReleaseTrigger : public Trigger<> {};

class Twist : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  int get_position() { return device_.get_count(); }
  bool get_click() { return device_.is_pressed(); }
  int get_turn_direction() { return turn_direction_; }
  void set_rgb(uint8_t red, uint8_t green, uint8_t blue);
  
  // Get current RGB values from device
  uint8_t get_red();
  uint8_t get_green();
  uint8_t get_blue();

  // Turn event detection
  bool get_turn_clockwise();
  bool get_turn_counter_clockwise();
  
  // Trigger setters
  void set_clockwise_trigger(TwistClockwiseTrigger *trigger) { clockwise_trigger_ = trigger; }
  void set_counter_clockwise_trigger(TwistCounterClockwiseTrigger *trigger) { counter_clockwise_trigger_ = trigger; }
  void set_press_trigger(TwistPressTrigger *trigger) { press_trigger_ = trigger; }
  void set_release_trigger(TwistReleaseTrigger *trigger) { release_trigger_ = trigger; }
  

 protected:
  TwistDevice device_;
  int last_position_{0};
  int turn_direction_{0};
  bool turn_clockwise_event_{false};
  bool turn_counter_clockwise_event_{false};
  bool last_button_state_{false};
  
  // Triggers
  TwistClockwiseTrigger *clockwise_trigger_{nullptr};
  TwistCounterClockwiseTrigger *counter_clockwise_trigger_{nullptr};
  TwistPressTrigger *press_trigger_{nullptr};
  TwistReleaseTrigger *release_trigger_{nullptr};
  
};

class TwistSensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_twist_parent(Twist *parent) { parent_ = parent; }
  void set_sensor_type(SensorType type) { sensor_type_ = type; }

  void update() override;

 protected:
  Twist *parent_{nullptr};
  SensorType sensor_type_;
};

class TwistBinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void set_twist_parent(Twist *parent) { parent_ = parent; }
  void set_sensor_type(BinarySensorType type) { sensor_type_ = type; }

  void update() override;

 protected:
  Twist *parent_{nullptr};
  BinarySensorType sensor_type_;
};

class TwistLight : public light::LightOutput, public Component {
 public:
  void set_twist_parent(Twist *parent) { parent_ = parent; }

  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }

  void write_state(light::LightState *state) override;

 protected:
  Twist *parent_{nullptr};
};

}  // namespace twist
}  // namespace esphome
