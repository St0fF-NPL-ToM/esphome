#pragma once
/**
 *  Credit notice:
 *  ==============
 *  This component was not written by me.  I merely copied the absolute_humidity component,
 *  as it was much better researched, and modified it's computation to instead return the
 *  VPD of an offset Temperature @ the current relative air humidity.
 *
 *  Biological background: see e.g.
 *  - https://nph.onlinelibrary.wiley.com/doi/10.1111/nph.16485
 *  - https://www.nature.com/nature-index/topics/l4/vapor-pressure-deficit-effects-on-plant-growth-and-physiology
 */
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome::vapor_pressure_deficit {

/// Enum listing all implemented saturation vapor pressure equations.
enum SaturationVaporPressureEquation {
  BUCK,
  TETENS,
  WOBUS,
};

/// This class implements calculation of absolute humidity from temperature and relative humidity.
class VPDComponent final : public sensor::Sensor, public Component {
 public:
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { this->humidity_sensor_ = humidity_sensor; }
  void set_equation(SaturationVaporPressureEquation equation) { this->equation_ = equation; }
  void set_delta_t(float dt) { this->delta_t_ = dt; }

  void setup() override;
  void dump_config() override;
  void loop() override;

 protected:
  /** Buck equation for saturation vapor pressure in kPa.
   *
   * @param temperature_c Air temperature in °C.
   */
  static float es_buck(float temperature_c);
  /** Tetens equation for saturation vapor pressure in kPa.
   *
   * @param temperature_c Air temperature in °C.
   */
  static float es_tetens(float temperature_c);
  /** Wobus equation for saturation vapor pressure in kPa.
   *
   * @param temperature_c Air temperature in °C.
   */
  static float es_wobus(float temperature_c);

  // From https://www.environmentalbiophysics.org/chalk-talk-how-to-calculate-absolute-humidity/
  static constexpr float vapor_pressure(float es, float hr) { return es * hr; }
  // From logically thinking 1 step further:
  static constexpr float vapor_pressure_deficit(float es, float ea) { return es - ea; }

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};

  float temperature_{NAN};
  float humidity_{NAN};
  float delta_t_{NAN};
  SaturationVaporPressureEquation equation_;
};

}  // namespace esphome::vapor_pressure_deficit
