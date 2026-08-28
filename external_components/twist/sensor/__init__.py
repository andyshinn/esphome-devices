import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_TYPE,
    ICON_ROTATE_RIGHT,
    UNIT_EMPTY,
)

from .. import twist_ns, Twist

DEPENDENCIES = ["twist"]

TwistSensor = twist_ns.class_("TwistSensor", sensor.Sensor, cg.PollingComponent)

CONF_TWIST_ID = "twist_id"

TWIST_SENSOR_TYPES = {
    "position": {
        "icon": ICON_ROTATE_RIGHT,
        "unit_of_measurement": UNIT_EMPTY,
    },
    "click": {
        "icon": "mdi:gesture-tap-button",
        "unit_of_measurement": UNIT_EMPTY,
    },
    "turn_direction": {
        "icon": "mdi:rotate-right-variant", 
        "unit_of_measurement": UNIT_EMPTY,
    },
}


def validate_sensor_type(value):
    if value not in TWIST_SENSOR_TYPES:
        raise cv.Invalid(f"Invalid sensor type: {value}")
    return value


CONFIG_SCHEMA = sensor.sensor_schema(
    TwistSensor,
    icon=ICON_ROTATE_RIGHT,
    unit_of_measurement=UNIT_EMPTY,
).extend(
    {
        cv.Required(CONF_TWIST_ID): cv.use_id(Twist),
        cv.Required(CONF_TYPE): validate_sensor_type,
    }
).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    parent = await cg.get_variable(config[CONF_TWIST_ID])
    cg.add(var.set_twist_parent(parent))
    
    sensor_type = config[CONF_TYPE]
    cg.add(var.set_sensor_type(cg.RawExpression(f"twist::SensorType::{sensor_type.upper()}")))
    
    # Apply sensor type specific configuration
    if sensor_type in TWIST_SENSOR_TYPES:
        sensor_config = TWIST_SENSOR_TYPES[sensor_type]
        if "icon" in sensor_config and CONF_ID not in config:
            cg.add(var.set_icon(sensor_config["icon"]))