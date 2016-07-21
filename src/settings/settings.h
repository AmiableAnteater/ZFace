#pragma once

typedef enum {
  BULGARIAN=500,
  CATALAN=1000,
  CHINESE_SIMPLIFIED=1500,
  CHINESE_TRADITIONAL=2000,
  CROATIAN=2500,
  DUTCH=3000,
  ENGLISH=3500,
  FINNISH=4000,
  FRENCH=4500,
  GERMAN=5000,
  ITALIAN=5500,
  POLISH=6000,
  PORTUGUESE=6500,
  ROMANIAN=7000,
  RUSSIAN=7500,
  SPANISH=8000,
  SWEDISH=8500,
  TURKISH=9000,
  UKRAINIAN=9500
} WeatherLocale_t;


bool setApiKey(char *owmApiKey);
char* getApiKey();

bool isJsReady();
void setJsReady();


void signalSuccessfulWeatherUpdate();
time_t getLastWeatherUpdate();


void init_settings();
void deinit_settings();


unsigned int getUpdateFrequencyInMinutes();
void setUpdateFrequencyInMinutes(unsigned int frequency);


void setTemperatureUnitToCelsius(bool useDegreesCelsius);
bool useCelsius();


void setWeatherLocale(WeatherLocale_t loc);
WeatherLocale_t getWeatherLocale();


bool weatherConfigChanged();
