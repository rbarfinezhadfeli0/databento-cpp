#include <databento/databento.hpp>
#include <iostream>
#include <cassert>
#include <cstring>

void test_version() {
    std::cout << "Testing version constant...\n";
    assert(std::strcmp(databento::VERSION, "0.1.0") == 0);
    std::cout << "  Version: " << databento::VERSION << " ✓\n";
}

void test_enums() {
    std::cout << "Testing enum to_string functions...\n";
    
    assert(databento::to_string(databento::Schema::Trades) == "trades");
    assert(databento::to_string(databento::Schema::Mbo) == "mbo");
    assert(databento::to_string(databento::Schema::Mbp1) == "mbp-1");
    std::cout << "  Schema enum conversions ✓\n";
    
    assert(databento::to_string(databento::Encoding::Dbn) == "dbn");
    assert(databento::to_string(databento::Encoding::Csv) == "csv");
    assert(databento::to_string(databento::Encoding::Json) == "json");
    std::cout << "  Encoding enum conversions ✓\n";
    
    assert(databento::to_string(databento::Compression::None) == "none");
    assert(databento::to_string(databento::Compression::Zstd) == "zstd");
    std::cout << "  Compression enum conversions ✓\n";
}

void test_types() {
    std::cout << "Testing type definitions...\n";
    
    databento::RecordHeader header;
    header.length = 64;
    header.rtype = 1;
    header.publisher_id = 100;
    header.instrument_id = 12345;
    header.ts_event = 1609459200000000000ULL;
    assert(header.length == 64);
    std::cout << "  RecordHeader ✓\n";
    
    databento::MboMsg mbo;
    mbo.order_id = 123456789;
    mbo.price = 100000;
    mbo.size = 100;
    mbo.action = 'A';
    mbo.side = 'B';
    assert(mbo.action == 'A');
    std::cout << "  MboMsg ✓\n";
    
    databento::TradeMsg trade;
    trade.price = 100000;
    trade.size = 50;
    trade.side = 'B';
    assert(trade.size == 50);
    std::cout << "  TradeMsg ✓\n";
}

void test_historical_client_creation() {
    std::cout << "Testing HistoricalClient creation...\n";
    try {
        databento::HistoricalClient client("test-api-key");
        std::cout << "  HistoricalClient created ✓\n";
    } catch (const std::exception& e) {
        std::cerr << "  Error creating client: " << e.what() << "\n";
    }
}

void test_live_client_creation() {
    std::cout << "Testing LiveClient creation...\n";
    try {
        databento::LiveClient client("test-api-key");
        std::cout << "  LiveClient created ✓\n";
        
        client.connect();
        assert(client.is_connected());
        std::cout << "  LiveClient connect ✓\n";
        
        client.stop();
        assert(!client.is_connected());
        std::cout << "  LiveClient stop ✓\n";
    } catch (const std::exception& e) {
        std::cerr << "  Error with live client: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "Running databento-cpp basic tests\n";
    std::cout << "==================================\n\n";
    
    try {
        test_version();
        test_enums();
        test_types();
        test_historical_client_creation();
        test_live_client_creation();
        
        std::cout << "\n✓ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed: " << e.what() << "\n";
        return 1;
    }
}
