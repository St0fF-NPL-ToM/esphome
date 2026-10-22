import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_EQUATION,
    CONF_HUMIDITY,
    CONF_TEMPERATURE,
    DEVICE_CLASS_PRESSURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_PASCAL,
)
from esphome.types import ConfigType

CONF_DELTA_T = "delta_t"

vapor_pressure_deficit_ns = cg.esphome_ns.namespace("vapor_pressure_deficit")
VPDComponent = vapor_pressure_deficit_ns.class_(
    "VPDComponent", sensor.Sensor, cg.Component
)

SaturationVaporPressureEquation = vapor_pressure_deficit_ns.enum(
    "SaturationVaporPressureEquation"
)
EQUATION = SaturationVaporPressureEquation.options("BUCK", "TETENS", "WOBUS")

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_PASCAL,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_PRESSURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(VPDComponent),
            cv.Required(CONF_TEMPERATURE): cv.use_id(sensor.Sensor),
            cv.Required(CONF_HUMIDITY): cv.use_id(sensor.Sensor),
            cv.Optional(CONF_EQUATION, default="WOBUS"): cv.enum(EQUATION, upper=True),
            cv.Optional(CONF_DELTA_T, default=-2.25): cv.float_with_unit(
                "temperature", "(°C|° C|C|°)?"
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config: ConfigType) -> None:
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    await cg.add_variable(var.set_temperature_sensor, config[CONF_TEMPERATURE])
    await cg.add_variable(var.set_humidity_sensor, config[CONF_HUMIDITY])

    cg.add(var.set_equation(config[CONF_EQUATION]))
    cg.add(var.set_delta_t(config[CONF_DELTA_T]))
