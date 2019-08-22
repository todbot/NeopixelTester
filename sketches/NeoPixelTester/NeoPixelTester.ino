/* NeoPixelTester 
 * ------------------- 
 * Simple Hello world for serial ports.
 * Print out "Hello world!" and blink pin 13 LED every second.
 *
 * Created 13 Aug 2019
 * 2019 Tod E. Kurt <tod@todbot.com>`http://todbot.com/
 */

#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

//#define NUMPIXELS 16 // Popular NeoPixel ring size
#define NUMPIXELS 50 

// Pins in use
const int LED_PIN     = 13; // onboard LED
const int BUTT_PIN    = 0;  // D0 A2
const int NEO_PIN     = 2;  // D2 A1
const int SLIDER1_PIN = A0; // D1
const int SLIDER2_PIN = A4; // D4
const int SLIDER3_PIN = A3; // D3
const int DOTSTAR_DAT = 7; 
const int DOTSTAR_CLK = 8; 

Adafruit_DotStar dotstrip = Adafruit_DotStar( 1, DOTSTAR_DAT, DOTSTAR_CLK, DOTSTAR_BGR);

Adafruit_NeoPixel* pixels;

uint8_t r,g,b,w;
int startLed = 0;
int endLed = NUMPIXELS;
uint8_t brightness = 255;
uint32_t debugNextMillis;

bool rgbw_mode = false;

void setup() 
{
    dotstrip.begin(); // Initialize pins for output
    dotstrip.setPixelColor(0,0x003300);
    dotstrip.show();
    
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT); 
    pinMode(SLIDER1_PIN, INPUT);
    pinMode(SLIDER2_PIN, INPUT);
    pinMode(SLIDER3_PIN, INPUT);
    pinMode(BUTT_PIN, INPUT_PULLUP);

    pixels = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
    pixels->begin();
    pixels->setBrightness(brightness);

    // set RGBW on startup
    if( digitalRead(BUTT_PIN) == LOW ) { 
        rgbw_mode = true;
        //pixels->updateType( NEO_RGBW );
        pixels = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRBW + NEO_KHZ800);
    }
  
    // say hello
    for(int i=0; i<NUMPIXELS; i++) { 
        pixels->setPixelColor(i, pixels->Color(0, 150, 0));
        pixels->show();   // Send the updated pixel colors to the hardware.
        delay(30);
    }  
}

void loop () 
{
    digitalWrite(LED_PIN, HIGH);
    int v0 = digitalRead(BUTT_PIN);
    int v1 = analogRead(SLIDER1_PIN);
    int v2 = analogRead(SLIDER2_PIN);
    int v3 = analogRead(SLIDER3_PIN);
  
    // debug once every 100 millis
    if( debugNextMillis - millis() > 100 ) { 
        debugNextMillis = millis() + 100;
        Serial.printf("mode:rgb%c ins: %d, %d, %d, %d, st:%d end:%d bri:%d\n",
            (rgbw_mode?'w':' '), v0, v1,v2,v3, startLed, endLed, brightness);
    }
 
    if( v0 ) {  // normal mode: sliders are RGB
        dotstrip.setPixelColor(0,0x330033); // mode
        dotstrip.show();
        r = v1 / 4;
        g = v2 / 4;
        b = v3 / 4;
    }
    else {      // alt mode
        dotstrip.setPixelColor(0,0x333333);
        dotstrip.show();
        brightness = v3 / 4;
        startLed = map( v1, 0,1023, 0,NUMPIXELS-1);
        int numLeds = map( v2, 0,1023, 1,NUMPIXELS);
        endLed = startLed + numLeds;
        if( endLed > NUMPIXELS ) { 
            endLed = NUMPIXELS;       
        }
        pixels->setBrightness(brightness);
    }

    pixels->clear();
    for(int i=startLed; i<endLed; i++) { 
        pixels->setPixelColor(i, pixels->Color(r,g,b));
    }
    pixels->show();
  
    digitalWrite(LED_PIN, LOW);
}
