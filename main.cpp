#include "Board.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/* 
Jack Rosenberg
Simon Logan
CSCI 1300 with Prof. Hoenigman
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

Character loadedCharacters[5];
RandomEvent events[100];
Riddle riddles[25];
int eventCount = 0;
int riddleCount = 0;

void loadCharacters() {
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
        
        loadedCharacters[index].name = fields[0];
        loadedCharacters[index].experience = stoi(fields[1]);
        loadedCharacters[index].accuracy = stoi(fields[2]);
        loadedCharacters[index].efficiency = stoi(fields[3]);
        loadedCharacters[index].insight = stoi(fields[4]);
        loadedCharacters[index].discoveryPoints = stoi(fields[5]);
        
        index++;
    }
    
    file.close();
}

void loadRandomEvents() {
    ifstream file("random_events.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open random_events.txt\n";
        return;
    }
    
    string line;
    getline(file, line);
    getline(file, line);
    
    eventCount = 0;
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
}

void loadRiddles() {
    ifstream file("riddles.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open riddles.txt\n";
        return;
    }
    
    string line;
    getline(file, line);
    
    riddleCount = 0;
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
            riddles[riddleCount].answer = line.substr(pipePos + 1);
            riddleCount++;
        }
    }
    
    file.close();
}

void displayCharacters(bool available[5]) {
    cout << "\n========== Available Characters ==========\n";
    for (int i = 0; i < 5; i++) {
        if (available[i]) {
            cout << (i + 1) << ". " << loadedCharacters[i].name << "\n";
            cout << "   Experience: " << loadedCharacters[i].experience << "\n";
            cout << "   Accuracy: " << loadedCharacters[i].accuracy << "\n";
            cout << "   Efficiency: " << loadedCharacters[i].efficiency << "\n";
            cout << "   Insight: " << loadedCharacters[i].insight << "\n";
            cout << "   Discovery Points: " << loadedCharacters[i].discoveryPoints << "\n\n";
        }
    }
}

Player setupPlayer(int playerNum, bool available[5]) {
    Player player;
    
    cout << "\n========== Player " << playerNum << " Setup ==========\n";
    cout << "Enter your name: ";
    string playerName;
    cin >> playerName;
    player.setPlayerName(playerName);
    
    displayCharacters(available);
    
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
    
    Character chosen = loadedCharacters[charChoice];
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
    
    for (int i = 0; i <= input_strand.length() - target_strand.length(); i++) {
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

void transcribeDNAtoRNA(string strand) {
    cout << "\n========== DNA to RNA Transcription ==========\n";
    cout << "DNA: " << strand << "\n";
    cout << "RNA: ";
    
    for (int i = 0; i < strand.length(); i++) {
        if (strand[i] == 'T' || strand[i] == 't') {
            cout << 'U';
        } else {
            cout << strand[i];
        }
    }
    cout << "\n";
}

void handleGreenTile(Player &player, Board &board, int playerIndex) {
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
}

void handleBlueTile(Player &player) {
    cout << "\n*** Blue Tile: DNA Similarity Test (Equal Length) ***\n";
    cout << "Enter first DNA strand: ";
    string strand1;
    cin >> strand1;
    cout << "Enter second DNA strand: ";
    string strand2;
    cin >> strand2;
    
    double similarity = strandSimilarity(strand1, strand2);
    cout << "Similarity score: " << similarity << "\n";
    
    if (similarity > 0.7) {
        cout << "Excellent work! +300 Discovery Points, +50 Accuracy\n";
        player.addDiscoveryPoints(300);
        player.changeStats(50, 0, 0);
    } else if (similarity > 0.4) {
        cout << "Good effort! +100 Discovery Points\n";
        player.addDiscoveryPoints(100);
    } else {
        cout << "Poor match. -100 Discovery Points\n";
        player.addDiscoveryPoints(-100);
    }
}

void handlePinkTile(Player &player) {
    cout << "\n*** Pink Tile: DNA Similarity Test (Unequal Length) ***\n";
    cout << "Enter input DNA strand: ";
    string input;
    cin >> input;
    cout << "Enter target DNA strand: ";
    string target;
    cin >> target;
    
    int bestMatch = bestStrandMatch(input, target);
    cout << "Best alignment starts at position: " << bestMatch << "\n";
    
    if (bestMatch >= 0) {
        cout << "Match found! +250 Discovery Points, +50 Efficiency\n";
        player.addDiscoveryPoints(250);
        player.changeStats(0, 50, 0);
    } else {
        cout << "No good match. -50 Discovery Points\n";
        player.addDiscoveryPoints(-50);
    }
}

void handleRedTile(Player &player) {
    cout << "\n*** Red Tile: Mutation Identification ***\n";
    cout << "Enter input DNA strand: ";
    string input;
    cin >> input;
    cout << "Enter target DNA strand: ";
    string target;
    cin >> target;
    
    identifyMutations(input, target);
    
    cout << "\nMutation analysis complete! +400 Discovery Points, +100 Insight\n";
    player.addDiscoveryPoints(400);
    player.changeStats(0, 0, 100);
}

void handleBrownTile(Player &player) {
    cout << "\n*** Brown Tile: DNA to RNA Transcription ***\n";
    cout << "Enter DNA strand to transcribe: ";
    string dna;
    cin >> dna;
    
    transcribeDNAtoRNA(dna);
    
    cout << "\nTranscription successful! +200 Discovery Points, +50 Accuracy\n";
    player.addDiscoveryPoints(200);
    player.changeStats(50, 0, 0);
}

void handlePurpleTile(Player &player) {
    cout << "\n*** Purple Tile: Riddle Time! ***\n";
    
    int riddleIndex = rand() % riddleCount;
    Riddle riddle = riddles[riddleIndex];
    
    cout << riddle.question << "\n";
    cout << "Your answer: ";
    string answer;
    cin >> answer;
    
    if (answer == riddle.answer) {
        cout << "Correct! +500 Insight Points\n";
        player.changeStats(0, 0, 500);
    } else {
        cout << "Incorrect. The answer was: " << riddle.answer << "\n";
        cout << "-200 Discovery Points\n";
        player.addDiscoveryPoints(-200);
    }
}

void displayMainMenu(Player &player, Board &board, int playerIndex) {
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
}

int main() {
    srand(time(0));
    
    cout << "========================================\n";
    cout << "  JOURNEY THROUGH THE GENOME\n";
    cout << "========================================\n\n";
    
    loadCharacters();
    loadRandomEvents();
    loadRiddles();
    
    bool availableCharacters[5] = {true, true, true, true, true};
    
    Player player1 = setupPlayer(1, availableCharacters);
    Player player2 = setupPlayer(2, availableCharacters);
    
    Board gameBoard;
    gameBoard.initializeBoard();
    
    cout << "\n\nGame Starting!\n";
    gameBoard.displayBoard();
    
    int currentPlayer = 0;
    bool gameOver = false;
    
    while (!gameOver) {
        Player* activePlayer = (currentPlayer == 0) ? &player1 : &player2;
        
        cout << "\n\n========================================\n";
        cout << activePlayer->getPlayerName() << "'s Turn (" << activePlayer->getCharacterName() << ")\n";
        cout << "========================================\n";
        
        displayMainMenu(*activePlayer, gameBoard, currentPlayer);
        
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
            }
            
            switch(tileColor) {
                case 'G':
                    handleGreenTile(*activePlayer, gameBoard, currentPlayer);
                    break;
                case 'B':
                    handleBlueTile(*activePlayer);
                    break;
                case 'P':
                    handlePinkTile(*activePlayer);
                    break;
                case 'R':
                    handleRedTile(*activePlayer);
                    break;
                case 'T':
                    handleBrownTile(*activePlayer);
                    break;
                case 'U':
                    handlePurpleTile(*activePlayer);
                    break;
            }
            
            cout << "\nCurrent Stats:\n";
            cout << "Discovery Points: " << activePlayer->getDiscoveryPoints() << "\n";
            cout << "Accuracy: " << activePlayer->getAccuracy() << "\n";
            cout << "Efficiency: " << activePlayer->getEfficiency() << "\n";
            cout << "Insight: " << activePlayer->getInsight() << "\n";
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