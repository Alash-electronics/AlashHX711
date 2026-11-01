// MIT License. Alash Electronics.
#include "AlashHX711.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

bool AlashHX711::begin(Gain gain) {
  pinMode(_sck, OUTPUT);
  pinMode(_dt, INPUT);
  digitalWrite(_sck, LOW);
  _gain = gain;
  powerUp();
  long v = readRaw();                          // первое чтение заодно задаёт усиление и проверяет связь
  (void)v;
  return _err != ERR_TIMEOUT;
}

bool AlashHX711::isReady() { return digitalRead(_dt) == LOW; }

bool AlashHX711::waitReady(uint16_t timeoutMs) {
  uint32_t t0 = millis();
  while (!isReady()) {
    if ((uint32_t)(millis() - t0) > timeoutMs) return false;
    delay(1);
  }
  return true;
}

// 24 бита данных старшим вперёд, затем 1-3 лишних такта выбирают канал и усиление для СЛЕДУЮЩЕГО измерения
long AlashHX711::readRaw() {
  if (!waitReady(1000)) {
    _err = ERR_TIMEOUT;
    return 0;
  }
  uint32_t v = 0;
  noInterrupts();                              // такты нельзя растягивать: если SCK держать высоким дольше 60 мкс, модуль уснёт
  for (uint8_t i = 0; i < 24; i++) {
    digitalWrite(_sck, HIGH);
    delayMicroseconds(1);
    v = (v << 1) | (digitalRead(_dt) ? 1 : 0);
    digitalWrite(_sck, LOW);
    delayMicroseconds(1);
  }
  for (uint8_t i = 0; i < (uint8_t)_gain; i++) {
    digitalWrite(_sck, HIGH);
    delayMicroseconds(1);
    digitalWrite(_sck, LOW);
    delayMicroseconds(1);
  }
  interrupts();
  if (v & 0x800000UL) v |= 0xFF000000UL;       // знаковое расширение 24 -> 32 бита
  long raw = (long)(int32_t)v;
  _err = (raw >= 0x7FFFFF || raw <= -0x800000L) ? ERR_SATURATED : ERR_OK;
  return raw;
}

void AlashHX711::setGain(Gain gain) {
  _gain = gain;
  readRaw();                                   // новое усиление вступает в силу со следующего измерения
}

// Среднее без выбросов: при пяти и более замерах отбрасываем самый большой и самый маленький
long AlashHX711::readAverage(uint8_t samples) {
  if (samples < 1) samples = 1;
  if (samples > 64) samples = 64;
  int64_t sum = 0;
  long mn = 0x7FFFFFFFL, mx = -0x7FFFFFFFL - 1;
  uint8_t ok = 0;
  for (uint8_t i = 0; i < samples; i++) {
    long r = readRaw();
    if (_err == ERR_TIMEOUT) return 0;
    sum += r;
    if (r < mn) mn = r;
    if (r > mx) mx = r;
    ok++;
  }
  if (samples >= 5) {
    sum -= mn;
    sum -= mx;
    ok -= 2;
  }
  return (long)(sum / ok);
}

void AlashHX711::tare(uint8_t samples) {
  long avg = readAverage(samples);
  if (_err == ERR_TIMEOUT) return;
  _offset = avg;
  _histN = 0;
}

bool AlashHX711::calibrate(float knownWeight, uint8_t samples) {
  if (knownWeight <= 0) return false;
  long avg = readAverage(samples);
  if (_err == ERR_TIMEOUT) return false;
  long diff = avg - _offset;
  if (diff == 0) return false;                 // груз не виден: пустая платформа или не подключён тензодатчик
  _scale = (float)diff / knownWeight;
  _scaled = true;
  _histN = 0;
  return true;
}

float AlashHX711::weight(uint8_t samples) {
  long avg = readAverage(samples);
  if (_err == ERR_TIMEOUT) return NAN;
  if (_scale == 1.0f && !_scaled) _err = (_err == ERR_OK) ? ERR_NO_SCALE : _err;   // работает, но в отсчётах АЦП
  float w = (float)(avg - _offset) / _scale;
  _hist[_histN % 6] = w;
  _histN++;
  return w;
}

// Устойчиво, если последние шесть значений лежат в пределах tolerance (нагрузку не трогали)
bool AlashHX711::isStable(float tolerance) {
  if (_histN < 6) return false;
  float mn = _hist[0], mx = _hist[0];
  for (uint8_t i = 1; i < 6; i++) {
    if (_hist[i] < mn) mn = _hist[i];
    if (_hist[i] > mx) mx = _hist[i];
  }
  return (mx - mn) <= tolerance;
}

void AlashHX711::powerDown() {
  digitalWrite(_sck, LOW);
  digitalWrite(_sck, HIGH);                    // SCK выше 60 мкс: модуль засыпает
  delayMicroseconds(70);
}

void AlashHX711::powerUp() {
  digitalWrite(_sck, LOW);
  delay(1);
}

// ---- Диагностика -----------------------------------------------------------------------------

const __FlashStringHelper *AlashHX711::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_TIMEOUT:
      return T("Модуль HX711 не отвечает: проверьте провода DT и SCK (не перепутаны ли), питание VCC 2.7-5 В и GND",
               "HX711 модулі жауап бермейді: DT және SCK сымдарын (шатыспағанын), VCC 2.7-5 В қуатын және GND тексеріңіз",
               "The HX711 module does not answer: check the DT and SCK wires (not swapped), VCC 2.7-5 V power and GND");
    case ERR_SATURATED:
      return T("Сигнал упёрся в предел АЦП: проверьте подключение тензодатчика (E+, E-, A+, A- не перепутаны, провода целы) и что он не перегружен",
               "Сигнал АЦП шегіне тірелді: тензодатчиктің қосылуын (E+, E-, A+, A-) және асқын жүктелмегенін тексеріңіз",
               "The signal hit the ADC limit: check the load cell wiring (E+, E-, A+, A- not swapped, wires intact) and that it is not overloaded");
    case ERR_NO_SCALE:
      return T("Коэффициент не задан: вес показан в отсчётах АЦП. Выполните calibrate(известный_вес) или setScale()",
               "Коэффициент берілмеген: салмақ АЦП есебімен. calibrate(белгілі_салмақ) не setScale() орындаңыз",
               "No scale factor set: the weight is in ADC counts. Run calibrate(known_weight) or setScale()");
  }
  return F("");
}

void AlashHX711::printCalibration(Print &out) {
  out.print(F("scale.setOffset("));
  out.print((int32_t)_offset);
  out.println(F(");"));
  out.print(F("scale.setScale("));
  out.print(_scale, 4);
  out.println(F(");"));
}

void AlashHX711::diagnose(Print &out) {
  out.println(T("AlashHX711: диагностика", "AlashHX711: диагностика", "AlashHX711: diagnostics"));
  out.print(T("Пины DT / SCK: ", "DT / SCK пиндері: ", "Pins DT / SCK: "));
  out.print((int)_dt);
  out.print(F(" / "));
  out.println((int)_sck);
  long r = readRaw();
  if (_err == ERR_TIMEOUT) {
    out.println(errorText());
    return;
  }
  out.print(T("Сырое значение: ", "Шикі мән: ", "Raw value: "));
  out.println((int32_t)r);
  if (_err != ERR_OK) out.println(errorText());
  out.println(T("Положите и снимите груз: сырое значение должно заметно меняться (на килограмм порядка десятков тысяч отсчётов). Если не меняется, проверьте тензодатчик; если знак обратный, поменяйте местами провода A+ и A-.",
                "Жүк қойып, алыңыз: шикі мән айтарлықтай өзгеруі керек. Өзгермесе, тензодатчикті тексеріңіз; таңба кері болса, A+ мен A- сымдарының орнын ауыстырыңыз.",
                "Put a load on and take it off: the raw value must change noticeably (tens of thousands of counts per kilogram). If it does not, check the load cell; if the sign is reversed, swap the A+ and A- wires."));
}
