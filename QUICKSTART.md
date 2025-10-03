# Quick Start Guide

This guide will help you get started with databento-cpp in minutes.

## Prerequisites

Before you begin, make sure you have:

- A [Databento API key](https://databento.com) (sign up for free)
- C++17 compatible compiler
- CMake 3.14 or higher
- libcurl development libraries

### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libcurl4-openssl-dev
```

**macOS:**
```bash
brew install cmake curl
```

**Windows:**
- Install Visual Studio 2017 or later
- Install CMake from https://cmake.org/download/
- Install vcpkg and use it to install curl: `vcpkg install curl`

## Installation

### Clone and Build

```bash
git clone https://github.com/rbarfinezhadfeli0/databento-cpp.git
cd databento-cpp
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install  # Optional: install system-wide
```

## Your First Program

Create a file `my_first_app.cpp`:

```cpp
#include <databento/databento.hpp>
#include <iostream>
#include <cstdlib>

int main() {
    // Get API key from environment variable
    const char* api_key = std::getenv("DATABENTO_API_KEY");
    if (!api_key) {
        std::cerr << "Please set DATABENTO_API_KEY environment variable\n";
        return 1;
    }
    
    // Create a historical data client
    databento::HistoricalClient client(api_key);
    
    // List available datasets
    auto datasets = client.list_datasets();
    std::cout << "Available datasets:\n";
    for (const auto& dataset : datasets) {
        std::cout << "  - " << dataset << "\n";
    }
    
    return 0;
}
```

### Compile and Run

```bash
# Set your API key
export DATABENTO_API_KEY="your-api-key-here"

# Compile
g++ -std=c++17 my_first_app.cpp -o my_first_app -ldatabento -lcurl -lpthread

# Run
./my_first_app
```

Or with CMake:

Create `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.14)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)

find_package(databento REQUIRED)

add_executable(my_first_app my_first_app.cpp)
target_link_libraries(my_first_app databento::databento)
```

Then build:
```bash
mkdir build && cd build
cmake ..
make
DATABENTO_API_KEY=your-api-key ./my_first_app
```

## Historical Data Example

Fetch historical trade data:

```cpp
#include <databento/databento.hpp>
#include <iostream>

int main() {
    databento::HistoricalClient client(std::getenv("DATABENTO_API_KEY"));
    
    // Set up query parameters
    databento::TimeseriesGetParams params;
    params.dataset = "GLBX.MDP3";              // CME Globex MDP 3.0
    params.symbols = {"ES.FUT"};                // E-mini S&P 500 futures
    params.schema = databento::Schema::Trades;  // Trade data
    params.start = "2024-01-01T00:00:00Z";     // Start date
    params.end = "2024-01-01T01:00:00Z";       // End date
    
    // Fetch data and save to file
    client.timeseries_get_range_to_file(params, "es_trades.dbn");
    
    std::cout << "Data saved to es_trades.dbn\n";
    return 0;
}
```

## Live Data Example

Subscribe to live market data:

```cpp
#include <databento/databento.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    databento::LiveClient client(std::getenv("DATABENTO_API_KEY"));
    
    // Connect to live feed
    client.connect();
    
    // Subscribe to symbols
    databento::SubscriptionParams params;
    params.dataset = "GLBX.MDP3";
    params.symbols = {"ES.FUT", "NQ.FUT"};
    params.schema = databento::Schema::Trades;
    client.subscribe(params);
    
    // Start receiving data
    client.start([](const std::uint8_t* data, std::size_t size) {
        // Process each record
        std::cout << "Received " << size << " bytes\n";
    });
    
    // Run for 60 seconds
    std::this_thread::sleep_for(std::chrono::seconds(60));
    
    // Stop receiving
    client.stop();
    
    return 0;
}
```

## Common Datasets

- `GLBX.MDP3` - CME Globex MDP 3.0
- `XNAS.ITCH` - Nasdaq TotalView-ITCH
- `DBEQ.BASIC` - Databento Equities Basic
- `XNYS.TRADES` - NYSE Trades

## Data Schemas

- `Schema::Trades` - Trade data
- `Schema::Mbo` - Market by Order (full order book)
- `Schema::Mbp1` - Market by Price Level 1 (top of book)
- `Schema::Mbp10` - Market by Price Level 10
- `Schema::Ohlcv1s` - 1-second OHLCV bars
- `Schema::Ohlcv1m` - 1-minute OHLCV bars
- `Schema::Ohlcv1d` - Daily OHLCV bars

## Next Steps

- Read the full [README](README.md) for detailed API documentation
- Check out the [examples](examples/) directory for more examples
- Visit [Databento documentation](https://docs.databento.com) for data details
- Join the Databento community for support

## Troubleshooting

### Build Errors

If you get CMake errors about missing CURL:
```bash
# Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# macOS
brew install curl
```

### Runtime Errors

If you get "CURL request failed":
- Check your internet connection
- Verify your API key is valid
- Ensure you have access to the requested dataset

### API Key Issues

If you get authentication errors:
- Make sure `DATABENTO_API_KEY` is set correctly
- Check that your API key is active on https://databento.com
- Verify you have sufficient credits for the request

## Getting Help

- Open an issue on [GitHub](https://github.com/rbarfinezhadfeli0/databento-cpp/issues)
- Read the [Databento documentation](https://docs.databento.com)
- Check existing issues for similar problems

Happy coding!
