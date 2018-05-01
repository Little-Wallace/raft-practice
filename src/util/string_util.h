//
// Created by liuwei on 4/29/18.
//

#ifndef RAFT_STRING_UTIL_H
#define RAFT_STRING_UTIL_H
#include <string>
#include <vector>

class StringUtil {
public:
    static bool Split(const std::string& content, std::vector<std::string>& result,
                      const char& splim);
};



#endif //RAFT_STRING_UTIL_H
