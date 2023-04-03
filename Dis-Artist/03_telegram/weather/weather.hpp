#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <boost/json/value.hpp>
#include <string>

class weather {
   public:
    explicit weather(const std::string& api);

    std::string get_city() const;
    void set_city(const std::string& city);
    bool check_city() const;

    void refresh();

    std::string get_weather() const;
    int get_temp() const;
    float get_wind() const;

   private:
    std::string curl_data_;
    boost::json::value parsed_data_;

    std::string api_;
    std::string url_;
    std::string city_;
};

#endif
