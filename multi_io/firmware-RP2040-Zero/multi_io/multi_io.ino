#include <osap.h>
#include <Adafruit_NeoPixel.h>

#define builtin_LED_Pin 16

  int DigitalIn[4]  = { 0,1,2,3 };
  int DigitalOut[4] = { 4,5,6,7 };
  int AnalogIn[2]   = { 26,27 };
  int AnalogOut[2]  = { 28,29 };

  OSAP_Runtime osap;
  OSAP_Gateway_USBSerial serLink(&Serial);
  OSAP_Port_DeviceNames namePort("multi_io");

/**** RGB Control ****/

  Adafruit_NeoPixel builtin_LED(1, builtin_LED_Pin, NEO_GRB + NEO_KHZ800);

  void _setRGB (uint8_t* Data, size_t Length) {
    int R = (Length < 2 ? 0 : Data[1]);
    int G = (Length < 4 ? 0 : Data[3]);
    int B = (Length < 6 ? 0 : Data[5]);

    builtin_LED.setPixelColor(0,builtin_LED.Color(R,G,B));
    builtin_LED.show();
  }
  OSAP_Port_Named setRGB("setRGB",_setRGB);

/**** Digital Input ("active low" because of pull-up resistor) ****/

  size_t _getDigital (uint8_t* Data, size_t Length, uint8_t* Response) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 3)) {
        int Value = digitalRead(DigitalIn[Port]);
          Response[0] = (Value == LOW ? 1 : 0);                      // no typo!
        return 1;     // when connected to GND, _getDigital sends 1, otherwise 0
      }
    }
    return 0;
  }
  OSAP_Port_Named getDigital("getDigital",_getDigital);

/**** Digital Output ****/

  void _setDigital (uint8_t* Data, size_t Length) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 3)) {
        digitalWrite(DigitalOut[Port], (Length == 1) || (Data[1] == 0) ? LOW : HIGH);
      }
    }
  }
  OSAP_Port_Named setDigital("setDigital",_setDigital);

/**** Analog Input ****/

  size_t _getAnalog (uint8_t* Data, size_t Length, uint8_t* Response) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 1)) {
        uint16_t Value = analogRead(AnalogIn[Port]);
          Response[0] = Value & 0xFF;
          Response[1] = Value >> 8 & 0xFF;
        return 2;
      }
    }
    return 0;
  }
  OSAP_Port_Named getAnalog("getAnalog",_getAnalog);

/**** Analog Output ****/

  void _setAnalog (uint8_t* Data, size_t Length) {
    if (Length > 0) {
      int Port = Data[0];
      if ((Port >= 0) && (Port <= 1)) {
        analogWrite(AnalogOut[Port], (Length < 2 ? 0 : Data[1] + Data[2]*255));
      }
    }
  }
  OSAP_Port_Named setAnalog("setAnalog",_setAnalog);

/**** Startup ****/

  void setup() {
    osap.begin();

    analogWriteResolution(16);             // according to RP2040 specifications
    analogReadResolution(12);                                            // dto.

    builtin_LED.begin();
    builtin_LED.setPixelColor(0,builtin_LED.Color(0,16,0));
    builtin_LED.show();

    for (int Port = 0; Port < 4; Port++) {
      pinMode (DigitalIn[Port],INPUT_PULLUP);    // for a well-defined potential
      pinMode(DigitalOut[Port],OUTPUT);
    }

    for (int Port = 0; Port < 2; Port++) {
      analogRead(AnalogIn[Port]);

      pinMode(AnalogOut[Port],OUTPUT);
      analogWrite(AnalogOut[Port],0);
    }
  }

/**** Operation ****/

  void loop() {
    osap.loop();
  }