#pragma once

#include "types.hpp"
#include "enums.hpp"
#include <vector>
#include <memory>
#include <istream>
#include <ostream>

namespace databento {

class DbnDecoder {
public:
    explicit DbnDecoder(std::istream& stream);
    
    const Metadata& metadata() const { return metadata_; }
    
    bool has_next() const;
    
    template<typename T>
    bool read_record(T& record);
    
    std::vector<std::uint8_t> read_raw_record();

private:
    std::istream& stream_;
    Metadata metadata_;
    bool metadata_read_;
    
    void read_metadata();
};

class DbnEncoder {
public:
    explicit DbnEncoder(std::ostream& stream, const Metadata& metadata);
    
    template<typename T>
    void write_record(const T& record);
    
    void write_raw_record(const std::vector<std::uint8_t>& data);

private:
    std::ostream& stream_;
    bool metadata_written_;
    
    void write_metadata(const Metadata& metadata);
};

std::vector<std::uint8_t> read_dbn_file(const std::string& path);
void write_dbn_file(const std::string& path, const std::vector<std::uint8_t>& data);

} // namespace databento
