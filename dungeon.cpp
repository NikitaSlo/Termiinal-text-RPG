#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <algorithm> 
#include <random>   
#include <chrono>  

int calculateRandomValue(int value, int offsetRange) {
    int offset = (std::rand() % (2 * offsetRange + 1)) - offsetRange;

    int result = value + offset;

    return result;
}

class Character {

public:
    std::string name;
    int health;
    int maxHealth;
    int mana;
    int maxMana;    
    int speed;

    int attackValue;
    int healingValue;

    bool stunned = false;
    bool poisoned = false;
    int poisonDuration = 0;

    int accuracy = 10;

    static constexpr int POWER_ATTACK_COST = 10;
    static constexpr int HEAL_COST = 15;
    static constexpr int STUN_COST = 20;
    static constexpr int DODGE_COST = 5;
    static constexpr int POISON_DAMAGE = 5;
    static constexpr int POISON_DURATION = 3;

    Character(std::string n, int h, int m, int s, int av, int hv) 
        : name(n), health(h), maxHealth(h), mana(m), maxMana(m), speed(s), attackValue(av), healingValue(hv) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        std::cout << name << " took " << damage << " damage!" << std::endl;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " healed for " << amount << " health!" << std::endl;
    }

    void applyPoison(int duration) {
        poisoned = true;
        poisonDuration = duration;
    }

    void recoverMana(int amount) {
        mana += amount;
        if (mana > maxMana) mana = maxMana;
        std::cout << name << " recovered " << amount << " mana!" << std::endl;
    }

    void showStatus() const {
        std::cout << name << " | Health: " << health << "/" << maxHealth
                  << " | Mana: " << mana << "/" << maxMana;
        if (poisoned) std::cout << " | Poisoned(" << poisonDuration << " turns)";
        if (stunned) std::cout << " | Stunned";
        std::cout << std::endl;
    }

    void takePoisonDamage() {
        if (poisoned) {
            int poisonDamage = 5;
            health -= poisonDamage;
            if (health < 0) health = 0;
            std::cout << name << " took " << poisonDamage << " poison damage!" << std::endl;
            poisonDuration--;
            if (poisonDuration <= 0) {
                poisoned = false;
                std::cout << name << " is no longer poisoned." << std::endl;
            }
        }
    }

    bool attemptEscape(int playerSpeed, int monsterSpeed) {
    double escapeProbability = static_cast<double>(playerSpeed) / (playerSpeed + monsterSpeed);
    double escapeChance = static_cast<double>(std::rand()) / RAND_MAX;
    return escapeChance <= escapeProbability;
}
};

namespace DisplayUI {
        void displayStartingScreen() {
            std::cout << R"(
            ⠀⠀⠀⠀⠀⠀⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢄⣀⠀⢀⣎⣧⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠘⣮⡿⡿⣍⠉⠛⢯⡽⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⢀⣀⣤⡏⢀⡛⠋⠸⣏⡟⣷⠤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠈⠉⠒⣷⠯⠽⢠⠴⡎⢀⡟⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢠⣏⡷⢦⣤⣵⠷⣯⡻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠁⠀⠀⢧⠇⠀⠈⠻⣮⡹⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠈⠻⣮⡻⣦⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⡹⣦⡀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣮⡻⣦⡀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⡻⣦⣀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⡏⠈⡷
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠋⠁      _                                    
    | |                                   
  __| |_   _ _ __   __ _  ___  ___  _ __  
 / _` | | | | '_ \ / _` |/ _ \/ _ \| '_ \ 
| (_| | |_| | | | | (_| |  __/ (_) | | | |
 \__,_|\__,_|_| |_|\__, |\___|\___/|_| |_|
                    __/ |                 
                   |___/                  
)" << "\n\n";
        }

        void displayDeathScreen() {
            std::cout << R"(                       ,--.
                           {    }
                           K,   }
                          /  ~Y`
                     ,   /   /
                    {_'-K.__/
                      `/-.__L._
                      /  ' /`\_}
                     /  ' /
             ____   /  ' /
      ,-'~~~~    ~~/  ' /_
    ,'             ``~~~  ',
   (                        Y
  {                         I
 {      -                    `,
 |       ',                   )
 |        |   ,..__      __. Y
 |    .,_./  Y ' / ^Y   J   )|
 \           |' /   |   |   ||
  \          L_/    . _ (_,.'(
   \,   ,      ^^""' / |      )
     \_  \          /,L]     /
       '-_~-,       ` `   ./`
          `'{_            )
              ^^\..___,.--`
)" << "\n" << "\n" << "     YOU DIED        ";
        }

        void displayCrossroad() {
            std::cout << R"( 
   _________________________________________________________
 /|     -_-                                             _-  |\
/ |_-_- _                                         -_- _-   -| \   
  |                            _-  _--                      | 
  |                            ,                            |
  |      .-'````````'.        '(`        .-'```````'-.      |
  |    .` |           `.      `)'      .` |           `.    |          
  |   /   |   ()        \      U      /   |    ()       \   |
  |  |    |    ;         | o   T   o |    |    ;         |  |
  |  |    |     ;        |  .  |  .  |    |    ;         |  |
  |  |    |     ;        |   . | .   |    |    ;         |  |
  |  |    |     ;        |    .|.    |    |    ;         |  |
  |  |    |____;_________|     |     |    |____;_________|  |  
  |  |   /  __ ;   -     |     !     |   /     `'() _ -  |  |
  |  |  / __  ()        -|        -  |  /  __--      -   |  |
  |  | /        __-- _   |   _- _ -  | /        __--_    |  |
  |__|/__________________|___________|/__________________|__|
 /                                             _ -        lc \
/   -_- _ -             _- _---                       -_-  -_ \
 )" << "\n";
        }

        void displayDivision() {
            std::cout << R"(
         />_______________________________________
[########[]_______________________________________>
         \>
            )";
        }

        void displayMenu() {
        std::cout << "\n    Your turn. Choose an action:\n";
        std::cout << "      1. Basic Attack\n";
        std::cout << "      2. Power Attack (" << Character::POWER_ATTACK_COST << " mana)\n";
        std::cout << "      3. Heal (" << Character::HEAL_COST << " mana)\n";
        std::cout << "      4. Dodge (" << Character::DODGE_COST << " mana)\n";
        std::cout << "      5. Stun (" << Character::STUN_COST << " mana)\n";
        std::cout << "      6. Run away\n";
        std::cout << "  Your choice: ";
    }

        void displayHealthBar(const Character& character) {
        int barLength = 20;
        int filledLength = (character.health * barLength) / character.maxHealth;

        std::cout << character.name << "\nHealth: [";
        for (int i = 0; i < barLength; ++i) {
            if (i < filledLength)
                std::cout << "#";
            else
                std::cout << "-";
        }
        std::cout << "] " << character.health << "/" << character.maxHealth << std::endl;

        std::cout << "Mana: [";
        filledLength = (character.mana * barLength) / character.maxMana;
        for (int i = 0; i < barLength; ++i) {
            if (i < filledLength)
                std::cout << "#";
            else
                std::cout << "-";
        }
        std::cout << "] " << character.mana << "/" << character.maxMana << std::endl;

        if (character.poisoned || character.stunned) {
            std::cout << "Status: ";
            if (character.poisoned) std::cout << "Poisoned(" << character.poisonDuration << " turns) ";
            if (character.stunned) std::cout << "Stunned";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

        void displayLog(std::vector<std::string> actionLog) {
        std::cout << "\nAction Log:\n";
        for (const std::string& action : actionLog) {
            std::cout << action << std::endl;
        }
        std::cout << "\n";
    }

};


class Warrior : public Character {
public:
    Warrior(std::string name) : Character(name, 120, 50, 6, 12, 15) {    }
};

class Wizard : public Character {
public:
    Wizard(std::string name) : Character(name, 80, 100, 3, 12, 15) {    }
};

class Slave : public Character {
public:
    Slave(std::string name) : Character(name, 50, 30, 8, 12, 15) {    }
};

std::unique_ptr<Character> createCharacter() {
    std::cout << "            --What is your name?--" << std::endl;
    std::string name;
    std::cin >> name;

    DisplayUI::displayDivision();

    std::cout << "    --Who are you?--" << std::endl;
    std::cout << "\n 1. Warrior. Goes into the dungeon to kill the Dungeon Lord.\n";
    std::cout << "\n 2. Wizard. Goes into the dungeon to find an ancient spell scroll.\n";
    std::cout << "\n 3. Dude. Just a dude (not from Postal).\n";

    DisplayUI::displayDivision();

    std::cout << "\n    Your choise: ";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            return std::make_unique<Warrior>(name);
        case 2:
            return std::make_unique<Wizard>(name);
        case 3:
            return std::make_unique<Slave>(name);
        default:
            std::cout << "Invalid choice! A Warrior has been created by default." << std::endl;
            return std::make_unique<Warrior>(name);
    }
}

class TurnActions {
protected:
    std::unique_ptr<Character> player;
    Character monster;

    bool gameRunning = true;

    TurnActions(std::unique_ptr<Character> playerCharacter, const std::string& monsterName, int monsterHealth, int monsterMana, int monsterSpeed, int monsterAttack, int monsterHealing)
        : player(std::move(playerCharacter)), monster(monsterName, monsterHealth, monsterMana, monsterSpeed, monsterAttack, monsterHealing) {}

    void clearScreen() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void dealDamage(Character& target, int damage) {
        target.takeDamage(damage);
    }

    void playerTurn() {
        clearScreen();
        DisplayUI::displayHealthBar(*player);
        DisplayUI::displayHealthBar(monster);

        DisplayUI::displayMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            dealDamage(monster, calculateRandomValue(player->attackValue, 4));
            break;
        case 2:
            if (player->mana >= Character::POWER_ATTACK_COST) {
                dealDamage(monster, calculateRandomValue(player->attackValue, 10));
                player->mana -= Character::POWER_ATTACK_COST;
            } else {
                std::cout << "Not enough mana for Power Attack!" << std::endl;
            }
            break;
        case 3:
            if (player->mana >= Character::HEAL_COST) {
                int healAmount = calculateRandomValue(player->healingValue, 5);
                player->heal(healAmount);
                player->mana -= Character::HEAL_COST;
            } else {
                std::cout << "Not enough mana to Heal!" << std::endl;
            }
            break;
        case 4:
            if (player->mana >= Character::DODGE_COST) {
                std::cout << "You prepared to dodge the next attack!" << std::endl;
                player->mana -= Character::DODGE_COST;
            } else {
                std::cout << "Not enough mana to Dodge!" << std::endl;
            }
            break;
        case 5:
            if (player->mana >= Character::STUN_COST) {
                if (std::rand() % 2 == 0) {
                    monster.stunned = true;
                    std::cout << "You stunned the monster!" << std::endl;
                } else {
                    std::cout << "Stun failed!" << std::endl;
                }
                player->mana -= Character::STUN_COST;
            } else {
                std::cout << "Not enough mana to Stun!" << std::endl;
            }
            break;
        case 6:
            if (player->attemptEscape(player->speed, monster.speed)) {
                std::cout << "You have escaped!" << std::endl;
                gameRunning = false;
            } else {
                std::cout << "You are trying to escape but" << monster.name << "blocks your way!" << std::endl;
            }
            break;
        default:
            std::cout << "Invalid choice! Try again." << std::endl;
            break;
        }
    }

    void monsterTurn() {
        if (monster.stunned) {
            std::cout << "The monster is stunned and misses its turn!" << std::endl;
            monster.stunned = false;
            return;
        }

        int action = std::rand() % 3;
        switch (action) {
        case 0:
            dealDamage(*player, calculateRandomValue(monster.attackValue, 4));
            break;
        case 1:
            if (!player->poisoned) {
                player->applyPoison(Character::POISON_DURATION);
                std::cout << monster.name << " poisons " << player->name << " for " << Character::POISON_DURATION << "turns!";
            } else {
                std::cout << monster.name << " attempts to poison " << player->name << " but he is already poisoned!";
            }
            break;
        case 2:
            monster.recoverMana(10);
            break;
        }

        bool decideToEscape = (std::rand() % 2 == 0);

        if (monster.health < 15) {

            if (decideToEscape) {
                std::cout << monster.name << " decides to escape!" << std::endl;

                std::cout << "Want to chase it? (1 - Yes, 0 - No): ";
                int chaseChoice;
                std::cin >> chaseChoice;

                if (chaseChoice == 1) {
                    if (monster.attemptEscape(player->speed, monster.speed)) {
                        std::cout << monster.name << " succesfully escaped!" << std::endl;
                        gameRunning = false;
                    } else {
                        std::cout << " You have chased " << monster.name << " down and keep fighting!" << std::endl;
                    }
                } else {
                    std::cout << monster.name << " escaped!" << std::endl;
                    gameRunning = false;
                }
                return;
            }
        }
    }
};

class Battle : public TurnActions {
private:
    int turnNumber = 0;

public:
    Battle(std::unique_ptr<Character> character)
        : TurnActions(std::move(character), "Goblin", 80, 50, 5, 30, 15) {}

    bool start() {
        while (gameRunning) {
            turnNumber++;
            std::cout << "\n(----------------------[ Turn "<< turnNumber << " ]----------------------)\n\n";


            player->takePoisonDamage();
            if (player->health == 0) {
                std::cout << player->name << " has been defeated! Game over.\n";
                DisplayUI::displayDeathScreen();
                break;
            }

            playerTurn();

            if (!gameRunning)
                break;

            if (monster.health == 0) {
                std::cout << "You defeated the monster!\n";
                return true;
            }

            monster.takePoisonDamage();
            if (monster.health == 0) {
                std::cout << "Congratulations! You defeated the monster!\n";
                return true;
            }

            monsterTurn();

            if (player->health == 0) {
                DisplayUI::displayDeathScreen();
                return false;
            }

            std::cout << "\n\n(-------------------[ End of turn ]--------------------)\n\n";
        }
        return true;
    }
};

class Exploration {
private:
    std::vector<std::string> dungeon; 
    std::vector<bool> clearedRooms;  
    int currentRoomIndex;            
    std::unique_ptr<Character> player;

public:
    Exploration(std::unique_ptr<Character> character, const std::vector<std::string>& dungeonLayout)
        : player(std::move(character)), dungeon(dungeonLayout), currentRoomIndex(0) {
        clearedRooms.resize(dungeon.size(), false);
        clearedRooms[0] = true; 
    }

    void moveForward() {
        if (currentRoomIndex == dungeon.size() - 1) {
            std::cout << "It seems like dungeon ends here.\n";
            return;
        }

        currentRoomIndex++;
        enterRoom();
    }

    void moveBackward() {
        if (currentRoomIndex == 0) {
            std::cout << "It is not time to go back home yet.\n";
            return;
        }

        currentRoomIndex--;
        enterRoom();
    }

    void moveToCrossroadChoice(char direction) {
        if (dungeon[currentRoomIndex] != "Crossroad") {
            std::cout << "You are not at a crossroad!\n";
            return;
        }

        if (direction == 'L' || direction == 'l') {
            currentRoomIndex++; 
        } else if (direction == 'R' || direction == 'r') {
            currentRoomIndex += 2; 
        } else {
            std::cout << "Invalid choice! Choose 'L' for left or 'R' for right.\n";
            return;
        }

        enterRoom();
    }

    void enterRoom() {
        std::cout << "\n    You enter the " << dungeon[currentRoomIndex] << ".\n";

        if (dungeon[currentRoomIndex] != "Entrance" && !clearedRooms[currentRoomIndex]) {
            std::cout << "  An enemy appears!\n";
            Battle battle(std::make_unique<Character>(*player)); 
            bool playerWon = battle.start();

            if (playerWon) {
                clearedRooms[currentRoomIndex] = true; 
            } else {
                std::cout << "Game over! You were defeated in the " << dungeon[currentRoomIndex] << ".\n";
                DisplayUI::displayDeathScreen();
                return;
            }
        } else {
            std::cout << "  The room is empty.\n";
        } 

        if (dungeon[currentRoomIndex] == "Lord`s Room") {
            std::cout << "You have reached the Dungeon Lord's room. Prepare for the final battle!\n";
            Battle finalBattle(std::make_unique<Character>(*player));
            bool playerWon = finalBattle.start();

            if (playerWon) {
                std::cout << "Congratulations! You have defeated the Dungeon Lord and completed the dungeon!\n";
                exit(0);
            } else {
                std::cout << "You were defeated by the Dungeon Lord. Game over!\n";
                exit(0);
            }
            
        }
    }
    std::string displayCurrentRoom() const {
        return dungeon[currentRoomIndex];
    }

    int getPlayerHealth() const {
        return player->health;
    }

    bool isAtCrossroad() const {
        return dungeon[currentRoomIndex] == "Crossroad";
    }
};


int main() {
    DisplayUI::displayStartingScreen();

    std::vector<std::string> dungeon = {"Entrance", "Hallway", "Torture Hall", "Armory", "Treasury", "Crossroad", "Prison Cells", "Lord`s Room"};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(dungeon.begin() + 1, dungeon.end() - 1, rng); // Shuffle rooms except Entrance and Lord's Room

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::unique_ptr<Character> character = createCharacter();

    Exploration exploration(std::move(character), dungeon);

    std::cout << "\n\n\n          --Your adventure begins!--\n\n\n" << std::endl;

    while (true) {
        std::cout << "\n(----------------[ You are at the "<< exploration.displayCurrentRoom() << " ]----------------)\n\n";

        if (exploration.isAtCrossroad()) {
            DisplayUI::displayCrossroad();
            std::cout << "\nYou are at a crossroad. Choose where to go:\n";
            std::cout << "1. Go left\n";
            std::cout << "2. Go right\n";
            std::cout << "Your choice: ";

            char direction;
            std::cin >> direction;

            exploration.moveToCrossroadChoice(direction);
        } else {
            std::cout << "\nChoose an action:\n";
            std::cout << "1. Move forward\n";
            std::cout << "2. Move backward\n";
            std::cout << "3. Check current room\n";
            std::cout << "4. Quit\n";
            std::cout << "Your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    exploration.moveForward();
                    break;
                case 2:
                    exploration.moveBackward();
                    break;
                case 3:
                    exploration.displayCurrentRoom();
                    break;
                case 4:
                    std::cout << "Thanks for playing! Goodbye!\n";
                    return 0;
                default:
                    std::cout << "Invalid choice! Try again.\n";
                    break;
            }
        }

        if (exploration.getPlayerHealth() <= 0) {
            DisplayUI::displayDeathScreen();
            break;
        }
    }

    return 0;
}
    