// terminal-input.adapter.cpp - Simplified Terminal Input FFI
#include <termios.h>
#include <unistd.h>
#include <string>
#include <emojicode/s/String.h>

namespace {

struct Term {
    struct termios old; bool ok = false;
    Term() {
        if (isatty(0) && tcgetattr(0, &old) == 0) {
            struct termios n = old; n.c_iflag &= ~ICRNL; n.c_lflag &= ~ECHOCTL; n.c_cc[VEOL] = 13;
            tcsetattr(0, TCSANOW, &n); ok = true;
        }
    }
    ~Term() { if (ok) tcsetattr(0, TCSANOW, &old); }
};

} // namespace

extern "C" {

s::String* stdin_readline(void*, s::String* prompt) {
    std::string promptStr = (prompt && prompt->count > 0) ? prompt->stdString() : "";
    Term t; std::string res; bool first = true;

    while (true) {
        write(1, first ? promptStr.c_str() : "  ", first ? promptStr.size() : 2);
        first = false; char buf[4096]; ssize_t n = read(0, buf, 4095);
        if (n <= 0) return res.empty() ? s::String::init("__EOF__") : s::String::init(res.c_str());
        
        char last = buf[n - 1];
        if (last == 13) { buf[n - 1] = 0; res += buf; if (t.ok) write(1, "\n", 1); break; }
        else if (last == 10) res += std::string(buf, n);
        else res += std::string(buf, n);
    }
    return s::String::init(res.c_str());
}

} // extern "C"
