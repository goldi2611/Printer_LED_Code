#include <JC_Button.h>
#include "FastLED.h"


FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    3
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

Button myButton(4, true, true, 50);  // Declare the butto

void setup() {
  Serial.begin(115200);
  delay(3000); // 3 second delay for recovery
 
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {fill_solid1,fill_solid2,fill_solid3};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
void loop()
{ 
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show(); 
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  readbutton();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  
}

//White
void fill_solid1()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,255,255)); 
}

//Red
void fill_solid2()
{
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0)); 
}

//Blue
void fill_solid3()
{
  fill_solid( leds, NUM_LEDS, CRGB(0,0,255)); 
}

//BUTTON STUFF
//---------Function to read the button and do something----------
void readbutton() 
{
  myButton.read();
  if(myButton.wasPressed()) 
  {
    Serial.println("Button pressed!  Next pattern...   ");
    nextPattern();  // Change to the next pattern
  }
}//end_readbutton



