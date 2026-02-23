// native-file-system.adapter.cpp - Simplified FFI file operations
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <emojicode/s/String.h>

namespace fs = std::filesystem;

extern "C" {

s::String* toolexec_read_file(void*, s::String* pathStr) {
    std::string path = pathStr->stdString();
    std::ifstream f(path, std::ios::binary);
    if (!f) return s::String::init(("Error: Could not read " + path).c_str());
    std::stringstream ss; ss << f.rdbuf();
    return s::String::init(ss.str().c_str());
}

s::String* toolexec_write_file(void*, s::String* pathStr, s::String* contentStr) {
    std::string path = pathStr->stdString();
    std::ofstream f(path, std::ios::binary);
    if (!f) return s::String::init(("Error: Could not write " + path).c_str());
    f << contentStr->stdString();
    return s::String::init("Success");
}

s::String* toolexec_listdir(void*, s::String* pathStr, bool recursive) {
    std::string path = pathStr->stdString();
    if (path.empty()) path = ".";
    std::error_code ec;
    if (!fs::is_directory(path, ec)) return s::String::init(("Error: Not a directory: " + path).c_str());
    std::string res;
    if (recursive) {
        for (const auto& entry : fs::recursive_directory_iterator(path, ec)) {
            if (ec) { ec.clear(); continue; }
            if (!res.empty()) res += "\n";
            res += entry.path().string();
        }
    } else {
        for (const auto& entry : fs::directory_iterator(path, ec)) {
            if (ec) { ec.clear(); continue; }
            if (!res.empty()) res += "\n";
            res += entry.path().filename().string();
        }
    }
    return s::String::init(res.c_str());
}

s::String* toolexec_replace_in_file(void*, s::String* pathStr, s::String* diffStr) {
    std::string path = pathStr->stdString(), diff = diffStr->stdString();
    size_t searchStart = diff.find("\n") + 1, searchEnd = diff.find("\n=======");
    if (searchEnd == std::string::npos) return s::String::init("Error: Invalid diff format");
    std::string searchString = diff.substr(searchStart, searchEnd - searchStart);
    size_t replacementStart = diff.find("\n", searchEnd + 1) + 1, replacementEnd = diff.find("\n>>>>>>>");
    std::string replacementString = diff.substr(replacementStart, (replacementEnd == std::string::npos ? diff.size() : replacementEnd) - replacementStart);

    std::ifstream inputFile(path);
    std::stringstream contentStream; contentStream << inputFile.rdbuf();
    std::string content = contentStream.str();
    size_t matchPosition = content.find(searchString);
    if (matchPosition == std::string::npos) return s::String::init("Error: Search string not found");
    content.replace(matchPosition, searchString.size(), replacementString);
    std::ofstream(path) << content;
    return s::String::init("Success");
}

s::String* toolexec_search_files(void*, s::String* pathStr, s::String* searchStr) {
    std::string path = pathStr->stdString(), search = searchStr->stdString(), resultAccumulator;
    if (search.empty()) return s::String::init("");
    std::error_code ec;
    for (const auto& entry : fs::recursive_directory_iterator(path, ec)) {
        if (ec) { ec.clear(); continue; }
        if (entry.is_regular_file()) {
            std::ifstream fileStream(entry.path());
            std::string line;
            for (int lineNumber = 1; std::getline(fileStream, line); ++lineNumber) {
                if (line.find(search) != std::string::npos) {
                    if (!resultAccumulator.empty()) resultAccumulator += "\n";
                    resultAccumulator += entry.path().string() + ":" + std::to_string(lineNumber) + ":" + line;
                    if (resultAccumulator.size() > 500000) return s::String::init((resultAccumulator + "\n(truncated)").c_str());
                }
            }
        }
    }
    return s::String::init(resultAccumulator.empty() ? "(no matches)" : resultAccumulator.c_str());
}

}
