#include "GameManager.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <thread>
#include <chrono>
#include "Utils.h"
using namespace std;

/**
 * @brief GameManager 建構子
 * 初始化起始關卡為 1。
 */
GameManager::GameManager():stage(1){}

int GameManager::getStage()const{
    return stage;
}

void GameManager::setStage(int s){
    stage = s;
}

/**
 * @brief 顯示戰鬥界面 (UI 渲染引擎)
 * 
 * 本函式負責將目前的戰鬥狀態轉換為視覺化的文字面板。
 * 渲染流程：
 * 1. 歷史日誌渲染：從 Utils::Logger::history 中提取最近的 X 條紀錄，讓玩家知道剛剛發生了什麼。
 * 2. 狀態面板渲染：
 *    - 使用 std::setw 和 std::left/right 確保面板對齊，形成表格感。
 *    - 呼叫 Utils::generateHPBar 生成動態顏色血條，提供直觀的生命值百分比視覺化。
 *    - 使用 ANSI 顏色（COLOR_BLUE）來區分界面邊界與內容。
 */
void GameManager::showHTMLUpdate(Role&player,Role&monster){
    // 1. 先打印最近的日誌紀錄
    // 目的：提供戰鬥上下文，避免清屏後玩家不知道之前的攻擊結果
    if (!Utils::Logger::history.empty()) {
        cout << "--- 近期紀錄 ---" << endl;
        for (const auto& log : Utils::Logger::history) {
            cout << log << endl;
        }
        cout << "------------------" << endl;
    }

    // 2. 接著打印狀態面板
    int width = 60;
    cout << "\n" << Utils::COLOR_BLUE;
    Utils::printLine('-', width);
    // 顯示當前關卡，使用 left-align 確保格式統一
    cout << "| " << left << setw(10) << "關卡 " + to_string(stage) 
         << " | " << right << setw(45) << "戰鬥進行中..." << " |" << endl;
    Utils::printLine('-', width);
    
    // 玩家狀態面板 (包含動態血條)
    // 格式：| 角色類型 : 名稱 [######---] HP/MaxHP |
    cout << "| " << left << setw(12) << "玩家" << ": " 
         << left << setw(12) << player.getName() 
         << " " << Utils::generateHPBar(player.getHp(), player.getMaxHp(), 15) 
         << " " << right << setw(3) << player.getHp() << "/" << setw(3) << player.getMaxHp() 
         << " |" << endl;
         
    // 怪物狀態面板 (包含動態血條)
    cout << "| " << left << setw(12) << "怪物" << ": " 
         << left << setw(12) << monster.getName() 
         << " " << Utils::generateHPBar(monster.getHp(), monster.getMaxHp(), 15) 
         << " " << right << setw(3) << monster.getHp() << "/" << setw(3) << monster.getMaxHp() 
         << " |" << endl;
         
    Utils::printLine('-', width);
    cout << Utils::COLOR_RESET << endl;
}

/**
 * @brief 戰鬥主迴圈 (核心邏輯)
 * 
 * 實作一個嚴格的「同步回合制」戰鬥系統：
 * 
 * 流程分析：
 * [開始] -> 清屏 -> 進入迴圈
 *   |
 *   V
 * [玩家回合] 
 *   1. 渲染 UI (showHTMLUpdate)
 *   2. 獲取玩家輸入 (getSafeInt) -> 確保輸入為 1 或 2
 *   3. 執行行動 (attack 或 useSkill)
 *   4. 檢查目標死亡 -> 若死亡則跳出迴圈 (玩家獲勝)
 *   |
 *   V
 * [怪物回合]
 *   1. 隨機選擇技能 (getRandomSkill)
 *   2. 執行攻擊 -> 作用於玩家
 *   3. 暫停 (sleep_for) -> 模擬思考時間，防止文字瞬間刷屏導致玩家看不清
 *   |
 *   V
 * [檢查狀態] -> 若玩家死亡則跳出迴圈 (玩家失敗) -> 返回結果
 * 
 * @param player 玩家角色
 * @param monster 怪物角色
 * @return 玩家生存則回傳 true (勝利)，否則回傳 false (失敗)
 */
bool GameManager::battle(Role&player,Role& monster){
    Utils::clearScreen();
    Utils::printBoxHeader("戰鬥開始", '=');
    cout << Utils::COLOR_BOLD << player.getName() << " VS " << monster.getName() << Utils::COLOR_RESET << endl;
    Utils::printLine(' ', 40);

    while(player.isAlive()&&monster.isAlive()){
        Utils::clearScreen();
        showHTMLUpdate(player,monster);
        
        // --- 玩家行動階段 ---
        int action;
        while (true) {
            action = Utils::getSafeInt("\n選擇行動: (1)普通攻擊 (2)使用技能 > ");
            if (action == 1 || action == 2) break;
            cout << Utils::COLOR_RED << "無效選擇，請輸入 1 或 2" << Utils::COLOR_RESET << endl;
        }

        if(action==1){
            player.attack(monster);
        }
        else if(action==2){
            player.printSkills();
            int sid;
            int skillCount = player.getSkillCount();
            // 動態生成提示文字，適應不同職業的技能數量
            while (true) {
                sid = Utils::getSafeInt("選擇技能 (1-" + to_string(skillCount) + ") > ");
                if (sid >= 1 && sid <= skillCount) break;
                cout << Utils::COLOR_RED << "無效技能 ID，請輸入 1-" << skillCount << Utils::COLOR_RESET << endl;
            }
            player.useSkill(monster,sid);
        }

        // 關鍵檢查點：如果怪物在玩家回合被擊殺，立即結束戰鬥，不進入怪物回合
        if(!monster.isAlive()){
            break;
        }

        // --- 怪物行動階段 ---
        Utils::Logger::addLog("\n" + Utils::COLOR_YELLOW + "[怪物回合]" + Utils::COLOR_RESET);
        // 隨機 AI 邏輯：簡單的隨機選擇
        int mskill = monster.getRandomSkill();
        Utils::Logger::addLog(monster.getName() + " 使用了 " + Utils::COLOR_YELLOW + monster.getSkillName(mskill) + Utils::COLOR_RESET);
        monster.useSkill(player, mskill);
        
        // UX 優化：讓玩家能閱讀剛才發生的事件
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    Utils::printLine(' ', 40);
    Utils::printBoxHeader("戰鬥結束", '=');
    if(player.isAlive()){
        Utils::Logger::addLog(Utils::COLOR_GREEN + Utils::COLOR_BOLD + "[勝利] " + player.getName() + " 擊敗了 " + monster.getName() + "!" + Utils::COLOR_RESET);
        cout << Utils::COLOR_GREEN << Utils::COLOR_BOLD << "[勝利] " << player.getName() << " 擊敗了 " + monster.getName() + "!" << Utils::COLOR_RESET << endl;
        return true;
    }
    else{
        Utils::Logger::addLog(Utils::COLOR_RED + Utils::COLOR_BOLD + "[失敗] " + player.getName() + " 被 " + monster.getName() + " 擊敗了!" + Utils::COLOR_RESET);
        cout << Utils::COLOR_RED << Utils::COLOR_BOLD << "[失敗] " + player.getName() + " 被 " + monster.getName() + " 擊敗了!" << Utils::COLOR_RESET << endl;
        return false;
    }
}
