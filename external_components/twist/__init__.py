import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']

CONF_ON_CLOCKWISE = "on_clockwise"
CONF_ON_COUNTER_CLOCKWISE = "on_counter_clockwise"
CONF_ON_PRESS = "on_press"
CONF_ON_RELEASE = "on_release"

twist_ns = cg.esphome_ns.namespace('twist')
Twist = twist_ns.class_('Twist', cg.PollingComponent, i2c.I2CDevice)

# Trigger classes
TwistClockwiseTrigger = twist_ns.class_("TwistClockwiseTrigger", automation.Trigger.template())
TwistCounterClockwiseTrigger = twist_ns.class_("TwistCounterClockwiseTrigger", automation.Trigger.template())
TwistPressTrigger = twist_ns.class_("TwistPressTrigger", automation.Trigger.template())
TwistReleaseTrigger = twist_ns.class_("TwistReleaseTrigger", automation.Trigger.template())

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Twist),
    cv.Optional(CONF_ON_CLOCKWISE): automation.validate_automation(
        {cv.GenerateID(): cv.declare_id(TwistClockwiseTrigger)}
    ),
    cv.Optional(CONF_ON_COUNTER_CLOCKWISE): automation.validate_automation(
        {cv.GenerateID(): cv.declare_id(TwistCounterClockwiseTrigger)}
    ),
    cv.Optional(CONF_ON_PRESS): automation.validate_automation(
        {cv.GenerateID(): cv.declare_id(TwistPressTrigger)}
    ),
    cv.Optional(CONF_ON_RELEASE): automation.validate_automation(
        {cv.GenerateID(): cv.declare_id(TwistReleaseTrigger)}
    ),
}).extend(cv.polling_component_schema('100ms')).extend(i2c.i2c_device_schema(0x3F))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for conf in config.get(CONF_ON_CLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_ID])
        cg.add(var.set_clockwise_trigger(trigger))
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_COUNTER_CLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_ID])
        cg.add(var.set_counter_clockwise_trigger(trigger))
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_PRESS, []):
        trigger = cg.new_Pvariable(conf[CONF_ID])
        cg.add(var.set_press_trigger(trigger))
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_RELEASE, []):
        trigger = cg.new_Pvariable(conf[CONF_ID])
        cg.add(var.set_release_trigger(trigger))
        await automation.build_automation(trigger, [], conf)
