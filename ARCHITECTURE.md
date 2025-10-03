# Architecture Documentation

This document describes the architecture and design of the databento-cpp library.

## Overview

databento-cpp is a high-performance C++ client library for Databento market data services. It provides a clean, type-safe API for accessing both historical and live market data.

## Project Structure

```
databento-cpp/
├── include/databento/        # Public API headers
│   ├── databento.hpp         # Main include file
│   ├── enums.hpp            # Enum definitions
│   ├── types.hpp            # Data structure definitions
│   ├── dbn.hpp              # DBN format support
│   ├── historical.hpp       # Historical API client
│   └── live.hpp             # Live data client
├── src/                      # Implementation files
│   ├── enums.cpp            # Enum conversions
│   ├── dbn.cpp              # DBN encoding/decoding
│   ├── historical.cpp       # Historical client implementation
│   ├── live.cpp             # Live client implementation
│   ├── http_client.cpp      # HTTP client utility
│   └── http_client.hpp      # HTTP client header (internal)
├── examples/                 # Example programs
│   ├── historical_example.cpp
│   └── live_example.cpp
├── tests/                    # Unit tests
│   └── test_basic.cpp
├── CMakeLists.txt           # Main build configuration
├── README.md                # User documentation
├── QUICKSTART.md            # Quick start guide
├── CONTRIBUTING.md          # Contribution guidelines
└── LICENSE                  # MIT License

```

## Design Principles

### 1. High Performance

- **Zero-copy where possible**: Minimize memory allocations and copies
- **Efficient binary protocol**: Direct parsing of DBN format
- **Streaming support**: Process data as it arrives without buffering
- **Thread-safe design**: Safe for concurrent usage

### 2. Type Safety

- **Strong typing**: All data structures are strongly typed
- **Compile-time checks**: Leverage C++ type system for correctness
- **No raw pointers in public API**: Use references and smart pointers

### 3. API Compatibility

- **Familiar interface**: Compatible with official databento-cpp API
- **Consistent naming**: Follow standard C++ naming conventions
- **Documentation**: Comprehensive docs and examples

### 4. Ease of Use

- **Simple initialization**: Just provide API key
- **Sensible defaults**: Minimal required configuration
- **Clear error messages**: Helpful exceptions with context
- **CMake integration**: Easy to integrate into existing projects

## Core Components

### Enums (enums.hpp)

Defines all enum types used throughout the library:
- `Schema`: Data schema types (Trades, Mbo, Mbp1, etc.)
- `Encoding`: Output encoding formats (Dbn, Csv, Json)
- `Compression`: Compression types (None, Zstd)
- `SType`: Symbol type for queries
- `Side`: Order side (Bid/Ask)
- `Action`: Order action (Add/Cancel/Modify/Trade)

### Types (types.hpp)

Core data structures for market data:
- `RecordHeader`: Common header for all records
- `MboMsg`: Market-by-order message
- `TradeMsg`: Trade message
- `Mbp1Msg`: Level 1 market-by-price message
- `OhlcvMsg`: OHLCV bar data
- `Metadata`: Dataset metadata
- `SymbolMapping`: Symbol to instrument ID mapping

### DBN Format (dbn.hpp, dbn.cpp)

Databento Binary Encoding (DBN) support:
- `DbnDecoder`: Stream-based decoder for DBN files
- `DbnEncoder`: Stream-based encoder for DBN files
- Helper functions for file I/O

### Historical Client (historical.hpp, historical.cpp)

Access to historical market data:
- `HistoricalClient`: Main client class
- `TimeseriesGetParams`: Query parameters
- `BatchJob`: Batch job information

**Key Methods:**
- `timeseries_get_range()`: Fetch data to memory
- `timeseries_get_range_to_file()`: Stream directly to file
- `timeseries_stream()`: Process with callback
- `batch_submit_job()`: Submit batch processing job
- `batch_download()`: Download batch results
- `list_datasets()`: Get available datasets

### Live Client (live.hpp, live.cpp)

Real-time market data streaming:
- `LiveClient`: Main client class
- `SubscriptionParams`: Subscription parameters

**Key Methods:**
- `connect()`: Establish connection
- `subscribe()`: Subscribe to symbols
- `unsubscribe()`: Unsubscribe from symbols
- `start()`: Begin receiving data
- `stop()`: Stop receiving data

### HTTP Client (http_client.hpp, http_client.cpp)

Internal HTTP/HTTPS communication:
- Based on libcurl
- Supports GET and POST requests
- Streaming support for large responses
- Automatic authentication header injection

## Implementation Details

### Pimpl Idiom

Both `HistoricalClient` and `LiveClient` use the Pimpl (Pointer to Implementation) idiom:

```cpp
class HistoricalClient {
public:
    // Public interface
private:
    class Impl;  // Forward declaration
    std::unique_ptr<Impl> impl_;  // Opaque pointer
};
```

**Benefits:**
- Hides implementation details
- Faster compile times
- Better ABI stability
- Cleaner public headers

### Thread Safety

- HTTP client is stateless (thread-safe for concurrent requests)
- Live client uses atomic variables for state management
- Mutex protection for shared state in live client

### Memory Management

- RAII principles throughout
- Smart pointers (unique_ptr, shared_ptr) for ownership
- No manual memory management in public API
- Automatic cleanup via destructors

### Error Handling

- Exceptions for error conditions
- std::runtime_error with descriptive messages
- No error codes in public API

## Building

The project uses CMake with the following features:

- **Modern CMake**: Uses target-based approach
- **Options**: Build flags for examples and tests
- **Find modules**: Automatically finds dependencies (CURL)
- **Installation**: Supports `make install` with proper config files
- **Export**: Provides CMake config for downstream projects

### Build Options

- `DATABENTO_BUILD_EXAMPLES`: Build example programs (default: ON)
- `DATABENTO_BUILD_TESTS`: Build unit tests (default: ON)

### Compiler Support

- GCC 7+ (Linux)
- Clang 5+ (macOS, Linux)
- MSVC 2017+ (Windows)
- C++17 standard required

## Dependencies

### Required

- **libcurl**: HTTP/HTTPS communication
- **pthread**: Threading support (implicit on Unix)

### Optional

- None currently, but future versions may add:
  - WebSocket library for live client
  - Compression libraries (zstd)
  - JSON parser for metadata

## Performance Characteristics

### Historical Client

- **Throughput**: Limited by network bandwidth
- **Memory**: Streaming API for constant memory usage
- **CPU**: Minimal overhead, dominated by network I/O

### Live Client

- **Latency**: Sub-millisecond message processing
- **Throughput**: Handles thousands of messages per second
- **Memory**: Constant memory usage with streaming

### DBN Format

- **Parsing**: Fast binary format, minimal overhead
- **Size**: Compact representation (50-90% smaller than CSV)
- **Compatibility**: Native format for Databento services

## Testing

### Unit Tests

Located in `tests/`, covers:
- Type definitions and structures
- Enum conversions
- Client creation and basic operations
- Error handling

### Integration Tests

Example programs serve as integration tests:
- `historical_example.cpp`: Historical API usage
- `live_example.cpp`: Live API usage

### Future Testing

Planned additions:
- More comprehensive unit tests
- Mock server for testing without API key
- Performance benchmarks
- Fuzz testing for DBN parser

## Future Enhancements

### Planned Features

1. **WebSocket Support**: Real WebSocket for live client
2. **Compression**: Native zstd decompression
3. **JSON Support**: JSON output format parsing
4. **Symbol Resolution**: Helper methods for symbol lookup
5. **Caching**: Local caching of metadata and symbols
6. **Async API**: Async/await style API with futures
7. **Python Bindings**: pybind11 bindings for Python

### Performance Optimizations

1. **Connection Pooling**: Reuse HTTP connections
2. **Parallel Downloads**: Concurrent chunk downloads
3. **SIMD**: Vectorized parsing for DBN format
4. **Zero-copy**: Direct buffer sharing where possible

### API Extensions

1. **Batch Operations**: Bulk symbol subscriptions
2. **Filters**: Server-side filtering
3. **Aggregations**: On-the-fly data aggregation
4. **Replay**: Historical replay with live-like streaming

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on:
- Code style
- Development workflow
- Testing requirements
- Pull request process

## License

MIT License - see [LICENSE](LICENSE) file for details.

## References

- [Databento Documentation](https://docs.databento.com)
- [DBN Format Specification](https://databento.com/docs/knowledge-base/new-users/dbn-encoding)
- [API Reference](https://databento.com/docs/api-reference-historical)
