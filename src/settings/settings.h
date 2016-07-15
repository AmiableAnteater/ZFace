#pragma once

void setApiKey(char *owmApiKey);
char* getApiKey();

bool isJsReady();
void setJsReady();

void init_settings();
void deinit_settings();