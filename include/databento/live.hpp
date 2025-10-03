#pragma once

#include "types.hpp"
#include "enums.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace databento {

struct SubscriptionParams {
    std::string dataset;
    std::vector<std::string> symbols;
    Schema schema;
    SType stype = SType::RawSymbol;
    std::uint64_t start = 0;
};

class LiveClient {
public:
    explicit LiveClient(const std::string& api_key);
    ~LiveClient();
    
    void connect();
    
    void subscribe(const SubscriptionParams& params);
    
    void unsubscribe(const std::vector<std::string>& symbols);
    
    void start(std::function<void(const std::uint8_t*, std::size_t)> callback);
    
    void stop();
    
    bool is_connected() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace databento
