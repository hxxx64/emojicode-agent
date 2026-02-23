// openrouter-ai.adapter.cpp - Simplified OpenRouter AI FFI
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <emojicode/s/String.h>

namespace {

size_t write_cb(void* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

std::string json_escape(const std::string& s) {
    std::ostringstream oss;
    for (unsigned char c : s) {
        if (c == '"') oss << "\\\"";
        else if (c == '\\') oss << "\\\\";
        else if (c == '\n') oss << "\\n";
        else if (c == '\r') oss << "\\r";
        else if (c == '\t') oss << "\\t";
        else if (c < 32) { char b[8]; snprintf(b, 8, "\\u%04x", c); oss << b; }
        else oss << c;
    }
    return oss.str();
}

void append_utf8(std::string& out, unsigned cp) {
    if (cp <= 0x7F) out += (char)cp;
    else if (cp <= 0x7FF) { out += (char)(0xC0 | (cp >> 6)); out += (char)(0x80 | (cp & 0x3F)); }
    else if (cp <= 0xFFFF) { out += (char)(0xE0 | (cp >> 12)); out += (char)(0x80 | ((cp >> 6) & 0x3F)); out += (char)(0x80 | (cp & 0x3F)); }
    else if (cp <= 0x10FFFF) { out += (char)(0xF0 | (cp >> 18)); out += (char)(0x80 | ((cp >> 12) & 0x3F)); out += (char)(0x80 | ((cp >> 6) & 0x3F)); out += (char)(0x80 | (cp & 0x3F)); }
}

std::string extract_json_content(const std::string& json) {
    const char *key = "\"content\":\"", *p = strstr(json.c_str(), key);
    if (!p) return "";
    p += strlen(key);
    std::string res;
    while (*p && *p != '"') {
        if (*p == '\\') {
            p++;
            switch (*p) {
                case 'n': res += '\n'; break;
                case 'r': res += '\r'; break;
                case 't': res += '\t'; break;
                case 'u': {
                    unsigned cp = strtol(std::string(p + 1, 4).c_str(), nullptr, 16);
                    if (cp >= 0xD800 && cp <= 0xDBFF && p[5] == '\\' && p[6] == 'u') {
                        unsigned cp2 = strtol(std::string(p + 7, 4).c_str(), nullptr, 16);
                        cp = 0x10000 + ((cp - 0xD800) << 10) + (cp2 - 0xDC00); p += 6;
                    }
                    append_utf8(res, cp); p += 4; break;
                }
                default: res += *p; break;
            }
        } else res += *p;
        p++;
    }
    return res;
}

} // namespace

extern "C" {

s::String* openrouter_complete(void*, s::String* prompt) {
    const char *api_key = getenv("OPENROUTER_API_KEY"), *model = getenv("OPENROUTER_MODEL"), *verbose = getenv("OPENROUTER_VERBOSE");
    if (!api_key) return s::String::init("[Error: OPENROUTER_API_KEY not set]");
    
    std::string body = "{\"model\":\"" + json_escape(model ? model : "openrouter/free") + 
                       "\",\"messages\":[{\"role\":\"user\",\"content\":\"" + 
                       json_escape(prompt->stdString()) + "\"}]}";
    
    if (verbose && std::string(verbose) == "1") {
        printf("\n[OpenRouter Request]\nModel: %s\nBody: %s\n", model ? model : "openrouter/free", body.c_str());
    }

    CURL* curl = curl_easy_init();
    std::string response;
    struct curl_slist* headers = curl_slist_append(NULL, (std::string("Authorization: Bearer ") + api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    // headers = curl_slist_append(headers, "HTTP-Referer: XXXX");
    // headers = curl_slist_append(headers, "X-Title: emojicode-agent");

    curl_easy_setopt(curl, CURLOPT_URL, "https://openrouter.ai/api/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return s::String::init(("[Error: " + std::string(curl_easy_strerror(res)) + "]").c_str());
    
    if (verbose && std::string(verbose) == "1") {
        printf("\n[OpenRouter Response]\n%s\n", response.c_str());
    }

    std::string content = extract_json_content(response);
    return s::String::init(content.empty() ? ("[Error: Empty response. Raw: " + response.substr(0, 100) + "]").c_str() : content.c_str());
}

} // extern "C"
