/*
 Copyright (c) 2014-present PlatformIO <contact@platformio.org>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
**/

#ifndef SECURE_OTA_H
#define SECURE_OTA_H

#include <Arduino.h>
#include <BintrayClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Updater.h>

#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
#endif

void checkFirmwareUpdates();
void processOTAUpdate(const String &version);

#endif // SECURE_OTA_H