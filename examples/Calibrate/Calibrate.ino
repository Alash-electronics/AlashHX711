// [ru] AlashHX711: калибровка весов. Нужен груз с известной массой (например, бутылка воды 0.5 л = 500 г, или гиря).
//      1. Запустите скетч с пустой платформой: весы обнулятся.
//      2. Когда в мониторе порта появится просьба, положите груз и отправьте его массу числом (например 500).
//      3. Скетч напечатает две строки: вставьте их в setup() ВМЕСТО калибровки, и весы будут готовы с первого включения.
// [kk] AlashHX711: таразыны калибрлеу. Массасы белгілі жүк керек (мысалы, 0.5 л су бөтелкесі = 500 г немесе гирь).
//      1. Скетчті бос платформамен іске қосыңыз: таразы нөлденеді.
//      2. Порт мониторында сұрау шыққанда жүкті қойып, массасын санмен жіберіңіз (мысалы 500).
//      3. Скетч екі жол басып шығарады: оларды калибрлеудің ОРНЫНА setup() ішіне қойыңыз, таразы бірінші қосудан дайын болады.
// [en] AlashHX711: scale calibration. You need a weight of known mass (for example a 0.5 L bottle of water = 500 g, or a dumbbell).
//      1. Run the sketch with an empty platform: the scale zeroes.
//      2. When the serial monitor asks, put the weight on and send its mass as a number (for example 500).
//      3. The sketch prints two lines: paste them into setup() INSTEAD of the calibration, and the scale is ready from the first power-on.
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
