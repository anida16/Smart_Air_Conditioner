/* Copyright 2016, 2018 David Conran
   Copyright 2020 Sadid Rafsun Tulon

  An IR LED circuit *MUST* be connected to the ESP8266 on a pin
  as specified by kIrLed below.

  TL;DR: The IR LED needs to be driven by a transistor for a good result.

  Suggested circuit:
      https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending

  Common mistakes & tips:
*   * Don't just connect the IR LED directly to the pin, it won't
      have enough current to drive the IR LED effectively.
*   * Make sure you have the IR LED polarity correct.
      See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
*   * Typical digital camera/phones can be used to see if the IR LED is flashed.
      Replace the IR LED with a normal LED if you don't have a digital camera
      when debugging.
*   * Avoid using the following pins unless you really know what you are doing:
*     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
*     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
*     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
*   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
      for your first time. e.g. ESP-12 etc.
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const uint16_t kIrLed = 10;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRGreeAC ac(kIrLed);  // Set the GPIO to be used for sending messages.

void printState() {
  // Display the settings.
  Serial.println("GREE A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kGreeStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

int power_value;
int Auto_value;
int AC_temp = 28;
int fan_speed_value;
int send_signal;
int ac_mode_value;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "eQrZfi7NwABgPwL_V1OuDid65YxKJimZ";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dalvi 2.4GHz";
char pass[] = "anishaditi";

BLYNK_WRITE(V0)
{
  power_value = param.asInt();
}

BLYNK_WRITE(V1)
{
  Auto_value = param.asInt();
}

BLYNK_WRITE(V2) {
  switch (param.asInt())
  {
    case 1: // Item 1
      Serial.println("kGreeAuto selected");
      ac_mode_value = 1;
      break;
    case 2: // Item 2
      Serial.println("kGreeCool selected");
      ac_mode_value = 2;
      break;
    case 3: // Item 3
      Serial.println("kGreeDry selected");
      ac_mode_value = 3;
      break;
    case 4: // Item 4
      Serial.println("kGreeFan selected");
      ac_mode_value = 4;
      break;
    case 5: // Item 5
      Serial.println("kGreeHeat selected");
      ac_mode_value = 5;
      break;
    default:
      Serial.println("Unknown item selected");
  }
}
BLYNK_WRITE(V3)
{
  AC_temp = param.asInt();
}

BLYNK_WRITE(V4)
{
  fan_speed_value = param.asInt();
}

BLYNK_WRITE(V5)
{
  send_signal = param.asInt();
}

void setup() {
  ac.begin();
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  delay(200);
  
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
}

void loop() {

  ac.on();
  ac.setFan(fan_speed_value);

  if (ac_mode_value == 1)
  { Serial.println("AC Default Auto Mode");
    ac.setMode(kGreeAuto);
  }

  if (ac_mode_value == 2)
  { Serial.println("Cool Mode");
    ac.setMode(kGreeCool);
  }

  if (ac_mode_value == 3)
  { Serial.println("Dry Mode");
    ac.setMode(kGreeDry);
  }

  if (ac_mode_value == 4)
  { Serial.println("Fan Mode");
    ac.setMode(kGreeFan);
  }

  if (ac_mode_value == 5)
  { Serial.println("Heat Mode");
    ac.setMode(kGreeHeat);
  }


  // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
  //ac.setMode(kGreeFan);
  //ac.setTemp(28);  // 16-30C
  ac.setTemp(AC_temp);
  ac.setSwingVertical(true, kGreeSwingAuto);
  ac.setXFan(false);
  ac.setLight(true);
  ac.setSleep(true);
  ac.setTurbo(false);

  Blynk.run();

  Serial.print("Power ");
  Serial.println(power_value);
  /*
    if (ac_mode_value == 1) { ac_mode[] = "kGreeAuto";}
    if (ac_mode_value == 2) { ac_mode[] = "kGreeCool";}
    if (ac_mode_value == 3) { ac_mode[] = "kGreeDry";}
    if (ac_mode_value == 4) { ac_mode[] = "kGreeFan";}
    if (ac_mode_value == 5) { ac_mode[] = "kGreeHeat";}
  */

  Serial.print("AC Mode ");
  Serial.println(ac_mode_value);

  Serial.print("Auto mode ");
  Serial.println(Auto_value);

  Serial.print("AC Temperature ");
  Serial.println(AC_temp);

  Serial.print("Fan Speed ");
  Serial.println(fan_speed_value);

  // Now send the IR signal.

  if (send_signal == 1)
  {
#if SEND_GREE
    Serial.println("Sending IR command to A/C ...");
    ac.send();
#endif  // SEND_GREE
    printState();
  }

  delay(5000);
}
