import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID

from .. import twist_ns, Twist

DEPENDENCIES = ["twist"]

TwistLight = twist_ns.class_("TwistLight", light.LightOutput, cg.Component)

CONF_TWIST_ID = "twist_id"

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(TwistLight),
        cv.Required(CONF_TWIST_ID): cv.use_id(Twist),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)

    parent = await cg.get_variable(config[CONF_TWIST_ID])
    cg.add(var.set_twist_parent(parent))