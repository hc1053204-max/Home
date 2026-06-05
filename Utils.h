#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

namespace Utils {
    // 顏色開關：設為 false 以解決部分終端的文字疊加/截斷問題
    const bool USE_COLORS = true;

    // ANSI 顏色常數
    const std::string COLOR_RESET  = USE_COLORS ? "\033[0m" : "";
    const std::string COLOR_RED    = USE_COLORS ? "\033[31m" : "";    // 傷害 / 危險
    const std::string COLOR_GREEN  = USE_COLORS ? "\033[32m" : "";    // 回復 / 成功
    const std::string COLOR_YELLOW = USE_COLORS ? "\033[33m" : "";    // 警告 / 技能
    const std::string COLOR_BLUE   = USE_COLORS ? "\033[34m" : "";    // 資訊 / 系統
    const std::string COLOR_MAGENTA= USE_COLORS ? "\033[35m" : "";    // 特殊 / 掉落
    const std::string COLOR_CYAN   = USE_COLORS ? "\033[36m" : "";    // 提示
    const std::string COLOR_WHITE  = USE_COLORS ? "\033[37m" : "";    // 一般
    const std::string COLOR_BOLD   = USE_COLORS ? "\033[1m" : "";     // 加粗

    // 繪製一條指定長度的線
    inline void printLine(char c, int width = 40) {
        for (int i = 0; i < width; ++i) std::cout << c;
        std::cout << std::endl;
    }

    // 繪製一個簡單的方框標題
    inline void printBoxHeader(const std::string& text, char borderChar = '#') {
        int width = text.length() + 4;
        printLine(borderChar, width);
        std::cout << borderChar << " " << text << " " << borderChar << std::endl;
        printLine(borderChar, width);
    }

    // 居中打印文字
    inline void printCentered(const std::string& text, int width = 40) {
        int padding = (width - (int)text.length()) / 2;
        if (padding > 0) {
            for (int i = 0; i < padding; ++i) std::cout << " ";
        }
        std::cout << text << std::endl;
    }

    // 生成視覺化血條
    inline std::string generateHPBar(int current, int max, int width = 20) {
        float percentage = (float)current / max;
        int filledWidth = (int)(percentage * width);
        
        std::string bar = "[";
        for (int i = 0; i < width; ++i) {
            if (i < filledWidth) bar += "#";
            else bar += "-";
        }
        bar += "] " + std::to_string((int)(percentage * 100)) + "%";
        
        if (percentage < 0.3f) return COLOR_RED + bar + COLOR_RESET;
        if (percentage < 0.6f) return COLOR_YELLOW + bar + COLOR_RESET;
        return COLOR_GREEN + bar + COLOR_RESET;
    }

    // 清理螢幕 - 針對不穩定終端進行優化
    inline void clearScreen() {
        #ifdef _WIN32
            // 嘗試使用 cls，如果仍然疊加，可考慮改用打印 100 個 \n
            std::system("cls");
        #else
            std::cout << "\033[2J\033[1;1H";
        #endif
    }

    // 安全地獲取整數輸入
    inline int getSafeInt(const std::string& prompt) {
        std::string input;
        int value;
        while (true) {
            // 增加換行確保不會與上方內容疊加
            std::cout << "\n" << prompt << std::flush;
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
                std::cout << COLOR_RED << "輸入錯誤！請輸入有效的數字。" << COLOR_RESET << std::endl;
            }
        }
    }
}

#endif
