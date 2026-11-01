// AlashHX711: весы на тензодатчике с модулем HX711: тарирование, калибровка по эталонному грузу, вес в граммах, устойчивость показаний.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashHX711
#ifndef ALASH_HX711_H
#define ALASH_HX711_H

#include <Arduino.h>

class AlashHX711 {
 public:
  // Канал и усиление: A128 (по умолчанию, для обычных тензодатчиков), B32, A64
  enum Gain : uint8_t { GAIN_A128 = 1, GAIN_B32 = 2, GAIN_A64 = 3 };
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_TIMEOUT,      // модуль не отвечает: провода DT / SCK, питание
    ERR_SATURATED,    // сигнал упёрся в предел АЦП: тензодатчик подключён неверно или перегружен
    ERR_NO_SCALE      // не задан коэффициент (вызовите setScale или calibrate)
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  // dtPin: DT (DOUT) модуля, sckPin: SCK (PD_SCK)
  AlashHX711(uint8_t dtPin, uint8_t sckPin) : _dt(dtPin), _sck(sckPin) {}

  bool begin(Gain gain = GAIN_A128);        // false, если модуль не отвечает
  bool isReady();                           // модуль выдал новое измерение (DT в LOW)

  // ---- Вес --------------------------------------------------------------------
  void tare(uint8_t samples = 15);          // обнулить весы (на платформе ничего нет)
  bool calibrate(float knownWeight, uint8_t samples = 15);   // положите груз известной массы: коэффициент найдётся сам
  void setScale(float countsPerUnit) { _scale = countsPerUnit; }   // отсчётов АЦП на единицу массы (если уже знаете)
  float scale() const { return _scale; }
  void setOffset(long counts) { _offset = counts; }
  long offset() const { return _offset; }
  float weight(uint8_t samples = 5);        // масса в тех единицах, которыми калибровали (граммы, кг...); NAN при ошибке
  bool isStable(float tolerance = 1.0f);    // показания не меняются больше чем на tolerance за последние измерения

  long readRaw();                           // «сырое» значение (24 бита со знаком); 0 и ошибка при таймауте
  long readAverage(uint8_t samples);        // среднее с отбрасыванием крайних значений (выбросов)
  void setGain(Gain gain);
  void powerDown();                         // спать (меньше 1 мкА)
  void powerUp();

  // ---- Диагностика -----------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);
  void printCalibration(Print &out);        // строки setOffset(...) и setScale(...) для вставки в скетч

 private:
  uint8_t _dt, _sck;
  Gain _gain = GAIN_A128;
  long _offset = 0;
  float _scale = 1.0f;
  bool _scaled = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;
  float _hist[6] = {0};
  uint8_t _histN = 0;

  bool waitReady(uint16_t timeoutMs);
};

#endif  // ALASH_HX711_H
