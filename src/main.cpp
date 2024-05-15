#include <Arduino.h>
#include "TFTDisplay.h"
#include "RP2040_PWM.h"
#include "CANReceiver.h"
#include "DS18B20_INT.h"

constexpr uint8_t COMPRESSOR_PWM_PIN = 18u;
constexpr uint8_t FAN_PWM_PIN = 19u;
constexpr float PWM_FREQUENCY = 20000.0f;
constexpr uint8_t TEMP_SENS_PIN = 7u;

TFTDisplay screen = TFTDisplay();
CANReceiver footpedal = CANReceiver();
RP2040_PWM *PWM_Instance[2];
OneWire oneWire(TEMP_SENS_PIN);
DS18B20_INT tempSensor(&oneWire);
float tempValue = 0.0f;

void setup(void)
{
    Serial.begin(115200);
    screen.initDisplay();
    footpedal.initCAN();
    PWM_Instance[0] = new RP2040_PWM(COMPRESSOR_PWM_PIN, PWM_FREQUENCY, 0);
    PWM_Instance[1] = new RP2040_PWM(FAN_PWM_PIN, PWM_FREQUENCY, 0);

    tempSensor.begin();
    tempSensor.setResolution(12);
    tempSensor.requestTemperatures();
}

void loop()
{
    footpedal.readCAN();

    for (int i = 0; i <= 1024; i++)
    {
        screen.updateDisplay(i, tempValue);
        if(tempSensor.isConversionComplete())
        {
            tempValue = tempSensor.getTempCentiC() / 100.0f;
            tempSensor.requestTemperatures();
        }
    }
}
