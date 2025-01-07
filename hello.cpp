// Include necessary headers for input/output, file handling, vectors, strings, random number generation, and JSON parsing
#include <iostream>     // For standard input/output operations
#include <fstream>      // For file stream operations
#include <vector>       // For using dynamic arrays (vectors)
#include <string>       // For using strings
#include <cstdlib>      // For random number generation functions
#include <ctime>        // For time functions to seed random number generator
#include <nlohmann/json.hpp>  // For JSON parsing

// Use the nlohmann::json namespace for convenience
using json = nlohmann::json;

/**
 * WordManager class handles loading words from a JSON file and providing random words.
 */
class WordManager {
private:
    std::vector<std::string> wordList;  // Vector to store the list of words

public:
    /**
     * Constructor that loads words from a specified JSON file.
     * @param filePath Path to the JSON file containing the word list.
     */
    WordManager(const std::string& filePath) {
        std::ifstream file(filePath);  // Open the file stream
        if (file.is_open()) {  // Check if the file was successfully opened
            json j;  // Create a JSON object
            file >> j;  // Parse the JSON file into the JSON object
            wordList = j.get<std::vector<std::string>>();  // Convert JSON array to vector of strings
        } else {
            std::cerr << "Unable to open file: " << filePath << std::endl;  // Print error message if file cannot be opened
        }
    }

    /**
     * Returns a random word from the loaded word list.
     * @return A randomly selected word from the word list.
     */
    std::string getRandomWord() {
        srand(time(nullptr));  // Seed the random number generator with the current time
        return wordList[rand() % wordList.size()];  // Return a random word from the list
    }
};

/**
 * DisplayManager class handles displaying the game's state to the player.
 */
class DisplayManager { 
public:
    /**
     * Displays the current state of the word with revealed letters.
     * @param word The complete word being guessed.
     * @param revealed A vector indicating which letters have been revealed.
     */
    void showWord(const std::string& word, const std::vector<bool>& revealed) {
        for (size_t i = 0; i < word.size(); ++i) {  // Loop through each letter in the word
            if (revealed[i]) {  // If the letter is revealed
                std::cout << word[i] << " ";  // Print the letter
            } else {
                std::cout << "_ ";  // Print an underscore for hidden letters
            }
        }
        std::cout << std::endl;  // Print a newline at the end
    }

    /**
     * Displays the hangman ASCII art based on the number of mistakes.
     * @param mistakes The current number of wrong guesses.
     */
    void showHangman(int mistakes) {
        // Define the different stages of the hangman drawing
        const std::vector<std::string> hangmanStages = {
            "\n\n\n\n",
            "\n\n\n\n___",
            "  |\n  |\n  |\n___",
            "  ____\n  |\n  |\n  |\n___",
            "  ____\n  |   |\n  |\n  |\n___",
            "  ____\n  |   |\n  |   O\n  |\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n  |  /\n___",
            "  ____\n  |   |\n  |   O\n  |  /|\\\n  |  / \\\n___",
        };

        // Print the appropriate hangman stage based on the number of mistakes
        std::cout << hangmanStages[std::min(mistakes, (int)hangmanStages.size() - 1)] << std::endl; 
    }

    /**
     * Displays the game over message.
     * @param won Indicates whether the player won or lost.
     * @param word The complete word that was being guessed.
     */
    void showGameOver(bool won, const std::string& word) {
        if (won) {
            std::cout << "Congratulations! You've guessed the word: " << word << std::endl;  // Print win message
        } else {
            std::cout << "Game Over! The word was: " << word << std::endl;  // Print lose message
        }
    }
};

/**
 * GameLogic class manages the game state and input processing.
 */
class GameLogic {
private:
    std::string word;  // The word to be guessed
    std::vector<bool> revealed;  // Vector indicating which letters have been revealed
    int maxMistakes;  // Maximum allowed mistakes
    int currentMistakes;  // Current number of mistakes
    std::vector<char> guessedLetters;  // List of guessed letters

public:
    /**
     * Constructor to initialize the game logic.
     * @param chosenWord The word to be guessed.
     * @param maxMistakesAllowed The maximum number of allowed mistakes.
     */
    GameLogic(const std::string& chosenWord, int maxMistakesAllowed)
        : word(chosenWord), maxMistakes(maxMistakesAllowed), currentMistakes(0) {
        revealed.resize(word.size(), false);  // Initialize the revealed vector with false values
    }

    /**
     * Checks if the game is over.
     * @return True if the game is over, otherwise false.
     */
    bool isGameOver() const {
        return currentMistakes >= maxMistakes || std::all_of(revealed.begin(), revealed.end(), [](bool r) { return r; });
    }

    /**
     * Checks if the player has won.
     * @return True if the player has won, otherwise false.
     */
    bool hasWon() const {
        return std::all_of(revealed.begin(), revealed.end(), [](bool r) { return r; });
    }

    /**
     * Processes a guessed letter.
     * @param letter The guessed letter.
     * @return True if the guess is correct, otherwise false.
     */
    bool guessLetter(char letter) {
        if (std::find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            return false;  // Return false if the letter has already been guessed
        }

        guessedLetters.push_back(letter);  // Add the guessed letter to the list

        bool correctGuess = false;  // Flag to track if the guess is correct
        for (size_t i = 0; i < word.size(); ++i) {  // Loop through each letter in the word
            if (word[i] == letter) {  // If the guessed letter matches a letter in the word
                revealed[i] = true;  // Reveal the letter
                correctGuess = true;  // Set the correct guess flag to true
            }
        }

        if (!correctGuess) {  // If the guess is incorrect
            ++currentMistakes;  // Increment the mistake counter
        }

        return correctGuess;  // Return whether the guess was correct
    }

    /**
     * Gets the current number of mistakes.
     * @return The current number of mistakes.
     */
    int getMistakes() const {
        return currentMistakes;
    }

    /**
     * Gets the vector indicating which letters have been revealed.
     * @return The vector of revealed letters.
     */
    const std::vector<bool>& getRevealed() const {
        return revealed;
    }
};

/**
 * Main function: Entry point of the program.
 */
int main() {
    WordManager wordManager("words.json");  // Create a WordManager object to load words from the JSON file
    std::string chosenWord = wordManager.getRandomWord();  // Get a random word from the WordManager
    DisplayManager displayManager;  // Create a DisplayManager object to handle display operations

    GameLogic gameLogic(chosenWord, 8);  // Create a GameLogic object with the chosen word and a maximum of 8 mistakes

    std::cout << "Welcome to Hangman!\n";  // Print welcome message

    while (!gameLogic.isGameOver()) {  // Main game loop: Continue until the game is over
        displayManager.showHangman(gameLogic.getMistakes());  // Display the current hangman state
        displayManager.showWord(chosenWord, gameLogic.getRevealed());  // Display the current state of the word

        std::cout << "Enter your guess: ";  // Prompt the player for a guess
        char guess;
        std::cin >> guess;  // Read the player's guess

        if (!gameLogic.guessLetter(guess)) {  // Process the guessed letter and check if it was incorrect
            std::cout << "Wrong guess!" << std::endl;  // Print a message if the guess was incorrect
        }
    }

    displayManager.showHangman(gameLogic.getMistakes());  // Display the final hangman state
    displayManager.showGameOver(gameLogic.hasWon(), chosenWord);  // Display the game over message

    return 0;  // Return 0 to indicate successful execution
}