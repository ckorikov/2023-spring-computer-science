#include "weather.cpp"
#include "network/network.cpp"

#include <boost/json.cpp>

weather::weather(const std::string& api) {
    api_ = api;
}

std::string weather::get_city() const {
    return boost::json::value_to<std::string>(parsed_data_.at("name"));
}

void weather::set_city(const std::string& city) {
    city_ = city;

    url_ =
        "http://api.openweathermap.org/data/2.5/"
        "weather?q=" +
        city_ + "&appid=" + api_ + "&lang=ru";
}

bool weather::check_city() const {
    if (parsed_data_.at("cod").is_int64()) {
        return true;
    } else {
        return false;
    }
}

void weather::refresh() {
    curl_data_ = get_request(url_);
    parsed_data_ = boost::json::parse(curl_data_);
}

std::string weather::get_weather() const {
    return boost::json::value_to<std::string>(
        parsed_data_.at("weather").at(0).at("description"));
}

int weather::get_temp() const {
    // kelvin_to_celsius
    return (boost::json::value_to<float>(parsed_data_.at("main").at("temp"))) -
           273;
}

float weather::get_wind() const {
    return boost::json::value_to<float>(parsed_data_.at("wind").at("speed"));
}
