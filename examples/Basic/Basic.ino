// [ru] AlashHX711: самый простой пример. Сырые значения с тензодатчика через модуль HX711 (ещё без калибровки).
//      Положите и снимите груз: число должно заметно меняться.
//
//      Подключение: VCC к 5 В (или 3.3 В), GND к GND, DT к пину 2, SCK к пину 3 (ESP32: GPIO16 и GPIO17).
//      Тензодатчик к клеммам E+, E-, A+, A- (цвета проводов зависят от датчика, см. наклейку).
// [kk] AlashHX711: ең қарапайым мысал. HX711 модулі арқылы тензодатчиктен шикі мәндер (әлі калибрлеусіз).
//      Жүк қойып, алыңыз: сан айтарлықтай өзгеруі керек.
//
//      Қосу: VCC 5 В-қа (немесе 3.3 В), GND GND-ға, DT 2 пинге, SCK 3 пинге (ESP32: GPIO16 және GPIO17).
//      Тензодатчик E+, E-, A+, A- қыстырғыштарына (сым түстері датчикке байланысты, жапсырманы қараңыз).
// [en] AlashHX711: the simplest example. Raw values from a strain gauge through an HX711 module (no calibration yet).
//      Put a weight on and take it off: the number should change noticeably.
//
//      Wiring: VCC to 5 V (or 3.3 V), GND to GND, DT to pin 2, SCK to pin 3 (ESP32: GPIO16 and GPIO17).
//      The load cell to terminals E+, E-, A+, A- (wire colors depend on the sensor, see the label).
#include <AlashHX711.h>

#if defined(ESP32)
AlashHX711 scale(16, 17);                  // DT, SCK
#else
AlashHX711 scale(2, 3);
#endif

void setup() {
  Serial.begin(115200);
  if (!scale.begin()) {                    // false, если модуль не отвечает
    Serial.println(scale.errorText());     // объяснит, что проверить
    while (true) delay(1000);
  }
}

void loop() {
  Serial.println(scale.readAverage(5));    // среднее из 5 измерений без выбросов
  delay(300);
}
