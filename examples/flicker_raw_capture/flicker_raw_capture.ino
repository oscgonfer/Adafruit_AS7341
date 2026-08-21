/* This example captures raw, evenly-spaced samples from the AS7341's
   flicker photodiode (ADC5). */

#include <Adafruit_AS7341.h>

Adafruit_AS7341 as7341;

// FD_TIME sets the sample period as `fd_time * 2.78us`. 700 gives a sample
// rate of roughly 1e6 / (700 * 2.78) =~ 515 Hz, i.e. Nyquist ~257Hz -
// comfortably covers mains flicker (50/60/100/120Hz) and typical
// low-frequency PWM dimming. Raise fd_time to slow down/increase
// resolution, lower it to chase higher frequencies (up to ~2kHz per the
// datasheet, with an appropriately small fd_time).
#define FD_TIME 700
#define FD_GAIN AS7341_GAIN_16X
#define NUM_SAMPLES 256

uint16_t samples[NUM_SAMPLES];

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  if (!as7341.begin()) {
    Serial.println("Could not find AS7341");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  bool ok = as7341.captureFlickerRaw(samples, NUM_SAMPLES, FD_TIME, FD_GAIN);

  if (!ok) {
    Serial.println("Capture failed (timed out or FIFO overflowed)");
    delay(1000);
    return;
  }

  Serial.print("Captured ");
  Serial.print(NUM_SAMPLES);
  Serial.print(" samples at ");
  Serial.print(as7341.getFlickerSampleRateHz());
  Serial.println(" Hz");

  for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
    Serial.println(samples[i]);
  }

  delay(2000);
}
