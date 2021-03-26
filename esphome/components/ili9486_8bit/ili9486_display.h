#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "ili9486_defines.h"
#include "ili9486_init.h"

namespace esphome {
namespace ili9486_8bit {

enum ILI9486Model { TFT_35 };

class ILI9486Display : public PollingComponent, public display::DisplayBuffer {
 public:
  float get_setup_priority() const override;
  void set_reset_pin(GPIOPin *reset) { this->reset_pin_ = reset; }

  void set_cs_pin(GPIOPin *cs_pin) { cs_pin_ = cs_pin; }
  void set_dc_pin(GPIOPin *dc_pin) { dc_pin_ = dc_pin; }
  void set_wr_pin(GPIOPin *wr_pin) { wr_pin_ = wr_pin; }
  void set_rd_pin(GPIOPin *rd_pin) { rd_pin_ = rd_pin; }
  void set_data_pins(GPIOPin *d0, GPIOPin *d1, GPIOPin *d2, GPIOPin *d3, GPIOPin *d4, GPIOPin *d5, GPIOPin *d6,
                     GPIOPin *d7) {
    this->data_pins_[0] = d0;
    this->data_pins_[1] = d1;
    this->data_pins_[2] = d2;
    this->data_pins_[3] = d3;
    this->data_pins_[4] = d4;
    this->data_pins_[5] = d5;
    this->data_pins_[6] = d6;
    this->data_pins_[7] = d7;
  }

  void set_model(ILI9486Model model) { this->model_ = model; }

  void command(uint8_t value);
  void data(uint8_t value);
  void send_command(uint8_t command_byte, const uint8_t *data_bytes, uint8_t num_data_bytes);
  virtual void initialize() = 0;

  void update() override;

  void fill(Color color) override;

  void dump_config() override;
  void setup() override {
    this->setup_pins_();
    this->initialize();
  }

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void setup_pins_();

  void init_lcd_(const uint8_t *init_cmd);
  void set_addr_window_(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void invert_display_(bool invert);
  void reset_();
  void fill_internal_(Color color);
  void display_();
  uint16_t convert_to_16bit_color_(uint8_t color_8bit);
  uint8_t convert_to_8bit_color_(uint16_t color_16bit);

  ILI9486Model model_;
  int16_t width_{320};   ///< Display width as modified by current rotation
  int16_t height_{480};  ///< Display height as modified by current rotation
  uint16_t x_low_{0};
  uint16_t y_low_{0};
  uint16_t x_high_{0};
  uint16_t y_high_{0};

  uint32_t get_buffer_length_();
  int get_width_internal() override;
  int get_height_internal() override;

  void start_command_();
  void end_command_();
  void start_data_();
  void end_data_();

  void write_byte(uint8_t data);
  uint8_t read_byte();
  void write_array(const uint8_t *data, size_t length);
  void enable();
  void disable();

  GPIOPin *reset_pin_{nullptr};
  GPIOPin *cs_pin_;
  GPIOPin *dc_pin_;
  GPIOPin *wr_pin_;
  GPIOPin *rd_pin_;
  GPIOPin *data_pins_[8];
};

//-----------   ILI9486_24_TFT display --------------
class ILI9486TFT35 : public ILI9486Display {
 public:
  void initialize() override;
};
}  // namespace ili9486_8bit
}  // namespace esphome
