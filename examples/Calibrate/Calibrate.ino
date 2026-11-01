// AlashHX711: калибровка весов. Нужен груз с известной массой (например, бутылка воды 0.5 л = 500 г, или гиря).
// 1. Запустите скетч с пустой платформой: весы обнулятся.
// 2. Когда в мониторе порта появится просьба, положите груз и отправьте его массу числом (например 500).
// 3. Скетч напечатает две строки: вставьте их в setup() ВМЕСТО калибровки, и весы будут готовы с первого включения.
#include <AlashHX711.h>

#if defined(ESP32)
AlashHX711 scale(16, 17);
#else
AlashHX711 scale(2, 3);
#endif

void setup() {
  Serial.begin(115200);
  if (!scale.begin()) {
    Serial.println(scale.errorText());
    while (true) delay(1000);
  }
  Serial.println(F("Уберите всё с платформы. Обнуляю..."));
  delay(2000);
  scale.tare();
  Serial.println(F("Положите груз известной массы и отправьте массу числом (например 500):"));
  while (!Serial.available()) delay(10);
  float known = Serial.parseFloat();
  if (scale.calibrate(known)) {
    Serial.println(F("Готово. Вставьте в setup():"));
    scale.printCalibration(Serial);        // setOffset(...) и setScale(...)
  } else {
    Serial.println(F("Не получилось: груз не виден. Проверьте подключение тензодатчика."));
  }
}

void loop() {
  Serial.print(scale.weight());
  Serial.println(F(" г"));
  delay(500);
}
