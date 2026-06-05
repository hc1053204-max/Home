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
 * @brief 顯示戰鬥界面
 * 包含兩個部分：
 * 1. 顯示最近的戰鬥日誌 (Logger::history)
 * 2. 顯示包含玩家與怪物 HP 條的狀態面板
 */
void GameManager::showHTMLUpdate(Role&player,Role&monster){
    // 1. 先打印最近的日誌紀錄
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
    cout << "| " << left << setw(10) << "關卡 " + to_string(stage) 
         << " | " << right << setw(45) << "戰鬥進行中..." << " |" << endl;
    Utils::printLine('-', width);
    
    // 玩家狀態面板 (包含 HP Bar)
    cout << "| " << left << setw(12) << "玩家" << ": " 
         << left << setw(12) << player.getName() 
         << " " << Utils::generateHPBar(player.getHp(), player.getMaxHp(), 15) 
         << " " << right << setw(3) << player.getHp() << "/" << setw(3) << player.getMaxHp() 
         << " |" << endl;
         
    // 怪物狀態面板 (包含 HP Bar)
    cout << "| " << left << setw(12) << "怪物" << ": " 
         << left << setw(12) << monster.getName() 
         << " " << Utils::generateHPBar(monster.getHp(), monster.getMaxHp(), 15) 
         << " " << right << setw(3) << monster.getHp() << "/" << setw(3) << monster.getMaxHp() 
         << " |" << endl;
         
    Utils::printLine('-', width);
    cout << Utils::COLOR_RESET << endl;
}

/**
 * @brief 戰鬥主迴圈
 * 實作回合制戰鬥：玩家選擇行動 -> 檢查怪物死亡 -> 怪物隨機行動 -> 檢查玩家死亡。
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
        
        // 玩家行動選擇
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
                while (true) {
                    sid = Utils::getSafeInt("選擇技能 (1-" + to_string(skillCount) + ") > ");
                    if (sid >= 1 && sid <= skillCount) break;
                    cout << Utils::COLOR_RED << "無效技能 ID，請輸入 1-" << skillCount << Utils::COLOR_RESET << endl;
                }
            player.useSkill(monster,sid);
        }

        // 檢查怪物是否在玩家攻擊後死亡
        if(!monster.isAlive()){
            break;
        }

        // 怪物回合
        Utils::Logger::addLog("\n" + Utils::COLOR_YELLOW + "[怪物回合]" + Utils::COLOR_RESET);
        int mSkillCount = monster.getSkillCount();
        int mskill = (rand() % mSkillCount) + 1;
        Utils::Logger::addLog(monster.getName() + " 使用了 " + Utils::COLOR_YELLOW + monster.getSkillName(mskill) + Utils::COLOR_RESET);
        monster.useSkill(player, mskill);
        
        // 暫停一小段時間，讓玩家能看清戰鬥紀錄
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
