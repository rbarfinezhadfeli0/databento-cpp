#pragma once

#include <cstdint>
#include <string>

namespace databento {

enum class Schema : std::uint8_t {
    Mbo = 0,
    Mbp1 = 1,
    Mbp10 = 2,
    Tbbo = 3,
    Trades = 4,
    Ohlcv1s = 5,
    Ohlcv1m = 6,
    Ohlcv1h = 7,
    Ohlcv1d = 8,
    Definition = 9,
    Statistics = 10,
    Status = 11,
    Imbalance = 12,
};

enum class Encoding : std::uint8_t {
    Dbn = 0,
    Csv = 1,
    Json = 2,
};

enum class Compression : std::uint8_t {
    None = 0,
    Zstd = 1,
};

enum class SType : std::uint8_t {
    RawSymbol = 0,
    InstrumentId = 1,
    Parent = 2,
    Continuous = 3,
};

enum class Side : char {
    Ask = 'A',
    Bid = 'B',
    None = 'N',
};

enum class Action : char {
    Add = 'A',
    Cancel = 'C',
    Modify = 'M',
    Clear = 'R',
    Trade = 'T',
};

std::string to_string(Schema schema);
std::string to_string(Encoding encoding);
std::string to_string(Compression compression);

} // namespace databento
