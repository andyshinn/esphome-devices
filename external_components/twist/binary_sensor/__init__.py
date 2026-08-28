import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_TYPE

from .. import twist_ns, Twist

DEPENDENCIES = ["twist"]

TwistBinarySensor = twist_ns.class_("TwistBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent)

CONF_TWIST_ID = "twist_id"

TWIST_BINARY_SENSOR_TYPES = {
    "click": "CLICK",
    "turn_clockwise": "TURN_CLOCKWISE", 
    "turn_counter_clockwise": "TURN_COUNTER_CLOCKWISE",
}


def validate_binary_sensor_type(value):
    if value not in TWIST_BINARY_SENSOR_TYPES:
        raise cv.Invalid(f"Invalid binary sensor type: {value}")
    return value


CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(TwistBinarySensor).extend(
    {
        cv.Required(CONF_TWIST_ID): cv.use_id(Twist),
        cv.Required(CONF_TYPE): validate_binary_sensor_type,
    }
).extend(cv.polling_component_schema("100ms"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)

    parent = await cg.get_variable(config[CONF_TWIST_ID])
    cg.add(var.set_twist_parent(parent))
    
    sensor_type = config[CONF_TYPE]
    cg.add(var.set_sensor_type(cg.RawExpression(f"twist::BinarySensorType::{TWIST_BINARY_SENSOR_TYPES[sensor_type]}")))