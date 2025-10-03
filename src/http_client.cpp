#include "http_client.hpp"
#include <curl/curl.h>
#include <sstream>
#include <stdexcept>

namespace databento {
namespace internal {

namespace {

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    auto* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), total_size);
    return total_size;
}

size_t stream_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    auto* callback = static_cast<std::function<void(const std::uint8_t*, std::size_t)>*>(userp);
    (*callback)(static_cast<const std::uint8_t*>(contents), total_size);
    return total_size;
}

} // anonymous namespace

HttpClient::HttpClient(const std::string& api_key)
    : api_key_(api_key) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

std::string HttpClient::build_url(const std::string& path,
                                   const std::map<std::string, std::string>& params) {
    std::string url = "https://hist.databento.com" + path;
    if (!params.empty()) {
        url += "?";
        bool first = true;
        for (const auto& [key, value] : params) {
            if (!first) url += "&";
            url += key + "=" + value;
            first = false;
        }
    }
    return url;
}

HttpResponse HttpClient::get(const std::string& url,
                             const std::map<std::string, std::string>& params) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    HttpResponse response;
    std::string full_url = build_url(url, params);
    
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    
    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    
    long status_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    response.status_code = static_cast<int>(status_code);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    return response;
}

HttpResponse HttpClient::post(const std::string& url,
                              const std::string& body,
                              const std::string& content_type) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    HttpResponse response;
    std::string full_url = "https://hist.databento.com" + url;
    
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    
    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    std::string content_header = "Content-Type: " + content_type;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, content_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    
    long status_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    response.status_code = static_cast<int>(status_code);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    return response;
}

void HttpClient::stream_get(const std::string& url,
                           const std::map<std::string, std::string>& params,
                           std::function<void(const std::uint8_t*, std::size_t)> callback) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    std::string full_url = build_url(url, params);
    
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback);
    
    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        throw std::runtime_error("CURL streaming failed: " + std::string(curl_easy_strerror(res)));
    }
}

} // namespace internal
} // namespace databento
