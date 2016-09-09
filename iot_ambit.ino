/*
* Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
* All rights reserved
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <user_interface.h>
}

const boolean CONFIG_MODE_WIFI_OPEN = true; //change to false to create password-protected WiFi network in config mode

const unsigned int WEB_SERVER_PORT = 80;
WiFiServer webServer(WEB_SERVER_PORT);

#include "version.h"

#include "diag.h"
#include "webcc.h"
#include "webconfig.h"

using TempDiagLog = diag::TempDiagLog;
using WebConfig = webconfig::WebConfig<TempDiagLog>;
using WebConfigControl = webcc::WebConfigControl<TempDiagLog, HTTPReqParserStateMachine, HTTPStream,
      WebConfig, TempDiagLog>;

namespace diag {
const Texts PROGMEM texts;
};
namespace webcc {
const Texts PROGMEM texts;
};
namespace webconfig {
const Texts PROGMEM texts;
};

#include "diag_legacy.h"

#define BLYNK_PRINT DiagLog

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <BlynkSimpleEsp8266.h>
#pragma GCC diagnostic pop

#include <Math.h>
#include <EEPROM.h>

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "adc.h"
#include "eeprom_config.h"
#include "stringmap.h"
#include "uiTexts.h"

#include "webconfig.h"
#include "webcc.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

/*
 * Config mode SSID and password prefix
 * Config SSID will be ssidConfigModePrefix + ESP8266.ChipId
 * Config Password will be ssidConfigModePrefix + ESP8266.FlashChipId
 */

const char PROGMEM ssidConfigModePrefix[] = "ESP8266-";
const char PROGMEM passwordConfigModePrefix[] = "ESP";

/*
 * Pin mapping
 */

const int PIN_SWITCH_PROG = 0;    //GPIO0 = PROG switch
const int PIN_LED_FAULT = 2;      //GPIO2 = fault (red LED)
const int PIN_SENSOR_ONEWIRE = 4; //GPIO4 = optional OneWire signal
const int PIN_SENSOR_DHT = 5;     //GPIO5 = AM2321 signal
const int PIN_SWITCH_CONFIG = 12; //GPIO12 = config switch
const int PIN_SENSOR_MG811 = A0;  //ADC = MG811 analog signal

/*
 * Update timings
 */

const unsigned long UPDATE_TIME_SENSORS = 2500;//ms
const unsigned long UPDATE_TIME_MG811 = 200;//ms
const unsigned long UPDATE_TIME_STATUS_LEDS = 250;//ms
const unsigned long UPDATE_TIME_STATUS_VPINS = 500;//ms
const unsigned long UPDATE_TIME_VALUE_VPINS = 500;//ms

/*
 * Data and status values
 */

boolean isFaultDHT = false;
boolean isFaultOneWire = false;
boolean isFaultMG811 = false;

float valueTemperatureDHT = 0.0;
float valueHumidityDHT = 0.0;
float valueTemperatureOneWire = 0.0;
float valueMG811 = 0.0;
int valueMG811uncal = 0;

boolean statusProgLED = false;
boolean statusFaultLED = false;
boolean statusOperateLED = false;

/*
 * DS18B20 sensor
 */

OneWire oneWire(PIN_SENSOR_ONEWIRE);
DallasTemperature sensorsDS18B20(&oneWire);

void updateSensorOneWire(void) {
  const int ONEWIRE_SENSOR_INDEX = 0;
  const float ONEWIRE_ALARM_TRESHOULD = -127.0;
  sensorsDS18B20.requestTemperatures();
  valueTemperatureOneWire = (float)sensorsDS18B20.getTempCByIndex(ONEWIRE_SENSOR_INDEX);
  isFaultOneWire = (valueTemperatureOneWire <= ONEWIRE_ALARM_TRESHOULD);
  if (isFaultOneWire) valueTemperatureOneWire = NAN;
}

/*
 * DHT temperture/humidity sensor
 */

//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(PIN_SENSOR_DHT, DHTTYPE, 15);

void updateSensorDHT(void) {
  valueTemperatureDHT = (float)dht.readTemperature();
  valueHumidityDHT = (float)dht.readHumidity();
  isFaultDHT = (isnan(valueTemperatureDHT)) || (isnan(valueHumidityDHT));
}

/*
 * MG811
 */

const int MG811_ALARM_TRESHOULD = 64;

//Calibration data
double calDataMG811_a = -0.02;
double calDataMG811_b = 18;

void calcCalDataMG811(void) {
  const double Y1 = log((double)eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  const double Y2 = log((double)eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  const double X1 = (double)eepromSavedParametersStorage.MG811CalPoint0Raw;
  const double X2 = (double)eepromSavedParametersStorage.MG811CalPoint1Raw;
  DiagLog.println(F("Calculating MG811 calibration data..."));
  DiagLog.print(F("Reference points: "));
  DiagLog.print(F("Raw="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint0Raw);
  DiagLog.print(F(" Calibrated="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  DiagLog.print(F("ppm / Raw="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint1Raw);
  DiagLog.print(F(" Calibrated="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  DiagLog.println(F("ppm"));
  double tempCalDataMG811_a = (Y2 - Y1) / (X2 - X1);
  double tempCalDataMG811_b = Y1 - calDataMG811_a * X1;
  DiagLog.print("Calibration data: a=");
  DiagLog.print(calDataMG811_a);
  DiagLog.print(" b=");
  DiagLog.println(calDataMG811_b);
  if (isnan(tempCalDataMG811_a) ||
      isinf(tempCalDataMG811_a) ||
      isnan(tempCalDataMG811_b) ||
      isinf(tempCalDataMG811_b)) {
    DiagLog.println(F("Calibration data error: calibration data rejected"));
  }
  else {
    calDataMG811_a = tempCalDataMG811_a;
    calDataMG811_b = tempCalDataMG811_b;
    DiagLog.println(F("Calibration data accepted"));
  }
  if (eepromSavedParametersStorage.rejectCalibrationMG811) {
    DiagLog.println(F("Uncalibrated mode selected, no ppm value will be calculated, the output value is 1024 - ADC_value."));
  }
}

unsigned int calcConcentrationCO2 (unsigned int rawAdcValue) {
  if (eepromSavedParametersStorage.rejectCalibrationMG811) return (1024 - rawAdcValue);
  return ((unsigned int)exp(calDataMG811_a * ((double)rawAdcValue) + calDataMG811_b));
}

unsigned int movingAverage(unsigned int input) {
  const static unsigned int AVERAGE_POINTS = 64;
  static unsigned int inputValuesHistory[AVERAGE_POINTS] = {0};
  static unsigned int currentValue = 0;
  static unsigned long totalValue;
  static boolean firstRun = true;
  if (firstRun) {
    firstRun = false;
    totalValue = input * AVERAGE_POINTS;
    for (unsigned int i = 0; i < AVERAGE_POINTS; i++)
      inputValuesHistory[i] = input;
  }
  totalValue += input;
  totalValue -= inputValuesHistory[currentValue];
  inputValuesHistory[currentValue] = input;
  currentValue++;
  if (currentValue >= AVERAGE_POINTS) currentValue = 0;
  return ((unsigned int)(totalValue / (unsigned long)AVERAGE_POINTS));
}

unsigned int lowPass(unsigned int input, float dt, float fc) {
  //See https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter for details
  if ((dt == 0.0) || (fc == 0.0)) return (0);
  float rc = 1 / (2 * PI * fc);
  float alpha = dt / (dt + rc);
  static float lastOutput = 0.0;
  float currentOutput = lastOutput + alpha * ((float)input - lastOutput);
  lastOutput = currentOutput;
  return ((unsigned int)currentOutput);
}

void updateSensorMG811(void) {
  unsigned int raw = analogRead(PIN_SENSOR_MG811);
  valueMG811uncal = raw;
  isFaultMG811 = (raw < MG811_ALARM_TRESHOULD);
  if (isFaultMG811) {
    valueMG811 = NAN;
    return;
  }
  valueMG811 = calcConcentrationCO2(raw);
  const float millisPerSecond = 1000.0;
  const float unitsPerHz = 100.0;
  switch (eepromSavedParametersStorage.filterMG811) {
    case ADCFilter::OFF:
      break;
    case ADCFilter::AVERAGE:
      valueMG811 = movingAverage(valueMG811);
      break;
    case ADCFilter::LOWPASS:
      valueMG811 = lowPass(valueMG811, (float)UPDATE_TIME_MG811 / millisPerSecond, (float)eepromSavedParametersStorage.filterMG811LowPassFrequency / unitsPerHz);
      break;
    default:
      eepromSavedParametersStorage.filterMG811 = ADCFilter::OFF;
      break;
  }
}

/*
 * Set status LEDs on front panel
 */

void updateStatusLEDs (boolean isConfigMode) {
  static boolean blinkBit = false;
  //Prog LED (set externally)
  statusProgLED = !digitalRead(PIN_SWITCH_PROG);
  //Operate LED
  boolean isConnect = false;
  if (!isConfigMode) isConnect = Blynk.connected();
  statusOperateLED = isConnect;
  //Fault LED
  if (!isConfigMode)
    statusFaultLED = isFaultDHT || isFaultMG811 || (!Blynk.connected());
  else
    statusFaultLED = isFaultDHT || isFaultMG811 || blinkBit;
  blinkBit = ! blinkBit;
  //Set final LED statuses
  //Note: true = LED off, false = LED on
  digitalWrite(PIN_LED_FAULT, !statusFaultLED);
}

/*
 * Send virtual pins state to Blynk server
 * Setting all the virtual pins at once will result in too flood error
 * To prevent disconnecting from server, data are sent one at a time
 */

void updateValueVirtualPins(void) {
  static int currentPhase = 0;
  switch (currentPhase) {
    case 0:
      //V4 = DHT tempertaure
      Blynk.virtualWrite(V4, valueTemperatureDHT);
      break;
    case 1:
      //V5 = DHT humidity
      Blynk.virtualWrite(V5, valueHumidityDHT);
      break;
    case 2:
      //V6 = OneWire temperature
      Blynk.virtualWrite(V6, valueTemperatureOneWire);
      break;
    case 3:
      //V7 = MG811 signal
      Blynk.virtualWrite(V7, valueMG811);
      break;
  }
  currentPhase++;
  if (currentPhase > 3) currentPhase = 0;
}

void updateStatusVirtualPins(void) {
  static boolean lifeBit = false;
  const int BLYNK_WIDGET_LED_ON = 255;
  const int BLYNK_WIDGET_LED_OFF = 0;
  static int currentPhase = 0;
  switch (currentPhase) {
    case 0:
      //V0 = DHT fault
      Blynk.virtualWrite(V0, isFaultDHT ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 1:
      //V1 = OneWire fault
      Blynk.virtualWrite(V1, isFaultOneWire ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 2:
      //V2 = MQ2 fault
      Blynk.virtualWrite(V2, isFaultMG811 ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
    case 3:
      //V7 = lifebit
      lifeBit = !lifeBit;
      Blynk.virtualWrite(V3, lifeBit ? BLYNK_WIDGET_LED_ON : BLYNK_WIDGET_LED_OFF);
      break;
  }
  currentPhase++;
  if (currentPhase > 3) currentPhase = 0;
}

void printSensorDebugInfo(void) {
  if (!eepromSavedParametersStorage.sensorSerialOutput) return;

  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.print(F("] sensor values:"));

  DiagLog.print(F(" DHT T = "));
  DiagLog.print(valueTemperatureDHT);
  DiagLog.print(F(" RH = "));
  DiagLog.print(valueHumidityDHT);

  DiagLog.print(F(" OneWire(0) T = "));
  DiagLog.print(valueTemperatureOneWire);

  if (!eepromSavedParametersStorage.rejectCalibrationMG811)
    DiagLog.print(F(" MG811 ppm = "));
  else
    DiagLog.print(F(" MG811 value = "));
  DiagLog.print(valueMG811);
  DiagLog.print(F(" raw = "));
  DiagLog.print(valueMG811uncal);

  DiagLog.println();
}

boolean checkTimedEvent (const unsigned long period, unsigned long * tempTimeValue) {
  if ((millis() - (*tempTimeValue)) < period) return false;
  *tempTimeValue = millis();
  return (true);
}

boolean isConfigMode = false;

void setup() {
  Serial.begin(9600);
  webServer.begin();

  TempDiagLog::instance()->begin();
  WebConfigControl::instance()->setServer(webServer);
  WebConfigControl::instance()->begin();
  WebConfig::instance()->begin();

  DiagLog.println(F("\nInit started."));
  DiagLog.print(F("Firmware version "));
  DiagLog.print(FIRMWARE_VERSION_MAJOR);
  DiagLog.print(F("."));
  DiagLog.println(FIRMWARE_VERSION_MINOR);



  pinMode(PIN_SWITCH_PROG, INPUT);
  pinMode(PIN_SWITCH_CONFIG, INPUT_PULLUP);
  pinMode(PIN_LED_FAULT, OUTPUT);

  char chipId[16];
  const int RADIX_DECIMAL = 10;
  ltoa(ESP.getChipId(), chipId, RADIX_DECIMAL);
  char flashId[16];
  ltoa(ESP.getFlashChipId(), flashId, RADIX_DECIMAL);
  DiagLog.print(F("Chip ID: "));
  DiagLog.println(chipId);
  DiagLog.print(F("Flash chip ID:"));
  DiagLog.println(flashId);

  loadConfig();

  calcCalDataMG811();

  isConfigMode = !digitalRead(PIN_SWITCH_CONFIG);

  if (isConfigMode) WebConfigControl::instance()->setRootRedirect((const char *)F("webconfig"));
  
  if (isConfigMode) {
    DiagLog.println(F("Config Mode enabled."));
    const size_t TEXT_SIZE = 32;
    char ssid[TEXT_SIZE + 1] = {0};
    strncpy_P(ssid, ssidConfigModePrefix, sizeof(ssid));
    strncat(ssid, chipId, sizeof(ssid) - strlen(ssid));
    char password[TEXT_SIZE + 1] = {0};
    strncpy_P(password, passwordConfigModePrefix, sizeof(password));
    strncat(password, flashId, sizeof(password) - strlen(password));
    WiFi.mode(WIFI_AP);
    if (CONFIG_MODE_WIFI_OPEN)
      WiFi.softAP(ssid);
    else
      WiFi.softAP(ssid, password);
    DiagLog.println(F("Access point created: "));
    DiagLog.print(F("SSID: "));
    DiagLog.println(ssid);
    if (CONFIG_MODE_WIFI_OPEN)
      DiagLog.println(F("Open WiFi network"));
    else {
      DiagLog.print(F("Password: "));
      DiagLog.println(password);
    }
    DiagLog.print(F("IP address: "));
    DiagLog.println(WiFi.softAPIP());
  }
  else {
    DiagLog.println(F("Config Mode not enabled."));
    if (eepromSavedParametersStorage.startupDelay) {
      DiagLog.print(F("["));
      DiagLog.print(millis());
      DiagLog.print(F("] startup delay: "));
      float delaySeconds = eepromSavedParametersStorage.startupDelay / 10.0;
      DiagLog.println(delaySeconds);
      DiagLog.print(F(" seconds"));
      DiagLog.print(F("["));
      DiagLog.print(millis());
      DiagLog.print(F("] Waiting"));
      for (int i = 0; i < (int)eepromSavedParametersStorage.startupDelay; i++) {
        delay(100);
        if (!(i % 10)) DiagLog.print(F("."));
        yield();
      }
      DiagLog.println();
      DiagLog.print(F("["));
      DiagLog.print(millis());
      DiagLog.print(F("] Startup delay finished\n"));
    };
    DiagLog.print(F("["));
    DiagLog.print(millis());
    DiagLog.print(F("] connecting to "));
    DiagLog.print(eepromSavedParametersStorage.wifiSsid);
    WiFi.mode(WIFI_STA);
    if (strlen(eepromSavedParametersStorage.wifiPassword))
      WiFi.begin(eepromSavedParametersStorage.wifiSsid, eepromSavedParametersStorage.wifiPassword);
    else
      WiFi.begin(eepromSavedParametersStorage.wifiSsid);
    const int connectWiFiDelay = 500;
    const long connectWiFiTimeout = 50000;
    int connectWiFiCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(connectWiFiDelay);
      connectWiFiCount++;
      yield();
      DiagLog.print(F("."));
      if (((long)connectWiFiCount * (long)connectWiFiDelay) > connectWiFiTimeout) {
        DiagLog.println();
        DiagLog.print(F("["));
        DiagLog.print(millis());
        DiagLog.println(F("] Unable to connect, resetting..."));
        system_restart();
      }
    }
    DiagLog.println();
    DiagLog.print(F("["));
    DiagLog.print(millis());
    DiagLog.println(F("] connected."));

    Blynk.config(eepromSavedParametersStorage.authToken,
                 eepromSavedParametersStorage.blynkServer,
                 eepromSavedParametersStorage.blynkServerPort);
    DiagLog.println(F("Blynk init completed."));
  }
  dht.begin();
  sensorsDS18B20.begin();
  DiagLog.println(F("Init completed."));
}

void loop() {

  TempDiagLog::instance()->run();
  WebConfig::instance()->run();
  WebConfigControl::instance()->run();

  static unsigned long lastMillisSensors = 0;
  if (checkTimedEvent(UPDATE_TIME_SENSORS, &lastMillisSensors)) {
    updateSensorDHT();
    updateSensorOneWire();
    printSensorDebugInfo();
  }

  static unsigned long lastMillisMG811 = 0;
  if (checkTimedEvent(UPDATE_TIME_MG811, &lastMillisMG811)) {
    updateSensorMG811();
  }

  static unsigned long lastMillisStatusLEDs = 0;
  if (checkTimedEvent(UPDATE_TIME_STATUS_LEDS, &lastMillisStatusLEDs))
    updateStatusLEDs(isConfigMode);

  if (!isConfigMode) {
    static unsigned long lastMillisStatusVirtualPins = 0;
    if (checkTimedEvent(UPDATE_TIME_STATUS_VPINS, &lastMillisStatusVirtualPins))
      updateStatusVirtualPins();
    static unsigned long lastMillisValueVirtualPins = 0;
    if (checkTimedEvent(UPDATE_TIME_VALUE_VPINS, &lastMillisValueVirtualPins))
      updateValueVirtualPins();
    Blynk.run();
  }
}
