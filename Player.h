#ifndef PLAYER_H
#define PLAYER_H
#include <string>


class Player {
    private: 
    std::string playerName;
    std::string pathType;
    std::string characterName;
    std::string advisorName;
    int experience; 
    int accuracy;
    int efficiency; 
    int insight;
    int discoveryPoints;
    int position;


    public:
        Player()
                : playerName(""), pathType(""), characterName(""), advisorName(""),
                    experience(0), accuracy(0), efficiency(0), insight(0), discoveryPoints(0), position(0)
        {}
    Player(std::string name, std::string character);
    std::string getPlayerName() const;
    std::string getCharacterName() const;
    std::string getPathType() const;
    std::string getAdvisorName() const;

    int getExperience() const;
    int getAccuracy() const;
    int getEfficiency() const;
    int getInsight() const;
    int getDiscoveryPoints() const;
    int getPosition() const;
    //setters now
    void setPlayerName(std::string name);
    void setCharacterName(std::string c);
    void setPathType(std::string type);
    void setAdvisorName(std::string advisor);
    void setPosition(int pos);

    void setExperience(int e);
    void setAccuracy(int a);
    void setEfficiency(int e);
    void setInsight(int i);
    void setDiscoveryPoints(int dp);
    //change stats
    void addDiscoveryPoints(int amount);
    void changeStats(int acc, int eff, int ins);
    //discoveryPoints based on chosen path
    void FellowshipBonus();
    void LabBonus();
};
#endif

