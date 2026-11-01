# AlashHX711

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для электронных **весов на тензодатчике** с модулем **HX711**: обнуление (`tare()`), калибровка по любому грузу известной массы (`calibrate(500)`), вес в граммах и значок «показание устойчиво». Выбросы отбрасываются, модуль без ответа не вешает программу. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashHX711.h>
AlashHX711 scale(2, 3);           // DT, SCK

void setup() {
  Serial.begin(115200);
  scale.begin();
  scale.tare();                   // платформа пустая / empty platform
  // положите груз 500 г / put a 500 g weight, then: scale.calibrate(500);
}
void loop() {
  Serial.println(scale.weight()); // граммы / grams
  delay(300);
}
```

### Что умеет
- **Весы за три строки:** `tare()` обнуляет, `calibrate(500)` находит коэффициент по грузу в 500 г, `weight()` отдаёт граммы.
- **Без выбросов:** среднее по нескольким измерениям, самые большие и самые маленькие отбрасываются.
- **Устойчивость:** `isStable(1.0)` говорит, что вес перестал меняться (можно записывать результат).
- **Готовые строки калибровки:** `printCalibration(Serial)` печатает `setOffset(...)` и `setScale(...)` для вставки в скетч, чтобы весы работали с первого включения.
- **Не виснет:** если модуль не отвечает, через секунду будет `ERR_TIMEOUT` и объяснение (проверьте DT, SCK, питание).
- **Честные ошибки:** упор АЦП это `ERR_SATURATED` (датчик подключён неверно или перегружен), отсутствие калибровки это `ERR_NO_SCALE`.
- **Каналы и усиление:** A128 (по умолчанию), A64, B32.
- **Понятные подсказки** на трёх языках: `errorText()`, `diagnose(Serial)`.
- Работает на **Arduino AVR, ESP32, ESP8266** на любых двух пинах.

### Подключение
| Модуль HX711 | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| DT (DOUT) | D2 | GPIO16 |
| SCK | D3 | GPIO17 |
| E+ / E- | питание тензодатчика | то же |
| A+ / A- | сигнал тензодатчика | то же |

Цвета проводов тензодатчика разные у разных производителей. Обычно красный E+, чёрный E-, белый A-, зелёный A+. Если вес растёт при снятии груза, поменяйте местами A+ и A-.

### Чем отличается от других библиотек
Мы посмотрели HX711 (bogde), HX711_ADC (olkal) и HX711 (RobTillaart):
- Калибровка в них это отдельный скетч и ручной подбор числа; у нас `calibrate(500)` и строки для вставки.
- Выбросы приходится фильтровать самому; у нас среднее без крайних значений.
- При отсутствии модуля некоторые версии зависают в ожидании; у нас таймаут и подсказка.
- Устойчивости показаний нет, а для весов она нужна: `isStable()`.

### Примеры
`Basic`, `Calibrate`, `Scale`, `Diagnose`

### Тесты
Чтение 24-битных значений со знаком и на границах, число тактов усиления (25, 26, 27), тара, калибровка по грузу и вес, отброс выбросов, устойчивость, таймаут при отсутствии и неготовности модуля, упор АЦП проверены без железа на имитации модуля, читающей фронты SCK: `extras/test`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**HX711** модулі бар **тензодатчикті таразыларға** арналған қарапайым кітапхана: нөлдеу (`tare()`), белгілі салмақ бойынша калибрлеу (`calibrate(500)`), грамм және «көрсеткіш тұрақты» белгісі. Шығарындылар алынады, жауап бермейтін модуль бағдарламаны іліктірмейді. Тәуелділіксіз.

```cpp
#include <AlashHX711.h>
AlashHX711 scale(2, 3);           // DT, SCK

void setup() {
  Serial.begin(115200);
  scale.begin();
  scale.tare();                   // платформа пустая / empty platform
  // положите груз 500 г / put a 500 g weight, then: scale.calibrate(500);
}
void loop() {
  Serial.println(scale.weight()); // граммы / grams
  delay(300);
}
```

### Мүмкіндіктері
- **Үш жолмен таразы:** `tare()` нөлдейді, `calibrate(500)` 500 г жүк бойынша коэффициент табады, `weight()` грамм береді.
- **Шығарындысыз:** бірнеше өлшемнің орташасы.
- **Тұрақтылық:** `isStable(1.0)` салмақ тоқтағанын айтады.
- **Дайын калибрлеу жолдары:** `printCalibration(Serial)`.
- **Іліспейді:** модуль жауап бермесе, бір секундтан кейін `ERR_TIMEOUT`.
- **Адал қателер:** `ERR_SATURATED`, `ERR_NO_SCALE`.
- **Арналар мен күшейту:** A128, A64, B32.
- **Түсінікті кеңестер** үш тілде.
- **Arduino AVR, ESP32, ESP8266**.

### Қосу
| HX711 модулі | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| DT (DOUT) | D2 | GPIO16 |
| SCK | D3 | GPIO17 |
| E+ / E- | питание тензодатчика | то же |
| A+ / A- | сигнал тензодатчика | то же |

Тензодатчик сымдарының түстері әртүрлі. Салмақ жүкті алғанда өссе, A+ мен A- орнын ауыстырыңыз.

### Басқа кітапханалардан айырмашылығы
HX711 (bogde), HX711_ADC (olkal) және HX711 (RobTillaart) қарадық:
- Калибрлеу бөлек скетч пен қолмен таңдау; бізде `calibrate(500)`.
- Шығарындыны өзің сүзесің; бізде орташа.
- Модуль болмаса кейбіреулері ілінеді; бізде таймаут.
- Тұрақтылық бізде: `isStable()`.

### Мысалдар
`Basic`, `Calibrate`, `Scale`, `Diagnose`

### Тесттер
24 биттік таңбалы мәндер, күшейту тактілері, тара, калибрлеу, шығарындыларды алып тастау, тұрақтылық, таймаут тексерілген: `extras/test`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library for **load cell scales** with the **HX711** module: zeroing (`tare()`), calibration with any known weight (`calibrate(500)`), weight in grams and a "reading is stable" flag. Outliers are dropped and a silent module does not hang the program. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashHX711.h>
AlashHX711 scale(2, 3);           // DT, SCK

void setup() {
  Serial.begin(115200);
  scale.begin();
  scale.tare();                   // платформа пустая / empty platform
  // положите груз 500 г / put a 500 g weight, then: scale.calibrate(500);
}
void loop() {
  Serial.println(scale.weight()); // граммы / grams
  delay(300);
}
```

### Features
- **Scales in three lines:** `tare()` zeroes, `calibrate(500)` finds the factor from a 500 g weight, `weight()` returns grams.
- **No outliers:** averages several readings, dropping the largest and the smallest.
- **Stability:** `isStable(1.0)` says the weight stopped changing (safe to record).
- **Ready calibration lines:** `printCalibration(Serial)` prints `setOffset(...)` and `setScale(...)` to paste into your sketch so the scale works from the first power-up.
- **Never hangs:** if the module is silent you get `ERR_TIMEOUT` with an explanation after a second (check DT, SCK, power).
- **Honest errors:** ADC saturation is `ERR_SATURATED` (wrong wiring or overload), a missing calibration is `ERR_NO_SCALE`.
- **Channels and gain:** A128 (default), A64, B32.
- **Clear tips** in three languages: `errorText()`, `diagnose(Serial)`.
- Works on **Arduino AVR, ESP32, ESP8266** with any two pins.

### Wiring
| HX711 module | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| DT (DOUT) | D2 | GPIO16 |
| SCK | D3 | GPIO17 |
| E+ / E- | питание тензодатчика | то же |
| A+ / A- | сигнал тензодатчика | то же |

Load cell wire colors vary by maker. Usually red E+, black E-, white A-, green A+. If the weight grows when you remove the load, swap A+ and A-.

### How it differs from other libraries
We looked at HX711 (bogde), HX711_ADC (olkal) and HX711 (RobTillaart):
- Calibration there is a separate sketch and hand-tuning a number; we have `calibrate(500)` and paste-ready lines.
- Outliers must be filtered by you; we average without extremes.
- Some versions hang when the module is absent; we have a timeout and a tip.
- No stability check, which scales need; we have `isStable()`.

### Examples
`Basic`, `Calibrate`, `Scale`, `Diagnose`

### Tests
Reading signed 24-bit values including the limits, gain pulse counts (25, 26, 27), tare, calibration and weight, outlier rejection, stability, timeouts for an absent or unready module and ADC saturation are checked without hardware on a simulator that reads the SCK edges: `extras/test`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
