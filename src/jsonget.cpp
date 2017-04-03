#include <curl/curl.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include "jsonget.h"

namespace{
    static size_t write_callback(const void* buffer, const size_t size, const size_t members, void* user_pointer){
        if(user_pointer){
            size_t length = size * members;
            std::stringstream& stream = *reinterpret_cast<std::stringstream*>(user_pointer);
            stream.write(reinterpret_cast<const char*>(buffer), length);
            return length;
        }
        else{
            return 0;
        }
    }
}

Json::Value json_get(const std::string& url, const header_t& headers, const long timeout){

    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();
    std::stringstream json_text;
    if(curl){
        struct curl_slist *list = NULL;
        for(auto i: headers){
            std::string head = i.first + ": " + i.second;
            list = curl_slist_append(list, head.c_str());
        }
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json_text))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list)))
        {
            code = curl_easy_perform(curl);
        }
    }
    if(code != CURLE_OK){
        Json::Value null;
        return null;
    }
    else{
        Json::Value out;
        json_text >> out;
        return out;
    }
}
