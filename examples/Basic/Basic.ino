// AlashHX711: самый простой пример. Сырые значения с тензодатчика через модуль HX711 (ещё без калибровки).
// Положите и снимите груз: число должно заметно меняться.
//
// Подключение: VCC к 5 В (или 3.3 В), GND к GND, DT к пину 2, SCK к пину 3 (ESP32: GPIO16 и GPIO17).
// Тензодатчик к клеммам E+, E-, A+, A- (цвета проводов зависят от датчика, см. наклейку).
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
