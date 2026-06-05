#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>

namespace Utils {
    // 安全地獲取整數輸入
    inline int getSafeInt(const std::string& prompt) {
        std::string input;
        int value;
        while (true) {
            std::cout << prompt << std::flush;
            if (!std::getline(std::cin, input)) {
                return -1;
            }
            if (input.empty()) {
                continue;
            }
            try {
                value = std::stoi(input);
                return value;
            } catch (...) {
                std::cout << "輸入錯誤！請輸入有效的數字。" << std::endl;
            }
        }
    }
}

#endif
