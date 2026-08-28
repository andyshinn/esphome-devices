# BQ4050 BMS ESPHome Component

This is an ESPHome external component for interfacing with the Texas Instruments BQ4050 Battery Management System (BMS) over I2C.

## Features

This component provides access to the following BQ4050 sensors:
- Battery voltage
- Battery current (charge/discharge)
- Battery temperature
- State of charge (SoC) percentage
- Remaining capacity (mAh)
- Full charge capacity (mAh)
- Cycle count
- Battery status register
- Individual cell voltages (up to 4 cells)

## Configuration

Add this to your ESPHome configuration:

```yaml
external_components:
  - source:
      type: local
      path: external_components

i2c:
  sda: 21
  scl: 22
  scan: true

bq4050:
  update_interval: 30s

sensor:
  - platform: bq4050
    voltage:
      name: "Battery Voltage"
    current:
      name: "Battery Current"
    temperature:
      name: "Battery Temperature"
    state_of_charge:
      name: "Battery SoC"
    remaining_capacity:
      name: "Battery Remaining Capacity"
    full_charge_capacity:
      name: "Battery Full Charge Capacity"
    cycle_count:
      name: "Battery Cycle Count"
    battery_status:
      name: "Battery Status"
    cell1_voltage:
      name: "Cell 1 Voltage"
    cell2_voltage:
      name: "Cell 2 Voltage"
    cell3_voltage:
      name: "Cell 3 Voltage"
    cell4_voltage:
      name: "Cell 4 Voltage"
```

## Wiring

The BQ4050 communicates over I2C:
- Connect SDA to your ESP's SDA pin (typically GPIO 21 on ESP32)
- Connect SCL to your ESP's SCL pin (typically GPIO 22 on ESP32)
- Connect VCC to 3.3V
- Connect GND to ground

The default I2C address is 0x0B.

## Notes

- The component polls the BMS every 30 seconds by default (configurable)
- Temperature is converted from Kelvin to Celsius
- Voltages are in volts, current in amperes, capacity in ampere-hours
- The battery status register provides a raw 16-bit status value
- Not all sensors are required - configure only the ones you need