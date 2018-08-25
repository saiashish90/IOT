#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <BlynkSimpleEsp8266.h>
#define FAVCOLORS sizeof(myColors) / 3
#define PIN D6
#define Pixels 54
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
char auth[] = "57d1620dbb894e1e89b923a054a1c710";
char ssid[] = "D-link";
char pass[] = "Ayyagari_123";

int red = 0;
int green = 0;
int blue = 0;

int strb;
int rain;
int rndstrb;
int twinkle;

uint8_t myColors[][3] = {{232, 100, 255},   // purple
  {255,0,0},
  {255,255,255},
  {50,0,0},
  {0,255,255},
  {125,125,0},
  {0,125,0},
  {0,0,125},
  {0,0,255},
  {59,59,59},
  {255,255,0},
  {0, 255, 0},
  {255, 0, 255},
  {200, 200, 20},// yellow
  {30, 200, 200},   // blue
  {237, 159, 176},  // pink
  {255, 0, 0},      // red
  {108, 87, 229},    // Dark blue
  {162, 211, 172},   // Lt Green
};
int r,g,b;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);

void reconnectBlynk()
{
   if(!Blynk.connected())
   {
    if(Blynk.connect())
      BLYNK_LOG("Reconnected");
    else 
      BLYNK_LOG("Not Reconnected");
   }
}

void setall(int R, int G, int B)
{
  for (int i = 0; i < Pixels; i++) {
    strip.setPixelColor(i, strip.Color(R, G, B)); //  background color leds
    // This sends the updated pixel color to the hardware.
  }
  strip.show();
}

void Blynk_Delay(int milli)
{
   int end_time = millis() + milli;
   while(millis() < end_time){
       Blynk.run();
       yield();
   }
}

void fadein()
{
  for(uint8_t b=0; b <255; b++) 
  {
     for(uint8_t i=0; i < strip.numPixels(); i++) 
     {
        strip.setPixelColor(i, r*b/255, g*b/255, b*b/255);
     }
     strip.show();
     delay(5);
  }
}

void fadeout()
{
      for(uint8_t b=255; b > 0; b--) 
      {
        for(uint8_t i=0; i < strip.numPixels(); i++) 
        {
          strip.setPixelColor(i, r*b/255, g*b/255, b*b/255);
        }
        strip.show();
        delay(5);
      } 
  
}

void rainbowCycle(int SpeedDelay)
{
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycle of all colors on wheel
    for (i = 0; i < Pixels; i++) {
      c = Wheel(((i * 256 / Pixels) + j) & 255);
      strip.setPixelColor(i, strip.Color(*c, *(c + 1), *(c + 2)));
    }
    strip.show();
    delay(SpeedDelay);
  }
  setall(red, green, blue);
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void strobe()
{
  int c = random(FAVCOLORS);
  Serial.print(c);
  r = myColors[c][0];
  g = myColors[c][1];
  b = myColors[c][2];
  for (int i = 0; i < 5; i++)
  {
    setall(r, g, b);
    delay(1);
    setall(0, 0, 0);
    delay(100);
  }
  setall(red, green, blue);
}

void randstrobe()
{
    for(int i=0;i<1;i++)
    {  
    int c = random(FAVCOLORS);
    Serial.print(c);
    r = myColors[c][0];
    g = myColors[c][1];
    b = myColors[c][2];
    fadein();
    Blynk_Delay(5000);
    fadeout();
    delay(200);
    //fadeout(r,g,b);
    
    //setall(0, 0, 0);
    //strip.show();
    //delay(100);
    }
}

void Twinkle()
{
  for (int i = 0; i < 10; i++)
  {
    strip.setPixelColor(random(Pixels), strip.Color(random(0, 255), random(0, 255), random(0, 255)));
    strip.show();
    delay(50);
  }
  setall(red, green, blue);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(60000,reconnectBlynk);
  delay(50);
  strip.begin();
  strip.setBrightness(255);
  strip.show();
}

BLYNK_WRITE(V1)//color
{
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  red = R;
  green = G;
  blue = B;
  for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(R, G, B)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(0);
  
  //setall(red,green,blue);
  }
}
  
BLYNK_WRITE(V2)//strobe
{
  strb = param.asInt();
}

BLYNK_WRITE(V3)//rainbow
{
  rain = param.asInt();
}

BLYNK_WRITE(V4)//random colours
{
  rndstrb = param.asInt();
}

BLYNK_WRITE(V5)//twinkle
{
  twinkle = param.asInt();
}

BLYNK_WRITE(V6)//purple
{
  red = 255;
  green = 0;
  blue = 255;
  setall(red,green,blue);
}

BLYNK_WRITE(V7)//hot pink
{
   red = 255;
   green = 30;
   blue = 45;
   setall(red,green,blue);
  
}

BLYNK_WRITE(V8)//aqua
{
   red = 0;
   green = 255;
   blue = 255;
   setall(red,green,blue);
  
}
BLYNK_WRITE(V9)//nightlight
{
  red = 20;
  green = 20;
  blue = 20;
  setall(red,green,blue);
  
}

void loop()
{
  if(Blynk.connected())
    Blynk.run();
  timer.run();
  if (strb == 1 && rain != 1 && rndstrb != 1 && twinkle != 1)
    strobe();
  
  if (rain == 1 && strb != 1 && rndstrb != 1 && twinkle != 1)
    rainbowCycle(5);
  
  if (rndstrb == 1 && strb != 1 && rain != 1 && twinkle != 1)
      randstrobe();
  
  if (twinkle == 1 && strb != 1 && rain != 1 && rndstrb != 1)
    Twinkle();
  
  if (strb == 0 && rain == 0 && rndstrb == 0 && twinkle == 0)
    setall(red,green,blue);
}
