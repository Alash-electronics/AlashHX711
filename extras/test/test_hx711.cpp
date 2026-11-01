// Тесты AlashHX711 без железа: имитация модуля читает фронты SCK, отдаёт 24 бита и считает такты усиления.
//   g++ -std=c++17 -Wall -I mock -I ../../src test_hx711.cpp ../../src/AlashHX711.cpp && ./a.out
#include <stdio.h>
#include <stdlib.h>
#include "Arduino.h"
#include "AlashHX711.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static bool near(float a, float b, float e) { return fabsf(a - b) <= e; }
static void reset() { g_hx = Hx711Sim(); g_ms += 100; }
// Следующее измерение с лёгким шумом ±3 и выбросом раз в 7 измерений
static long noisy() {
  static int n = 0; n++;
  long base = g_hx.value;
  long noise = (n * 7919) % 7 - 3;
  return (n % 7 == 0) ? base + 5000 : base + noise;
}

int main() {
  // 1) чтение 24 бит со знаком: положительные, отрицательные, границы
  reset(); AlashHX711 h(2, 3);
  CHECK(h.begin());
  const long vals[] = {0, 1, 12345, -1, -12345, 8388606, -8388607, 4194304, -4194304};
  for (long v : vals) { g_hx.value = v; long r = h.readRaw(); if (r != v) printf("  %ld -> %ld\n", v, r); CHECK(r == v); }
  CHECK(h.error() == AlashHX711::ERR_OK);

  // 2) число тактов выбирает усиление: 25 (A128), 26 (B32), 27 (A64)
  reset(); AlashHX711 a(2, 3); a.begin(AlashHX711::GAIN_A128); g_hx.value = 100; a.readRaw();
  g_hx.measurements = 0; a.readRaw(); hxFinishReadIfDone(); digitalRead(2);
  CHECK(g_hx.lastGainPulses == 1);
  a.setGain(AlashHX711::GAIN_B32); a.readRaw(); digitalRead(2); CHECK(g_hx.lastGainPulses == 2);
  a.setGain(AlashHX711::GAIN_A64); a.readRaw(); digitalRead(2); CHECK(g_hx.lastGainPulses == 3);

  // 3) тарирование, калибровка по грузу и вес: груз 500 г даёт 100000 отсчётов над нулём
  reset(); AlashHX711 s(2, 3); s.begin();
  g_hx.value = 8000;                 // пустая платформа, в отсчётах
  s.tare(10);
  CHECK(s.offset() == 8000);
  g_hx.value = 108000;               // положили 500 г
  CHECK(s.calibrate(500.0f, 10));
  CHECK(near(s.scale(), 200.0f, 0.01f));
  CHECK(near(s.weight(5), 500.0f, 0.1f) && s.error() == AlashHX711::ERR_OK);
  g_hx.value = 8000 + 200 * 123;     // 123 г
  CHECK(near(s.weight(5), 123.0f, 0.1f));
  g_hx.value = 8000 - 400;           // чуть ниже нуля: минус 2 г
  CHECK(near(s.weight(5), -2.0f, 0.1f));

  // 4) выбросы отбрасываются: среднее по шумному источнику близко к настоящему значению
  reset(); AlashHX711 n(2, 3); n.begin(); g_hx.value = 50000; g_hx.source = noisy;
  long avg = n.readAverage(10);
  CHECK(labs(avg - 50000) <= 4);
  g_hx.source = nullptr;

  // 5) без калибровки: вес в отсчётах и предупреждение ERR_NO_SCALE
  reset(); AlashHX711 u(2, 3); u.begin(); g_hx.value = 1000;
  CHECK(near(u.weight(3), 1000.0f, 0.5f) && u.error() == AlashHX711::ERR_NO_SCALE);
  u.setScale(2.5f); CHECK(near(u.weight(3), 400.0f, 0.5f));
  u.calibrate(1.0f);

  // 6) калибровка без груза не принимается
  reset(); AlashHX711 e(2, 3); e.begin(); g_hx.value = 5000; e.tare(5);
  CHECK(!e.calibrate(100.0f, 5) && !e.calibrate(0.0f, 5) && !e.calibrate(-5.0f, 5));

  // 7) устойчивость: после шести одинаковых значений true, при скачке false
  reset(); AlashHX711 st(2, 3); st.begin(); st.setScale(1.0f); g_hx.value = 1000;
  for (int i = 0; i < 6; i++) st.weight(1);
  CHECK(st.isStable(2.0f));
  g_hx.value = 1500; st.weight(1);
  CHECK(!st.isStable(2.0f));
  for (int i = 0; i < 6; i++) st.weight(1);
  CHECK(st.isStable(2.0f));

  // 8) нет модуля: таймаут и NAN; модуль, который не готов
  reset(); g_hx.present = false; AlashHX711 t(2, 3);
  CHECK(!t.begin() && t.error() == AlashHX711::ERR_TIMEOUT);
  CHECK(isnan(t.weight()) && t.error() == AlashHX711::ERR_TIMEOUT);
  reset(); AlashHX711 b(2, 3); b.begin(); g_hx.dataReady = false;
  uint32_t t0 = g_ms; CHECK(b.readRaw() == 0 && b.error() == AlashHX711::ERR_TIMEOUT && g_ms - t0 >= 1000);
  g_hx.dataReady = true; g_hx.value = 7; CHECK(b.readRaw() == 7 && b.error() == AlashHX711::ERR_OK);

  // 9) упор АЦП: значение 0x7FFFFF это ERR_SATURATED
  reset(); AlashHX711 sat(2, 3); sat.begin(); g_hx.saturate = true; sat.readRaw();
  CHECK(sat.error() == AlashHX711::ERR_SATURATED);

  // 10) печать калибровки и диагностика на трёх языках
  reset(); AlashHX711 d(2, 3); d.begin(); g_hx.value = 12345; d.setOffset(100); d.setScale(420.5f);
  printf("--- calibration ---\n"); d.printCalibration(*(new Print));
  d.setLanguage(AlashHX711::LANG_KK); const char *kk = (const char *)d.errorText();
  d.setLanguage(AlashHX711::LANG_EN); d.diagnose(*(new Print));
  CHECK(kk != nullptr);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
