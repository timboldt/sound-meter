#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel neo_featherwing(32, D8, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(1200);
    pinMode(LED_BUILTIN, OUTPUT);
    neo_featherwing.begin();
}

void loop()
{
    const float alpha = 0.9;
    const float beta = 1.0 - alpha;
    static float val[4] = {0, 0, 0, 0};
    val[0] = min(8, max(0, random(3) + val[0] - 1));
    val[1] = alpha * val[1] + beta * val[0];
    val[2] = alpha * val[2] + beta * val[1];
    val[3] = alpha * val[3] + beta * val[2];
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (val[row] < col)
            {
                neo_featherwing.setPixelColor(row * 8 + col, 0);
            }
            else
            {
                const uint16_t range = 30000;
                const uint16_t offset = 25000;
                const uint16_t hue = offset - (uint16_t)(min(val[row], col + 1) * range / 8);
                const uint32_t hsv = neo_featherwing.ColorHSV(hue, 255, 4);
                neo_featherwing.setPixelColor(row * 8 + col, hsv);
            }
        }
    }
    neo_featherwing.show();
    delay(100);
}
