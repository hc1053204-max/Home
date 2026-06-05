#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>

/**
 * @brief Utils 命名空間：提供遊戲通用工具函式與常量
 * 
 * 包含終端顏色控制、日誌系統、UI 繪製工具以及輸入驗證函式。
 */
namespace Utils {
    // 恢復顏色開關 (可用於在不支持 ANSI 的環境中禁用顏色)
    const bool USE_COLORS = true;

    // ANSI 顏色常數：用於在終端顯示不同顏色的文字
    const std::string COLOR_RESET  = USE_COLORS ? "\033[0m" : "";
    const std::string COLOR_RED    = USE_COLORS ? "\033[31m" : "";
    const std::string COLOR_GREEN  = USE_COLORS ? "\033[32m" : "";
    const std::string COLOR_YELLOW = USE_COLORS ? "\033[33m" : "";
    const std::string COLOR_BLUE   = USE_COLORS ? "\033[34m" : "";
    const std::string COLOR_MAGENTA= USE_COLORS ? "\033[35m" : "";
    const std::string COLOR_CYAN   = USE_COLORS ? "\033[36m" : "";
    const std::string COLOR_WHITE  = USE_COLORS ? "\033[37m" : "";
    const std::string COLOR_BOLD   = USE_COLORS ? "\033[1m" : "";

    /**
     * @brief Logger 命名空間：實作簡單的戰鬥紀錄系統
     */
    namespace Logger {
        // 儲存最近的日誌訊息
        inline std::vector<std::string> history;
        // 最大保留日誌數量
        const int MAX_LOGS = 15;

        /**
         * @brief 將訊息輸出到終端並存入歷史紀錄
         * @param msg 要記錄的訊息
         */
        inline void addLog(const std::string& msg) {
            std::cout << msg << std::endl;
            history.push_back(msg);
            if (history.size() > MAX_LOGS) {
                history.erase(history.begin());
            }
        }

        /**
         * @brief 清空所有日誌紀錄
         */
        inline void clear() {
            history.clear();
        }
    }

    /**
     * @brief 繪製一條指定長度的線 (用於 UI 分割)
     * @param c 線條使用的字符
     * @param width 線條長度
     */
    inline void printLine(char c, int width = 40) {
        for (int i = 0; i < width; ++i) std::cout << c;
        std::cout << std::endl;
    }

    /**
     * @brief 繪製一個簡單的方框標題
     * @param text 標題文字
     * @param borderChar 邊框字符
     */
    inline void printBoxHeader(const std::string& text, char borderChar = '#') {
        int width = text.length() + 4;
        printLine(borderChar, width);
        std::cout << borderChar << " " << text << " " << borderChar << std::endl;
        printLine(borderChar, width);
    }

    /**
     * @brief 居中打印文字
     * @param text 文字內容
     * @param width 總寬度
     */
    inline void printCentered(const std::string& text, int width = 40) {
        int padding = (width - (int)text.length()) / 2;
        if (padding > 0) {
            for (int i = 0; i < padding; ++i) std::cout << " ";
        }
        std::cout << text << std::endl;
    }

    /**
     * @brief 生成視覺化血條
     * @param current 當前生命值
     * @param max 最大生命值
     * @param width 血條寬度
     * @return 包含顏色和進度條的字串
     */
    inline std::string generateHPBar(int current, int max, int width = 20) {
        float percentage = (float)current / max;
        int filledWidth = (int)(percentage * width);
        
        std::string bar = "[";
        for (int i = 0; i < width; ++i) {
            if (i < filledWidth) bar += "#";
            else bar += "-";
        }
        bar += "] " + std::to_string((int)(percentage * 100)) + "%";
        
        // 根據生命值百分比改變血條顏色
        if (percentage < 0.3f) return COLOR_RED + bar + COLOR_RESET;
        if (percentage < 0.6f) return COLOR_YELLOW + bar + COLOR_RESET;
        return COLOR_GREEN + bar + COLOR_RESET;
    }

    /**
     * @brief 清理終端螢幕
     */
    inline void clearScreen() {
        #ifdef _WIN32
            std::system("cls");
        #else
            std::cout << "\033[2J\033[1;1H";
        #endif
    }

    /**
     * @brief 安全地獲取整數輸入
     * 處理非數字輸入，防止程序崩潰
     * @param prompt 輸入提示文字
     * @return 驗證後的整數值
     */
    inline int getSafeInt(const std::string& prompt) {
        std::string input;
        int value;
        while (true) {
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
