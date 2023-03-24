#ifndef CURRENCY_HPP
#define CURRENCY_HPP

#include <boost/json/value.hpp>
#include <string>

class course {
   public:
    std::string get_valute();
    void set_valute(const std::string& valute);
    bool check_valute() const;

    void refresh();

    float get_course() const;

   private:
    std::string curl_data_;
    boost::json::value parsed_data_;

    inline const static std::string url_ =
        "https://www.cbr-xml-daily.ru/daily_json.js";
    std::string valute_;
};

#endif
