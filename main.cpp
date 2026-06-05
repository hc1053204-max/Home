#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "Monster.h"
#include "GameManager.h"
#include <memory>
#include <vector>
#include <ctime>
#include <iostream>
#include <limits>
#include "Utils.h"
using namespace std;
// 定義常數
const int MAX_STAGE=3;
const int BASE_MONSTER_COUNT=2;
const int BASE_MONSTER_HP=30;
const int BASE_MONSTER_ATK=8;

int main(){
    srand((unsigned)time(nullptr));
    // 選擇職業
    int choice;
    while (true) {
        choice = Utils::getSafeInt("選擇職業:(1)戰士(2)法師(3)射手:");
        if (choice >= 1 && choice <= 3) break;
        cout << "無效選擇，請重試\n";
    }
    unique_ptr<Role>hero;
    if(choice==1)hero=make_unique<Warrior>("戰士");
    else if(choice==2)hero=make_unique<Mage>("法師");
    else hero=make_unique<Archer>("射手");
    GameManager gm;
    int stage=1;
    while(hero->isAlive()&& stage<=MAX_STAGE){
        cout<<"\n=== Stage"<<stage<<"開始 ==="<<endl;
        // 生成小怪
        vector<Monster> smalls;
        int smallCount=BASE_MONSTER_COUNT+stage;
        for(int i=0;i<smallCount;i++){
            smalls.emplace_back("小怪" + to_string(i + 1),BASE_MONSTER_HP+stage*10,BASE_MONSTER_ATK+stage*2);
        }
        // 擊敗小怪
        bool stageWon=true;
        for(auto&m:smalls){
            bool won=gm.battle(*hero,m);
            if(!won){
                cout<<"失敗了！\n";
                stageWon=false;
                break;
            }
            hero->gainXp(30+stage*10);
        }
        if(!hero->isAlive())break;
        if(!stageWon)break;
        // Boss 戰
        Monster boss("StageBoss_"+to_string(stage),80+stage*60,12+stage* 8);
        bool bossWon=gm.battle(*hero,boss);
        if(bossWon){
            hero->gainXp(100+stage*50);
            int atkBonus=5+stage*2;
            cout<<"獲得掉落：攻擊力飾品(+"<<atkBonus<<")"<<endl;
            hero->addAtk(atkBonus);
            stage++;
        }
        else{
            cout<<"Boss 擊敗了你！\n";
            break;
        }
    }
    if(hero->isAlive() && stage > MAX_STAGE){
        cout<<"\n恭喜，你通關所有關卡！"<<endl;
    }
    else{
        cout<<"\n遊戲結束，請再接再厲。"<<endl;
    }
    // unique_ptr 自動清理，無需 delete
}