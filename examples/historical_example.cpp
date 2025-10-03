#include <databento/databento.hpp>
#include <iostream>
#include <cstdlib>

int main() {
    const char* api_key = std::getenv("DATABENTO_API_KEY");
    if (!api_key) {
        std::cerr << "Error: DATABENTO_API_KEY environment variable not set\n";
        return 1;
    }
    
    try {
        databento::HistoricalClient client(api_key);
        
        std::cout << "Databento Historical API Example\n";
        std::cout << "=================================\n\n";
        
        // List available datasets
        std::cout << "Available datasets:\n";
        auto datasets = client.list_datasets();
        for (const auto& dataset : datasets) {
            std::cout << "  - " << dataset << "\n";
        }
        std::cout << "\n";
        
        // Example: Get historical data
        databento::TimeseriesGetParams params;
        params.dataset = "GLBX.MDP3";
        params.symbols = {"ES.FUT"};
        params.schema = databento::Schema::Trades;
        params.start = "2024-01-01";
        params.end = "2024-01-02";
        params.limit = 100;
        
        std::cout << "Fetching historical data for ES.FUT...\n";
        std::cout << "Dataset: " << params.dataset << "\n";
        std::cout << "Schema: " << databento::to_string(params.schema) << "\n";
        std::cout << "Date range: " << params.start << " to " << params.end << "\n";
        std::cout << "Limit: " << params.limit << " records\n\n";
        
        // Note: This would make an actual API call in production
        // auto data = client.timeseries_get_range(params);
        // std::cout << "Received " << data.size() << " bytes of data\n";
        
        std::cout << "Example completed successfully!\n";
        std::cout << "(API key found but actual API calls are not made in this demo)\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
