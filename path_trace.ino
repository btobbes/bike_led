#include <FastLED.h>

#define DATA_PIN_FRAME 5
#define DATA_PIN_MAST 7
#define NUM_LEDS_FRAME 120
#define NUM_LEDS_MAST 115

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds_frame[NUM_LEDS_FRAME];
CRGB leds_mast[NUM_LEDS_MAST];

#define BRIGHTNESS  96
#define FRAMES_PER_SECOND 120



void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<NEOPIXEL, DATA_PIN_FRAME>(leds_frame, NUM_LEDS_FRAME);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_MAST>(leds_mast, NUM_LEDS_MAST);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {sinelon};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gCurrentPattern_frame = 0;
uint8_t gCurrentPattern_mast = 1;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  // Call the current pattern function once, updating the 'leds' array

  //fill_rainbow( leds_frame, NUM_LEDS_FRAME, gHue, 7);

  sinelon();
  sinelon_frame();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_mast, NUM_LEDS_MAST, 20);
  int pos = beatsin16( 8, 0, NUM_LEDS_MAST-1 );
  leds_mast[pos] += CHSV( gHue, 255, 192);
}

void sinelon_frame()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_frame, NUM_LEDS_FRAME, 20);
  int pos = beatsin16( 8, 0, NUM_LEDS_FRAME-1 );
  leds_frame[pos] += CHSV( gHue, 255, 192);
}
