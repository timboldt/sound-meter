#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

// const int16_t I2S_BCLK = 10;
// const int16_t I2S_LRCLK = I2S_BCLK + 1;  // Mandatory: BCLK + 1.
// const int16_t I2S_DOUT = 12;

// I2S i2s(INPUT);

const int16_t MIC_PIN = 10;

const int16_t NEO_FEATHERWING_PIN = 8;
Adafruit_NeoPixel neo_featherwing(32, NEO_FEATHERWING_PIN,
                                  NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);

    neo_featherwing.begin();

    pinMode(MIC_PIN, INPUT);

    // i2s.setDATA(I2S_DOUT);
    // i2s.setBCLK(I2S_BCLK);
    // i2s.setBitsPerSample(16);
    // i2s.setFrequency(22050);
    // i2s.begin();
}

void loop() {
    static float noise = 0;
    if (digitalRead(MIC_PIN)) {
        noise = noise * 0.5;
    } else {
        noise = 1.0;
    }

    const float alpha = 0.9;
    const float beta = 1.0 - alpha;
    static float val[4] = {0, 0, 0, 0};
    val[0] = min(8, max(0, noise * 8));
    val[1] = alpha * val[1] + beta * val[0];
    val[2] = alpha * val[2] + beta * val[1];
    val[3] = alpha * val[3] + beta * val[2];
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 8; col++) {
            const uint16_t range = 31000;
            const uint16_t offset = 31000;
            const uint16_t hue =
                offset - (uint16_t)(min(val[row], col + 1) * range / 8);
            const uint8_t brightness = 64 * max(0.0, min(1.0, val[row] - col));
            const uint32_t hsv = neo_featherwing.gamma32(
                neo_featherwing.ColorHSV(hue, 255, brightness));
            neo_featherwing.setPixelColor(row * 8 + col, hsv);
        }
    }
    neo_featherwing.show();
    delay(100);
}
