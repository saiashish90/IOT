/*
 Version 0.1 - March 17 2018
*/ 
#define BLYNK_PRINT Serial
#include <Adafruit_NeoPixel.h>
#include <BlynkSimpleEsp8266.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> //  https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries
#include <ArduinoJson.h> // https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries
#include <StreamString.h>
#define Pixels 60
#define PIN D6


ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;
BlynkTimer timer;
BlynkTimer timer1;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);



#define FAVCOLORS sizeof(myColors) / 3

#define MyApiKey "eb6b3eed-6f13-4e69-acfc-39ed42573771" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MySSID "D-link" // TODO: Change to your Wifi network SSID
#define MyWifiPassword "Ayyagari_123" // TODO: Change to your Wifi network password
#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

// You should get Auth Token in the Blynk App.
char auth[] = "c14bb65343614e929f3a81a302e1ac29";
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

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void reconnectBlynk()
{
   if(!Blynk.connected())
   {
    if(Blynk.connect())
      Serial.println("Connected");
    else 
      Serial.println("Not connected");
   }
   ESP.wdtFeed();
}

void setall(int R, int G, int B)
{
  for (int i = 0; i < Pixels; i++) 
  {
    strip.setPixelColor(i, strip.Color(R, G, B));
    strip.show();
  }
  
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
}

void conpin()
{
  webSocket.sendPing();
  ESP.wdtFeed();
}

void turnOn(String deviceId) 
{
  if (deviceId == "5bcb7c61a4582b53259db378") // Device ID of first device
  {  
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    red = 255;
    green = 255;
    blue = 255;
    setall(red,green.blue);
  }      
}

void turnOff(String deviceId) 
{
   if (deviceId == "5bcb7c61a4582b53259db378") // Device ID of first device
   {  
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     setall(0,0,0);
   }
}

void brightness(String deviceId,int brightness) 
{
   if (deviceId == "5bcb7c61a4582b53259db378") // Device ID of first device
   {  
     Serial.println(brigh); 
     strip.setBrightness(255*brightness/100);
     strip.show();
   }
}

void color(String deviceId, int color)
{
          if (deviceId == "5bcb7c61a4582b53259db378")
          {
            red = c/(65536);

            green = (c/256) % 256;

            blue = c%256;
            
            setall(red,green,blue);
          
            Serial.println(c);
            Serial.println(red);
            Serial.println(green);
            Serial.println(blue);  
          }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) 
{
  switch(type) 
  {
    case WStype_DISCONNECTED:
      isConnected = false;    
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: 
    {
      isConnected = true;
      Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
      Serial.printf("Waiting for commands from sinric.com ...\n");        
     }
      break;
    case WStype_TEXT: 
    {
        Serial.printf("[WSc] get text: %s\n", payload);
        // Example payloads

        // For Switch  types
        // {"deviceId":"xxx","action":"action.devices.commands.OnOff","value":{"on":true}} // https://developers.google.com/actions/smarthome/traits/onoff
        // {"deviceId":"xxx","action":"action.devices.commands.OnOff","value":{"on":false}}

        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload); 
        String deviceId = json ["deviceId"];     
        String action = json ["action"];
        
        if(action == "action.devices.commands.OnOff") 
        {
            String value = json ["value"]["on"];
            Serial.println(value);           
            if(value == "true") 
            {
                turnOn(deviceId);
            } 
            else 
            {
                turnOff(deviceId);
            }
        }
        else if (action == "action.devices.commands.BrightnessAbsolute")
        {
          int a = json ["value"]["brightness"];
          brightness(deviceId,a);          
        }
        else if (action == "action.devices.commands.ColorAbsolute")
        {
          int c = json["value"]["color"]["spectrumRGB"];
          color(deviceId,c);
        }
        
        else if (action == "test") 
        {
            Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
      case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() 
{
  Serial.begin(115200);
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
    
  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);  

  // Waiting for Wifi connect
  while(WiFiMulti.run() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  if(WiFiMulti.run() == WL_CONNECTED) 
  {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/"); //"iot.sinric.com", 80

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);
  
  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(60000,reconnectBlynk);
  timer1.setInterval(5000,conpin);
  
  strip.begin();
  strip.setBrightness(255);
  strip.show();
}

BLYNK_WRITE(V1)//color
{
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
   
  setall(red,green,blue);
  
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
  setall(50,50,50);
}


void loop() 
{
  Blynk.run();
  timer.run();
  timer1.run();
  webSocket.loop();
  
  if(isConnected) 
  {
      uint64_t now = millis();
      
      // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
      if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) 
      {
          heartbeatTimestamp = now;
          webSocket.sendTXT("H");          
      }
  }
    
    if (strb == 1 && rain != 1 && rndstrb != 1 && twinkle != 1)
      strobe();
  
    if (rain == 1 && strb != 1 && rndstrb != 1 && twinkle != 1)
      rainbowCycle(5);
  
    if (rndstrb == 1 && strb != 1 && rain != 1 && twinkle != 1)
        randstrobe();
  
    if (twinkle == 1 && strb != 1 && rain != 1 && rndstrb != 1)
      Twinkle();
     
    ESP.wdtFeed();
      delay(500);
 
  
}
 
