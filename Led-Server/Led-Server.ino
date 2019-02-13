#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "FastLED.h"

#define DATA_PIN    5
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
#define BRIGHTNESS          100
#define FRAMES_PER_SECOND  120
int selectedEffect=0;
boolean working = false;
const char* ssid = "SSID";
const char* password = "PASSWORD";

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleLED();
void handleNotFound();

void setup(void){

  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

//  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
//    Serial.println("mDNS responder started");
//  } else {
//    Serial.println("Error setting up MDNS responder!");
//  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/MOD=rainbow", HTTP_POST,{
    working = true;
    rainbow();
    working = false;
  }); 
  server.on("/MOD=confetti", HTTP_POST, confetti);
  server.on("/MOD=strobe", HTTP_POST,str);
  server.on("/MOD=sinelon", HTTP_POST, sinelon);
  server.on("/MOD=cylonBounce", HTTP_POST, cylnb);
  server.on("/MOD=newKitt", HTTP_POST, newkt);
  server.on("/MOD=juggle", HTTP_POST, juggle);
  server.on("/MOD=twinkleRandom", HTTP_POST, tkr);
  server.on("/MOD=sparkle", HTTP_POST, sprk);
  server.on("/MOD=runningLights", HTTP_POST, rnnl);
  server.on("/MOD=colorWipe", HTTP_POST, clrw);
  server.on("/MOD=allColor", HTTP_POST, allColor);
  server.on("/MOD=colorWipe", HTTP_POST, stripes);
  server.on("/MOD=disolve", HTTP_POST, disolve);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}
String webPage = R"=====(
 <head>
  <title>LED</title>
  <meta charset=\"utf-8\">
  <title></title>
  </head>
  <body>
  <br><br>  
<form action="/MOD=rainbow" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Rainbow </b></button></form>
<form action="/MOD=confetti" method="POST">  <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> Confetti </b></button></form>
<form action="/MOD=strobe" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Strobe </b></button></form>
<form action="/MOD=sinelon" method="POST">  <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> Sinelon </b></button></form>
<form action="/MOD=cylonBounce" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Cylon Bounce </b></button></form>
<form action="/MOD=newKitt" method="POST"> <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> New Kitt </b></button></form>
<form action="/MOD=juggle" method="POST"> <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Juggle </b></button></form>
<form action="/MOD=twinkleRandom" method="POST">  <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> Twinkle Random </b></button></form>
<form action="/MOD=sparkle" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Sparkle </b></button></form>
<form action="/MOD=runningLights" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Running Lights </b></button></form>
<form action="/MOD=colorWipe" method="POST">  <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> Color Wipe </b></button></form>
<form action="/MOD=rainbowCycle" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Rainbow Cycle </b></button></form>
<form action="/MOD=allColor" method="POST">  <button style="background-color: #4CAF50; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-bottom-right-radius: 20px;border-bottom-left-radius: 20px;width: 100%;height: 20%;"><b> All Color </b></button></form>
<form action="/MOD=disolve" method="POST">  <button style="background-color: #f44336; border: none; color: white; padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 50px;border-top-left-radius: 20px;border-top-right-radius: 20px;width: 100%;height: 20%;"><b> Close </b></button></form>
</body>
</html>

 )=====";

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", webPage);
}


void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}



/////MODS////
void str(){
  while(true){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  strobe(0xff, 0xff, 0xff, 10, 50, 1000);
  }
}
void cylnb(){
  while(true){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  cylonBounce(0xff, 0x00, 0x00, 4, 10, 50);
  }
}
void newkt(){
  while(true){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  newKitt(0xff, 0x00, 0x00, 8, 10, 50);
  }
}
void rainbow(){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  while(working){
  fill_rainbow( leds, NUM_LEDS, gHue, 15);
  }
}
void tkr(){
  while(true){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  twinkleRandom(20, 100, false);
  }
}
void sprk(){
  while(true){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  sparkle(0xff, 0xff, 0xff, 0);
  }
}
void rnnl(){
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  int i = 0;
  while(working){
   i++;
   if(i)
  runningLights(0xff,0x00,0x00, 50);  // red
  runningLights(0xff,0xff,0xff, 50);  // white
  runningLights(0x00,0x00,0xff, 50);  // blue
  }
}
void clrw(){
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  while(working){
 colorWipe(0x00,0xff,0x00, 50);
 colorWipe(0x00,0x00,0x00, 50);
  }
}
void rs(){
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  while(true){
  rainbowCycle(20);
  }
}
void confetti()
{ 
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);

}

void sinelon()
{
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);

}



void juggle() 
{
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
void stripes() {
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  byte * c1 = Wheel(256 / NUM_LEDS);
  byte * c2 = Wheel(256 / NUM_LEDS);
  int width = 10;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % (width * 2) < width) {
      setPixel(i, *c1, 0, 0);
    }
    else {
      setPixel(i, *c2, 0, 0);
    }
  }
}

void disolve()
{
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  int simultaneous = 15;
  int cycles = 2;
  int speed = 10;
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < simultaneous; j++) {
      int idx = random(NUM_LEDS);
      leds[idx] = CRGB::Black;
    }
    FastLED.show();
    delay(speed);
  }

  allColor();
}

void allColor()
{
 server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303); 
  CRGB c = CRGB::Black;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }

}

//void colorWipe()
//{
//  CRGB c = Wheel();
//  int speed = 5;
//  boolean direction = 0;
//  for (int i = 0; i < 2; i++) {
//    direction = !direction;
//    for (int i = 0; i < NUM_LEDS; i++) {
//      if (direction == 0) {
//        leds[i] = c;
//      }
//      else {
//        leds[NUM_LEDS - 1 - i] = c;
//      }
//      FastLED.show();
//      delay(speed);
//    }
//  }
//}
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}

void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}
void cylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void newKitt(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}
// used by NewKITT
void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUM_LEDS-EyeSize)/2); i>=0; i--) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUM_LEDS-EyeSize)/2); i++) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

// used by NewKITT
void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void twinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void runningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int i=0; i<NUM_LEDS*2; i++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      
      showStrip();
      delay(WaveDelay);
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

/////////////////////////
byte * Wheel(byte WheelPos) {
    static byte c[3];
    
    if(WheelPos < 85) {
     c[0]=WheelPos * 3;
     c[1]=255 - WheelPos * 3;
     c[2]=0;
    } else if(WheelPos < 170) {
     WheelPos -= 85;
     c[0]=255 - WheelPos * 3;
     c[1]=0;
     c[2]=WheelPos * 3;
    } else {
     WheelPos -= 170;
     c[0]=0;
     c[1]=WheelPos * 3;
     c[2]=255 - WheelPos * 3;
    }
  
    return c;
  }
  
void setPixel(int Pixel, byte red, byte green, byte blue) {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}
void showStrip() {
   FastLED.show();
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
