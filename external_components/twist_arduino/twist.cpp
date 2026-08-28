#include "twist.h"
#include "esphome/core/log.h"

namespace esphome {
namespace twist {

static const char *const TAG = "twist";

void Twist::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Twist...");
  
  // Give extra time for I2C power to stabilize
  delay(100);
  
  // Initialize the device with ESPHome's I2C API
  if (!device_.begin(this)) {
    ESP_LOGE(TAG, "Device did not acknowledge at address 0x%02X! Check wiring.", this->address_);
    this->mark_failed();
    return;
  }
  
  // Test basic communication
  int test_position = device_.get_count();
  ESP_LOGD(TAG, "Initial position reading: %d", test_position);
  
  last_position_ = test_position;
  ESP_LOGCONFIG(TAG, "Twist setup complete at address 0x%02X", this->address_);
}

void Twist::update() {
  int position = device_.get_count();
  turn_direction_ = position - last_position_;
  last_position_ = position;
}

void Twist::dump_config() {
  ESP_LOGCONFIG(TAG, "Twist:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with Twist failed!");
  }
}

void TwistSensor::update() {
  if (parent_ == nullptr || parent_->is_failed()) {
    ESP_LOGD("twist", "Skipping sensor update - parent device failed");
    return;
  }

  float value = 0;
  switch (sensor_type_) {
    case SensorType::POSITION:
      value = parent_->get_position();
      break;
    case SensorType::CLICK:
      value = parent_->get_click() ? 1.0f : 0.0f;
      break;
    case SensorType::TURN_DIRECTION:
      value = parent_->get_turn_direction();
      break;
  }
  publish_state(value);
}

void TwistLight::write_state(light::LightState *state) {
  if (parent_ == nullptr) return;

  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  parent_->set_rgb(red * 255, green * 255, blue * 255);
}

}  // namespace twist
}  // namespace esphome
