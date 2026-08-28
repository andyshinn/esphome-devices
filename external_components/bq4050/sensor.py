import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_BATTERY,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_AMPERE_HOUR,
)

from . import BQ4050Component, bq4050_ns

CONF_VOLTAGE = "voltage"
CONF_CURRENT = "current"
CONF_TEMPERATURE = "temperature"
CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_REMAINING_CAPACITY = "remaining_capacity"
CONF_FULL_CHARGE_CAPACITY = "full_charge_capacity"
CONF_CYCLE_COUNT = "cycle_count"
CONF_BATTERY_STATUS = "battery_status"
CONF_CELL1_VOLTAGE = "cell1_voltage"
CONF_CELL2_VOLTAGE = "cell2_voltage"
CONF_CELL3_VOLTAGE = "cell3_voltage"
CONF_CELL4_VOLTAGE = "cell4_voltage"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(BQ4050Component),
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_STATE_OF_CHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_BATTERY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_REMAINING_CAPACITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FULL_CHARGE_CAPACITY): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CYCLE_COUNT): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_BATTERY_STATUS): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CELL1_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CELL2_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CELL3_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CELL4_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ID])

    if voltage_config := config.get(CONF_VOLTAGE):
        sens = await sensor.new_sensor(voltage_config)
        cg.add(hub.set_voltage_sensor(sens))

    if current_config := config.get(CONF_CURRENT):
        sens = await sensor.new_sensor(current_config)
        cg.add(hub.set_current_sensor(sens))

    if temperature_config := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature_config)
        cg.add(hub.set_temperature_sensor(sens))

    if state_of_charge_config := config.get(CONF_STATE_OF_CHARGE):
        sens = await sensor.new_sensor(state_of_charge_config)
        cg.add(hub.set_state_of_charge_sensor(sens))

    if remaining_capacity_config := config.get(CONF_REMAINING_CAPACITY):
        sens = await sensor.new_sensor(remaining_capacity_config)
        cg.add(hub.set_remaining_capacity_sensor(sens))

    if full_charge_capacity_config := config.get(CONF_FULL_CHARGE_CAPACITY):
        sens = await sensor.new_sensor(full_charge_capacity_config)
        cg.add(hub.set_full_charge_capacity_sensor(sens))

    if cycle_count_config := config.get(CONF_CYCLE_COUNT):
        sens = await sensor.new_sensor(cycle_count_config)
        cg.add(hub.set_cycle_count_sensor(sens))

    if battery_status_config := config.get(CONF_BATTERY_STATUS):
        sens = await sensor.new_sensor(battery_status_config)
        cg.add(hub.set_battery_status_sensor(sens))

    if cell1_voltage_config := config.get(CONF_CELL1_VOLTAGE):
        sens = await sensor.new_sensor(cell1_voltage_config)
        cg.add(hub.set_cell1_voltage_sensor(sens))

    if cell2_voltage_config := config.get(CONF_CELL2_VOLTAGE):
        sens = await sensor.new_sensor(cell2_voltage_config)
        cg.add(hub.set_cell2_voltage_sensor(sens))

    if cell3_voltage_config := config.get(CONF_CELL3_VOLTAGE):
        sens = await sensor.new_sensor(cell3_voltage_config)
        cg.add(hub.set_cell3_voltage_sensor(sens))

    if cell4_voltage_config := config.get(CONF_CELL4_VOLTAGE):
        sens = await sensor.new_sensor(cell4_voltage_config)
        cg.add(hub.set_cell4_voltage_sensor(sens))