const ROLES = {
    'Warrior': {
        name: '戰士', icon: '⚔️', hp: 120, atk: 15,
        skills: {
            1: { name: '斬擊', damage: 25 },
            2: { name: '盾擊', damage: 18 },
            3: { name: '狂戰', damage: 35 }
        }
    },
    'Mage': {
        name: '法師', icon: '🔥', hp: 80, atk: 22,
        skills: {
            1: { name: '火球術', damage: 30 },
            2: { name: '冰凍', damage: 20 },
            3: { name: '流星火雨', damage: 40 }
        }
    },
    'Archer': {
        name: '弓箭手', icon: '🏹', hp: 100, atk: 18,
        skills: {
            1: { name: '連射', damage: 24 },
            2: { name: '穿透箭', damage: 32 },
            3: { name: '箭雨', damage: 28 }
        }
    }
};

const STAGE_CONFIG = {
    1: { name: '第一關 - 新手村', smallMonsters: 2, smallMonsterHp: 40, smallMonsterAtk: 8, bossHp: 100, bossAtk: 12 },
    2: { name: '第二關 - 黑暗森林', smallMonsters: 3, smallMonsterHp: 60, smallMonsterAtk: 12, bossHp: 150, bossAtk: 15 },
    3: { name: '第三關 - 龍之城堡', smallMonsters: 4, smallMonsterHp: 80, smallMonsterAtk: 15, bossHp: 200, bossAtk: 18 }
};

let gameState = {
    player: null, monster: null, currentStage: 1, currentMonsterIndex: 0,
    totalMonstersInStage: 0, inBattle: false, actionInProgress: false, 
    bossFightActive: false, defeatedInStage: 0
};

const cityInput = document.getElementById('cityInput');
const searchBtn = document.getElementById('searchBtn');
const locationBtn = document.getElementById('locationBtn');
const suggestions = document.getElementById('suggestions');
const errorAlert = document.getElementById('errorAlert');
const loadingSpinner = document.getElementById('loadingSpinner');
const weatherMain = document.getElementById('weatherMain');
const initialScreen = document.getElementById('initialScreen');

searchBtn.addEventListener('click', () => searchCity(cityInput.value));
locationBtn.addEventListener('click', getUserLocation);
cityInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') searchCity(cityInput.value);
});
cityInput.addEventListener('input', showSuggestions);
document.addEventListener('click', (e) => {
    if (!e.target.closest('.search-section')) {
        suggestions.classList.remove('active');
    }
});

function showScreen(id) {
    document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));
    document.getElementById(id).classList.add('active');
}

function addLog(msg) {
    const log = document.getElementById('logContent');
    const p = document.createElement('p');
    p.className = 'log-item';
    p.textContent = msg;
    log.appendChild(p);
    log.scrollTop = log.scrollHeight;
}

function createDamageNumber(damage) {
    const div = document.createElement('div');
    div.className = 'damage-number damage';
    div.textContent = '-' + damage;
    div.style.left = (Math.random() * 400 + 300) + 'px';
    div.style.top = (Math.random() * 300 + 400) + 'px';
    document.getElementById('damageNumbers').appendChild(div);
    setTimeout(() => div.remove(), 1000);
}

function updateUI() {
    const hpPercent = (gameState.player.hp / gameState.player.maxHp) * 100;
    const bar = document.getElementById('playerHpBar');
    bar.style.width = hpPercent + '%';
    if (hpPercent < 30) bar.classList.add('low');
    else bar.classList.remove('low');
    document.getElementById('playerHpText').textContent = gameState.player.hp + '/' + gameState.player.maxHp;
    
    const mhpPercent = (gameState.monster.hp / gameState.monster.maxHp) * 100;
    const mbar = document.getElementById('monsterHpBar');
    mbar.style.width = mhpPercent + '%';
    document.getElementById('monsterHpText').textContent = gameState.monster.hp + '/' + gameState.monster.maxHp;
}

function calculateDamage(atk, isSkill = false, skillId = 1) {
    let dmg = isSkill ? atk.skills[skillId].damage : atk.atk;
    const variance = 0.8 + Math.random() * 0.4;
    return Math.floor(dmg * variance);
}

function selectRole(role, el) {
    const cfg = ROLES[role];
    gameState.player = { ...cfg, maxHp: cfg.hp, level: 1, xp: 0, xpToNext: 100 };
    document.querySelectorAll('.card').forEach(c => c.style.opacity = '0.2');
    el.style.opacity = '1';
    setTimeout(() => startNewGame(), 800);
}

function startNewGame() {
    gameState.currentStage = 1;
    gameState.currentMonsterIndex = 0;
    gameState.defeatedInStage = 0;
    document.getElementById('playerNameBattle').textContent = gameState.player.name;
    document.getElementById('playerIcon').textContent = gameState.player.icon;
    startStage();
}

function startStage() {
    const cfg = STAGE_CONFIG[gameState.currentStage];
    gameState.totalMonstersInStage = cfg.smallMonsters;
    gameState.currentMonsterIndex = 0;
    gameState.bossFightActive = false;
    gameState.defeatedInStage = 0;
    
    document.getElementById('stageName').textContent = cfg.name;
    document.getElementById('logContent').innerHTML = '';
    addLog('🗡️ ' + cfg.name + ' 開始！');
    addLog('本關需要擊敗 ' + cfg.smallMonsters + ' 隻小怪和 1 個 Boss！');
    
    startNextBattle();
}

function startNextBattle() {
    gameState.actionInProgress = false;
    const cfg = STAGE_CONFIG[gameState.currentStage];
    gameState.currentMonsterIndex++;
    const isBoss = (gameState.currentMonsterIndex > cfg.smallMonsters);
    gameState.bossFightActive = isBoss;
    
    if (isBoss) {
        gameState.monster = {
            name: '【第' + gameState.currentStage + '關Boss】',
            icon: '🔥',
            hp: cfg.bossHp,
            maxHp: cfg.bossHp,
            atk: cfg.bossAtk,
            isBoss: true,
            skills: { 1: { name: '力量一擊', damage: 25 }, 2: { name: '地獄猛擊', damage: 35 }, 3: { name: '毀滅之力', damage: 45 } }
        };
        document.getElementById('stageProgress').textContent = '⚡ Boss戰 - 最終決戰';
        addLog('⚠️ Boss 出現！');
    } else {
        gameState.monster = {
            name: '小怪 ' + gameState.currentMonsterIndex + '/' + cfg.smallMonsters,
            icon: '👹',
            hp: cfg.smallMonsterHp + gameState.currentStage * 10,
            maxHp: cfg.smallMonsterHp + gameState.currentStage * 10,
            atk: cfg.smallMonsterAtk + gameState.currentStage * 2,
            isBoss: false,
            skills: { 1: { name: '普通攻擊', damage: 15 }, 2: { name: '重擊', damage: 25 }, 3: { name: '狂暴', damage: 35 } }
        };
        document.getElementById('stageProgress').textContent = '小怪 ' + gameState.currentMonsterIndex + '/' + cfg.smallMonsters;
        addLog('小怪出現了！第 ' + gameState.currentMonsterIndex + ' 個小怪！');
    }
    
    gameState.inBattle = true;
    document.getElementById('monsterName').textContent = gameState.monster.name;
    document.getElementById('monsterIcon').textContent = gameState.monster.icon;
    document.getElementById('attackBtn').disabled = false;
    document.getElementById('skillBtn').disabled = false;
    document.getElementById('skillMenu').style.display = 'none';
    updateUI();
    showScreen('battleScreen');
}

function playerAttack() {
    if (gameState.actionInProgress) return;
    gameState.actionInProgress = true;
    
    const damage = calculateDamage(gameState.player);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - damage);
    
    addLog(gameState.player.name + ' 攻擊，造成 ' + damage + ' 點傷害！');
    createDamageNumber(damage);
    updateUI();
    
    setTimeout(() => {
        if (gameState.monster.hp <= 0) battleEnd(true);
        else monsterTurn();
    }, 800);
}

function showSkillMenu() {
    const menu = document.getElementById('skillMenu');
    menu.style.display = menu.style.display === 'none' ? 'block' : 'none';
}

function hideSkillMenu() {
    document.getElementById('skillMenu').style.display = 'none';
}

function playerUseSkill(id) {
    if (gameState.actionInProgress) return;
    gameState.actionInProgress = true;
    hideSkillMenu();
    
    const skill = gameState.player.skills[id];
    const damage = calculateDamage(gameState.player, true, id);
    gameState.monster.hp = Math.max(0, gameState.monster.hp - damage);
    
    addLog(gameState.player.name + ' 使用【' + skill.name + '】，造成 ' + damage + ' 點傷害！');
    createDamageNumber(damage);
    updateUI();
    
    setTimeout(() => {
        if (gameState.monster.hp <= 0) battleEnd(true);
        else monsterTurn();
    }, 800);
}

function monsterTurn() {
    addLog(gameState.monster.name + ' 發動攻擊...');
    setTimeout(() => {
        const skillId = Math.floor(Math.random() * 3) + 1;
        const skill = gameState.monster.skills[skillId];
        const damage = calculateDamage(gameState.monster, true, skillId);
        gameState.player.hp = Math.max(0, gameState.player.hp - damage);
        
        addLog(gameState.monster.name + ' 使用【' + skill.name + '】，造成 ' + damage + ' 點傷害！');
        createDamageNumber(damage);
        updateUI();
        
        setTimeout(() => {
            if (gameState.player.hp <= 0) gameOver();
            else {
                gameState.actionInProgress = false;
                document.getElementById('attackBtn').disabled = false;
                document.getElementById('skillBtn').disabled = false;
            }
        }, 800);
    }, 500);
}

function battleEnd(won) {
    gameState.inBattle = false;
    if (!won) { gameOver(); return; }
    
    gameState.defeatedInStage++;
    const reward = 30 + gameState.currentStage * 10;
    gameState.player.xp += reward;
    
    let msg = '<p>你擊敗了 ' + gameState.monster.name + '！</p><p>獲得 ' + reward + ' XP</p>';
    
    if (gameState.player.xp >= gameState.player.xpToNext) {
        gameState.player.level++;
        gameState.player.xpToNext = Math.floor(gameState.player.xpToNext * 1.2);
        gameState.player.maxHp += 10;
        gameState.player.hp = Math.min(gameState.player.hp + 20, gameState.player.maxHp);
        gameState.player.atk += 5;
        msg += '<p>⭐ 升級到 Lv.' + gameState.player.level + '！</p>';
    }
    
    if (gameState.monster.isBoss) {
        const bonus = 5 + gameState.currentStage * 2;
        gameState.player.atk += bonus;
        msg += '<p>🎁 Boss掉落：攻擊力 +' + bonus + '</p>';
    }
    
    document.getElementById('resultDetails').innerHTML = msg;
    
    if (gameState.monster.isBoss && gameState.currentStage >= 3) {
        document.getElementById('nextBtn').textContent = '🏆 查看通關結果';
    } else if (gameState.monster.isBoss) {
        document.getElementById('nextBtn').textContent = '進入下一關';
    } else {
        document.getElementById('nextBtn').textContent = '繼續戰鬥';
    }
    
    showScreen('resultScreen');
}

function nextBattle() {
    const cfg = STAGE_CONFIG[gameState.currentStage];
    
    // ✅ 修復：正確判斷是否需要進行下一場戰鬥
    if (gameState.bossFightActive) {
        // 剛擊敗了 Boss
        if (gameState.currentStage >= 3) {
            // 所有關都完成 - 通關！
            const stats = '<p>🏆 恭喜通關所有 3 關！</p><p>最終等級：Lv.' + gameState.player.level + '</p><p>最終攻擊力：' + gameState.player.atk + '</p><p>最終 HP：' + gameState.player.maxHp + '</p>';
            document.getElementById('finalStats').innerHTML = stats;
            showScreen('completeScreen');
            return;
        }
        // 進入下一關
        gameState.currentStage++;
        startStage();
    } else {
        // 繼續本關的戰鬥（還有小怪或 Boss）
        startNextBattle();
    }
}

function gameOver() {
    const stats = '<p>到達階段：第 ' + gameState.currentStage + ' 關</p><p>擊敗小怪數：' + gameState.defeatedInStage + '</p><p>最終等級：Lv.' + gameState.player.level + '</p><p>最終攻擊力：' + gameState.player.atk + '</p>';
    document.getElementById('gameOverStats').innerHTML = stats;
    showScreen('gameOverScreen');
}

function restartGame() {
    gameState = { player: null, monster: null, currentStage: 1, currentMonsterIndex: 0, totalMonstersInStage: 0, inBattle: false, actionInProgress: false, bossFightActive: false, defeatedInStage: 0 };
    document.querySelectorAll('.card').forEach(c => c.style.opacity = '1');
    showScreen('selectionScreen');
}

document.addEventListener('DOMContentLoaded', () => showScreen('selectionScreen'));
