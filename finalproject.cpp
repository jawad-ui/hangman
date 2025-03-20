#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <memory>
#include <vector>


using namespace std;

// Node of Score
struct ScoreNode {
    string username;
    int score;
    shared_ptr<ScoreNode> left;
    shared_ptr<ScoreNode> right;

    ScoreNode() : username(""), score(0), left(nullptr), right(nullptr) {}
    ScoreNode(const string& name, int s) : username(name), score(s), left(nullptr), right(nullptr) {}
};

// Score's Binary Search Tree
class ScoreBST {
private:
    shared_ptr<ScoreNode> root;

    shared_ptr<ScoreNode> insert(shared_ptr<ScoreNode> node, const string& username, int score) {
        if (node == nullptr) {
            return make_shared<ScoreNode>(username, score);
        }

        if (score < node->score) {
            node->left = insert(node->left, username, score);
        }
        else {
            node->right = insert(node->right, username, score);
        }

        return node;
    }

    void saveScoresToFileHelper(shared_ptr<ScoreNode> node, ofstream& outFile) {
        if (node != nullptr) {
            saveScoresToFileHelper(node->left, outFile);
            outFile << node->username << " " << node->score << endl;
            saveScoresToFileHelper(node->right, outFile);
        }
    }

    void printPreOrder(shared_ptr<ScoreNode> node) {
        if (node != nullptr) {
            cout << node->username << ": " << node->score << endl;
            printPreOrder(node->left);
            printPreOrder(node->right);
        }
    }

    shared_ptr<ScoreNode> findPlayerHelper(shared_ptr<ScoreNode> node, const string& playerName) {
        if (node == nullptr) {
            return nullptr; // Player not found
        }

        if (playerName == node->username) {
            return node; // Player found
        }
        else if (playerName < node->username) {
            return findPlayerHelper(node->left, playerName);
        }
        else {
            return findPlayerHelper(node->right, playerName);
        }
    }

    void printScoresInOrder(shared_ptr<ScoreNode> node, const string& difficulty) {
        if (node != nullptr) {
            printScoresInOrder(node->left, difficulty);
            if (!node->username.empty() && node->score > 0) {
                cout << node->username << ": " << node->score << " points in " << difficulty << " mode" << endl;
            }
            printScoresInOrder(node->right, difficulty);
        }
    }

public:
    ScoreBST() : root(nullptr) {}

    void insertScore(const string& username, int score) {
        root = insert(root, username, score);
    }

    void printScores() {
        cout << "High Scores:" << endl;
        printPreOrder(root);
    }

    shared_ptr<ScoreNode> findPlayer(const string& playerName) {
        return findPlayerHelper(root, playerName);
    }

    void printScores(const string& difficulty) {
        printScoresInOrder(root, difficulty);
    }

    void saveScoresToFile() {
        ofstream outFile("scores.txt");
        if (outFile.is_open()) {
            saveScoresToFileHelper(root, outFile);
            outFile.close();
        }
    }
};

// User's Node
struct UserNode {
    string username;
    shared_ptr<UserNode> prev;
    shared_ptr<UserNode> next;

    UserNode(const string& name) : username(name), prev(nullptr), next(nullptr) {}
};

// Username's Doubly Linked List
class UsernameDLL {
private:
    shared_ptr<UserNode> head;
    shared_ptr<UserNode> tail;

    void insertEnd(const string& username) {
        shared_ptr<UserNode> newNode = make_shared<UserNode>(username);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
    }

public:
    UsernameDLL() : head(nullptr), tail(nullptr) {}

    void insertUsername(const string& username) {
        insertEnd(username);
    }

    void printUsernames() {
        cout << "Player Order:" << endl;
        shared_ptr<UserNode> current = head;
        while (current != nullptr) {
            cout << current->username << endl;
            current = current->next;
        }
    }
};

void LoadScoresFromFile(ScoreBST& scoreBST) {
    ifstream scoreFile("scores.txt");
    if (scoreFile.is_open()) {
        string username;
        int score;
        while (scoreFile >> username >> score) {
            scoreBST.insertScore(username, score);
        }
        scoreFile.close();
    }
}

void LoadUsernamesFromFile(UsernameDLL& usernameDLL) {
    ifstream usernameFile("players.txt");
    if (usernameFile.is_open()) {
        string username;
        while (usernameFile >> username) {
            usernameDLL.insertUsername(username);
        }
        usernameFile.close();
    }
}

// Function to convert upper case letters to lower case
char convertToLowerCase(char inputChar) {
    return isupper(inputChar) ? tolower(inputChar) : inputChar;
}

// Main Game's Class
class Hangman {
protected:
    string word, guess;
    int wrong;
    virtual void Print_Character() {}
    void Print_Guess() {
        for (int i = 0; i < guess.length(); i++) {
            cout << guess[i] << " ";
        }
        cout << endl;
    }
    bool Game_Over() {
        return wrong >= max_wrong || guess == word;
    }

public:
    int max_wrong;
    Hangman() : word(""), guess(""), max_wrong(0), wrong(0) {}
    virtual ~Hangman() {}

    int getwrong() {
        return wrong;
    }

    int getmaxwrong() {
        return max_wrong;
    }

    void play();
    void WriteWinner(const string& playerName, const string& difficulty, int scoreIncrement);
    void PrintWinners(const string& difficulty);
};

// Singleplayer Easy Class
class Singleplayer_Easy : public Hangman {

private:
    vector<string> words = { "cat", "dog", "cow", "sun", "sky", "sea", "ice", "car" };

public:

    Singleplayer_Easy() {
        max_wrong = 7;
        srand(time(NULL));
        int index = rand() % words.size();
        word = words[index];
        guess = string(word.length(), '_');
        wrong = 0;
    }
    void Print_Character() override;
};

// Singleplayer Medium Class
class Singleplayer_Medium : public Hangman {
private:
    vector<string> words = { "banana", "orange", "pencil", "school", "bottle", "camera", "coffee", "picture" };

public:
    Singleplayer_Medium() {
        max_wrong = 6;
        srand(time(NULL));
        int index = rand() % words.size();
        word = words[index];
        guess = string(word.length(), '_');
        wrong = 0;
    }
    void Print_Character() override;
};

// Singleplayer Hard Difficulty Class
class Singleplayer_Hard : public Hangman {
private:
    vector<string> words = { "abstract", "algorithm", "arbitrary", "automaton", "binary", "circuitry", "cyberspace", "decomposition" };

public:
    Singleplayer_Hard() {
        max_wrong = 4;
        srand(time(NULL));
        int index = rand() % words.size();
        word = words[index];
        guess = string(word.length(), '_');
        wrong = 0;
    }
    void Print_Character() override;
};

// Play Function
void Hangman::play() {
    while (!Game_Over()) {
        Print_Character();
        Print_Guess();
        char c;
        cout << "Guess a letter: ";
        cin >> c;
        c = convertToLowerCase(c);
        cout << endl;

        bool found = false;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == c) {
                guess[i] = c;
                found = true;
            }
        }

        if (!found) {
            wrong++;
            cout << "Wrong guess. You have " << max_wrong - wrong << " chances left." << endl;
        }

        cout << endl;
    }

    if (wrong < max_wrong) {
        cout << "Congratulations! You guessed the word: " << word << endl;
        string difficulty;
        // Prompt for the player's name
        string playerName;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, playerName);

        // Update score based on difficulty
        int scoreIncrement = 0;
        if (max_wrong == 4) {
            // Hard mode
            scoreIncrement = 3;
            difficulty = "Hard";
        }
        else if (max_wrong == 6) {
            // Medium mode
            scoreIncrement = 2;
            difficulty = "Medium";
        }
        else {
            // Easy mode
            scoreIncrement = 1;
            difficulty = "Easy";
        }

        // Write the winner with updated score
        WriteWinner(playerName, difficulty, scoreIncrement);
    }
    else {
        Print_Character();
        cout << "Sorry, you ran out of chances. The word was: " << word << endl;
    }
}

// Write the winner into file
void Hangman::WriteWinner(const string& playerName, const string& difficulty, int scoreIncrement) {
    // Load existing scores from file
    ScoreBST scoreBST;
    LoadScoresFromFile(scoreBST);

    // Check if the player already exists in the BST
    shared_ptr<ScoreNode> existingPlayer = scoreBST.findPlayer(playerName);

    // Update the player's score
    if (existingPlayer != nullptr) {
        // Player exists, update their score
        existingPlayer->score += scoreIncrement;
    }
    else {
        // Player does not exist, insert a new node
        scoreBST.insertScore(playerName, scoreIncrement);
    }

    // Save the updated scores back to the file
    scoreBST.saveScoresToFile();
}

// Print the winners from the file based on the difficulty
void Hangman::PrintWinners(const string& difficulty) {
    // Load existing scores from file
    ScoreBST scoreBST;
    LoadScoresFromFile(scoreBST);

    // Print scores with difficulty
    scoreBST.printScores(difficulty);
}

// Print the character for Singleplayer Easy
void Singleplayer_Easy::Print_Character() {
    switch (wrong) {

    case 0:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 1:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 2:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 3:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 4:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 5:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|  /" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 6:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|  / \\" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;

    }
}

// Print the character for Singleplayer Medium
void Singleplayer_Medium::Print_Character() {
    switch (wrong) {

    case 0:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 1:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 2:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 3:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 4:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 5:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|  /" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 6:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|  / \\" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;

    }
}

// Print the character for Singleplayer Hard
void Singleplayer_Hard::Print_Character() {
    switch (wrong) {
    case 0:

        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 1:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 2:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|   |" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 3:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;
    case 4:
        cout << "_" << endl;
        cout << "|   |" << endl;
        cout << "|   O" << endl;
        cout << "|  /|\\" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "|" << endl;
        cout << "---------" << endl;
        break;

    }
}

int main() {

    Hangman* game = nullptr;

    cout << "--------------------------Welcome to Hangman!--------------------------" << endl;
    cout << "Select the difficulty level:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
        game = new Singleplayer_Easy();
        break;
    case 2:
        game = new Singleplayer_Medium();
        break;
    case 3:
        game = new Singleplayer_Hard();
        break;
    default:
        cout << "Invalid choice!" << endl;
        return 1;
    }

    game->play();

    // Free allocated memory
    delete game;

    // Display the high scores
    cout << endl;
    cout << "High Scores:" << endl;
    string difficulty;
    switch (choice) {
    case 1:
        difficulty = "Easy";
        break;
    case 2:
        difficulty = "Medium";
        break;
    case 3:
        difficulty = "Hard";
        break;
    }
    game->PrintWinners(difficulty);


    return 0;
}