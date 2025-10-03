#include <databento/databento.hpp>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <atomic>

int main() {
    const char* api_key = std::getenv("DATABENTO_API_KEY");
    if (!api_key) {
        std::cerr << "Error: DATABENTO_API_KEY environment variable not set\n";
        return 1;
    }
    
    try {
        databento::LiveClient client(api_key);
        
        std::cout << "Databento Live API Example\n";
        std::cout << "==========================\n\n";
        
        // Connect to live feed
        std::cout << "Connecting to live feed...\n";
        client.connect();
        std::cout << "Connected: " << (client.is_connected() ? "Yes" : "No") << "\n\n";
        
        // Subscribe to live data
        databento::SubscriptionParams params;
        params.dataset = "GLBX.MDP3";
        params.symbols = {"ES.FUT", "NQ.FUT"};
        params.schema = databento::Schema::Trades;
        
        std::cout << "Subscribing to live data:\n";
        std::cout << "Dataset: " << params.dataset << "\n";
        std::cout << "Symbols: ";
        for (size_t i = 0; i < params.symbols.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << params.symbols[i];
        }
        std::cout << "\n";
        std::cout << "Schema: " << databento::to_string(params.schema) << "\n\n";
        
        client.subscribe(params);
        
        // Start receiving data
        std::cout << "Starting to receive data...\n";
        std::atomic<int> record_count{0};
        
        client.start([&record_count](const std::uint8_t* data, std::size_t size) {
            record_count++;
            // In a real implementation, would decode and process records
        });
        
        // Run for a short time
        std::cout << "Receiving data for 5 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        std::cout << "Stopping...\n";
        client.stop();
        
        std::cout << "\nReceived " << record_count << " records\n";
        std::cout << "Example completed successfully!\n";
        std::cout << "(This is a demo - actual WebSocket connection not implemented)\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
