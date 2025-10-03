#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cstdint>

namespace databento {
namespace internal {

struct HttpResponse {
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
};

class HttpClient {
public:
    explicit HttpClient(const std::string& api_key);
    
    HttpResponse get(const std::string& url, 
                    const std::map<std::string, std::string>& params = {});
    
    HttpResponse post(const std::string& url,
                     const std::string& body,
                     const std::string& content_type = "application/json");
    
    void stream_get(const std::string& url,
                   const std::map<std::string, std::string>& params,
                   std::function<void(const std::uint8_t*, std::size_t)> callback);

private:
    std::string api_key_;
    std::string build_url(const std::string& path, 
                         const std::map<std::string, std::string>& params);
};

} // namespace internal
} // namespace databento
