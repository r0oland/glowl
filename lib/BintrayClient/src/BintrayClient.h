#ifndef BINTRAY_CLIENT_H
#define BINTRAY_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
#include <utility>
#include <WString.h>

class BintrayClient {

public:
    BintrayClient(const String& user, const String& repository, const String& package);
    String getUser() const;
    String getRepository() const;
    String getPackage() const;
    String getStorageHost() const;
    String getApiHost() const;
    const char* getCertificate(const String& url) const;
    String getLatestVersion() const;
    String getBinaryPath(const String& version) const;

private:
    String requestHTTPContent(const String& url) const;
    String getLatestVersionRequestUrl() const;
    String getBinaryRequestUrl(const String& version) const;
    String m_user;
    String m_repo;
    String m_package;
    const String m_storage_host;
    const String m_api_host;
    std::vector<std::pair<String, const char*>> m_certificates;
};

#endif // BINTRAY_CLIENT_H
