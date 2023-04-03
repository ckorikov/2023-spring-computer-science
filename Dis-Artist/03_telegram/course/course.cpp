#include "course.hpp"
#include "network/network.hpp"

#include <boost/json.hpp>

std::string course::get_valute() {
    return valute_;
}

void course::set_valute(const std::string& valute) {
    valute_ = valute;
}

bool course::check_valute() const {
    try {
        boost::json::value_to<std::string>(
            parsed_data_.at("Valute").at(valute_).at("ID"));
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void course::refresh() {
    curl_data_ = get_request(url_);
    parsed_data_ = boost::json::parse(curl_data_);
}

float course::get_course() const {
    return boost::json::value_to<float>(
        parsed_data_.at("Valute").at(valute_).at("Value"));
}
