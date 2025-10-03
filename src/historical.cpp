#include "databento/historical.hpp"
#include "http_client.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace databento {

class HistoricalClient::Impl {
public:
    explicit Impl(const std::string& api_key)
        : http_client_(api_key) {}
    
    std::vector<std::uint8_t> timeseries_get_range(const TimeseriesGetParams& params);
    void timeseries_get_range_to_file(const TimeseriesGetParams& params, const std::string& output_file);
    void timeseries_stream(const TimeseriesGetParams& params,
                          std::function<void(const std::uint8_t*, std::size_t)> callback);
    std::string batch_submit_job(const TimeseriesGetParams& params);
    BatchJob batch_get_job(const std::string& job_id);
    std::vector<BatchJob> batch_list_jobs();
    std::vector<std::uint8_t> batch_download(const std::string& job_id);
    std::vector<std::string> list_datasets();

private:
    internal::HttpClient http_client_;
    
    std::map<std::string, std::string> build_params(const TimeseriesGetParams& params);
};

HistoricalClient::HistoricalClient(const std::string& api_key)
    : impl_(std::make_unique<Impl>(api_key)) {}

HistoricalClient::~HistoricalClient() = default;

std::vector<std::uint8_t> HistoricalClient::timeseries_get_range(const TimeseriesGetParams& params) {
    return impl_->timeseries_get_range(params);
}

void HistoricalClient::timeseries_get_range_to_file(const TimeseriesGetParams& params,
                                                     const std::string& output_file) {
    impl_->timeseries_get_range_to_file(params, output_file);
}

void HistoricalClient::timeseries_stream(const TimeseriesGetParams& params,
                                        std::function<void(const std::uint8_t*, std::size_t)> callback) {
    impl_->timeseries_stream(params, callback);
}

std::string HistoricalClient::batch_submit_job(const TimeseriesGetParams& params) {
    return impl_->batch_submit_job(params);
}

BatchJob HistoricalClient::batch_get_job(const std::string& job_id) {
    return impl_->batch_get_job(job_id);
}

std::vector<BatchJob> HistoricalClient::batch_list_jobs() {
    return impl_->batch_list_jobs();
}

std::vector<std::uint8_t> HistoricalClient::batch_download(const std::string& job_id) {
    return impl_->batch_download(job_id);
}

std::vector<std::string> HistoricalClient::list_datasets() {
    return impl_->list_datasets();
}

// Impl methods

std::map<std::string, std::string> HistoricalClient::Impl::build_params(const TimeseriesGetParams& params) {
    std::map<std::string, std::string> result;
    result["dataset"] = params.dataset;
    result["schema"] = to_string(params.schema);
    result["start"] = params.start;
    result["end"] = params.end;
    result["encoding"] = to_string(params.encoding);
    result["compression"] = to_string(params.compression);
    
    if (!params.symbols.empty()) {
        std::string symbols_str;
        for (size_t i = 0; i < params.symbols.size(); ++i) {
            if (i > 0) symbols_str += ",";
            symbols_str += params.symbols[i];
        }
        result["symbols"] = symbols_str;
    }
    
    if (params.limit > 0) {
        result["limit"] = std::to_string(params.limit);
    }
    
    return result;
}

std::vector<std::uint8_t> HistoricalClient::Impl::timeseries_get_range(const TimeseriesGetParams& params) {
    auto query_params = build_params(params);
    auto response = http_client_.get("/v0/timeseries.get_range", query_params);
    
    if (response.status_code != 200) {
        throw std::runtime_error("Request failed with status " + std::to_string(response.status_code));
    }
    
    return std::vector<std::uint8_t>(response.body.begin(), response.body.end());
}

void HistoricalClient::Impl::timeseries_get_range_to_file(const TimeseriesGetParams& params,
                                                          const std::string& output_file) {
    auto data = timeseries_get_range(params);
    std::ofstream out(output_file, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open output file: " + output_file);
    }
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void HistoricalClient::Impl::timeseries_stream(const TimeseriesGetParams& params,
                                               std::function<void(const std::uint8_t*, std::size_t)> callback) {
    auto query_params = build_params(params);
    http_client_.stream_get("/v0/timeseries.get_range", query_params, callback);
}

std::string HistoricalClient::Impl::batch_submit_job(const TimeseriesGetParams& params) {
    std::ostringstream json;
    json << "{";
    json << "\"dataset\":\"" << params.dataset << "\",";
    json << "\"schema\":\"" << to_string(params.schema) << "\",";
    json << "\"start\":\"" << params.start << "\",";
    json << "\"end\":\"" << params.end << "\"";
    if (!params.symbols.empty()) {
        json << ",\"symbols\":[";
        for (size_t i = 0; i < params.symbols.size(); ++i) {
            if (i > 0) json << ",";
            json << "\"" << params.symbols[i] << "\"";
        }
        json << "]";
    }
    json << "}";
    
    auto response = http_client_.post("/v0/batch.submit_job", json.str());
    if (response.status_code != 200) {
        throw std::runtime_error("Failed to submit batch job: " + response.body);
    }
    
    // Parse job ID from response (simplified)
    return response.body;
}

BatchJob HistoricalClient::Impl::batch_get_job(const std::string& job_id) {
    auto response = http_client_.get("/v0/batch/" + job_id, {});
    if (response.status_code != 200) {
        throw std::runtime_error("Failed to get batch job: " + response.body);
    }
    
    BatchJob job;
    job.id = job_id;
    job.state = "completed"; // Simplified parsing
    return job;
}

std::vector<BatchJob> HistoricalClient::Impl::batch_list_jobs() {
    auto response = http_client_.get("/v0/batch.list_jobs", {});
    if (response.status_code != 200) {
        throw std::runtime_error("Failed to list batch jobs: " + response.body);
    }
    
    // Simplified - return empty vector
    return {};
}

std::vector<std::uint8_t> HistoricalClient::Impl::batch_download(const std::string& job_id) {
    auto response = http_client_.get("/v0/batch.download/" + job_id, {});
    if (response.status_code != 200) {
        throw std::runtime_error("Failed to download batch result: " + response.body);
    }
    
    return std::vector<std::uint8_t>(response.body.begin(), response.body.end());
}

std::vector<std::string> HistoricalClient::Impl::list_datasets() {
    auto response = http_client_.get("/v0/metadata.list_datasets", {});
    if (response.status_code != 200) {
        throw std::runtime_error("Failed to list datasets: " + response.body);
    }
    
    // Simplified - return common datasets
    return {"GLBX.MDP3", "XNAS.ITCH", "DBEQ.BASIC"};
}

} // namespace databento
