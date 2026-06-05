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
 * 本命名空間採取了「工具箱」設計模式，將所有無狀態的 UI 繪製與輸入處理函式集中管理。
 * 包含終端顏色控制、日誌系統、視覺化 UI 繪製以及魯棒的輸入驗證函式。
 */
namespace Utils {
    // 恢復顏色開關 (可用於在不支持 ANSI 的環境中禁用顏色)
    const bool USE_COLORS = true;

    // ANSI 顏色常數：利用終端轉義序列 (Escape Sequences) 改變文字顏色
    // 格式：\033[Xm，其中 X 是顏色代碼。
    const std::string COLOR_RESET  = USE_COLORS ? "\033[0m" : "";  // 重置為默認顏色
    const std::string COLOR_RED    = USE_COLORS ? "\033[31m" : "";  // 警告/傷害
    const std::string COLOR_GREEN  = USE_COLORS ? "\033[32m" : "";  // 成功/恢復
    const std::string COLOR_YELLOW = USE_COLORS ? "\033[33m" : "";  // 技能/重要訊息
    const std::string COLOR_BLUE   = USE_COLORS ? "\033[34m" : "";  // 提示/界面
    const std::string COLOR_MAGENTA= USE_COLORS ? "\033[35m" : "";  // 物品/裝備
    const std::string COLOR_CYAN   = USE_COLORS ? "\033[36m" : "";  // 獲得物品
    const std::string COLOR_WHITE  = USE_COLORS ? "\033[37m" : "";  // 普通攻擊
    const std::string COLOR_BOLD   = USE_COLORS ? "\033[1m" : "";   // 粗體強調

    /**
     * @brief Logger 命名空間：實作簡單的戰鬥紀錄系統
     * 
     * 採用了類似於「循環緩衝區」的邏輯：
     * 1. 每條日誌都被 push 到 vector 末尾。
     * 2. 當 vector 大小超過 MAX_LOGS 時，移除最舊的一條 (erase begin)。
     * 這樣能確保 UI 面板始終顯示最新的戰鬥動態，而不會無限增長佔用內存。
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
     * 
     * 實作原理：
     * 1. 計算生命值百分比 (current/max)。
     * 2. 將百分比映射到指定寬度 (width) 的字符數 (filledWidth)。
     * 3. 構建字串：[填充符# + 空白符-] + 百分比數值。
     * 4. 根據百分比區間賦予不同顏色 (紅 < 30%, 黃 < 60%, 綠 60%+)。
     * 
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
     * 考慮了跨平台兼容性 (Windows 使用 cls, Unix/Linux 使用 ANSI 轉義序列)。
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
     * 
     * 魯棒性設計：
     * 1. 使用 std::getline 而非 std::cin >> value，避免輸入非數字時進入無限循環。
     * 2. 使用 try-catch 捕獲 std::stoi 的異常 (invalid_argument, out_of_range)。
     * 3. 只有在成功轉換為整數後才返回結果，否則持續提示用戶。
     * 
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
