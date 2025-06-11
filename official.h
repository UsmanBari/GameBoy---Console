#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace sf;

// Constants for Hangman
const int MAX_LIVES = 6;

// Word category system

const string animalWords[] =

{
    "dog", "cat", "bird", "fish", "snake", "lion", "tiger", "bear", "wolf", "fox", "horse", "cow", "pig", "sheep", "goat", "chicken", "duck", "goose", "turkey", "deer",
    "rabbit", "mouse", "rat", "squirrel", "chipmunk", "beaver", "otter", "seal", "whale", "dolphin"
};


const int numAnimalWords = sizeof(animalWords) / sizeof(animalWords[0]);


const string fruitWords[] =

{
    "apple", "banana", "orange", "grape", "strawberry", "blueberry", "raspberry", "mango", "pineapple", "watermelon","peach", "pear", "plum", "cherry", "apricot", "kiwi", "lemon", "lime", "coconut", "avocado"
};


const int numFruitWords = sizeof(fruitWords) / sizeof(fruitWords[0]);


const string countryWords[] =

{
    "china", "india", "usa", "indonesia", "pakistan", "brazil", "nigeria", "bangladesh", "russia", "mexico","japan", "ethiopia", "philippines", "egypt", "vietnam", "drcongo", "turkey", "iran", "germany", "thailand"
};


const int numCountryWords = sizeof(countryWords) / sizeof(countryWords[0]);


const string wordList[50] =

{
    "apple", "table", "grape", "brush", "cream", "house", "heart", "bread", "dream", "light","stone", "plant", "chair", "river", "cloud", "piano", "money", "grass", "scale", "flame",
    "paper", "pearl", "glass", "space", "clock", "mouse", "crowd", "drama", "water", "flute", "white", "smile", "shape", "blood", "round", "dance", "block", "plaza", "store", "beach",
    "shade", "horse", "shine", "ocean", "fruit", "plume", "dance", "sword", "vocal", "wings"
};

// Abstract base class for games 
class Game

{

public:


    virtual void run() = 0; // game ke liye pure virtual method)
    virtual ~Game() {} // Virtual destructor 


};

// Player Profile Class 

class PlayerProfile

{

private:

    string playerName;
    int highScore;

public:

    PlayerProfile() : playerName(""), highScore(0) {}

    void setPlayerName(const string& name)



    {
        playerName = name;
    }

    string getPlayerName() const

    {
        return playerName;
    }

    void setHighScore(int score)

    {
        highScore = score;
    }

    int getHighScore() const

    {
        return highScore;
    }

    // Profile ko file se load karna 

    void loadProfile(const string& filename)

    {
        ifstream file(filename);

        if (file.is_open())

        {
            getline(file, playerName);
            file >> highScore;
            file.close();
        }


    }

    // Profile ko file mein save karna 
    void saveProfile(const string& filename) const

    {
        ofstream file(filename);


        if (file.is_open())

        {
            file << playerName << endl;
            file << highScore << endl;
            file.close();

        }
    }
};

// Save player profile

void savePlayerProfile(const PlayerProfile& profile)

{
    const string filename = "players.txt";

    ofstream file(filename, ios::app); // Open in append mode 

    if (file.is_open())

    {
        file << profile.getPlayerName() << "," << profile.getHighScore() << endl;
        file.close();
        cout << "Profile saved!" << endl;

    }


}

// Load player profile
void loadPlayerProfile(PlayerProfile& profile)

{
    const string filename = "players.txt";


    string line;
    ifstream file(filename);
    bool found = false;

    cout << "Enter your profile name: ";

    string name;
    cin >> name;


    profile.setPlayerName(name);

    if (file.is_open())

    {

        while (getline(file, line))

        {

            size_t commaPos = line.find(",");

            if (commaPos != string::npos) {


                string name = line.substr(0, commaPos);


                if (name == profile.getPlayerName()) {


                    profile.setHighScore(stoi(line.substr(commaPos + 1)));


                    found = true;


                    break;


                }

            }

        }
        file.close();
    }

    if (!found)

    {
        cout << "Welcome, " << profile.getPlayerName() << "! Your high score is: " << profile.getHighScore() << endl;

        savePlayerProfile(profile); // Save the new player to the file 
    }

    else

    {
        cout << "Welcome back, " << profile.getPlayerName() << "! Your high score is: " << profile.getHighScore() << endl;
    }
}

// Hangman game class 

class HangmanGame : public Game {
private:
    string wordToGuess; // Guess karne wala lafz
    bool guessedLetters[50] = { false }; // Guess kiye gaye hurf
    int lives; // Baqi jindariyan
    string displayedWord; // Display hone wala lafz
    int category; // 0 for animals, 1 for fruits, 2 for countries (0 janwaron ke liye, 1 phaloun ke liye, 2 mulkoun ke liye)
    PlayerProfile playerProfile; // Khiladi ki profile

    // Display hone wale lafz ko update karna (Update the displayed word)
    void updateDisplayedWord() {
        displayedWord = "";
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (guessedLetters[i]) {
                displayedWord += wordToGuess[i];
            }
            else {
                displayedWord += "_";
            }
            displayedWord += " ";
        }
    }

    // Kya lafz guess ho gaya hai? (Is the word guessed?)
    bool isWordGuessed() {
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (!guessedLetters[i])
                return false;
        }
        return true;
    }

    // Guess ko process karna (Process the guess)
    void processGuess(char guess) {
        bool correct = false;
        for (size_t i = 0; i < wordToGuess.length(); i++) {
            if (tolower(wordToGuess[i]) == tolower(guess) && !guessedLetters[i]) {
                guessedLetters[i] = true;
                correct = true;
            }
        }
        if (!correct)
            lives--;
    }

    // Hangman ko draw karna (Draw the hangman)
    void drawHangman(sf::RenderWindow& window, sf::Font& font) {
        sf::Text hangmanText;
        hangmanText.setFont(font);
        hangmanText.setCharacterSize(40);
        hangmanText.setFillColor(sf::Color::Red);
        string hangmanString = "";

        switch (lives) {
        case 6:
            hangmanString = ""; // Nothing to draw (Kuch nahi draw karna)
            break;
        case 5:
            hangmanString = "O";
            break;
        case 4:
            hangmanString = "O\n |";
            break;
        case 3:
            hangmanString = " O\n/|";
            break;
        case 2:
            hangmanString = " O\n/|\\";
            break;
        case 1:
            hangmanString = " O\n/|\\\n/";
            break;
        case 0:
            hangmanString = " O\n/|\\\n/ \\";
            break;
        }

        hangmanText.setString(hangmanString);
        hangmanText.setPosition(10, 10);
        window.draw(hangmanText);
    }

public:
    HangmanGame() : lives(MAX_LIVES), category(0) { // Start with animal category (Janwaron ki category se shuru)
        srand(static_cast<unsigned>(time(0)));
        selectWord();
        updateDisplayedWord();
    }

    // Lafz select karna (Select a word)
    void selectWord() {
        switch (category) {
        case 0:
            wordToGuess = animalWords[rand() % numAnimalWords];
            break;
        case 1:
            wordToGuess = fruitWords[rand() % numFruitWords];
            break;
        case 2:
            wordToGuess = countryWords[rand() % numCountryWords];
            break;
        }
        // Reset guessed letters and lives when selecting a new word (Naya lafz select karte waqt guess kiye gaye hurf aur jindariyan reset karna)
        for (int i = 0; i < 50; i++) {
            guessedLetters[i] = false;
        }
        lives = MAX_LIVES; // Jindariyan reset karna (Reset lives)
    }

    // Khel chalana (Run the game)
    void run() override {
        loadPlayerProfile(playerProfile); // Khiladi ki profile ko load karna (Load player profile)

        sf::RenderWindow window(sf::VideoMode(640, 640), "Hangman Game");
        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            cerr << "Error: Could not load font!" << endl;
            return;
        }

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("hangmanbackground.png")) {
            cerr << "Error: Could not load background image!" << endl;
            return;
        }

        sf::Sprite background(backgroundTexture);
        background.setScale(
            window.getSize().x / static_cast<float>(background.getTexture()->getSize().x),
            window.getSize().y / static_cast<float>(background.getTexture()->getSize().y)
        );

        sf::Text wordText, messageText, livesText, categoryText;
        wordText.setFont(font);
        wordText.setCharacterSize(30);
        wordText.setFillColor(sf::Color::White);
        wordText.setPosition(50, 500);

        messageText.setFont(font);
        messageText.setCharacterSize(24);
        messageText.setFillColor(sf::Color::Yellow);
        messageText.setPosition(100, 550);

        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(420, 20);

        categoryText.setFont(font);
        categoryText.setCharacterSize(24);
        categoryText.setFillColor(sf::Color::White);
        categoryText.setPosition(420, 50);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::TextEntered && lives > 0 && !isWordGuessed()) {
                    char guess = static_cast<char>(event.text.unicode);
                    if (isalpha(guess)) {
                        processGuess(guess);
                        updateDisplayedWord();
                    }
                }
                // Change category using number keys (Number keys se category badalna)
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        category = 0;
                        selectWord(); // Select a new word from the chosen category (Chuni hui category se naya lafz select karna)
                        updateDisplayedWord();
                        messageText.setString(""); // Clear any previous messages (Pehle ke messages ko clear karna)
                    }
                    else if (event.key.code == sf::Keyboard::Num2) {
                        category = 1;
                        selectWord(); // Select a new word from the chosen category (Chuni hui category se naya lafz select karna)
                        updateDisplayedWord();
                        messageText.setString("");
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        category = 2;
                        selectWord(); // Select a new word from the chosen category (Chuni hui category se naya lafz select karna)
                        updateDisplayedWord();
                        messageText.setString("");
                    }
                }
            }

            wordText.setString("Word: " + displayedWord);
            livesText.setString("Lives: " + to_string(lives));

            // Display category (Category display karna)
            switch (category) {
            case 0:
                categoryText.setString("Category: Animals");
                break;
            case 1:
                categoryText.setString("Category: Fruits");
                break;
            case 2:
                categoryText.setString("Category: Countries");
                break;
            }

            if (isWordGuessed()) {
                messageText.setString("Congratulations! You guessed the word!");
                for (int i = 0; i < wordToGuess.length(); i++) {
                    guessedLetters[i] = false;
                }
                // Score बढ़ाना (Increment score)
                playerProfile.setHighScore(playerProfile.getHighScore() + 1);
                savePlayerProfile(playerProfile);
            }
            else if (lives <= 0) {
                messageText.setString("Game Over! The word was: " + wordToGuess);
                for (int i = 0; i < wordToGuess.length(); i++) {
                    guessedLetters[i] = false;
                }
                // Score घटाना (Decrement score) (negative scores se bachne ke liye check ke saath)
                if (playerProfile.getHighScore() > 0) {
                    playerProfile.setHighScore(playerProfile.getHighScore() - 1);
                    savePlayerProfile(playerProfile);
                }
            }

            window.clear(sf::Color::Black);
            window.draw(background);
            drawHangman(window, font);
            window.draw(wordText);
            window.draw(messageText);
            window.draw(livesText);
            window.draw(categoryText);
            window.display();
        }
    }
};

// Constants for the grid (Grid ke liye constants)
const int resolutionX = 640;
const int resolutionY = 640;
const int boxSize = 64;
const int gridRows = 6; // 6 guesses (6 andaze)
const int gridColumns = 5; // 5-letter words (5-harf ke alfaaz)

// Function to check character feedback (Hruf ke feedback ki jaanch karne ka function)
void evaluateGuess(const string& guess, string& feedback,
    const string& secretWord) {
    feedback = "     "; // Initialize with spaces (Spaces se initialize karna)
    for (int i = 0; i < gridColumns; i++) {
        if (guess[i] == secretWord[i]) {
            feedback[i] = 'G'; // Green for correct position (Sahi jagah ke liye hara)
        }
        else if (secretWord.find(guess[i]) != string::npos) {
            feedback[i] = 'Y'; // Yellow for incorrect position but correct letter (Ghalat jagah lekin sahi harf ke liye peela)
        }
        else {
            feedback[i] = 'R'; // Black for incorrect letter (Ghalat harf ke liye kala)
        }
    }
}

// Class to handle the Wordle grid (Wordle grid ko handle karne ka class)
// Class to handle the Wordle grid (Wordle grid ko handle karne ka class)
class WordleGrid {
private:
    char guesses[gridRows][gridColumns]; // Stores the guesses (Andaze store karta hai)
    string feedback[gridRows]; // Feedback for each guess (Har andaze ke liye feedback)
    int currentRow; // Current row being guessed (Abhi jis row ka andaza lagaya ja raha hai)

public:
    WordleGrid() : currentRow(0) {
        for (int i = 0; i < gridRows; i++) {
            for (int j = 0; j < gridColumns; j++) {
                guesses[i][j] = ' '; // Initialize empty grid (Khali grid initialize karna)
            }
            feedback[i] = "     "; // Empty feedback (Khali feedback)
        }
    }

    // Andaza lagana (Make a guess)
    bool makeGuess(const string& guess, const string& secretWord) {
        if (currentRow >= gridRows)
            return false; // No more guesses (Zyada andaze nahi)
        for (int i = 0; i < gridColumns; i++) {
            guesses[currentRow][i] = guess[i];
        }
        evaluateGuess(guess, feedback[currentRow], secretWord);
        currentRow++;
        return true;
    }

    // Grid ko draw karna (Draw the grid)
    void draw(sf::RenderWindow& window, sf::Font& font,
        const string& currentGuess) {
        sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
        sf::Text letterText;
        letterText.setFont(font);
        letterText.setCharacterSize(32);
        letterText.setStyle(sf::Text::Bold);

        for (int i = 0; i < gridRows; i++) {
            for (int j = 0; j < gridColumns; j++) {
                // Set the box color based on feedback (Feedback ke hisab se box ka rang set karna)
                if (feedback[i][j] == 'G')
                    box.setFillColor(sf::Color::Green);
                else if (feedback[i][j] == 'Y')
                    box.setFillColor(sf::Color::Yellow);
                else if (feedback[i][j] == 'R')
                    box.setFillColor(sf::Color::Red); // Dark gray for incorrect (Ghalat ke liye dark gray)
                else
                    box.setFillColor(sf::Color::White);

                box.setOutlineColor(sf::Color::Black);
                box.setOutlineThickness(2);
                box.setPosition(j * boxSize, i * boxSize);
                window.draw(box);

                // Draw the letters (Hruf ko draw karna)
                if (i < currentRow ||
                    (i == currentRow && j < currentGuess.length())) {
                    letterText.setString(
                        string(1, (i == currentRow ? currentGuess[j]
                            : guesses[i][j])));
                    letterText.setFillColor(sf::Color::Black);
                    letterText.setPosition(j * boxSize + boxSize / 4,
                        i * boxSize + boxSize / 4);
                    window.draw(letterText);
                }
            }
        }
    }

    // Kya khel khatam ho gaya hai? (Is the game over?)
    bool isGameOver() {
        return currentRow >= gridRows || feedback[currentRow - 1] == "GGGGG";
    }

    // Kya jeet gaya hai? (Has the player won?)
    bool hasWon() { return feedback[currentRow - 1] == "GGGGG"; }
};

// Wordle game class (Wordle khel ka class)
class WordleGame : public Game {
public:
    void run() override {
        srand(time(0));

        sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY),
            "Wordle Game");
        sf::Font font;

        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            cout << "Error loading font!" << endl;
            return;
        }
        // Select a random word (Random lafz select karna)
        string secretWord = wordList[rand() % 50];

        WordleGrid grid;
        string currentGuess = "";
        bool gameOver = false;

        // On-Screen keyboard buttons (a-z and Backspace, Enter) (On-screen keyboard buttons (a-z aur Backspace, Enter))
        sf::RectangleShape keys[30]; // 26 letters + Backspace + Enter (26 harf + Backspace + Enter)
        sf::Text keyLabels[30];

        for (int i = 0; i < 26; i++) {
            keys[i].setSize(sf::Vector2f(50, 50));
            keys[i].setFillColor(sf::Color::Cyan);
            keys[i].setOutlineColor(sf::Color::Black);
            keys[i].setOutlineThickness(2);
            keyLabels[i].setFont(font);
            keyLabels[i].setCharacterSize(24);
            keyLabels[i].setFillColor(sf::Color::Black);
            keyLabels[i].setString(string(1, 'a' + i)); // Lowercase letters (Chote harf)
        }

        // Set positions of the keys (Keys ki position set karna)
        for (int i = 0; i < 10; i++) {
            keys[i].setPosition(20 + i * 60, 440); // First row (Pehli row)
            keyLabels[i].setPosition(40 + i * 60, 450);
        }
        for (int i = 10; i < 20; i++) {
            keys[i].setPosition(20 + (i - 10) * 60, 500); // Second row (Dusri row)
            keyLabels[i].setPosition(40 + (i - 10) * 60, 510);
        }
        for (int i = 20; i < 26; i++) {
            keys[i].setPosition(20 + (i - 20) * 60, 560); // Third row (Teesri row)
            keyLabels[i].setPosition(40 + (i - 20) * 60, 570);
        }

        // Add special keys (Backspace and Enter) (Special keys add karna (Backspace aur Enter))
        keys[26].setSize(sf::Vector2f(120, 50)); // Backspace
        keys[26].setPosition(20, 660);
        keyLabels[26].setString("Back");
        keyLabels[26].setPosition(40, 670);

        keys[27].setSize(sf::Vector2f(120, 50)); // Enter
        keys[27].setPosition(160, 660);
        keyLabels[27].setString("Enter");
        keyLabels[27].setPosition(180, 670);

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("wordlebackground.png")) {
            cerr << "Error: Could not load background image!" << endl;
            return;
        }

        sf::Sprite background(backgroundTexture);
        background.setScale(
            window.getSize().x / static_cast<float>(background.getTexture()->getSize().x),
            window.getSize().y / static_cast<float>(background.getTexture()->getSize().y)
        );

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Handle keyboard input (laptop keyboard) (Keyboard input ko handle karna (laptop keyboard))
                if (event.type == sf::Event::KeyPressed && !gameOver) {
                    if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                        char keyPressed = static_cast<char>(tolower(event.key.code + 'A' - sf::Keyboard::A)); // Convert to lowercase (Chote harf mein badalna)
                        if (currentGuess.length() < gridColumns) {
                            currentGuess += keyPressed;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::BackSpace && !currentGuess.empty()) {
                        currentGuess.pop_back();
                    }
                    else if (event.key.code == sf::Keyboard::Enter && currentGuess.length() == gridColumns) {
                        gameOver = !grid.makeGuess(currentGuess, secretWord);
                        currentGuess = "";
                    }
                }

                // Handle mouse clicks (on-screen keyboard) (Mouse clicks ko handle karna (on-screen keyboard))
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (int i = 0; i < 26; i++) {
                            if (keys[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                                char keyPressed = 'a' + i; // Lowercase letters (Chote harf)
                                if (currentGuess.length() < gridColumns) {
                                    currentGuess += keyPressed;
                                }
                            }
                        }

                        // Handle Backspace button click (Backspace button click ko handle karna)
                        if (keys[26].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (!currentGuess.empty()) {
                                currentGuess.pop_back(); // Backspace
                            }
                        }

                        // Handle Enter button click (Enter button click ko handle karna)
                        if (keys[27].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (currentGuess.length() == gridColumns) {
                                gameOver = !grid.makeGuess(currentGuess, secretWord);
                                currentGuess = "";
                            }
                        }
                    }
                }
            }

            // Game Over logic (Game Over logic)
            if (grid.isGameOver()) {
                gameOver = true;
            }

            // Clear and render the window (Window ko clear aur render karna)
            window.clear(sf::Color::Black);
            window.draw(background);
            grid.draw(window, font, currentGuess);

            // Draw on-screen keyboard (On-screen keyboard ko draw karna)
            for (int i = 0; i < 28; i++) {
                window.draw(keys[i]);
                window.draw(keyLabels[i]);
            }

            if (gameOver) {
                sf::Text gameOverText;
                gameOverText.setFont(font);
                gameOverText.setCharacterSize(36);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setString(grid.hasWon() ? "You Won!" : "Game Over! The word was: " + secretWord);
                gameOverText.setPosition(100, 385);
                window.draw(gameOverText);
            }

            window.display();
        }
    }
};


// Grid Constants for Snake Game (Snake Game ke liye Grid Constants)
const int snakeResolutionX = 840;
const int snakeResolutionY = 840;
const int snakeBoxSize = 32;
const int snakeGridRows = snakeResolutionX / snakeBoxSize;
const int snakeGridColumns = snakeResolutionY / snakeBoxSize;
const int maxSnakeLength = snakeGridRows * snakeGridColumns; // Maximum possible snake size (Snake ki zyada se zyada mumkina size)

// Game States (Khel ki halat)
const int GAME_RUNNING = 0;
const int GAME_OVER = 1;
const int GAME_PAUSED = 2;

// Directions (Dishayen)
enum Direction { UP, DOWN, LEFT, RIGHT };

// Class: Grid for Snake Game (Snake Game ke liye Grid Class)
class SnakeGrid {
public:
    int rows, columns, boxSize;

    SnakeGrid(int rows, int columns, int boxSize) : rows(rows), columns(columns), boxSize(boxSize) {}

    // Grid ko render karna (Render the grid)
    void renderGrid(sf::RenderWindow& window) {
        sf::RectangleShape line(sf::Vector2f(snakeResolutionX, 1));
        line.setFillColor(sf::Color::White);

        for (int i = 0; i <= rows; i++) {
            line.setPosition(0, i * boxSize);
            window.draw(line);
        }

        line.setSize(sf::Vector2f(1, snakeResolutionY));
        for (int j = 0; j <= columns; j++) {
            line.setPosition(j * boxSize, 0);
            window.draw(line);
        }
    }
};

// Class: Food for Snake Game (Snake Game ke liye Khana Class)
class SnakeFood {
public:
    int x, y;

    SnakeFood() {
        srand(time(0));
        generateFood();
    }

    // Khana generate karna (Generate food)
    void generateFood() {
        x = rand() % snakeGridColumns;
        y = rand() % snakeGridRows;
    }

    // Khane ko draw karna (Draw the food)
    void draw(sf::RenderWindow& window, sf::RectangleShape& foodShape) {
        foodShape.setPosition(x * snakeBoxSize, y * snakeBoxSize);
        window.draw(foodShape);
    }
};

// Class: Snake (Saanp)
class Snake {
public:
    int segments[maxSnakeLength][2]; // Array to hold x, y coordinates (x, y coordinates rakhne ke liye array)
    int length;
    Direction direction;
    bool growFlag;

    Snake() : length(1), direction(UP), growFlag(false) {
        segments[0][0] = snakeGridColumns / 2; // Initial x position (Shuruati x position)
        segments[0][1] = snakeGridRows / 2; // Initial y position (Shuruati y position)
    }

    // Disha badalna (Change direction)
    void changeDirection(Direction newDirection) {
        // Prevent 180-degree turns (180-degree turns ko rokna)
        if ((direction == UP && newDirection != DOWN) ||
            (direction == DOWN && newDirection != UP) ||
            (direction == LEFT && newDirection != RIGHT) ||
            (direction == RIGHT && newDirection != LEFT)) {
            direction = newDirection;
        }
    }

    // Hilna (Move)
    void move() {
        // Shift the body segments backward (Body segments ko peeche shift karna)
        for (int i = length - 1; i > 0; --i) {
            segments[i][0] = segments[i - 1][0];
            segments[i][1] = segments[i - 1][1];
        }

        // Update the head position (Head position ko update karna)
        switch (direction) {
        case UP: segments[0][1]--; break;
        case DOWN: segments[0][1]++; break;
        case LEFT: segments[0][0]--; break;
        case RIGHT: segments[0][0]++; break;
        }

        if (growFlag) {
            ++length;
            growFlag = false;
        }
    }

    // Badhana (Grow)
    void grow() { growFlag = true; }

    // Takkar ki jaanch karna (Check for collision)
    bool checkCollision() {
        // Check collision with walls (Deewaron se takkar ki jaanch karna)
        if (segments[0][0] < 0 || segments[0][0] >= snakeGridColumns || segments[0][1] < 0 || segments[0][1] >= snakeGridRows) {
            return true;
        }

        // Check collision with itself (Khud se takkar ki jaanch karna)
        for (int i = 1; i < length; i++) {
            if (segments[i][0] == segments[0][0] && segments[i][1] == segments[0][1]) {
                return true;
            }
        }

        return false;
    }

    // Snake ko draw karna (Draw the snake)
    void draw(sf::RenderWindow& window, sf::RectangleShape& snakeShape) {
        for (int i = 0; i < length; i++) {
            snakeShape.setPosition(segments[i][0] * snakeBoxSize, segments[i][1] * snakeBoxSize);
            window.draw(snakeShape);
        }
    }
};

// Snake game class (Snake khel ka class)
class SnakeGame : public Game {
private:
    Snake snake;
    SnakeFood food;
    SnakeGrid grid;
    int score;
    int state;
    int level;
    float speed;
    sf::Clock clock;
    sf::Time elapsed;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    SnakeGame() : grid(snakeGridRows, snakeGridColumns, snakeBoxSize), score(0), state(GAME_RUNNING), level(1), speed(100.0f) {
        // Load the background image (Background image ko load karna)
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            // Handle error if background image fails to load (Agar background image load nahi hoti hai to error handle karna)
            std::cerr << "Error loading background image!" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
    }

    // Input ko handle karna (Handle input)
    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) snake.changeDirection(UP);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) snake.changeDirection(DOWN);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) snake.changeDirection(LEFT);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.changeDirection(RIGHT);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (state == GAME_RUNNING) state = GAME_PAUSED;
            else if (state == GAME_PAUSED) state = GAME_RUNNING;
        }
    }

    // Update karna (Update)
    void update() {
        if (state == GAME_OVER || state == GAME_PAUSED) return;

        elapsed += clock.restart();

        if (elapsed.asMilliseconds() > speed) {
            snake.move();
            elapsed -= sf::milliseconds(speed);

            // Check collision with food (Khane se takkar ki jaanch karna)
            if (snake.segments[0][0] == food.x && snake.segments[0][1] == food.y) {
                snake.grow();
                food.generateFood();
                score += 10;

                // Increase level and speed every 50 points (Har 50 points par level aur speed badhana)
                if (score % 50 == 0) {
                    level++;
                    speed = speed / 1.5f; // Double the speed (make it faster) (Speed ko double karna (tez karna))
                }
            }

            // Check collision with walls or itself (Deewaron ya khud se takkar ki jaanch karna)
            if (snake.checkCollision()) {
                state = GAME_OVER;
            }
        }
    }

    // Render karna (Render)
    void render(sf::RenderWindow& window) {
        if (state == GAME_OVER) {
            sf::Font font;
            if (font.loadFromFile("Movistar Text Regular.ttf")) {
                sf::Text gameOverText("Game Over! Press R to Restart", font, 30);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setPosition(snakeResolutionX / 4, snakeResolutionY / 2);
                window.draw(gameOverText);
            }
            return;
        }

        if (state == GAME_PAUSED) {
            sf::Font font;
            if (font.loadFromFile("Movistar Text Regular.ttf")) {
                sf::Text pauseText("Game Paused! Press Space to Resume", font, 30);
                pauseText.setFillColor(sf::Color::Yellow);
                pauseText.setPosition(snakeResolutionX / 4, snakeResolutionY / 2);
                window.draw(pauseText);
            }
            return;
        }

        // Grid rendering (Grid ko render karna)
        grid.renderGrid(window);

        // Food rendering (Khane ko render karna)
        sf::RectangleShape foodShape(sf::Vector2f(snakeBoxSize, snakeBoxSize));
        foodShape.setFillColor(sf::Color::Red);
        food.draw(window, foodShape);

        // Snake rendering (Snake ko render karna)
        sf::RectangleShape snakeShape(sf::Vector2f(snakeBoxSize - 2, snakeBoxSize - 2));
        snakeShape.setFillColor(sf::Color::Green);
        snake.draw(window, snakeShape);

        // Score and Level rendering (Score aur Level ko render karna)
        sf::Font font;
        if (font.loadFromFile("Movistar Text Regular.ttf")) {
            sf::Text scoreText("Score: " + std::to_string(score), font, 50);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(360, 20); // Position it at the top-left corner (Top-left corner par position karna)
            window.draw(scoreText);

            sf::Text levelText("Level: " + std::to_string(level), font, 50);
            levelText.setFillColor(sf::Color::White);
            levelText.setPosition(20, 20); // Position it at the top-left corner (Top-left corner par position karna)
            window.draw(levelText);
        }
    }

    // Dobara shuru karna (Restart)
    void restart() {
        state = GAME_RUNNING;
        snake = Snake();
        food.generateFood();
        score = 0;
        level = 1;
        speed = 100.0f;
        elapsed = sf::Time::Zero;
        clock.restart();
    }

    // Khel chalana (Run the game)
    void run() override {
        sf::RenderWindow window(sf::VideoMode(snakeResolutionX, snakeResolutionY), "Snake Game");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    restart();
                }
            }

            // Game logic (Khel ka logic)
            handleInput();
            update();

            // Rendering (Render karna)
            window.clear();
            render(window);
            window.display();
        }
    }
};

// GameBoy class (GameBoy class)
class GameBoy {
private:
    sf::RenderWindow window;
    sf::Music backgroundMusic;
    int currentSelection;
    Game* games[3]; // Array to hold three games (Teen khel rakhne ke liye array)
    bool isOn;

    // Function to display the main menu (Main menu display karne ka function)
    void displayMainMenu() {
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            std::cerr << "Error: Could not load background image!" << std::endl;
            return;
        }
        sf::Sprite backgroundSprite(backgroundTexture);

        // Scale the background to fit the window size (Window size ke hisab se background ko scale karna)
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        backgroundSprite.setScale(scaleX, scaleY);

        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            return;
        }

        sf::Text menuText[3];
        const std::string menuItems[3] = { "Play Game", "Instructions", "Exit" };

        for (int i = 0; i < 3; ++i) {
            menuText[i].setFont(font);
            menuText[i].setString(menuItems[i]);
            menuText[i].setCharacterSize(24);
            menuText[i].setFillColor(sf::Color::White);
        }

        const int windowWidth = window.getSize().x;
        const int windowHeight = window.getSize().y;

        const float itemSpacing = 50.0f;
        const float totalMenuHeight = 3 * menuText[0].getCharacterSize() + 2 * itemSpacing;
        const float startY = (windowHeight - totalMenuHeight) / 2.0f;

        for (int i = 0; i < 3; ++i) {
            sf::FloatRect textBounds = menuText[i].getGlobalBounds();
            menuText[i].setPosition((windowWidth - textBounds.width) / 2.0f, startY + i * (textBounds.height + itemSpacing));
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        currentSelection = (currentSelection - 1 + 3) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        currentSelection = (currentSelection + 1) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (currentSelection == 0) { // Play Game (Khel Khelo)
                            displayGameMenu();
                        }
                        else if (currentSelection == 1) { // Instructions (Hidayat)
                            displayInstructions();
                        }
                        else if (currentSelection == 2) { // Exit (Bahar Niklo)
                            window.close();
                        }
                    }
                }
            }

            for (int i = 0; i < 3; ++i) {
                menuText[i].setFillColor(currentSelection == i ? sf::Color::Red : sf::Color::White);
            }

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);
            for (int i = 0; i < 3; ++i) {
                window.draw(menuText[i]);
            }
            window.display();
        }
    }

    // Function to display the game selection menu (Khel select karne ka menu display karne ka function)
    void displayGameMenu() {
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            std::cerr << "Error: Could not load background image!" << std::endl;
            return;
        }

        sf::Sprite backgroundSprite(backgroundTexture);

        // Scale the background to fit the window size (Window size ke hisab se background ko scale karna)
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        backgroundSprite.setScale(scaleX, scaleY);

        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            return;
        }

        sf::Text menuText[3];
        const std::string menuItems[3] = { "1. Hangman", "2. Wordle", "3. Snake" };

        for (int i = 0; i < 3; ++i) {
            menuText[i].setFont(font);
            menuText[i].setString(menuItems[i]);
            menuText[i].setCharacterSize(24);
            menuText[i].setFillColor(sf::Color::White);
        }

        const int windowWidth = window.getSize().x;
        const int windowHeight = window.getSize().y;

        const float itemSpacing = 50.0f;
        const float totalMenuHeight = 3 * menuText[0].getCharacterSize() + 2 * itemSpacing;
        const float startY = (windowHeight - totalMenuHeight) / 2.0f;

        for (int i = 0; i < 3; ++i) {
            sf::FloatRect textBounds = menuText[i].getGlobalBounds();
            menuText[i].setPosition((windowWidth - textBounds.width) / 2.0f, startY + i * (textBounds.height + itemSpacing));
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        currentSelection = (currentSelection - 1 + 3) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        currentSelection = (currentSelection + 1) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        games[currentSelection]->run(); // Run the selected game (Chune hue khel ko chalana)
                        return; // Go back to the main menu after the game ends (Khel khatam hone ke baad main menu par wapas jana)
                    }
                    else if (event.key.code == sf::Keyboard::Escape) { // Add back button functionality (Wapas jane ka button add karna)
                        return; // Go back to the main menu (Main menu par wapas jana)
                    }
                }
            }

            for (int i = 0; i < 3; ++i) {
                menuText[i].setFillColor(currentSelection == i ? sf::Color::Red : sf::Color::White);
            }

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite); // Draw the scaled background (Scaled background ko draw karna)
            for (int i = 0; i < 3; ++i) {
                window.draw(menuText[i]);
            }
            window.display();
        }
    }

    // Function to display the instructions (Hidayat display karne ka function)
    void displayInstructions() {
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("menu1.png")) {
            std::cerr << "Error: Could not load background image!" << std::endl;
            return;
        }
        sf::Sprite backgroundSprite(backgroundTexture);

        // Scale the background to fit the window size (Window size ke hisab se background ko scale karna)
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        backgroundSprite.setScale(scaleX, scaleY);

        sf::Font font;
        if (!font.loadFromFile("Movistar Text Regular.ttf")) {
            std::cerr << "Error: Could not load font!" << std::endl;
            return;
        }

        sf::Text instructionsText;
        instructionsText.setFont(font);
        instructionsText.setCharacterSize(20);
        instructionsText.setFillColor(sf::Color::Red);

        // Set the instructions text (replace with your actual instructions) (Hidayat text set karna (apni asli hidayat se badalna))
        instructionsText.setString("Instructions:\n\n"
            "Hangman: Guess the word by entering letters.\n"
            "You have 6 lives. Incorrect guesses cost a life.\n\n"
            "Wordle: Guess the 5-letter word in 6 tries.\n"
            "Green means the letter is in the correct spot.\n"
            "Yellow means the letter is in the word but in the wrong spot.\n\n"
            "Snake: Use arrow keys to control the snake.\n"
            "Eat food to grow and avoid hitting walls or yourself.\n\n"
            "Press Esc to go back to the main menu.");

        // Center the instructions text (Hidayat text ko center karna)
        sf::FloatRect textBounds = instructionsText.getGlobalBounds();
        instructionsText.setPosition((window.getSize().x - textBounds.width) / 2.0f,
            (window.getSize().y - textBounds.height) / 2.0f);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) { // Back to main menu (Main menu par wapas)
                        return;
                    }
                }
            }

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);
            window.draw(instructionsText);
            window.display();
        }
    }

public:
    GameBoy() : window(sf::VideoMode(800, 600), "GameBoy Menu"), currentSelection(0), isOn(true) {
        games[0] = new HangmanGame();
        games[1] = new WordleGame();
        games[2] = new SnakeGame();

        // Load and play background music (Background music ko load aur play karna)
        if (!backgroundMusic.openFromFile("music.mp3")) {
            std::cerr << "Error: Could not load music file!" << std::endl;
        }
        backgroundMusic.play();
        backgroundMusic.setLoop(true);
    }

    ~GameBoy() {
        for (int i = 0; i < 3; ++i) {
            delete games[i];
        }
    }

    // Chalana (Run)
    void run() {
        displayMainMenu(); // Start with the main menu (Main menu se shuru karna)
    }
};

