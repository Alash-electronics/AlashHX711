// Имитация Arduino.h и модуля HX711 на уровне линий DT и SCK: готовность, сдвиг 24 бит, лишние такты выбирают усиление.
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
using __FlashStringHelper = char;
#define F(x) ((const __FlashStringHelper *)(x))
#define OUTPUT 1
#define INPUT 0
#define LOW 0
#define HIGH 1
inline uint32_t g_ms = 1000;
inline uint32_t millis() { return g_ms; }
inline void delay(uint32_t ms) { g_ms += ms; }
inline void delayMicroseconds(uint32_t) {}
inline void noInterrupts() {}
inline void interrupts() {}
struct Hx711Sim {
  uint8_t dt = 2, sck = 3;
  bool present = true;
  long value = 0;                     // что отдаёт модуль (24 бита со знаком)
  long (*source)() = nullptr;         // если задано, значение берётся из функции при каждом измерении (шум, разный груз)
  int pulses = 0; bool sckHigh = false; uint32_t shift = 0; bool dataReady = true;
  int lastGainPulses = 0; int gainSetting = 1; int measurements = 0; bool sleeping = false;
  uint32_t sckHighSince = 0; bool saturate = false; uint32_t latched = 0;
  int readDt() const { return (present && dataReady && !sleeping) ? 0 : 1; }
};
inline Hx711Sim g_hx;
inline void pinMode(uint8_t, uint8_t) {}
inline void hxFinishReadIfDone() {
  // Кадр закончен, когда после 24 бит прошли 1-3 лишних такта и SCK опустился: считаем это при следующем опросе готовности
  if (g_hx.dataReady && g_hx.pulses >= 25 && !g_hx.sckHigh) { g_hx.gainSetting = g_hx.pulses - 24; g_hx.lastGainPulses = g_hx.gainSetting; g_hx.pulses = 0; g_hx.measurements++; }
}
inline int digitalRead(uint8_t p) {
  if (p != g_hx.dt) return 0;
  hxFinishReadIfDone();
  if (!g_hx.present || !g_hx.dataReady || g_hx.sleeping) return 1;
  if (g_hx.pulses < 24 && g_hx.sckHigh) return (g_hx.latched >> (23 - g_hx.pulses)) & 1;
  return g_hx.pulses == 0 ? 0 : 1;        // между кадрами DT в LOW (данные готовы); после 24 бит HIGH
}
inline void digitalWrite(uint8_t p, uint8_t v) {
  if (p != g_hx.sck) return;
  if (v == HIGH && !g_hx.sckHigh) {
    g_hx.sckHigh = true; g_hx.sckHighSince = g_ms;
    if (g_hx.pulses == 0) {              // первый такт кадра: фиксируем значение этого измерения
      long val = g_hx.source ? g_hx.source() : g_hx.value;
      g_hx.latched = g_hx.saturate ? 0x7FFFFF : ((uint32_t)val & 0xFFFFFF);
    }
  } else if (v == LOW && g_hx.sckHigh) {
    g_hx.sckHigh = false;
    g_hx.pulses++;
  }
}
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(int v) { printf("%d", v); }
  void print(float v, int d = 2) { printf("%.*f", d, v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
};
