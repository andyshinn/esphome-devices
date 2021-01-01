#include "esphome.h"
#include "hpma115s0.h"

class HPMA115Sensor : public PollingComponent {
  public:
    Sensor *pm_2_5_sensor = new Sensor();
    Sensor *pm_10_sensor = new Sensor();
    HPMA115S0 *hpma;

    HPMA115Sensor(UARTComponent *uart) : PollingComponent(60000) {
      this->hpma = new HPMA115S0(*uart);
    }

  void setup() override {
    hpma->Init();
    hpma->StartParticleMeasurement();
  }

  void update() override {
    unsigned short pm2_5, pm10;

    if (hpma->ReadParticleMeasurement(&pm2_5, &pm10)) {
      pm_2_5_sensor->publish_state(pm2_5);
      pm_10_sensor->publish_state(pm10);
    }
  }
};
