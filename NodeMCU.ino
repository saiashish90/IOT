
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <BlynkSimpleEsp8266.h>
#define FAVCOLORS sizeof(myColors) / 3
#define PIN D6
#define Pixels 5
// You should get Auth Token in the Blynk App.
char auth[] = "75349cf51e1647bc8e25faf1af4a526e";
// Your WiFi credentials.
// Set password to "" for open networks.
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
  {0, 255, 0},
  {255, 0, 255},
  {200, 200, 20},// yellow
  {30, 200, 200},   // blue
  {237, 159, 176},  // pink
  {255, 0, 0},      // red
  {108, 87, 229},    // Dark blue
  {162, 211, 172},   // Lt Green
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);

void setall(int R, int G, int B)
{
  for (int i = 0; i < Pixels; i++) {
    strip.setPixelColor(i, strip.Color(R, G, B)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
  }
}

void rainbowCycle(int SpeedDelay)
{
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 1; j++) { // 1 cycle of all colors on wheel
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
  for (int i = 0; i < 10; i++)
  {
    setall(red, green, blue);
    strip.show();
    delay(50);
    setall(0, 0, 0);
    strip.show();
    delay(50);
  }
  setall(red, green, blue);
}

void randstrobe()
{
  for (int i = 0; i < 5; i++)
  {
    int c = random(FAVCOLORS);
    Serial.print(c);
    int r = myColors[c][0];
    int g = myColors[c][1];
    int b = myColors[c][2];
    setall(r, g, b);
    strip.show();
    delay(100);
    setall(0, 0, 0);
    strip.show();
    delay(100);
  }
  setall(red, green, blue);
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
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  delay(50);
  strip.begin();
  strip.setBrightness(255);
  strip.show();
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}
BLYNK_WRITE(V1)
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
    delay(50);
  
  //setall(red,green,blue);
  }
}
  
BLYNK_WRITE(V2)
{
  strb = param.asInt();
}

BLYNK_WRITE(V3)
{
  rain = param.asInt();
}

BLYNK_WRITE(V4)
{
  rndstrb = param.asInt();
}

BLYNK_WRITE(V5)
{
  twinkle = param.asInt();
}

BLYNK_WRITE(V6)//purple
{
   for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(255, 0, 255)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  
  //setall(red,green,blue);
  }
}

BLYNK_WRITE(V7)//hot pink
{
   for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(255, 30, 45)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  
  //setall(red,green,blue);
  }
}

BLYNK_WRITE(V8)//aqua
{
   for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(0, 255, 255)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  
  //setall(red,green,blue);
  }
}
BLYNK_WRITE(V9)//brightness
{
   for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(20, 20, 20)); //  background color leds
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  
  //setall(red,green,blue);
  }
}

void loop()
{
  Blynk.run();
  if (strb == 1 && rain != 1 && rndstrb != 1 && twinkle != 1)
    strobe();
  if (rain == 1 && strb != 1 && rndstrb != 1 && twinkle != 1)
    rainbowCycle(5);
  if (rndstrb == 1 && strb != 1 && rain != 1 && twinkle != 1)
    randstrobe();
  if (twinkle == 1 && strb != 1 && rain != 1 && rndstrb != 1)
    Twinkle();
}

