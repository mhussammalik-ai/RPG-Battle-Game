#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------- PLAYER STRUCT ----------------
struct Player {
    int hp;
    int mp;
    int attack;
    int level;
    int score;
    int potions;
};

// ---------------- ENEMY STRUCT ----------------
struct Enemy {
    int hp;
    int attack;
    int type; // 1 = normal, 2 = boss
};

// ---------------- FUNCTION DECLARATIONS ----------------
void displayStatus(struct Player p, struct Enemy e);
void playerTurn(struct Player *p, struct Enemy *e);
void enemyTurn(struct Player *p, struct Enemy *e);
int checkGameOver(struct Player p);
void levelUp(struct Player *p, struct Enemy *e);
void generateEnemy(struct Player p, struct Enemy *e);

// ---------------- MAIN ----------------
int main() {
    struct Player player = {100, 50, 10, 1, 0, 3};
    struct Enemy enemy;

    srand(time(0));
    generateEnemy(player, &enemy);

    printf("===== RPG BATTLE GAME START =====\n");

    while (1) {
        displayStatus(player, enemy);

        playerTurn(&player, &enemy);
        if (enemy.hp <= 0) {
            printf("\n?? Enemy Defeated!\n");
            player.score += 10 * player.level;
            levelUp(&player, &enemy);
            continue;
        }

        enemyTurn(&player, &enemy);
        if (checkGameOver(player)) break;
    }

    printf("\n===== GAME OVER =====\n");
    printf("Final Score: %d\n", player.score);

    return 0;
}

// ---------------- DISPLAY ----------------
void displayStatus(struct Player p, struct Enemy e) {
    printf("\n=================================\n");
    printf("Player ? HP:%d | MP:%d | Level:%d | Score:%d | Potions:%d\n",
           p.hp, p.mp, p.level, p.score, p.potions);

    if (e.type == 2)
        printf("?? BOSS Enemy ? HP:%d | ATK:%d\n", e.hp, e.attack);
    else
        printf("Enemy ? HP:%d | ATK:%d\n", e.hp, e.attack);

    printf("=================================\n");
}

// ---------------- PLAYER TURN ----------------
void playerTurn(struct Player *p, struct Enemy *e) {
    int choice;

    printf("\nYour Turn:\n");
    printf("1. Attack\n");
    printf("2. Magic (10 MP)\n");
    printf("3. Use Potion (+30 HP)\n");
    printf("4. Skip Turn\n");
    printf("Choose: ");

    // Input validation
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    switch (choice) {
        case 1: {
            int damage = (rand() % 10) + p->attack;

            // Critical Hit
            if (rand() % 5 == 0) {
                damage *= 2;
                printf("?? Critical Hit! ");
            }

            e->hp -= damage;
            printf("You attacked ? Damage: %d\n", damage);
            break;
        }

        case 2: {
            if (p->mp >= 10) {
                int damage = (rand() % 20) + 15;
                p->mp -= 10;
                e->hp -= damage;
                printf("?? Magic used ? Damage: %d\n", damage);
            } else {
                printf("? Not enough MP!\n");
            }
            break;
        }

        case 3: {
            if (p->potions > 0) {
                p->hp += 30;
                if (p->hp > 100) p->hp = 100;
                p->potions--;
                printf("?? Potion used! +30 HP\n");
            } else {
                printf("? No potions left!\n");
            }
            break;
        }

        case 4:
            printf("You skipped the turn.\n");
            break;

        default:
            printf("Invalid choice!\n");
    }

    if (e->hp < 0) e->hp = 0;
}

// ---------------- ENEMY AI ----------------
void enemyTurn(struct Player *p, struct Enemy *e) {
    printf("\nEnemy Turn...\n");

    int action;

    // Smarter AI
    if (e->hp < 20)
        action = 1; // aggressive
    else
        action = rand() % 2;

    int damage;

    if (action == 0) {
        damage = rand() % 10 + e->attack;
        printf("Enemy attacks! ");
    } else {
        damage = rand() % 15 + e->attack;
        printf("Enemy uses strong attack! ");
    }

    // Boss bonus damage
    if (e->type == 2) {
        damage += 5;
        printf("(Boss Power) ");
    }

    p->hp -= damage;
    printf("Damage: %d\n", damage);

    if (p->hp < 0) p->hp = 0;
}

// ---------------- GAME OVER ----------------
int checkGameOver(struct Player p) {
    if (p.hp <= 0) {
        printf("\n?? You Lost!\n");
        return 1;
    }
    return 0;
}

// ---------------- LEVEL UP ----------------
void levelUp(struct Player *p, struct Enemy *e) {
    p->level++;
    p->hp = 100;
    p->mp = 50;
    p->attack += 3;
    p->potions++;

    printf("?? Level Up! Now Level %d\n", p->level);

    generateEnemy(*p, e);
}

// ---------------- ENEMY GENERATION ----------------
void generateEnemy(struct Player p, struct Enemy *e) {

    // Boss every 5 levels
    if (p.level % 5 == 0) {
        e->type = 2;
        e->hp = 120 + (p.level * 10);
        e->attack = 15 + (p.level * 2);
        printf("\n?? BOSS APPEARED!\n");
    } else {
        e->type = 1;
        e->hp = 50 + (p.level * 10);
        e->attack = 8 + (p.level * 2);
    }
}
