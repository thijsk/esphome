#include "gxepd2Component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/display/display.h"
#include "esphome/components/display/display_color_utils.h"

namespace esphome {
namespace gxepd2 {

static const char *const TAG = "GxEPD2Component";

void GxEPD2Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SPI GxEPD2...");
  this->spi_setup();

  this->dc_pin_->setup();  // OUTPUT

  this->epd_ = new GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT>(
      GxEPD2_213_Z98c(this->cs_pin_->get_pin(), this->dc_pin_->get_pin(), -1, -1));

  this->epd_->init(115200, true, 2, false);
  this->epd_->setRotation(this->rotation_ / 90);
}

void GxEPD2Component::dump_config() {
  LOG_DISPLAY("", "SPI GxEPD2", this);
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_PIN("  DC Pin: ", this->dc_pin_);
  LOG_UPDATE_INTERVAL(this);
}

void HOT GxEPD2Component::draw_pixel_at(int x, int y, Color color) {
  ESP_LOGD(TAG, "GxEPD2Component::draw_pixel_at %d %d", x, y);
  auto color565 = display::ColorUtil::color_to_565(color);
  this->epd_->drawPixel(x, y, color565);
}

void HOT GxEPD2Component::fill(Color color) {
  auto color565 = display::ColorUtil::color_to_565(color);
  this->epd_->fillScreen(color565);
}

void HOT GxEPD2Component::update() {
  this->do_update_();

  this->epd_->display(false);
  this->epd_->powerOff();
  App.feed_wdt();
}

int HOT GxEPD2Component::get_width() { return this->epd_->width(); }
int HOT GxEPD2Component::get_height() { return this->epd_->height(); }

void GxEPD2Component::set_cs_pin(GPIOPin *cs) {
  ESP_LOGD(TAG, "GxEPD2Component::set_cs_pin");
  SPIDevice::set_cs_pin(cs);
  cs_pin_ = static_cast<InternalGPIOPin *>(cs);
}

}  // namespace gxepd2
}  // namespace esphome
