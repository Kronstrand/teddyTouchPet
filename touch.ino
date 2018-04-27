/*
  Copyright (c) 2015 NicoHood
  See the readme for credit to other people.

  AnalogTouch example
  Lights an Led if pin is touched and prints values to the Serial
*/

#include <FastLED.h>

#define NUM_LEDS 20
#define DATA_PIN 6

#define eyeOne 11
#define eyeTwo 13


// Define the array of leds
CRGB leds[NUM_LEDS];

// AnalogTouch
#include <AnalogTouch.h>

// Choose your analog and led pin
#define pinAnalog A0
#define pinLed LED_BUILTIN

// Slow down the automatic calibration cooldown
#define offset 2
#if offset > 6
#error "Too big offset value"
#endif

void setup()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setMaxRefreshRate(200);

  // Led setup
  pinMode(pinLed, OUTPUT);

  // Start Serial for debugging
  Serial.begin(115200);
}

//timer
float timer = 0;
float stateTimer = 0;
float lastMillis = 0;


//pet stats
float trust = 90; //from 0 to 100
float needy = 0; //from 0 to 100
String state = "sleeping"; //sleeping, active, angry
String lastState = "sleeping";
byte scale = 100; // 0-255


float activity = 0;
float oldCalibaretedValue = 0;

byte red = 0;
byte green = 0;
byte blue = 0;


void loop()
{
  //timer
  timer = timer + millis() - lastMillis;
  stateTimer = stateTimer + millis() - lastMillis;
  lastMillis = millis();


  // No second parameter will use 1 sample
  uint16_t value = analogTouchRead(pinAnalog);
  //value = analogTouchRead(pinAnalog, 100);

  // Self calibrate
  static uint16_t ref = 0xFFFF;
  if (value < (ref >> offset))
    ref = (value << offset);
  // Cool down
  else if (value > (ref >> offset))
    ref++;

  float calibratedValue = value - (ref >> offset);

  // Print touched?
  bool touched = (value - (ref >> offset)) > 40;



  
  // Light Led if touched
  // digitalWrite(pinLed, touched);

  float diff = calibratedValue - oldCalibaretedValue;
  if (diff < 0)
  {
    diff = diff * -1;
  }
  oldCalibaretedValue = calibratedValue;
  activity = activity * 0.999f + diff * 250.0f * 0.001f;

  float loweredActivity = activity - 1000; //700
  if (loweredActivity < 0)
  {
    loweredActivity = 0;
  }
  
  Serial.print(needy);
  Serial.print("\t");
  Serial.print(trust);
  Serial.print("\t");
  Serial.print(loweredActivity);
  Serial.print("\t");

  // Print calibrated value
  Serial.println(calibratedValue);
  //Serial.print("\t");

  // Print raw value
  //Serial.println(value);
  //Serial.print("\t");

  // Print raw ref
  //Serial.println(ref >> offset);
  // Serial.print("\t");
  //Serial.println(ref);

  
  //white -> green
  if (loweredActivity <= 255)
  {
    red = 255 - loweredActivity;
    green = 255;
    blue = 255 - loweredActivity;

  }
  //green -> yellow
  else if (loweredActivity <= 510)
  {
    red = loweredActivity - 255;
    green = 255;
    blue = 0;
  }
  //yellow -> red
  else if (loweredActivity <= 765)
  {
    red = 255;
    green = 255 - (loweredActivity - 510);
    blue = 0;
  }


  //still
  if (loweredActivity < 100)
  {
    //goes to sleep if not bored

    
    state = "sleeping";
    
    
    if (needy > 0)
    {
      trust -= 0.01f;
      
      /*
      //becomes calm by itself
      if (random(0, 6000) == 0)
      {
        needy = 0;
      }
      */
    }
    
    
    if (random(0, 50000) == 0)
    {
      needy = 100;
    }
  }

  
  // postive movement
  else if (loweredActivity < 510)
  {

    state = "active";
    needy -= 0.06f;
    trust += 0.01f;

  }
  //shaking
  else if (loweredActivity >= 510)
  {

    trust -= 0.02f;
    needy -= 0.06f;

  }

  //Serial.println(state);
  //Serial.print(trust);
  //Serial.print(",");
  //Serial.println(needy);


  float sinWave = (sin(timer / 2000) + 1) / 2 * 255;




    if (random(0, needy / 2) == 0) 
    {
      if (state == "sleeping")
      {
        //leds[i] = CRGB(sinWave, sinWave, sinWave);
        leds[eyeOne] = CRGB(red, green, blue);
        leds[eyeTwo] = CRGB(red, green, blue);
      }
      else if (state = "active")
      {
        leds[eyeOne] = CRGB(red, green, blue);
        leds[eyeTwo] = CRGB(red, green, blue);
      }
    }
    else // when needy it will add red lights
    {
      if (random(0, 3) == 0)
      {
        leds[eyeOne] = CRGB(255, 0, 0);
        leds[eyeTwo] = CRGB(255, 0, 0);
      }
      else
      {
        leds[eyeOne] = CRGB(0, 0, 0);
        leds[eyeTwo] = CRGB(0, 0, 0);
      }
    }
    yield();


  if (needy < 0)
  {
    needy = 0;
  }
  else if (needy > 100)
  {
    needy = 100;
  }

  if (trust < 0)
  {
    trust = 0;
  }
  else if (trust > 100)
  {
    trust = 100;
  }
  scale = trust * 2.55f;

  // Wait some time
  delay(1);
  FastLED.show(scale);
}

