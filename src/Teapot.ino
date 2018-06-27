#include <Homie.h>
#include "DHT.h"

const int PIN_BUTTON = 0;
const int PIN_LED    = 2;

const int PIN_HEAT   = 12;
const int PIN_DHT22  = 14;
const int PIN_SWITCH = 13;
const int PIN_WATHER = 16;

const int TEMPERATURE_INTERVAL = 15;
const int TEMPERATURE_INTERVAL_HEATING = 1;

bool heatingOn = false;

DHT dht(PIN_DHT22, DHT22);

// Делаем кучу удобных нод TODO: сделать одну ноду с этими свойствами 
HomieNode switchNode("switch", "switch");
HomieNode watherNode("wather", "wather");
HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");
HomieNode heatIndexNode("heat_index", "heat_index");

void heaterOn() {
  if (!heatingOn) {
    Homie.getLogger() << "Heater switched on"  << endl;
    heatingOn = true;
    digitalWrite(PIN_HEAT, HIGH);
    switchNode.setProperty("on").send("true");
  }
}

void heaterOff() {
  if (heatingOn) {
    Homie.getLogger() << "Heater switched off"  << endl;
    heatingOn = false;
    digitalWrite(PIN_HEAT, LOW);
    switchNode.setProperty("on").send("false");
  }
}

void heaterToggle() {
  if (heatingOn) {
    heaterOff();
  } else {
    heaterOn();
  }
}

unsigned long lastTemperatureTs = 0;
float lastH  = 0;
float lastT  = 0;
float lastHi = 0;

// Следим за датчиками
void updateSensors() {

  // Ограничим частоту опроса, если чайник не греется в TEMPERATURE_INTERVAL, и если греется - в TEMPERATURE_INTERVAL_HEATING (опрашиваем чаще)
  unsigned long ts = millis();
  if ((ts - lastTemperatureTs >= TEMPERATURE_INTERVAL * 1000UL)
    || (heatingOn && ts - lastTemperatureTs >= TEMPERATURE_INTERVAL_HEATING * 1000UL)) {

    lastTemperatureTs = ts;

    // Влажность с DHT
    float h  = dht.readHumidity();
    if (h != lastH) {
      lastH = h;
      Homie.getLogger() << "H: " << h << " %;" << endl;
      humidityNode.setProperty("percents").send(String(h));
    }

    // Температура с DHT
    float t  = dht.readTemperature();
    if (t != lastT) {
      lastT = t;
      Homie.getLogger() << "T: " << t << " C;" << endl;
      temperatureNode.setProperty("degrees").send(String(t));
    }

    // Индекс комфорта (ощущаемая температура)
    float hi = dht.computeHeatIndex(t, h, false);
    if (hi != lastHi) {
      lastHi = hi;
      heatIndexNode.setProperty("degrees").send(String(hi));
      Homie.getLogger() << "Hi: " << hi << " C;" << endl;
    }

    // Выключаем чайник если температура 100 (кипит)
    if (t >= 100 || hi >= 100) {
      heaterOff();
    }
  }
}

unsigned long lastButtonTs = 0;
bool buttonStateTrigger = false;
byte buttonStateLast = HIGH;

// Следим за состоянием кнопки
void watchButton() {
  // Антидребезг TODO: в homie уже есть Debouncer - использовать его
  unsigned long ts = millis();
  if (ts - lastButtonTs >= 100UL) {
    byte buttonState = digitalRead(PIN_SWITCH);
    if (buttonState != buttonStateLast) {
      lastButtonTs = ts;
      buttonStateLast = buttonState;
      if (buttonState == LOW) {
        heaterToggle();
      }
    }
  }
}

byte watherStateLast = -1;

// Следим за датчиком уровня воды (0 - пусто, 1 - есть что кипятить)
void watchWather() {
  byte watherState = digitalRead(PIN_WATHER);
  if (watherState != watherStateLast) {
    watherStateLast = watherState;
    watherNode.setProperty("level").send(String(watherState));
  }
}

bool switchOnHandler(const HomieRange& range, const String& value) {

  if (value != "true" && value != "false") {
    return false;
  }

  if(value == "true") {
    heaterOn();
  } else {
    heaterOff();
  }

  return true;
}

void loopHandler() {
  watchWather();
  updateSensors();
  watchButton();
}

void setupHandler() {

  // у ноды уровня воды будет свойство level 
  watherNode.advertise("level");

  // у ноды выключателя - свойство on которое можно менять, при этом будет вызываться switchOnHandler
  switchNode.advertise("on").settable(switchOnHandler);
  
  // нода датчика температуры - единица измерения (градусы Цельсия) и их количество 
  temperatureNode.advertise("unit");
  temperatureNode.advertise("degrees");
  temperatureNode.setProperty("unit").send("c"); // отправим сразу С

  // нода датчика влажности - единица измерения (проценты абсолютной влажности), их количество  
  humidityNode.advertise("unit");
  humidityNode.advertise("percents");
  humidityNode.setProperty("unit").send("%");

  // нода индекса комфорта, аналогично температуре
  heatIndexNode.advertise("unit");
  heatIndexNode.advertise("degrees");
  heatIndexNode.setProperty("unit").send("c");
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED,    OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  pinMode(PIN_HEAT,   OUTPUT);
  pinMode(PIN_DHT22,  INPUT);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_WATHER, INPUT);

  // бренд
  Homie_setBrand("ArtHome");

  // название прошивки, для обновления по воздуху
  Homie_setFirmware("teapot", "2.0.1");

  Homie.setLedPin(PIN_LED, LOW).setResetTrigger(PIN_BUTTON, LOW, 5000);
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  Homie.setup();

  dht.begin();
}

void loop() {
  loopHandler();
  Homie.loop();
}
