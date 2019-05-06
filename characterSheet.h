#include <stdio.h>

class characterSheet {
    public: 
    characterSheet();
    characterSheet(int level0, int intelligence0, int wisdom0, int charisma0, bool moonCircle);
    void printCharacter();
    void levelUp();
    void updateAttribute(int type, int nval);


    private:
    int level;
    int intelligence;
    int wisdom;
    int charisma;
    bool moonCircle;
}