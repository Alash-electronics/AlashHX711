// [ru] AlashHX711: весы не работают? Скетч прочитает модуль и подскажет, что проверить (провода DT / SCK, питание, тензодатчик).
// [kk] AlashHX711: таразы жұмыс істемей ме? Скетч модульді оқиды және нені тексеру керегін айтады (DT / SCK сымдары, қуат, тензодатчик).
// [en] AlashHX711: the scale does not work? The sketch reads the module and tells what to check (DT / SCK wires, power, the load cell).
#include <AlashHX711.h>

#if defined(ESP32)
AlashHX711 scale(16, 17);
#else
AlashHX711 scale(2, 3);
#endif

void setup() {
  Serial.begin(115200);
  delay(500);
  scale.setLanguage(AlashHX711::LANG_RU);  // LANG_KK: қазақша, LANG_EN: English
  scale.begin();
  scale.diagnose(Serial);
}

void loop() {}
