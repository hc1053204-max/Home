#include "GameManager.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include "Utils.h"
using namespace std;

GameManager::GameManager():stage(1){}
int GameManager::getStage()const{
    return stage;
}
void GameManager::showHTMLUpdate(Role&player,Role&monster){
    cout<<"\n----------------------------------"<<endl;
    cout<<"[Stage"<<stage<< "]In Battle..."<<endl;
    cout<< "  "<<player.getName()<<" HP:"<<player.getHp()<<"/"<<player.getMaxHp()<<endl;
    cout<<"  "<< monster.getName()<<" HP:"<< monster.getHp()<<"/"<< monster.getMaxHp()<<endl;
    cout<<"----------------------------------"<<endl;
}
bool GameManager::battle(Role&player,Role& monster){
    cout<<"\n========== Battle Start =========="<<endl;
    cout<<player.getName()<<"vs"<< monster.getName()<<endl;

    while(player.isAlive()&&monster.isAlive()){
        showHTMLUpdate(player,monster);
        // 玩家回合
        int action;
        while (true) {
            action = Utils::getSafeInt("\nChoose action:(1)Attack(2)Use Skill>");
            if (action == 1 || action == 2) break;
            cout << "Invalid choice, enter 1 or 2" << endl;
        }
        if(action==1){
            player.attack(monster);
        }
        else if(action==2){
                player.printSkills();
                int sid;
            while (true) {
                sid = Utils::getSafeInt("Choose skill (1-3) >");
                if (sid >= 1 && sid <= 3) break;
                cout << "Invalid skill id, enter 1-3" << endl;
            }
                player.useSkill(monster,sid);
            }
        // 檢查怪物是否還活著
        if(!monster.isAlive()){
            break;
        }
        // 怪物回合
        cout<<"\n[Monster Turn]"<<endl;
        int mskill=(rand()%3)+1;
        cout<<monster.getName()<<" uses " << monster.getSkillName(mskill) << endl;
        monster.useSkill(player,mskill);
    }
    // 戰鬥結果
    cout<<"\n========== Battle End =========="<<endl;
    if(player.isAlive()){
        cout<<"[Victory]"<<player.getName()<<" defeated "<<monster.getName()<<"!"<<endl;
        return true;
    }
    else{
        cout<<"[Defeat]"<<player.getName()<<" was defeated by "<<monster.getName()<<"!"<<endl;
        return false;
    }
}