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
  this->cs_pin_->setup();  // OUTPUT

  this->epd_ = new GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT>(
      GxEPD2_213_Z98c(this->cs_pin_->get_pin(), this->dc_pin_->get_pin(), -1, -1));

  this->epd_->init(115200, true, 2, false);
  switch (this->rotation_) {
    case esphome::display::DisplayRotation::DISPLAY_ROTATION_0_DEGREES:
      this->epd_->setRotation(0);
      break;
    case esphome::display::DisplayRotation::DISPLAY_ROTATION_90_DEGREES:
      this->epd_->setRotation(1);
      break;
    case esphome::display::DisplayRotation::DISPLAY_ROTATION_180_DEGREES:
      this->epd_->setRotation(2);
      break;
    case esphome::display::DisplayRotation::DISPLAY_ROTATION_270_DEGREES:
      this->epd_->setRotation(3);
      break;
  }
  this->epd_->setFullWindow();
  this->epd_->firstPage();
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

void GxEPD2Component::fill(Color color) {
  ESP_LOGD(TAG, "GxEPD2Component::fill");
  auto color565 = display::ColorUtil::color_to_565(color);
  this->epd_->fillScreen(color565);
}

void GxEPD2Component::update() {
  ESP_LOGD(TAG, "GxEPD2Component::update");

  this->do_update_();

  while (this->epd_->nextPage()) {
    App.feed_wdt();
  };
  this->epd_->powerOff();
  App.feed_wdt();
}

int HOT GxEPD2Component::get_width() {
  ESP_LOGD(TAG, "GxEPD2Component::get_width %d", this->epd_->width());
  return this->epd_->width();
}
int HOT GxEPD2Component::get_height() {
  ESP_LOGD(TAG, "GxEPD2Component::get_height %d", this->epd_->height());
  return this->epd_->height();
}

}  // namespace gxepd2
}  // namespace esphome
