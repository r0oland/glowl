#ifndef SECURE_OTA_H
#define SECURE_OTA_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <Update.h>
#include <BintrayClient.h>

void checkFirmwareUpdates();
void processOTAUpdate(const String &version);

#endif // SECURE_OTA_H