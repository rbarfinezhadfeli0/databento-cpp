#include "databento/enums.hpp"

namespace databento {

std::string to_string(Schema schema) {
    switch (schema) {
        case Schema::Mbo: return "mbo";
        case Schema::Mbp1: return "mbp-1";
        case Schema::Mbp10: return "mbp-10";
        case Schema::Tbbo: return "tbbo";
        case Schema::Trades: return "trades";
        case Schema::Ohlcv1s: return "ohlcv-1s";
        case Schema::Ohlcv1m: return "ohlcv-1m";
        case Schema::Ohlcv1h: return "ohlcv-1h";
        case Schema::Ohlcv1d: return "ohlcv-1d";
        case Schema::Definition: return "definition";
        case Schema::Statistics: return "statistics";
        case Schema::Status: return "status";
        case Schema::Imbalance: return "imbalance";
        default: return "unknown";
    }
}

std::string to_string(Encoding encoding) {
    switch (encoding) {
        case Encoding::Dbn: return "dbn";
        case Encoding::Csv: return "csv";
        case Encoding::Json: return "json";
        default: return "unknown";
    }
}

std::string to_string(Compression compression) {
    switch (compression) {
        case Compression::None: return "none";
        case Compression::Zstd: return "zstd";
        default: return "unknown";
    }
}

} // namespace databento
