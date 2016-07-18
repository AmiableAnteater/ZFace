#pragma once

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


bool weatherConfigChanged();
