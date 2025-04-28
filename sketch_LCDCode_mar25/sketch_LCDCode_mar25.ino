#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 2  // Water flow sensor connected to pin 2 (Interrupt pin)

class WaterFlowMeter {
private:
    volatile int pulseCount;
    float flowRate;
    float totalLiters;
    unsigned long lastTime;
    LiquidCrystal_I2C lcd;
    bool isFlowing;

public:
    WaterFlowMeter() : pulseCount(0), flowRate(0), totalLiters(0), lastTime(0), lcd(0x27, 16, 2), isFlowing(false) {}

    void begin() {
        pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounterStatic, RISING);
        
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Water Flow Meter");
        delay(2000);
        lcd.clear();
        lastTime = millis();
    }

    static void pulseCounterStatic() {
        instance->pulseCounter();
    }

    void pulseCounter() {
        pulseCount++;
        isFlowing = true;  // Start calculating only after detecting flow
    }

    void update() {
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - lastTime;

        if (elapsedTime >= 1000 && isFlowing) {  // Update every second if water is flowing
            detachInterrupt(FLOW_SENSOR_PIN);
            
            flowRate = (pulseCount / 7.5);  // Convert pulses to L/min
            totalLiters += (flowRate / 60.0);  // Convert L/min to total liters

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Rate: ");
            lcd.print(flowRate);
            lcd.print(" L/min");

            lcd.setCursor(0, 1);
            lcd.print("Total: ");
            lcd.print(totalLiters);
            lcd.print(" L");

            pulseCount = 0;
            lastTime = currentTime;
            attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounterStatic, RISING);
        }
    }

    static WaterFlowMeter* instance; // Singleton instance
};

WaterFlowMeter* WaterFlowMeter::instance = new WaterFlowMeter();

void setup() {
    WaterFlowMeter::instance->begin();
}

void loop() {
    WaterFlowMeter::instance->update();
}
