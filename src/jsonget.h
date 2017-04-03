#ifndef JSONGET_H
#define JSONGET_H
#include <json/json.h>
#include <string>
#include <map>

typedef std::map<std::string, std::string> header_t;
Json::Value json_get(const std::string& url, const header_t& headers, const long timeout = 30);

#endif
