#pragma once

#include "types.hpp"
#include "enums.hpp"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace databento {

struct TimeseriesGetParams {
    std::string dataset;
    std::vector<std::string> symbols;
    Schema schema = Schema::Trades;
    std::string start;
    std::string end;
    std::uint64_t limit = 0;
    Encoding encoding = Encoding::Dbn;
    Compression compression = Compression::Zstd;
    SType stype_in = SType::RawSymbol;
    SType stype_out = SType::InstrumentId;
};

struct BatchJob {
    std::string id;
    std::string state;
    std::string dataset;
    std::string ts_received;
    std::string ts_queued;
    std::string ts_process_start;
    std::string ts_process_done;
    std::string ts_expiration;
    std::uint64_t cost_usd;
    std::uint64_t record_count;
};

class HistoricalClient {
public:
    explicit HistoricalClient(const std::string& api_key);
    ~HistoricalClient();
    
    std::vector<std::uint8_t> timeseries_get_range(const TimeseriesGetParams& params);
    
    void timeseries_get_range_to_file(const TimeseriesGetParams& params, 
                                       const std::string& output_file);
    
    void timeseries_stream(const TimeseriesGetParams& params,
                          std::function<void(const std::uint8_t*, std::size_t)> callback);
    
    std::string batch_submit_job(const TimeseriesGetParams& params);
    
    BatchJob batch_get_job(const std::string& job_id);
    
    std::vector<BatchJob> batch_list_jobs();
    
    std::vector<std::uint8_t> batch_download(const std::string& job_id);
    
    std::vector<std::string> list_datasets();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace databento
