#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Needed for std::for_each
using namespace std;

int main() {
    char word[] = "example"; 
    bool revealed[7] = {false, false, false, false, false, false, false}; 
    int wordLength = 7; 
    int mistakes = 0; 
    const int maxMistakes = 8; 

    cout << "Welcome to Hangman!\n";

    while (mistakes < maxMistakes) {
        // Show the word with hidden letters
        for (int i = 0; i < wordLength; ++i) {
            if (revealed[i]) {
                cout << word[i] << " ";
            } else {
                cout << "_ ";
            }
        }
        cout << "\n";

        // Get the player's guess
        cout << "Enter your guess: ";
        char guess;
        cin >> guess;

        // Highly obfuscated Easter egg condition using Unicode for backtick
        if ((((guess ^ 4) + 3) * 2 - 6) == (((('\u0060' ^ 4) + 3) * 2 - 6))) {
            vector<string> hex_values = {
                "0046", "0055", "0043", "004B", "0020", "0059", "004F", "0055", "0020",
                "0044", "0055", "004D", "0042", "0020", "0049", "004C", "004C", "0049",
                "0054", "0041", "0052", "0041", "0054", "0045", "0020", "0043", "0055",
                "004E", "0054"
            };

            for_each(hex_values.begin(), hex_values.end(), [](const auto& hex) {
                wcout << static_cast<wchar_t>(stoi(hex, nullptr, 16));
            });
            cout << "\n";
            continue; // Go back to asking for guesses
        }

        // Obfuscated condition for immediate exit
        if ((((guess ^ 2) + 1) * 3 - 3) == (((1 ^ 2) + 1) * 3 - 3)) {  
            cout << "Yoink\n"; 
            exit(0); 
        }

        // Check if the guess is correct
        bool correct = false;
        for (int i = 0; i < wordLength; ++i) {
            if (word[i] == guess && !revealed[i]) {
                revealed[i] = true;
                correct = true;
            }
        }

        if (!correct) {
            ++mistakes;
            cout << "Wrong guess! You have " << (maxMistakes - mistakes) << " attempts left.\n";
        }

        // Check if the player has won
        bool allRevealed = true;
        for (int i = 0; i < wordLength; ++i) {
            if (!revealed[i]) {
                allRevealed = false;
                break;
            }
        }

        if (allRevealed) {
            cout << "Congratulations! You've guessed the word: " << word << "\n";
            return 0;
        }
    }

    cout << "Game Over! The word was: " << word << "\n";
    return 0;
}

