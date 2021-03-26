import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display, spi
from esphome.const import CONF_DATA_PINS, \
    CONF_ID, CONF_LAMBDA, CONF_MODEL, CONF_PAGES, CONF_RESET_PIN

DEPENDENCIES = []

ili9486_8bit_ns = cg.esphome_ns.namespace('ili9486_8bit')
ili9486 = ili9486_8bit_ns.class_('ILI9486Display', cg.PollingComponent,
                            display.DisplayBuffer)
ILI9486TFT35 = ili9486_8bit_ns.class_('ILI9486TFT35', ili9486)

ILI9486Model = ili9486_8bit_ns.enum('ILI9486Model')

MODELS = {
    'TFT_3.5': ILI9486Model.TFT_35,
}

ILI9486_MODEL = cv.enum(MODELS, upper=True, space="_")


def validate_pin_length(value):
    if len(value) != 8:
        raise cv.Invalid("Need 8 data pins")
    return value


CONFIG_SCHEMA = cv.All(display.FULL_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ili9486),
    cv.Required(CONF_MODEL): ILI9486_MODEL,
    cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_DATA_PINS): cv.All([pins.gpio_output_pin_schema], validate_pin_length),
    cv.Required('cs_pin'): pins.gpio_output_pin_schema,
    cv.Required('dc_pin'): pins.gpio_output_pin_schema,
    cv.Required('wr_pin'): pins.gpio_output_pin_schema,
    cv.Required('rd_pin'): pins.gpio_output_pin_schema, # This is just held to 1.
  }).extend( cv.polling_component_schema('1s')
    ), cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA))


def to_code(config):
    if config[CONF_MODEL] == 'TFT_3.5':
        lcd_type = ILI9486TFT35
    rhs = lcd_type.new()
    var = cg.Pvariable(config[CONF_ID], rhs)

    yield cg.register_component(var, config)
    yield display.register_display(var, config)
    cg.add(var.set_model(config[CONF_MODEL]))

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(config[CONF_LAMBDA], [(display.DisplayBufferRef, 'it')],
                                          return_type=cg.void)
        cg.add(var.set_writer(lambda_))
    if CONF_RESET_PIN in config:
        reset = yield cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))

    cs = yield cg.gpio_pin_expression(config['cs_pin'])
    cg.add(var.set_cs_pin(cs))
    dc = yield cg.gpio_pin_expression(config['dc_pin'])
    cg.add(var.set_dc_pin(dc))
    wr = yield cg.gpio_pin_expression(config['wr_pin'])
    cg.add(var.set_wr_pin(wr))
    rd = yield cg.gpio_pin_expression(config['rd_pin'])
    cg.add(var.set_rd_pin(rd))
    pins_ = []
    for conf in config[CONF_DATA_PINS]:
        pins_.append((yield cg.gpio_pin_expression(conf)))
    cg.add(var.set_data_pins(*pins_))

