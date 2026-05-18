let playerHP = 100;
let monsterHP = 100;

function selectRole(role) {
    alert("你選擇了 " + role);
    document.getElementById("battle").style.display = "block";
}

function attack() {
    let playerDamage = Math.floor(Math.random() * 20);
    let monsterDamage = Math.floor(Math.random() * 15);

    monsterHP -= playerDamage;
    playerHP -= monsterDamage;

    document.getElementById("player").innerText = "玩家 HP: " + playerHP;
    document.getElementById("monster").innerText = "怪物 HP: " + monsterHP;

    if (monsterHP <= 0) {
        alert("你贏了！");
    }

    if (playerHP <= 0) {
        alert("你輸了！");
    }
}
