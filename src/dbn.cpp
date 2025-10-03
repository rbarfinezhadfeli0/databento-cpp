#include "databento/dbn.hpp"
#include <fstream>
#include <cstring>
#include <stdexcept>

namespace databento {

DbnDecoder::DbnDecoder(std::istream& stream)
    : stream_(stream), metadata_read_(false) {
    read_metadata();
}

void DbnDecoder::read_metadata() {
    if (metadata_read_) {
        return;
    }
    
    // Simplified metadata reading
    // In real implementation, would parse DBN metadata header
    metadata_.dataset = "GLBX.MDP3";
    metadata_.schema = "trades";
    metadata_.start = 0;
    metadata_.end = 0;
    metadata_.limit = 0;
    
    metadata_read_ = true;
}

bool DbnDecoder::has_next() const {
    return stream_.peek() != EOF;
}

std::vector<std::uint8_t> DbnDecoder::read_raw_record() {
    if (!has_next()) {
        return {};
    }
    
    // Read record length (simplified - assuming first byte is length)
    std::uint8_t length;
    stream_.read(reinterpret_cast<char*>(&length), sizeof(length));
    
    if (stream_.gcount() == 0) {
        return {};
    }
    
    std::vector<std::uint8_t> record(length);
    record[0] = length;
    stream_.read(reinterpret_cast<char*>(record.data() + 1), length - 1);
    
    return record;
}

DbnEncoder::DbnEncoder(std::ostream& stream, const Metadata& metadata)
    : stream_(stream), metadata_written_(false) {
    write_metadata(metadata);
}

void DbnEncoder::write_metadata(const Metadata& metadata) {
    if (metadata_written_) {
        return;
    }
    
    // Simplified metadata writing
    // In real implementation, would write DBN metadata header
    
    metadata_written_ = true;
}

void DbnEncoder::write_raw_record(const std::vector<std::uint8_t>& data) {
    stream_.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::vector<std::uint8_t> read_dbn_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    
    file.seekg(0, std::ios::end);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<std::uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return data;
}

void write_dbn_file(const std::string& path, const std::vector<std::uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

} // namespace databento
