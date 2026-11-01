# AlashHX711: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для электронных **весов на тензодатчике** с модулем **HX711**: обнуление (`tare()`), калибровка по любому грузу известной массы (`calibrate(500)`), вес в граммах и значок «показание устойчиво». Выбросы отбрасываются, модуль без ответа не вешает программу. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashHX711** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

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

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin(gain)` | Настроить пины, проверить связь |
| `tare(samples)` | Обнулить весы |
| `calibrate(knownWeight)` | Найти коэффициент по грузу известной массы |
| `weight(samples)` | Масса в единицах калибровки (граммы) |
| `isStable(tolerance)` | Показание устойчиво |
| `readRaw() / readAverage(n)` | Сырое значение / среднее без выбросов |
| `setOffset(c) / setScale(k)` | Задать готовую калибровку |
| `printCalibration(Serial)` | Напечатать калибровку для скетча |
| `powerDown() / powerUp()` | Сон / пробуждение |
| `diagnose(Serial)` | Самопроверка |

### Если что-то не работает
**Модуль не отвечает (ERR_TIMEOUT)**

Проверьте DT и SCK (не перепутаны ли), питание и GND. Запустите пример Diagnose.

**Вес уходит со временем (дрейф)**

Тензодатчик греется и реагирует на сквозняки и температуру. Дайте ему прогреться 5-10 минут, закрепите его жёстко и обнуляйте `tare()` перед взвешиванием.

**Вес растёт, когда груз снимаешь**

Поменяйте местами провода A+ и A- тензодатчика или используйте отрицательный коэффициент `setScale(-420)`.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Calibrate`, `Scale`, `Diagnose`

---

## Қазақша

**HX711** модулі бар **тензодатчикті таразыларға** арналған қарапайым кітапхана: нөлдеу (`tare()`), белгілі салмақ бойынша калибрлеу (`calibrate(500)`), грамм және «көрсеткіш тұрақты» белгісі. Шығарындылар алынады, жауап бермейтін модуль бағдарламаны іліктірмейді. Тәуелділіксіз.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashHX711** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

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

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin(gain)` | Пиндерді баптау |
| `tare(samples)` | Таразыны нөлдеу |
| `calibrate(knownWeight)` | Белгілі жүк бойынша коэффициент |
| `weight(samples)` | Салмақ |
| `isStable(tolerance)` | Көрсеткіш тұрақты |
| `readRaw() / readAverage(n)` | Шикі мән / орташа |
| `setOffset(c) / setScale(k)` | Дайын калибрлеуді беру |
| `printCalibration(Serial)` | Калибрлеуді шығару |
| `powerDown() / powerUp()` | Ұйқы / ояну |
| `diagnose(Serial)` | Өзін тексеру |

### Бірдеңе жұмыс істемесе
**Модуль жауап бермейді**

DT және SCK, қуат пен GND тексеріңіз.

**Салмақ уақыт өте ауытқиды**

Тензодатчик қызады. 5-10 минут жылытыңыз, `tare()` қолданыңыз.

**Жүк алғанда салмақ өседі**

A+ мен A- сымдарын ауыстырыңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Calibrate`, `Scale`, `Diagnose`

---

## English

A simple library for **load cell scales** with the **HX711** module: zeroing (`tare()`), calibration with any known weight (`calibrate(500)`), weight in grams and a "reading is stable" flag. Outliers are dropped and a silent module does not hang the program. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashHX711** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

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

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `begin(gain)` | Set up pins, check the link |
| `tare(samples)` | Zero the scale |
| `calibrate(knownWeight)` | Find the factor from a known weight |
| `weight(samples)` | Weight in calibration units (grams) |
| `isStable(tolerance)` | The reading is stable |
| `readRaw() / readAverage(n)` | Raw value / average without outliers |
| `setOffset(c) / setScale(k)` | Apply a ready calibration |
| `printCalibration(Serial)` | Print calibration for the sketch |
| `powerDown() / powerUp()` | Sleep / wake |
| `diagnose(Serial)` | Self-check |

### If something does not work
**The module does not answer (ERR_TIMEOUT)**

Check DT and SCK (not swapped), power and GND. Run the Diagnose example.

**The weight drifts over time**

The load cell warms up and reacts to drafts and temperature. Let it warm up 5-10 minutes, mount it rigidly and `tare()` before weighing.

**The weight grows when the load is removed**

Swap the A+ and A- wires or use a negative factor `setScale(-420)`.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Calibrate`, `Scale`, `Diagnose`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashHX711
