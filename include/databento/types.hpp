#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace databento {

using InstrumentId = std::uint32_t;
using Price = std::int64_t;
using Quantity = std::uint32_t;
using UnixNanos = std::uint64_t;

struct RecordHeader {
    std::uint8_t length;
    std::uint8_t rtype;
    std::uint16_t publisher_id;
    InstrumentId instrument_id;
    UnixNanos ts_event;
};

struct MboMsg {
    RecordHeader header;
    std::uint64_t order_id;
    Price price;
    Quantity size;
    std::uint8_t flags;
    std::uint8_t channel_id;
    char action;
    char side;
    UnixNanos ts_recv;
    std::uint32_t sequence;
};

struct TradeMsg {
    RecordHeader header;
    Price price;
    Quantity size;
    char action;
    char side;
    std::uint8_t flags;
    std::uint8_t depth;
    UnixNanos ts_recv;
    std::uint32_t sequence;
};

struct Mbp1Msg {
    RecordHeader header;
    Price bid_px;
    Price ask_px;
    Quantity bid_sz;
    Quantity ask_sz;
    std::uint8_t bid_ct;
    std::uint8_t ask_ct;
    std::uint8_t flags;
    UnixNanos ts_recv;
    std::uint32_t sequence;
};

struct OhlcvMsg {
    RecordHeader header;
    Price open;
    Price high;
    Price low;
    Price close;
    std::uint64_t volume;
};

struct SymbolMapping {
    std::string raw_symbol;
    InstrumentId instrument_id;
    std::string stype_in;
    std::string stype_out;
    std::uint64_t start_ts;
    std::uint64_t end_ts;
};

struct Metadata {
    std::string dataset;
    std::string schema;
    std::uint64_t start;
    std::uint64_t end;
    std::uint64_t limit;
    std::vector<SymbolMapping> mappings;
    std::vector<std::string> symbols;
    std::vector<std::uint16_t> partial;
    std::vector<std::uint16_t> not_found;
};

} // namespace databento
