#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display.h"

#include <GxEPD2_3C.h>

namespace esphome {
namespace gxepd2 {

class GxEPD2Component : public PollingComponent,
                        public display::Display,
                        public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_HIGH,
                                              spi::CLOCK_PHASE_TRAILING, spi::DATA_RATE_10MHZ> {
 public:
  void set_dc_pin(InternalGPIOPin *dc_pin) { dc_pin_ = dc_pin; }
  void set_cs_pin(InternalGPIOPin *pin) { cs_pin_ = pin; }

  void setup() override;
  void dump_config() override;
  void update() override;

  void draw_pixel_at(int x, int y, Color color) override;
  void fill(Color color) override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }

 protected:
  GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> *epd_;
  InternalGPIOPin *dc_pin_;
  InternalGPIOPin *cs_pin_;

  int get_width() override;
  int get_height() override;
  // void draw_absolute_pixel_internal(int x, int y, Color color) override;
};

}  // namespace gxepd2
}  // namespace esphome
