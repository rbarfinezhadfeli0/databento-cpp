# 🔨 Build Instructions

## Quick Start

### On Linux/WSL (Recommended)

```bash
cd /home/nvidia/my_github_libraries/databento-cpp

# Make scripts executable
chmod +x build.sh test.sh install_python.sh

# Build everything
./build.sh

# Run tests
./test.sh

# Run standalone test (no external files needed)
./build/standalone_test
```

### On Windows (Using WSL)

#### Option 1: Run the batch file
```cmd
cd C:\path\to\databento-cpp
build_and_test.bat
```

#### Option 2: Use WSL directly
```powershell
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp && ./build.sh"
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp && ./test.sh"
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp/build && ./standalone_test"
```

---

## Detailed Steps

### 1. Prerequisites

**Linux/WSL:**
- GCC 10+ or Clang 12+
- CMake 3.14+
- Make

**Install on Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential cmake git
```

### 2. Build

```bash
cd /home/nvidia/my_github_libraries/databento-cpp

# Create build directory
mkdir -p build
cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DBUILD_TESTS=ON \
         -DBUILD_EXAMPLES=ON \
         -DBUILD_BENCHMARKS=ON

# Build (use all cores)
make -j$(nproc)
```

This will create:
- `libdatabento-cpp.so` - The shared library
- `simple_mbo_parsing` - Simple example
- `ultra_fast_parsing` - Fast example (200M+/s)
- `batch_processing` - Batch processing example
- `standalone_test` - Self-contained test
- `benchmark_all` - Comprehensive benchmark
- `test_parser` - Unit tests

### 3. Run Tests

```bash
# From build directory
ctest --output-on-failure

# Or run tests directly
./test_parser

# Run standalone test (creates its own test data)
./standalone_test
```

### 4. Try Examples

**Standalone test (no external files needed):**
```bash
./standalone_test
```

**With your own DBN file:**
```bash
./simple_mbo_parsing /path/to/your/data.dbn
./ultra_fast_parsing /path/to/your/data.dbn
./batch_processing /path/to/your/data.dbn
```

**Run benchmark:**
```bash
./benchmark_all /path/to/your/data.dbn
```

---

## Build Options

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \     # Release or Debug
  -DBUILD_TESTS=ON \               # Build unit tests
  -DBUILD_EXAMPLES=ON \            # Build examples
  -DBUILD_BENCHMARKS=ON \          # Build benchmarks
  -DBUILD_PYTHON=OFF               # Python bindings (requires pybind11)
```

---

## Troubleshooting

### Issue: "cmake: command not found"
**Solution:** Install CMake
```bash
sudo apt install cmake
```

### Issue: "g++: command not found"
**Solution:** Install build tools
```bash
sudo apt install build-essential
```

### Issue: Compilation errors about C++20
**Solution:** Update your compiler
```bash
sudo apt install gcc-11 g++-11
export CXX=g++-11
export CC=gcc-11
```

### Issue: Slow performance (< 50M rec/s)
**Solution:** Make sure you're using Release build with optimizations
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
# The build scripts already use -O3 -march=native
```

### Issue: PowerShell spawn errors on Windows
**Solution:** Use WSL commands directly or the batch file
```cmd
build_and_test.bat
```

---

## Python Installation

After building the C++ library:

```bash
cd /home/nvidia/my_github_libraries/databento-cpp

# Install Python bindings
./install_python.sh

# Or manually:
pip3 install -e . --user

# Test
python3 -c "import databento_cpp; print(databento_cpp.__version__)"
```

---

## Verification

After a successful build, verify everything works:

```bash
# Run the standalone test
./build/standalone_test
```

Expected output:
```
=========================================
Standalone Test Suite
databento-cpp-fast library
=========================================

Test 1: Basic file loading... PASSED
Test 2: Direct memory access... PASSED
Test 3: Callback parsing... PASSED
Test 4: Batch processing... PASSED
Test 5: Price conversion... PASSED
Test 6: Performance benchmark... PASSED
   Rate: 200M+ records/sec
   Checksum: 0x...

=========================================
Results: 6/6 tests passed
=========================================

✅ ALL TESTS PASSED!
```

---

## Next Steps

1. ✅ Build completed - Use `./build.sh` or `build_and_test.bat`
2. ✅ Tests pass - Run `./test.sh` or `./build/standalone_test`
3. ✅ Try examples - Use `./build/ultra_fast_parsing <file>`
4. ✅ Integrate - See [README.md](README.md) for integration options
5. ✅ Install Python - Run `./install_python.sh`

---

## Quick Reference

```bash
# Build
./build.sh

# Test
./test.sh
./build/standalone_test

# Examples (need DBN file)
./build/simple_mbo_parsing <file>
./build/ultra_fast_parsing <file>
./build/batch_processing <file>

# Benchmark (need DBN file)
./build/benchmark_all <file>

# Python
./install_python.sh
python3 python/example_python.py <file>
```

---

For more information, see:
- [README.md](README.md) - Complete documentation
- [QUICK_START.md](QUICK_START.md) - Getting started guide
- [USAGE_GUIDE.md](USAGE_GUIDE.md) - Usage examples

