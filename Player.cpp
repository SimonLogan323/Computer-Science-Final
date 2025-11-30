#include "Player.h"
#include <string>
using std::string;

Player::Player() {
    playerName = "";
    characterName = "";
    pathType = "";
    advisorName = "";
    experience = 0;
    accuracy = 100;
    efficiency = 100;
    insight = 100;
    discoveryPoints = 0;
    position = 0;
}
Player::Player(string name, string character) {
    playerName = name;
    characterName = character;
    pathType = "";
    advisorName = "";
    experience = 0;
    accuracy = 100;
    efficiency = 100;
    insight = 100;
    discoveryPoints = 0;
    position = 0;
}
//getters now
string Player::getPlayerName() const { return playerName; }
string Player::getCharacterName() const { return characterName; }
string Player::getPathType() const { return pathType; }
string Player::getAdvisorName() const { return advisorName; }
int Player::getExperience() const { return experience; }
int Player::getAccuracy() const { return accuracy; }
int Player::getEfficiency() const { return efficiency; }
int Player::getInsight() const { return insight; }
int Player::getDiscoveryPoints() const { return discoveryPoints; }
int Player::getPosition() const { return position; }
//setters now

void Player::setPlayerName(string name) { playerName = name; }
void Player::setCharacterName(string c) { characterName = c; }
void Player::setPathType(string type) { pathType = type; }
void Player::setAdvisorName(string advisor) { advisorName = advisor; }
void Player::setPosition(int pos) { position = pos; }
void Player::setExperience(int e) { experience = e; }
void Player::setAccuracy(int a)
{
    accuracy = a;
    if (accuracy < 100)
    {
        accuracy = 100;
    }
}
void Player::setEfficiency(int eff)
{
    efficiency = eff;
    if (efficiency < 100)
    {
        efficiency = 100;
    }
}
void Player::setInsight(int i)
{
    insight = i;
    if (insight < 100)
    {
        insight = 100;
    }
}
void Player::setDiscoveryPoints(int dp) { discoveryPoints = dp; }
//stat stuff now

void Player::addDiscoveryPoints(int amount)
{
    discoveryPoints = discoveryPoints + amount;
}

void Player::changeStats(int acc, int eff, int ins)
{
    accuracy = accuracy + acc;
    efficiency = efficiency + eff;
    insight = insight + ins;

    if (accuracy < 100)
    {
        accuracy = 100;
    }
    if (efficiency < 100)
    {
        efficiency = 100;
    }
    if (insight < 100)
    {
        insight = 100;
    }
}
void Player::FellowshipBonus()
{
    
    discoveryPoints = discoveryPoints - 5000;
    accuracy = accuracy + 500;
    efficiency = efficiency + 500;
    insight = insight + 1000;

    if (accuracy < 100) { accuracy = 100; }
    if (efficiency < 100) { efficiency = 100; }
    if (insight < 100) { insight = 100; }
}
void Player::LabBonus()
{
    
    discoveryPoints = discoveryPoints + 5000;
    accuracy = accuracy + 200;
    efficiency = efficiency + 200;
    insight = insight + 200;

    if (accuracy < 100) { accuracy = 100; }
    if (efficiency < 100) { efficiency = 100; }
    if (insight < 100) { insight = 100; }
}
