# teddyTouchPet
A prototype of an interactive digital teddybear using physical touch as sensor input


What you need:
  - Arduino. For this tutrial, an arduino uno R3 is used.
  - Piece of tinfoil
  - A teddy bear
  - LED strips lights
  - [FastLED API](https://github.com/FastLED/FastLED)
  - [AnalogTouch API](https://github.com/NicoHood/AnalogTouch)

Connect a piece of tinfoil to your ardunio with a wire. 
In our case we connect it to the analog pin A0

Connect the LED strips by 3 wires: 
  - GND -> GND
  - +5v -> 5v5
  - DO  -> 6 

Download appropriate APIs

Download and then upload the source code to your arduino.

Cut open your cute teddy bear and mount it with with the rigged ardunio.
Make sure to align the tinfoil with the surface of the teddybear you wish to be touch sensible.

In order for it to work, you need to calibrate touch input to have an baseline around 0.
Adjust the last number of the following code to achieve that.

```float loweredActivity = activity - 1000;```

You pet should now be ready for play.
