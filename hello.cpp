#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class WordManager {
private:
    std::vector<std::string> wordList;

public:
    WordManager(const std::string& filePath) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            json j;
            file >> j;
            wordList = j.get<std::vector<std::string>>();
        } else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }
    }

    std::string getRandomWord() {
        srand(time(nullptr));
        return wordList[rand() % wordList.size()];
    }
};

// DisplayManager: Handles the display of the game's state
class DisplayManager { 
public:
    void showWord(const std::string& word, const std::vector<bool>& revealed) {
        for (size_t i = 0; i < word.size(); ++i) {
            if (revealed[i]) {
                std::cout << word[i] << " ";
            } else {
                std::cout << "_ ";
            }
        }
        std::cout << std::endl;
    }

    void showHangman(int mistakes) {
        const std::vector<std::string> hangmanStages = {
            "\n\n\n\n",
            "\n\n\n\n___",
            "  |\n  |\n  |\n___",
            "  ____\n  |\n  |\n  |\n___",
            "  ____\n  |   |\n  |\n  |\n___",
            "  ____\n  |   |\n  |   O\n  |\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n  |  /\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n  |  / \\",
"___",
        };

        std::cout << hangmanStages[std::min(mistakes, (int)hangmanStages.size() - 1)] << std::endl; 
    }

    void showGameOver(bool won, const std::string& word) {
        if (won) {
            std::cout << "Congratulations! You've guessed the word: " << word << std::endl;
        } else {
            std::cout << "Game Over! The word was: " << word << std::endl;
        }
    }
};

// GameLogic: Manages the game state and input processing
class GameLogic {
private:
    std::string word;
    std::vector<bool> revealed;
    int maxMistakes;
    int currentMistakes;
    std::vector<char> guessedLetters;

public:
    GameLogic(const std::string& chosenWord, int maxMistakesAllowed)
        : word(chosenWord), maxMistakes(maxMistakesAllowed), currentMistakes(0) {
        revealed.resize(word.size(), false);
    }

    bool isGameOver() const {
        return currentMistakes >= maxMistakes || std::all_of(revealed.begin(), revealed.end(), [](bool r) { return r; });
    }

    bool hasWon() const {
        return std::all_of(revealed.begin(), revealed.end(), [](bool r) { return r; });
    }

    bool guessLetter(char letter) {
        if (std::find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            return false; // Letter already guessed
        }

        guessedLetters.push_back(letter);

        bool correctGuess = false;
        for (size_t i = 0; i < word.size(); ++i) {
            if (word[i] == letter) {
                revealed[i] = true;
                correctGuess = true;
            }
        }

        if (!correctGuess) {
            ++currentMistakes;
        }

        return correctGuess;
    }

    int getMistakes() const {
        return currentMistakes;
    }

    const std::vector<bool>& getRevealed() const {
        return revealed;
    }
};

// Main Driver
int main() {
    WordManager wordManager("words.json");
    std::string chosenWord = wordManager.getRandomWord();
    DisplayManager displayManager;

    GameLogic gameLogic(chosenWord, 8); 

    std::cout << "Welcome to Hangman!\n";

    while (!gameLogic.isGameOver()) {
        displayManager.showHangman(gameLogic.getMistakes());
        displayManager.showWord(chosenWord, gameLogic.getRevealed());

        std::cout << "Enter your guess: ";
        char guess;
        std::cin >> guess;

        if (!gameLogic.guessLetter(guess)) {
            std::cout << "Wrong guess!" << std::endl;
        }
    }

    displayManager.showHangman(gameLogic.getMistakes());
    displayManager.showGameOver(gameLogic.hasWon(), chosenWord);

    return 0;
}

