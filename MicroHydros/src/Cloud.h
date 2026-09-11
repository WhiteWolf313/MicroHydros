#ifndef CLOUD_H
#define CLOUD_H

void setupCloud();
bool connectMQTT();
void publishData(const char* feed, float value);
void maintainMQTT();
bool isMQTTConnected();
#endif