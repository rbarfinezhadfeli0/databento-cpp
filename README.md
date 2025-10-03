# databento-cpp

A high-performance C++ client library for [Databento](https://databento.com), providing API-compatible access to market data services.

## Features

- **Historical Data API**: Access historical market data with flexible queries
- **Live Data API**: Subscribe to real-time market data streams
- **DBN Format Support**: Native support for Databento Binary Encoding format
- **High Performance**: Optimized C++17 implementation with minimal overhead
- **Type Safety**: Strongly typed API with comprehensive data structures
- **Easy Integration**: CMake-based build system for simple integration

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher
- libcurl (for HTTP/HTTPS requests)
- pthread (for threading support)

## Installation

### Building from Source

```bash
git clone https://github.com/rbarfinezhadfeli0/databento-cpp.git
cd databento-cpp
mkdir build && cd build
cmake ..
make
sudo make install
```

### CMake Integration

Add to your `CMakeLists.txt`:

```cmake
find_package(databento REQUIRED)
target_link_libraries(your_target databento::databento)
```

## Quick Start

### Historical Data

```cpp
#include <databento/databento.hpp>

int main() {
    // Create client with your API key
    databento::HistoricalClient client(std::getenv("DATABENTO_API_KEY"));
    
    // Configure query parameters
    databento::TimeseriesGetParams params;
    params.dataset = "GLBX.MDP3";
    params.symbols = {"ES.FUT"};
    params.schema = databento::Schema::Trades;
    params.start = "2024-01-01";
    params.end = "2024-01-02";
    
    // Fetch data
    auto data = client.timeseries_get_range(params);
    
    // Or save directly to file
    client.timeseries_get_range_to_file(params, "output.dbn");
    
    return 0;
}
```

### Live Data

```cpp
#include <databento/databento.hpp>

int main() {
    // Create client
    databento::LiveClient client(std::getenv("DATABENTO_API_KEY"));
    
    // Connect
    client.connect();
    
    // Subscribe to symbols
    databento::SubscriptionParams params;
    params.dataset = "GLBX.MDP3";
    params.symbols = {"ES.FUT", "NQ.FUT"};
    params.schema = databento::Schema::Trades;
    client.subscribe(params);
    
    // Start receiving data
    client.start([](const std::uint8_t* data, std::size_t size) {
        // Process incoming records
    });
    
    // ... run event loop ...
    
    client.stop();
    return 0;
}
```

## API Documentation

### Data Schemas

- `Schema::Mbo` - Market by Order
- `Schema::Mbp1` - Market by Price (L1)
- `Schema::Mbp10` - Market by Price (L10)
- `Schema::Trades` - Trades
- `Schema::Ohlcv1s` - OHLCV 1 second bars
- `Schema::Ohlcv1m` - OHLCV 1 minute bars
- `Schema::Ohlcv1h` - OHLCV 1 hour bars
- `Schema::Ohlcv1d` - OHLCV 1 day bars
- `Schema::Tbbo` - Top of Book
- `Schema::Definition` - Instrument definitions
- `Schema::Statistics` - Statistics
- `Schema::Status` - Status messages
- `Schema::Imbalance` - Imbalance messages

### Historical Client Methods

- `timeseries_get_range()` - Fetch historical data
- `timeseries_get_range_to_file()` - Save historical data to file
- `timeseries_stream()` - Stream historical data with callback
- `batch_submit_job()` - Submit batch processing job
- `batch_get_job()` - Get batch job status
- `batch_list_jobs()` - List all batch jobs
- `batch_download()` - Download batch job result
- `list_datasets()` - List available datasets

### Live Client Methods

- `connect()` - Connect to live data feed
- `subscribe()` - Subscribe to symbols
- `unsubscribe()` - Unsubscribe from symbols
- `start()` - Start receiving data
- `stop()` - Stop receiving data
- `is_connected()` - Check connection status

## Examples

See the `examples/` directory for complete working examples:

- `historical_example.cpp` - Historical data API usage
- `live_example.cpp` - Live data streaming

Build and run examples:

```bash
cd build
./examples/historical_example
./examples/live_example
```

## Testing

Run the test suite:

```bash
cd build
make test
# or
ctest
```

## Performance

This library is designed for high-performance applications with:

- Zero-copy data handling where possible
- Efficient binary protocol parsing
- Minimal memory allocations
- Thread-safe design for concurrent usage

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

This library provides API compatibility with the official [databento-cpp](https://github.com/databento/databento-cpp) client while offering optimized performance characteristics.

## Support

- Documentation: https://docs.databento.com
- Issues: https://github.com/rbarfinezhadfeli0/databento-cpp/issues
- API Key: Sign up at https://databento.com