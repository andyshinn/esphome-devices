#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/light/light_output.h"
#include "twist_device.h"

namespace esphome {
namespace twist {

enum class SensorType {
  POSITION,
  CLICK,
  TURN_DIRECTION
};

class Twist : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  int get_position() { return device_.get_count(); }
  bool get_click() { return device_.is_pressed(); }
  int get_turn_direction() { return turn_direction_; }
  void set_rgb(uint8_t red, uint8_t green, uint8_t blue) { device_.set_color(red, green, blue); }

 protected:
  TwistDevice device_;
  int last_position_{0};
  int turn_direction_{0};
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

class TwistLight : public light::LightOutput {
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
