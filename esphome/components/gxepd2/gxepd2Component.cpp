#include "gxepd2Component.h"
#include "esphome/core/log.h"

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
  this->epd_->setRotation(1);
  this->epd_->setFullWindow();
  this->epd_->firstPage();
}

void GxEPD2Component::dump_config() {
  LOG_DISPLAY("", "SPI GxEPD2", this);
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_PIN("  DC Pin: ", this->dc_pin_);
  LOG_UPDATE_INTERVAL(this);
}

void HOT GxEPD2Component::draw_absolute_pixel_internal(int x, int y, Color color) {
  ESP_LOGD(TAG, "GxEPD2Component::draw_absolute_pixel_internal %d %d %d", x, y, color);
  auto color565 = display::ColorUtil::color_to_565(color);
  this->epd_->drawPixel(x, y, color565);
}

void GxEPD2Component::update() {
  ESP_LOGD(TAG, "GxEPD2Component::update");
  this->epd_->fillScreen(GxEPD_BLACK);
  while (this->epd_->nextPage())
    ;
  this->epd_->powerOff();
}

int HOT GxEPD2Component::get_width_internal() { return this->epd_->width(); }
int HOT GxEPD2Component::get_height_internal() { return this->epd_->height(); }

}  // namespace gxepd2
}  // namespace esphome
