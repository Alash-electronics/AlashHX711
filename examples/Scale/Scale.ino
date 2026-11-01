// AlashHX711: готовые весы. Вес в граммах, значок "стабильно", обнуление (тара) командой 't' из монитора порта.
// Подставьте в setOffset / setScale значения, которые напечатал скетч Calibrate.
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
  scale.setOffset(0);                      // замените значением из Calibrate
  scale.setScale(420.0);                   // замените значением из Calibrate (отсчётов на грамм)
  Serial.println(F("Команда t: обнулить (тара)"));
}

void loop() {
  if (Serial.available() && Serial.read() == 't') {
    scale.tare();                          // положите пустую тару и отправьте t: вес тары вычитается
    Serial.println(F("Обнулено"));
  }
  float g = scale.weight();
  if (isnan(g)) {
    Serial.println(scale.errorText());
  } else {
    Serial.print(g, 1);
    Serial.println(scale.isStable(1.0) ? F(" г  стабильно") : F(" г  ..."));
  }
  delay(300);
}
