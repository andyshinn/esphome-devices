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
  
  // Check for turn events and fire triggers
  if (turn_direction_ > 0) {
    turn_clockwise_event_ = true;
    if (clockwise_trigger_ != nullptr) {
      clockwise_trigger_->trigger();
    }
  } else if (turn_direction_ < 0) {
    turn_counter_clockwise_event_ = true;
    if (counter_clockwise_trigger_ != nullptr) {
      counter_clockwise_trigger_->trigger();
    }
  }
  
  // Check for button state changes
  bool current_button_state = device_.is_pressed();
  if (current_button_state != last_button_state_) {
    if (current_button_state && press_trigger_ != nullptr) {
      press_trigger_->trigger();
    } else if (!current_button_state && release_trigger_ != nullptr) {
      release_trigger_->trigger();
    }
    last_button_state_ = current_button_state;
  }
  
  last_position_ = position;
}

void Twist::set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
  device_.set_color(red, green, blue);
}

uint8_t Twist::get_red() {
  return device_.get_red();
}

uint8_t Twist::get_green() {
  return device_.get_green();
}

uint8_t Twist::get_blue() {
  return device_.get_blue();
}

void Twist::dump_config() {
  ESP_LOGCONFIG(TAG, "Twist:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with Twist failed!");
  }
}

bool Twist::get_turn_clockwise() {
  bool event = turn_clockwise_event_;
  turn_clockwise_event_ = false;  // Clear the event after reading
  return event;
}

bool Twist::get_turn_counter_clockwise() {
  bool event = turn_counter_clockwise_event_;
  turn_counter_clockwise_event_ = false;  // Clear the event after reading
  return event;
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
    case SensorType::TURN_DIRECTION:
      value = parent_->get_turn_direction();
      break;
  }
  publish_state(value);
}

void TwistBinarySensor::update() {
  if (parent_ == nullptr || parent_->is_failed()) {
    ESP_LOGD("twist", "Skipping binary sensor update - parent device failed");
    return;
  }

  bool state = false;
  switch (sensor_type_) {
    case BinarySensorType::CLICK:
      state = parent_->get_click();
      break;
    case BinarySensorType::TURN_CLOCKWISE:
      state = parent_->get_turn_clockwise();
      break;
    case BinarySensorType::TURN_COUNTER_CLOCKWISE:
      state = parent_->get_turn_counter_clockwise();
      break;
  }
  publish_state(state);
}



void TwistLight::write_state(light::LightState *state) {
  if (parent_ == nullptr) return;

  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  parent_->set_rgb(red * 255, green * 255, blue * 255);
}

}  // namespace twist
}  // namespace esphome
