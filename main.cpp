#include "Board.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
Jack Rosenberg
Simon Logan
Computer Science Final Project, CSCI 1300

*/


struct Character {
    string name;
    int experience;
    int accuracy;
    int efficiency;
    int insight;
    int discoveryPoints;
};

struct RandomEvent {
    string description;
    int pathType;
    int advisorId;
    int dpChange;
};

struct Riddle {
    string question;
    string answer;
};

struct LeaderboardEntry {
    string playerName;
    string characterName;
    int finalScore;
};

void loadCharacters(Character characters[5]) {
    ifstream file("characters.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open characters.txt\n";
        return;
    }
    
    string header;
    getline(file, header);
    
    int index = 0;
    string line;
    while (getline(file, line) && index < 5) {
        int pos = 0;
        int fieldCount = 0;
        string fields[6];
        
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                fields[fieldCount] = line.substr(pos, i - pos);
                fieldCount++;
                pos = i + 1;
            }
        }
        fields[fieldCount] = line.substr(pos);
        
        characters[index].name = fields[0];
        characters[index].experience = stoi(fields[1]);
        characters[index].accuracy = stoi(fields[2]);
        characters[index].efficiency = stoi(fields[3]);
        characters[index].insight = stoi(fields[4]);
        characters[index].discoveryPoints = stoi(fields[5]);
        
        index++;
    }
    
    file.close();
}

int loadRandomEvents(RandomEvent events[100]) {
    ifstream file("random_events.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open random_events.txt\n";
        return 0;
    }
    
    string line;
    getline(file, line);
    getline(file, line);
    
    int eventCount = 0;
    while (getline(file, line) && eventCount < 100) {
        if (line.length() < 3 || line[0] == '/' || line[0] == '\n') {
            continue;
        }
        
        int pos = 0;
        int fieldCount = 0;
        string fields[4];
        
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                fields[fieldCount] = line.substr(pos, i - pos);
                fieldCount++;
                pos = i + 1;
            }
        }
        fields[fieldCount] = line.substr(pos);
        
        if (fieldCount >= 3) {
            events[eventCount].description = fields[0];
            events[eventCount].pathType = stoi(fields[1]);
            events[eventCount].advisorId = stoi(fields[2]);
            events[eventCount].dpChange = stoi(fields[3]);
            eventCount++;
        }
    }
    
    file.close();
    return eventCount;
}

int loadRiddles(Riddle riddles[25]) {
    ifstream file("riddles.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open riddles.txt\n";
        return 0;
    }
    
    string line;
    getline(file, line);
    
    int riddleCount = 0;
    while (getline(file, line) && riddleCount < 25) {
        int pipePos = -1;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                pipePos = i;
                break;
            }
        }
        
        if (pipePos != -1) {
            riddles[riddleCount].question = line.substr(0, pipePos);
            string rawAnswer = line.substr(pipePos + 1);
            int start = 0;
            int end = rawAnswer.length() - 1;
            while (start <= end && (rawAnswer[start] == ' ' || rawAnswer[start] == '\t' || rawAnswer[start] == '\n' || rawAnswer[start] == '\r')) {
                start++;
            }
            while (end >= start && (rawAnswer[end] == ' ' || rawAnswer[end] == '\t' || rawAnswer[end] == '\n' || rawAnswer[end] == '\r')) {
                end--;
            }
            
            riddles[riddleCount].answer = (start <= end) ? rawAnswer.substr(start, end - start + 1) : "";
            riddleCount++;
        }
    }
    
    file.close();
    return riddleCount;
}

void displayCharacters(Character characters[5], bool available[5]) {
    cout << "\n========== Available Characters ==========\n";
    for (int i = 0; i < 5; i++) {
        if (available[i]) {
            cout << (i + 1) << ". " << characters[i].name << "\n";
            cout << "   Experience: " << characters[i].experience << "\n";
            cout << "   Accuracy: " << characters[i].accuracy << "\n";
            cout << "   Efficiency: " << characters[i].efficiency << "\n";
            cout << "   Insight: " << characters[i].insight << "\n";
            cout << "   Discovery Points: " << characters[i].discoveryPoints << "\n\n";
        }
    }
}

Player setupPlayer(int playerNum, Character characters[5], bool available[5]) {
    Player player;
    
    cout << "\n========== Player " << playerNum << " Setup ==========\n";
    cout << "Enter your name: ";
    string playerName;
    cin >> playerName;
    player.setPlayerName(playerName);
    
    displayCharacters(characters, available);
    
    int charChoice;
    bool validChoice = false;
    while (!validChoice) {
        cout << "Choose your character (1-5): ";
        cin >> charChoice;
        charChoice--;
        
        if (charChoice >= 0 && charChoice < 5 && available[charChoice]) {
            validChoice = true;
            available[charChoice] = false;
        } else {
            cout << "Invalid choice. Please choose an available character.\n";
        }
    }
    
    Character chosen = characters[charChoice];
    player.setCharacterName(chosen.name);
    player.setExperience(chosen.experience);
    player.setAccuracy(chosen.accuracy);
    player.setEfficiency(chosen.efficiency);
    player.setInsight(chosen.insight);
    player.setDiscoveryPoints(chosen.discoveryPoints);
    
    cout << "\nChoose your path:\n";
    cout << "1. Training Fellowship (-5000 DP, +500 Accuracy, +500 Efficiency, +1000 Insight, Choose Advisor)\n";
    cout << "2. Direct Lab Assignment (+5000 DP, +200 Accuracy, +200 Efficiency, +200 Insight)\n";
    cout << "Enter your choice (1 or 2): ";
    
    int pathChoice;
    cin >> pathChoice;
    
    if (pathChoice == 1) {
        player.setPathType("Fellowship");
        player.FellowshipBonus();
        
        cout << "\n========== Choose Your Advisor ==========\n";
        cout << "1. Dr. Aliquot - Master of wet lab, avoids contamination\n";
        cout << "2. Dr. Assembler - Improves efficiency and streamlines pipelines\n";
        cout << "3. Dr. Pop-Gen - Identifies rare genetic variants\n";
        cout << "4. Dr. Bio-Script - Helps debug code\n";
        cout << "5. Dr. Loci - Assists in learning equipment\n";
        cout << "Choose your advisor (1-5): ";
        
        int advisorChoice;
        cin >> advisorChoice;
        
        string advisorNames[5] = {"Dr. Aliquot", "Dr. Assembler", "Dr. Pop-Gen", "Dr. Bio-Script", "Dr. Loci"};
        if (advisorChoice >= 1 && advisorChoice <= 5) {
            player.setAdvisorName(advisorNames[advisorChoice - 1]);
        }
    } else {
        player.setPathType("Lab");
        player.LabBonus();
        player.setAdvisorName("none");
    }
    
    player.setPosition(0);
    
    cout << "\nPlayer setup complete!\n";
    cout << "Final Stats:\n";
    cout << "Discovery Points: " << player.getDiscoveryPoints() << "\n";
    cout << "Accuracy: " << player.getAccuracy() << "\n";
    cout << "Efficiency: " << player.getEfficiency() << "\n";
    cout << "Insight: " << player.getInsight() << "\n";
    
    return player;
}

void sortLeaderboard(LeaderboardEntry entries[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (entries[j].finalScore < entries[j + 1].finalScore) {
                LeaderboardEntry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }
}

void saveToLeaderboard(string playerName, string characterName, int finalScore) {
    ofstream outFile("leaderboard.txt", ios::app);
    if (outFile.is_open()) {
        outFile << playerName << "|" << characterName << "|" << finalScore << "\n";
        outFile.close();
    }
}

void displayLeaderboard() {
    ifstream inFile("leaderboard.txt");
    if (!inFile.is_open()) {
        cout << "\nNo leaderboard history found yet.\n";
        return;
    }
    
    LeaderboardEntry entries[100];
    int count = 0;
    
    string line;
    while (getline(inFile, line) && count < 100) {
        int firstPipe = -1;
        int secondPipe = -1;
        
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '|') {
                if (firstPipe == -1) {
                    firstPipe = i;
                } else {
                    secondPipe = i;
                    break;
                }
            }
        }
        
        if (firstPipe != -1 && secondPipe != -1) {
            entries[count].playerName = line.substr(0, firstPipe);
            entries[count].characterName = line.substr(firstPipe + 1, secondPipe - firstPipe - 1);
            entries[count].finalScore = stoi(line.substr(secondPipe + 1));
            count++;
        }
    }
    
    inFile.close();
    
    if (count == 0) {
        cout << "\nNo leaderboard entries found.\n";
        return;
    }
    
    sortLeaderboard(entries, count);
    
    cout << "\n========================================\n";
    cout << "       ALL-TIME LEADERBOARD\n";
    cout << "========================================\n";
    
    int displayCount = (count < 10) ? count : 10;
    for (int i = 0; i < displayCount; i++) {
        cout << (i + 1) << ". " << entries[i].playerName 
             << " (" << entries[i].characterName << ") - " 
             << entries[i].finalScore << " DP\n";
    }
    
    cout << "========================================\n";
}

string generateRandomDNAStrand(int length) {
    string bases = "ATGC";
    string dnaStrand = "";
    
    for (int i = 0; i < length; i++) {
        dnaStrand += bases[rand() % 4];
    }
    
    return dnaStrand;
}

double strandSimilarity(string strand1, string strand2) {
    if (strand1.length() != strand2.length()) {
        return 0.0;
    }
    
    int matches = 0;
    for (int i = 0; i < strand1.length(); i++) {
        if (strand1[i] == strand2[i]) {
            matches++;
        }
    }
    
    return (double)matches / strand1.length();
}

int bestStrandMatch(string input_strand, string target_strand) {
    if (input_strand.length() < target_strand.length()) {
        return -1;
    }
    
    int bestIndex = 0;
    int maxMatches = 0;
    
    for (int i = 0; i <= (int)input_strand.length() - (int)target_strand.length(); i++) {
        int matches = 0;
        for (int j = 0; j < target_strand.length(); j++) {
            if (input_strand[i + j] == target_strand[j]) {
                matches++;
            }
        }
        
        if (matches > maxMatches) {
            maxMatches = matches;
            bestIndex = i;
        }
    }
    
    if (maxMatches == 0) {
        return -1;
    }
    
    return bestIndex;
}

void identifyMutations(string input_strand, string target_strand) {
    int startIndex = bestStrandMatch(input_strand, target_strand);
    
    cout << "\n========== Mutation Analysis ==========\n";
    cout << "Best alignment starts at position " << startIndex << "\n";
    
    int targetPos = 0;
    for (int i = startIndex; i < input_strand.length() && targetPos < target_strand.length(); i++) {
        if (input_strand[i] != target_strand[targetPos]) {
            cout << "Position " << i << ": Substitution - " << input_strand[i] << " -> " << target_strand[targetPos] << "\n";
        }
        targetPos++;
    }
    
    if (startIndex > 0) {
        cout << "Deletion of " << startIndex << " bases at the beginning\n";
    }
    
    if (input_strand.length() - startIndex > target_strand.length()) {
        cout << "Insertion detected at end\n";
    }
}

string transcribeDNAtoRNA(string strand) {
    string rna;
    rna.reserve(strand.length());
    for (int i = 0; i < (int)strand.length(); i++) {
        char c = strand[i];
        if (c == 'T' || c == 't') {
            rna.push_back('U');
        } else {
            rna.push_back((char)toupper(c));
        }
    }
    return rna;
}

Player handleGreenTile(Player player, RandomEvent events[100], int eventCount) {
    if (rand() % 100 < 50) {
        cout << "\n*** Random Event Triggered! ***\n";
        
        int eventIndex = rand() % eventCount;
        RandomEvent event = events[eventIndex];
        
        cout << event.description << "\n";
        
        string advisorName = player.getAdvisorName();
        int advisorId = 0;
        if (advisorName == "Dr. Aliquot") advisorId = 1;
        else if (advisorName == "Dr. Assembler") advisorId = 2;
        else if (advisorName == "Dr. Pop-Gen") advisorId = 3;
        else if (advisorName == "Dr. Bio-Script") advisorId = 4;
        else if (advisorName == "Dr. Loci") advisorId = 5;
        
        if (event.dpChange < 0 && event.advisorId == advisorId && advisorId != 0) {
            cout << "Your advisor " << advisorName << " protected you from this event!\n";
        } else {
            player.addDiscoveryPoints(event.dpChange);
            cout << "Discovery Points changed by " << event.dpChange << "\n";
            cout << "New DP: " << player.getDiscoveryPoints() << "\n";
        }
    } else {
        cout << "\nNothing happens on this tile.\n";
    }
    return player;
}

Player handleBlueTile(Player player) {
    cout << "\n*** Blue Tile: DNA Similarity Test (Equal Length) ***\n";
    
    int length = 8;
    string strand1 = generateRandomDNAStrand(length);
    string strand2 = generateRandomDNAStrand(length);
    
    cout << "Strand 1: " << strand1 << endl;
    cout << "Strand 2: " << strand2 << endl;
    cout << "Guess the similarity score (0.00 to 1.00): ";
    
    double userGuess;
    if (!(cin >> userGuess)) {
        cin.clear();
        string discard;
        getline(cin, discard);
        cout << "Invalid input. -100 Discovery Points\n";
        player.addDiscoveryPoints(-100);
        return player;
    }
    
    double actualSimilarity = strandSimilarity(strand1, strand2);
    cout << "Actual similarity: " << actualSimilarity << "\n";
    
    double difference = actualSimilarity - userGuess;
    if (difference < 0) {
        difference = -difference;
    }
    
    if (difference <= 0.1) {
        cout << "Excellent guess! +300 Discovery Points, +50 Accuracy\n";
        player.addDiscoveryPoints(300);
        player.changeStats(50, 0, 0);
    } else if (difference <= 0.25) {
        cout << "Good estimate! +150 Discovery Points\n";
        player.addDiscoveryPoints(150);
    } else if (difference <= 0.4) {
        cout << "Not bad! +50 Discovery Points\n";
        player.addDiscoveryPoints(50);
    } else {
        cout << "Way off. -100 Discovery Points\n";
        player.addDiscoveryPoints(-100);
    }
    
    return player;
}

Player handlePinkTile(Player player) {
    cout << "\n*** Pink Tile: DNA Similarity Test (Unequal Length) ***\n";

    int longLen = 15;
    int shortLen = 6;
    string longStrand = generateRandomDNAStrand(longLen);
    string shortStrand = generateRandomDNAStrand(shortLen);

    cout << "Long strand:  " << longStrand << endl;
    cout << "Short strand: " << shortStrand << "\n";
    cout << "The short strand can slide along the long strand to find the best match.\n";
    cout << "Guess the similarity score at the BEST alignment position (0.00 to 1.00): ";

    double userGuess;
    if (!(cin >> userGuess)) {
        cin.clear();
        string discard;
        getline(cin, discard);
        cout << "Invalid input. -50 Discovery Points\n";
        player.addDiscoveryPoints(-50);
        return player;
    }

    int bestIndex = bestStrandMatch(longStrand, shortStrand);
    if (bestIndex < 0) {
        cout << "No alignment found. -50 Discovery Points\n";
        player.addDiscoveryPoints(-50);
        return player;
    }

    int matches = 0;
    for (int i = 0; i < shortLen; i++) {
        if (longStrand[bestIndex + i] == shortStrand[i]) {
            matches++;
        }
    }
    double actualSimilarity = (double)matches / shortLen;
    
    cout << "Best alignment at position " << (bestIndex + 1) << "\n";
    cout << "Actual similarity at best position: " << actualSimilarity << "\n";

    double difference = actualSimilarity - userGuess;
    if (difference < 0) {
        difference = -difference;
    }

    if (difference <= 0.15) {
        cout << "Excellent! +250 Discovery Points, +50 Efficiency\n";
        player.addDiscoveryPoints(250);
        player.changeStats(0, 50, 0);
    } else if (difference <= 0.3) {
        cout << "Good estimate! +100 Discovery Points\n";
        player.addDiscoveryPoints(100);
    } else {
        cout << "Not quite. -50 Discovery Points\n";
        player.addDiscoveryPoints(-50);
    }
    
    return player;
}

Player handleRedTile(Player player) {
    cout << "\n*** Red Tile: Mutation Identification ***\n";
    
    int length = 8;
    string originalStrand = generateRandomDNAStrand(length);
    
    cout << "Original DNA strand: " << originalStrand << endl;
    cout << "Enter a mutated version of this strand: ";
    string mutatedStrand;
    cin >> mutatedStrand;
    
    identifyMutations(originalStrand, mutatedStrand);
    
    cout << "\nHow many substitution mutations did you introduce? ";
    int userSubstitutions;
    if (!(cin >> userSubstitutions)) {
        cin.clear();
        string discard;
        getline(cin, discard);
        cout << "Invalid input. -100 Discovery Points\n";
        player.addDiscoveryPoints(-100);
        return player;
    }
    
    int actualSubstitutions = 0;
    int minLen = (originalStrand.length() < mutatedStrand.length()) ? originalStrand.length() : mutatedStrand.length();
    for (int i = 0; i < minLen; i++) {
        if (originalStrand[i] != mutatedStrand[i]) {
            actualSubstitutions++;
        }
    }
    
    if (userSubstitutions == actualSubstitutions) {
        cout << "Correct! +400 Discovery Points, +100 Insight\n";
        player.addDiscoveryPoints(400);
        player.changeStats(0, 0, 100);
    } else {
        cout << "Incorrect. There were " << actualSubstitutions << " substitutions. +100 Discovery Points for trying\n";
        player.addDiscoveryPoints(100);
    }
    
    return player;
}

Player handleBrownTile(Player player) {
    cout << "\n*** Brown Tile: DNA to RNA Transcription ***\n";
    string dnaStrand = generateRandomDNAStrand(8);
    cout << "DNA strand: " << dnaStrand << endl;
    cout << "Enter the RNA transcription (replace all T with U): ";
    string userRNA;
    cin >> userRNA;

    for (int i = 0; i < (int)userRNA.length(); i++) {
        userRNA[i] = (char)toupper(userRNA[i]);
    }
    string correctRNA = transcribeDNAtoRNA(dnaStrand);

    if (userRNA == correctRNA) {
        cout << "Correct! +200 Discovery Points, +50 Accuracy\n";
        player.addDiscoveryPoints(200);
        player.changeStats(50, 0, 0);
    } else {
        cout << "Incorrect. The correct RNA is: " << correctRNA << "\n";
        cout << "-100 Discovery Points\n";
        player.addDiscoveryPoints(-100);
    }
    
    return player;
}

Player handlePurpleTile(Player player, Riddle riddles[25], int riddleCount) {
    cout << "\n*** Purple Tile: Riddle Time! ***\n";
    
    int riddleIndex = rand() % riddleCount;
    Riddle riddle = riddles[riddleIndex];
    
    cout << riddle.question << "\n";
    cout << "Your answer: ";
    string answer;
    cin >> answer;
    
    string lowerAnswer = answer;
    string lowerCorrect = riddle.answer;
    for (int i = 0; i < lowerAnswer.length(); i++) {
        lowerAnswer[i] = tolower(lowerAnswer[i]);
    }
    for (int i = 0; i < lowerCorrect.length(); i++) {
        lowerCorrect[i] = tolower(lowerCorrect[i]);
    }
    
    if (lowerAnswer == lowerCorrect) {
        cout << "Correct! +500 Insight Points\n";
        player.changeStats(0, 0, 500);
    } else {
        cout << "Incorrect. The answer was: " << riddle.answer << "\n";
        cout << "-200 Discovery Points\n";
        player.addDiscoveryPoints(-200);
    }

    return player;
}

Player displayMainMenu(Player player, Board &board, int playerIndex) {
    bool turnComplete = false;
    
    while (!turnComplete) {
        cout << "\n========== Main Menu ==========\n";
        cout << "1. Check Player Progress\n";
        cout << "2. Review Character\n";
        cout << "3. Check Position\n";
        cout << "4. Review your Advisor\n";
        cout << "5. Move Forward\n";
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            cout << "\n1. Review Discovery Points\n";
            cout << "2. Review Trait Stats\n";
            cout << "Enter your choice: ";
            int subChoice;
            cin >> subChoice;
            
            if (subChoice == 1) {
                cout << "Discovery Points: " << player.getDiscoveryPoints() << "\n";
            } else if (subChoice == 2) {
                cout << "Accuracy: " << player.getAccuracy() << "\n";
                cout << "Efficiency: " << player.getEfficiency() << "\n";
                cout << "Insight: " << player.getInsight() << "\n";
            }
        }
        else if (choice == 2) {
            cout << "\nCharacter: " << player.getCharacterName() << "\n";
            cout << "Experience: " << player.getExperience() << "\n";
        }
        else if (choice == 3) {
            cout << "\nCurrent Board:\n";
            board.displayBoard();
            cout << "Your position: Tile " << board.getPlayerPosition(playerIndex) << "\n";
        }
        else if (choice == 4) {
            string advisor = player.getAdvisorName();
            if (advisor == "" || advisor == "none") {
                cout << "\nYou do not have an advisor.\n";
            } else {
                cout << "\nYour advisor: " << advisor << "\n";
                cout << "1. Display advisor's abilities\n";
                cout << "2. Continue\n";
                cout << "Enter your choice: ";
                int advChoice;
                cin >> advChoice;
                
                if (advChoice == 1) {
                    cout << "Your advisor protects you from specific negative events related to their expertise.\n";
                }
            }
        }
        else if (choice == 5) {
            int spinResult = (rand() % 6) + 1;
            cout << "\nYou spun a " << spinResult << "!\n";
            
            for (int i = 0; i < spinResult; i++) {
                bool reachedEnd = board.movePlayer(playerIndex);
                if (reachedEnd) {
                    cout << "You've reached the Genome Conference!\n";
                    player.setPosition(51);
                    turnComplete = true;
                    break;
                }
            }
            
            if (!turnComplete) {
                int newPos = board.getPlayerPosition(playerIndex);
                player.setPosition(newPos);
                
                cout << "\nYou landed on tile " << newPos << "\n";
                board.displayBoard();
                
                turnComplete = true;
            }
        }
    }

    return player;
}

int main() {
    srand(time(0));
    
    cout << "========================================\n";
    cout << "  JOURNEY THROUGH THE GENOME\n";
    cout << "========================================\n\n";
    
    Character characters[5];
    RandomEvent events[100];
    Riddle riddles[25];
    
    loadCharacters(characters);
    int eventCount = loadRandomEvents(events);
    int riddleCount = loadRiddles(riddles);
    
    bool availableCharacters[5] = {true, true, true, true, true};
    
    Player player1 = setupPlayer(1, characters, availableCharacters);
    Player player2 = setupPlayer(2, characters, availableCharacters);
    
    Board gameBoard;
    gameBoard.initializeBoard();
    
    cout << "\n\nGame Starting!\n";
    gameBoard.displayBoard();

    cout << "\nWould you like to view the all-time leaderboard? (y/n): ";
    char viewLeaderboard;
    cin >> viewLeaderboard;
    if (viewLeaderboard == 'y' || viewLeaderboard == 'Y') {
        displayLeaderboard();
    }
    
    int currentPlayer = 0;
    bool gameOver = false;
    while (!gameOver) {
        
        Player activePlayer = (currentPlayer == 0) ? player1 : player2;
        
        cout << "\n\n========================================\n";
        cout << activePlayer.getPlayerName() << "'s Turn (" << activePlayer.getCharacterName() << ")\n";
        cout << "========================================\n";
       
        activePlayer = displayMainMenu(activePlayer, gameBoard, currentPlayer);
        
        int pos = gameBoard.getPlayerPosition(currentPlayer);
        if (pos > 0 && pos < 51) {
            char tileColor = gameBoard.getTileColor(currentPlayer, pos);
            
            cout << "\nYou landed on a ";
            switch(tileColor) {
                case 'G': cout << "GREEN tile!\n"; break;
                case 'B': cout << "BLUE tile!\n"; break;
                case 'P': cout << "PINK tile!\n"; break;
                case 'R': cout << "RED tile!\n"; break;
                case 'T': cout << "BROWN tile!\n"; break;
                case 'U': cout << "PURPLE tile!\n"; break;
                default:  cout << "UNKNOWN tile!\n"; break;
            }
            
            switch(tileColor) {
                case 'G':
                    activePlayer = handleGreenTile(activePlayer, events, eventCount);
                    break;
                case 'B':
                    activePlayer = handleBlueTile(activePlayer);
                    break;
                case 'P':
                    activePlayer = handlePinkTile(activePlayer);
                    break;
                case 'R':
                    activePlayer = handleRedTile(activePlayer);
                    break;
                case 'T':
                    activePlayer = handleBrownTile(activePlayer);
                    break;
                case 'U':
                    activePlayer = handlePurpleTile(activePlayer, riddles, riddleCount);
                    break;
                default:
                    break;
            }
            
            cout << "\nCurrent Stats:\n";
            cout << "Discovery Points: " << activePlayer.getDiscoveryPoints() << "\n";
            cout << "Accuracy: " << activePlayer.getAccuracy() << "\n";
            cout << "Efficiency: " << activePlayer.getEfficiency() << "\n";
            cout << "Insight: " << activePlayer.getInsight() << "\n";
        }
        
        if (currentPlayer == 0) {
            player1 = activePlayer;
        } else {
            player2 = activePlayer;
        }
        
        if (gameBoard.getPlayerPosition(0) >= 51 && gameBoard.getPlayerPosition(1) >= 51) {
            gameOver = true;
        }
        
        currentPlayer = (currentPlayer + 1) % 2;
    }
    
    cout << "\n\n========================================\n";
    cout << "  GENOME CONFERENCE REACHED!\n";
    cout << "  Calculating Final Scores...\n";
    cout << "========================================\n\n";
    
    int finalDP1 = player1.getDiscoveryPoints();
    finalDP1 += (player1.getAccuracy() / 100) * 1000;
    finalDP1 += (player1.getEfficiency() / 100) * 1000;
    finalDP1 += (player1.getInsight() / 100) * 1000;
    
    int finalDP2 = player2.getDiscoveryPoints();
    finalDP2 += (player2.getAccuracy() / 100) * 1000;
    finalDP2 += (player2.getEfficiency() / 100) * 1000;
    finalDP2 += (player2.getInsight() / 100) * 1000;
    
    saveToLeaderboard(player1.getPlayerName(), player1.getCharacterName(), finalDP1);
    saveToLeaderboard(player2.getPlayerName(), player2.getCharacterName(), finalDP2);

    cout << player1.getPlayerName() << " (" << player1.getCharacterName() << ")\n";
    cout << "  Final Discovery Points: " << finalDP1 << "\n\n";
    
    cout << player2.getPlayerName() << " (" << player2.getCharacterName() << ")\n";
    cout << "  Final Discovery Points: " << finalDP2 << "\n\n";
    
    if (finalDP1 > finalDP2) {
    cout << "🏆 " << player1.getPlayerName() << " is the new Lead Genomicist! 🏆\n";
    } else if (finalDP2 > finalDP1) {
    cout << "🏆 " << player2.getPlayerName() << " is the new Lead Genomicist! 🏆\n";
    } else {
    cout << "🏆 It's a tie! Both scientists share the Lead Genomicist position! 🏆\n";
    }

    ofstream outFile("game_results.txt");
    if (outFile.is_open()) {
        outFile << "Journey Through the Genome - Final Results\n\n";
        outFile << player1.getPlayerName() << " (" << player1.getCharacterName() << "): " << finalDP1 << " DP\n";
        outFile << player2.getPlayerName() << " (" << player2.getCharacterName() << "): " << finalDP2 << " DP\n";
        outFile.close();
        cout << "\nResults saved to game_results.txt\n";
    }



    return 0;

}
