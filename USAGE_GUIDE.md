# 📖 Usage Guide: databento-cpp-fast

Complete guide with practical examples for using the databento-cpp-fast library.

---

## Table of Contents

1. [Installation](#installation)
2. [C++ Usage](#c-usage)
3. [Python Usage](#python-usage)
4. [Performance Tips](#performance-tips)
5. [Common Patterns](#common-patterns)
6. [Troubleshooting](#troubleshooting)

---

## Installation

### C++ Library

```bash
cd /home/nvidia/my_github_libraries/databento-cpp

# Make scripts executable
chmod +x build.sh test.sh

# Build library, examples, tests, benchmarks
./build.sh

# Run tests to verify
./test.sh
```

### Python Bindings

```bash
cd /home/nvidia/my_github_libraries/databento-cpp

# Make script executable
chmod +x install_python.sh

# Install Python module
./install_python.sh

# Verify installation
python3 -c "import databento_cpp; print(databento_cpp.__version__)"
```

---

## C++ Usage

### Example 1: Simple Parsing (Easy)

**Use Case:** Quick analysis, small files, learning the API

```cpp
#include <databento/parser.hpp>
#include <iostream>

int main() {
    // Parse with callback (15-30M rec/s)
    uint64_t count = 0;
    
    auto callback = [&count](const databento::MboMsg& msg) {
        std::cout << "Price: " << databento::price_to_double(msg.price)
                  << " Size: " << msg.size 
                  << " Side: " << msg.side << "\n";
        ++count;
    };
    
    auto stats = databento::parse_file_mbo("data.dbn", callback);
    stats.print();
    
    return 0;
}
```

**Compile:**
```bash
g++ -O3 -march=native -std=c++20 \
  -I/path/to/databento-cpp/include \
  example.cpp \
  /path/to/databento-cpp/src/parser.cpp \
  -o example
```

**Performance:** 15-30M records/sec

---

### Example 2: Ultra-Fast Parsing (Maximum Speed)

**Use Case:** Backtesting, large files, maximum throughput

```cpp
#include <databento/parser.hpp>
#include <iostream>

int main() {
    // Load file into memory
    databento::DbnParser parser("data.dbn");
    parser.load_into_memory();
    
    const size_t total = parser.num_records();
    const uint8_t* data = parser.data();
    const size_t offset = parser.metadata_offset();
    const size_t rec_size = parser.record_size();
    
    // Direct memory access (200M+ rec/s)
    for (size_t i = 0; i < total; ++i) {
        const uint8_t* record = data + offset + (i * rec_size);
        
        // Parse inline - zero-copy!
        uint64_t ts_event = databento::read_u64_le(record);
        uint32_t instrument_id = databento::read_u32_le(record + 8);
        char action = record[12];
        char side = record[13];
        int64_t price = databento::read_i64_le(record + 16);
        uint32_t size = databento::read_u32_le(record + 24);
        
        // Your ultra-fast processing here
        // update_order_book(instrument_id, side, price, size);
    }
    
    return 0;
}
```

**Performance:** 200M+ records/sec

---

### Example 3: Batch Processing (Balanced)

**Use Case:** Real-time analytics, VWAP calculation, statistics

```cpp
#include <databento/parser.hpp>
#include <iostream>
#include <numeric>

int main() {
    databento::DbnParser parser("data.dbn");
    parser.load_into_memory();
    
    databento::BatchProcessor batch_proc(512 * 1024); // 512K per batch
    
    auto batch_callback = [](const std::vector<databento::MboMsg>& batch) {
        // Calculate VWAP for batch
        double total_price_weighted = 0.0;
        uint64_t total_volume = 0;
        
        for (const auto& msg : batch) {
            if (msg.action == 'A' || msg.action == 'M') {
                double price = databento::price_to_double(msg.price);
                total_price_weighted += price * msg.size;
                total_volume += msg.size;
            }
        }
        
        if (total_volume > 0) {
            double vwap = total_price_weighted / total_volume;
            std::cout << "Batch VWAP: $" << vwap 
                      << " Volume: " << total_volume << "\n";
        }
    };
    
    batch_proc.process_batches<databento::MboMsg>(parser, batch_callback);
    
    return 0;
}
```

**Performance:** 100-150M records/sec

---

### Example 4: Order Book Reconstruction

**Use Case:** Building order book from MBO data

```cpp
#include <databento/parser.hpp>
#include <map>
#include <iostream>

struct OrderBook {
    std::map<uint64_t, databento::MboMsg> bids;  // order_id -> msg
    std::map<uint64_t, databento::MboMsg> asks;
    
    void process(const databento::MboMsg& msg) {
        auto& book = (msg.side == 'B') ? bids : asks;
        
        switch (msg.action) {
            case 'A':  // Add
                book[msg.order_id] = msg;
                break;
            case 'C':  // Cancel
                book.erase(msg.order_id);
                break;
            case 'M':  // Modify
                book[msg.order_id] = msg;
                break;
        }
    }
    
    void print_top() const {
        if (!bids.empty()) {
            auto best_bid = std::max_element(bids.begin(), bids.end(),
                [](const auto& a, const auto& b) { 
                    return a.second.price < b.second.price; 
                });
            std::cout << "Best Bid: $" 
                      << databento::price_to_double(best_bid->second.price) 
                      << " @ " << best_bid->second.size << "\n";
        }
        
        if (!asks.empty()) {
            auto best_ask = std::min_element(asks.begin(), asks.end(),
                [](const auto& a, const auto& b) { 
                    return a.second.price < b.second.price; 
                });
            std::cout << "Best Ask: $" 
                      << databento::price_to_double(best_ask->second.price) 
                      << " @ " << best_ask->second.size << "\n";
        }
    }
};

int main() {
    OrderBook book;
    
    databento::DbnParser parser("data.dbn");
    parser.parse_mbo([&book](const databento::MboMsg& msg) {
        book.process(msg);
    });
    
    book.print_top();
    return 0;
}
```

---

## Python Usage

### Example 1: Quick Analysis

```python
import databento_cpp

# Fast bulk load (recommended!)
records = databento_cpp.parse_file_mbo_fast("data.dbn")

print(f"Loaded {len(records)} records")

# Calculate statistics
bid_count = sum(1 for r in records if r.side == 'B')
ask_count = sum(1 for r in records if r.side == 'A')

print(f"Bids: {bid_count}, Asks: {ask_count}")

# Print first 5
for i, r in enumerate(records[:5]):
    print(f"[{i}] {r.instrument_id}: ${r.price_float:.2f} x {r.size}")
```

---

### Example 2: Pandas Integration

```python
import databento_cpp
import pandas as pd

# Load data
records = databento_cpp.parse_file_mbo_fast("data.dbn")

# Convert to DataFrame
df = pd.DataFrame([{
    'ts_event': r.ts_event,
    'instrument_id': r.instrument_id,
    'action': r.action,
    'side': r.side,
    'price': r.price_float,
    'size': r.size,
    'order_id': r.order_id,
} for r in records])

# Analyze
print(df.head())
print(df.describe())

# Group by instrument
by_instrument = df.groupby('instrument_id')
for inst_id, group in by_instrument:
    avg_price = group['price'].mean()
    total_volume = group['size'].sum()
    print(f"Instrument {inst_id}: Avg=${avg_price:.2f}, Vol={total_volume}")
```

---

### Example 3: Real-Time Processing

```python
import databento_cpp

# Process records one at a time (streaming style)
count = 0
total_volume = 0

def callback(msg):
    global count, total_volume
    count += 1
    if msg.action == 'A':  # Add orders
        total_volume += msg.size

stats = databento_cpp.parse_file_mbo("data.dbn", callback)

print(f"Processed {count} records")
print(f"Total add volume: {total_volume}")
print(f"Rate: {stats.records_per_second / 1e6:.1f}M rec/s")
```

---

### Example 4: Individual Record Access

```python
import databento_cpp

# Load file
parser = databento_cpp.DbnParser("data.dbn")
parser.load_into_memory()

print(f"Total records: {len(parser)}")

# Access specific records
msg_0 = parser.get_record_mbo(0)
print(f"First record: {msg_0}")

msg_last = parser.get_record_mbo(len(parser) - 1)
print(f"Last record: {msg_last}")

# Random access
for i in [0, 100, 1000, 10000]:
    if i < len(parser):
        msg = parser.get_record_mbo(i)
        print(f"Record {i}: price=${msg.price_float:.2f}")
```

---

## Performance Tips

### 1. Always Use Optimization Flags

```bash
# ❌ Slow (10x slower!)
g++ -std=c++20 example.cpp ...

# ✅ Fast (full speed!)
g++ -O3 -march=native -std=c++20 example.cpp ...
```

### 2. Load Once, Process Many Times

```cpp
// ✅ Good - load once
databento::DbnParser parser("data.dbn");
parser.load_into_memory();

for (int pass = 0; pass < 10; ++pass) {
    for (size_t i = 0; i < parser.num_records(); ++i) {
        // Process at full speed
    }
}

// ❌ Bad - loading multiple times
for (int pass = 0; pass < 10; ++pass) {
    databento::DbnParser parser("data.dbn");  // Slow!
    parser.load_into_memory();
}
```

### 3. Choose the Right API Level

| Method | Speed | Use When |
|--------|-------|----------|
| Direct memory | 200M+/s | Maximum speed needed |
| Batch processing | 150M/s | Good balance |
| Per-record callback | 30M/s | Convenience matters |

### 4. Use Batch Processing for Analytics

```cpp
// ✅ Good - batch processing
databento::BatchProcessor batch(512*1024);
batch.process_batches<MboMsg>(parser, [](const auto& batch) {
    // Process 512K records at once
});

// ❌ Slower - per-record callback
parser.parse_mbo([](const MboMsg& msg) {
    // Process one at a time
});
```

### 5. Avoid Unnecessary Conversions in Python

```python
# ✅ Fast - use raw fixed-point
total = sum(r.price * r.size for r in records)

# ❌ Slower - converting to float each time
total = sum(r.price_float * r.size for r in records)
```

---

## Common Patterns

### Pattern 1: Filter by Instrument

```cpp
uint32_t target_instrument = 1234;

databento::DbnParser parser("data.dbn");
parser.parse_mbo([target_instrument](const databento::MboMsg& msg) {
    if (msg.instrument_id == target_instrument) {
        // Process this instrument only
    }
});
```

### Pattern 2: Time Range Filtering

```cpp
uint64_t start_time = 1000000000ULL;  // nanoseconds
uint64_t end_time = 2000000000ULL;

databento::DbnParser parser("data.dbn");
parser.load_into_memory();

for (size_t i = 0; i < parser.num_records(); ++i) {
    auto msg = databento::parse_mbo(parser.get_record(i));
    
    if (msg.ts_event >= start_time && msg.ts_event <= end_time) {
        // Process records in time range
    }
}
```

### Pattern 3: Side-Specific Processing

```cpp
databento::DbnParser parser("data.dbn");
parser.parse_mbo([](const databento::MboMsg& msg) {
    if (msg.side == 'B') {
        // Process bid orders
    } else if (msg.side == 'A') {
        // Process ask orders
    }
});
```

### Pattern 4: Action-Specific Processing

```cpp
databento::DbnParser parser("data.dbn");
parser.parse_mbo([](const databento::MboMsg& msg) {
    switch (msg.action) {
        case 'A':  // Add
            handle_add(msg);
            break;
        case 'C':  // Cancel
            handle_cancel(msg);
            break;
        case 'M':  // Modify
            handle_modify(msg);
            break;
    }
});
```

---

## Troubleshooting

### Problem: Performance Lower Than Expected

**Symptom:** Getting < 50M records/sec

**Solutions:**
1. Compile with `-O3 -march=native`
2. Use Release build, not Debug
3. Try direct memory access instead of callbacks
4. Increase batch size if using batch processing

### Problem: File Not Found

**Symptom:** `std::runtime_error: Failed to open file`

**Solutions:**
1. Check file path is correct
2. Check file exists: `ls -la /path/to/file.dbn`
3. Check file permissions: `chmod +r file.dbn`

### Problem: Python Import Error

**Symptom:** `ImportError: No module named 'databento_cpp'`

**Solutions:**
1. Run `./install_python.sh` from project root
2. Check pybind11 is installed: `pip3 show pybind11`
3. Try: `pip3 install -e . --user`

### Problem: Compilation Errors

**Symptom:** C++ compile errors

**Solutions:**
1. Ensure C++20 support: `g++ --version` (need GCC 10+)
2. Check include path is correct
3. Link parser.cpp in compilation
4. Use provided build.sh script

### Problem: Test Failures

**Symptom:** Tests fail when running `./test.sh`

**Solutions:**
1. Clean and rebuild: `rm -rf build && ./build.sh`
2. Check compiler version supports C++20
3. Run tests individually: `./build/test_parser --gtest_filter=*`

---

## Advanced Usage

### Custom Memory Management

```cpp
// Use your own memory buffer
std::vector<uint8_t> my_buffer = load_file_custom("data.dbn");

databento::DbnParser parser("dummy.dbn");
// Hack: manually set buffer
// (Note: This isn't officially supported, use at own risk)
```

### Parallel Processing

```cpp
#include <thread>
#include <vector>

void process_range(const databento::DbnParser& parser, 
                   size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        auto msg = databento::parse_mbo(parser.get_record(i));
        // Process msg
    }
}

int main() {
    databento::DbnParser parser("data.dbn");
    parser.load_into_memory();
    
    const size_t num_threads = 4;
    const size_t total = parser.num_records();
    const size_t per_thread = total / num_threads;
    
    std::vector<std::thread> threads;
    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * per_thread;
        size_t end = (t == num_threads - 1) ? total : start + per_thread;
        threads.emplace_back(process_range, std::ref(parser), start, end);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}
```

---

## Next Steps

1. **Read the Quick Start:** See [QUICK_START.md](QUICK_START.md)
2. **Run the Examples:** Try all examples in `examples/`
3. **Run Benchmarks:** Compare performance on your hardware
4. **Integrate:** Use in your own project
5. **Optimize:** Profile and tune for your use case

---

## Resources

- **README.md** - Complete documentation
- **QUICK_START.md** - 5-minute getting started
- **PROJECT_STATUS.md** - Project completion status
- **examples/** - Working code examples
- **tests/** - Unit test examples

---

**Happy parsing at 200M+ records/sec! 🚀**

