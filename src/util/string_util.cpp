//
// Created by liuwei on 4/29/18.
//

#include "string_util.h"

using namespace std;
bool StringUtil::Split(const string& content, vector<string>& result,
                        const char& splim)
{
    const char* addr = content.c_str();
    size_t prev = 0;
    for (size_t pos = 0; pos < content.length(); pos ++) {
        if (addr[pos] == splim) {
            if (prev < pos)
                result.emplace_back(addr + prev, pos - prev);
            prev = pos + 1;
        }
    }
    if (prev < content.length()) {
        result.emplace_back(addr + prev, content.length() - prev);
    }
    return true;
}
