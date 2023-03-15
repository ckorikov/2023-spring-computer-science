#include "network.hpp"

#include <curl/curl.h>
#include <memory>

namespace {
size_t writer(char* buffer, size_t size, size_t nmemb, std::string* html) {
    int result = 0;

    if (buffer != NULL) {
        html->append(buffer, size * nmemb);
        result = size * nmemb;
    }

    return result;
}
}  // namespace

std::string get_request(std::string link) {
    // CURL* curl;
    std::unique_ptr<CURL, void (*)(CURL*)> curl(curl_easy_init(),
                                                curl_easy_cleanup);
    std::string data;

    // curl = curl_easy_init();
    curl_easy_setopt(curl.get(), CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &data);
    curl_easy_perform(curl.get());
    // curl_easy_cleanup(curl);

    return data;
}
