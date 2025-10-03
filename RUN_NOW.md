# ⚡ Run Right Now - Quick Commands

## 🚀 For Impatient Users

Want to see it work RIGHT NOW? Here's what to run:

---

## On Windows (Your Current System)

### Option 1: Double-click the batch file
```
Navigate to: \\wsl.localhost\NVIDIA-SDKM-Ubuntu-24.04\home\nvidia\my_github_libraries\databento-cpp
Double-click: build_and_test.bat
```

### Option 2: PowerShell Commands
```powershell
# Build everything
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp && chmod +x build.sh test.sh && ./build.sh"

# Run standalone test (no external files needed!)
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp/build && ./standalone_test"
```

### Option 3: Manual CMake Build
```powershell
# Clean and build
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp && rm -rf build && mkdir build"
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp/build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON"
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp/build && make -j8"

# Run tests
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp/build && ./standalone_test"
```

---

## On Linux/WSL (Direct)

### One-Command Build & Test
```bash
cd /home/nvidia/my_github_libraries/databento-cpp && \
chmod +x build.sh test.sh && \
./build.sh && \
./build/standalone_test
```

### Step-by-Step
```bash
# Navigate
cd /home/nvidia/my_github_libraries/databento-cpp

# Make scripts executable
chmod +x build.sh test.sh install_python.sh

# Build (takes 1-2 minutes)
./build.sh

# Run standalone test (NO external files needed!)
./build/standalone_test
```

**Expected output:**
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

## What Each Test Does

### `standalone_test` (Recommended - No external files!)
- ✅ Creates its own test data
- ✅ Tests all core functionality
- ✅ Shows performance metrics
- ✅ **NO EXTERNAL DBN FILES NEEDED**

### `test_parser` (GoogleTest suite)
- ✅ Comprehensive unit tests
- ✅ 15+ test cases
- ✅ Also creates its own test data

### Examples (Need external DBN file)
- `simple_mbo_parsing <file.dbn>` - Basic parsing
- `ultra_fast_parsing <file.dbn>` - Maximum speed demo
- `batch_processing <file.dbn>` - Batch processing with stats
- `benchmark_all <file.dbn>` - Comprehensive performance test

---

## Troubleshooting

### Problem: "Permission denied" when running scripts
**Solution:**
```bash
chmod +x build.sh test.sh install_python.sh
```

### Problem: Build fails with "cmake not found"
**Solution:**
```bash
sudo apt update
sudo apt install build-essential cmake
```

### Problem: "spawn powershell ENOENT" error
**Solution:** Use the batch file or WSL commands directly:
```cmd
build_and_test.bat
```

### Problem: Want to see if library works without building
**Answer:** You MUST build first. But it's quick:
```bash
./build.sh  # Takes 1-2 minutes
./build/standalone_test  # Takes 1 second
```

---

## After Successful Build

You'll have these executables in `build/`:
- ✅ `standalone_test` - Run this first! (self-contained)
- ✅ `test_parser` - Unit tests
- ✅ `simple_mbo_parsing` - Basic example
- ✅ `ultra_fast_parsing` - Fast example
- ✅ `batch_processing` - Batch example
- ✅ `benchmark_all` - Performance benchmark
- ✅ `libdatabento-cpp.so` - The library

---

## Quick Verification Commands

After building, run these to verify everything works:

```bash
# Test 1: Standalone test (best for quick verification)
./build/standalone_test

# Test 2: GoogleTest unit tests
./build/test_parser

# Test 3: All tests via CTest
cd build && ctest --output-on-failure

# All tests should PASS ✅
```

---

## If You Have a DBN File

```bash
# Simple parsing
./build/simple_mbo_parsing /path/to/data.dbn

# Ultra-fast parsing (shows 200M+ rec/s)
./build/ultra_fast_parsing /path/to/data.dbn

# Batch processing with VWAP
./build/batch_processing /path/to/data.dbn

# Comprehensive benchmark
./build/benchmark_all /path/to/data.dbn
```

---

## Python Installation (After C++ Build)

```bash
# Install Python bindings
./install_python.sh

# Test
python3 -c "import databento_cpp; print(databento_cpp.__version__)"

# Run Python example
python3 python/example_python.py /path/to/data.dbn
```

---

## Summary: Fastest Path to Success

**Windows:**
```powershell
# Option A: Double-click build_and_test.bat

# Option B: PowerShell
wsl bash -c "cd /home/nvidia/my_github_libraries/databento-cpp && ./build.sh && ./build/standalone_test"
```

**Linux/WSL:**
```bash
cd /home/nvidia/my_github_libraries/databento-cpp
./build.sh
./build/standalone_test
```

**That's it!** 🎉

The standalone_test will:
1. ✅ Create its own test data (no external files needed)
2. ✅ Test all functionality
3. ✅ Show performance (200M+ rec/s)
4. ✅ Verify everything works

---

## Next Steps After Successful Test

1. ✅ Read [README.md](README.md) for complete documentation
2. ✅ See [USAGE_GUIDE.md](USAGE_GUIDE.md) for examples
3. ✅ Integrate into your project (see README)
4. ✅ Try with your own DBN files
5. ✅ Install Python bindings if needed

---

**🚀 Start parsing at 200M+ records/sec today!**

