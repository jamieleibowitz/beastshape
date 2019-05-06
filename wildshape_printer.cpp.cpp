#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void printMainMenu(){
    printf("/***************************/\n\n");
    printf("1. New druid\n");
    printf("2. Load druid\n");
    printf("3. Save to file\n");
    printf("4. Lookup beast shapes\n");
    printf("5. Level up currently loaded druid\n");
    printf("6. Exit\n\n");
    printf("/***************************/\n\n");
}

struct beast{
    string name;
    string size;
    int hp;
    int ac;
    float cr;
    int swim;
    int fly;
};

struct druid{
    string name;
    int level;
    bool moon; //For checking if druid is of the Circle of the Moon
};

void loadcsv(vector<vector<string>> &data){
    //Open csv file with relevant data
    ifstream infile( "wild_shapes.csv" );
    //Looping through entire file
    while (infile) {
        string s;
        if (!getline(infile, s)) break; //Obtain next line, store in s
        //Place contents of s into an istringstream to delimit each line by commas
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string s;
            if (!getline( ss, s, ',' )) break; //Delimiting by commas
            record.push_back( s ); //Store in next spot in record vector
        }
        
        data.push_back( record ); //Place data from record into one element of data
    }
    /*Testing for successful data extraction
    for(int i = 1; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
           cout << data[i][j] << " ";
        }
        cout << endl;
    }*/
    infile.close();
}

int newDruid(druid &character) {
    string answer;
    bool exit = false;
    printf("Enter the name of your druid: ");
    cin >> character.name;
    printf("Enter your druid's level: ");
    cin >> character.level;
    while(!exit) { //loops until a valid answer is given
        printf("Is your druid initiated in the Circle of Moon? (y/n) ");
        cin >> answer;
        if(answer.compare("y")+1) {
            character.moon = true;
            exit = true;
        }
        else if (answer.compare("n")+1) {
            character.moon = false;
            exit = true;
        }
        else {
            printf("Error: please enter 'y' for yes or 'n' for no\n\n"); 
        }
    }
    printf("Character created!\n\n");
    return 1; //return value stored in the "loaded" variable in main as there is now valid data stored in the character struct
}

int loadDruid(druid &character) {
    printf("Enter your character's name (this is case sensitive!): ");
    string name;
    string csvIn;
    cin >> name; //get character name by user input
    name.append(".csv");
    ifstream saveFile(name); //open the file
    
    string s; //the string that will hold each cell from the csv as it comes in
    getline(saveFile, csvIn); //store the current line in csvIn
    istringstream ss(csvIn);
    getline(ss, s, ','); //Stores name in s
    character.name = s;
    getline(ss, s, ','); //Stores level in s
    character.level = stoi(s);
    getline(ss, s, ','); //Stores moon circle status in s
    character.moon = stoi(s);
    printf("Character loaded! \n\n");
    cout << "Name: " << character.name << ", Level: " << character.level << endl << endl;
    
    return 1; //return value stored in the "loaded" variable in main as there is now valid data stored in the character struct
    
}

void saveDruid(druid &character) {
    string outs = character.name; //This will be the string that is output to the csv file
    //Name the save file "{character's name}.csv"
    string outfile = character.name;
    outfile.append(".csv");
    outs.append(",");
    outs.append(to_string(character.level));
    outs.append(",");
    outs.append(to_string(character.moon)); //The output string is complete
    ofstream saveFile(outfile); //creating the save file and naming it
    saveFile << outs; //writing the string to the save file
    printf("Saved to file!\n\n");
}

int generateBeastList(druid &character, bool load, beast b[], int bsize) {
    if(!load) { //check to see if there is data in the character struct
        printf("Error: no druid loaded. Please create a new druid or load an existing one.\n\n");
        return 0;
    }
    float max_cr = 0;
    bool swim, fly; //level-based checks to see if character can transform into beasts with swim and/or fly speeds
    if(character.level < 2) max_cr = -1; //function will not print out any beasts for level 1 characters
    //setting conditions for what to print from b
    else if (character.level < 4) {
        max_cr = 0.25;
        swim = false;
        fly = false;
    }
    else if (character.level < 8) {
        max_cr = 0.5;
        swim = true;
        fly = false;
    }
    else if (character.level >= 8) {
        max_cr = 1;
        swim = true;
        fly = true;
    }
    if(character.moon && character.level >= 6) max_cr = character.level / 2;
    else if (character.moon) max_cr = 1;
    for(int i = 1; i < bsize; i++) {
        if(max_cr < 0) {
            printf("You must be at least level 2 to use beast shape!\n\n");
            return 0;
        }
        //all beasts with no flying or swimming speed are available as soon as the max_cr is within range
        if(b[i].swim == 0 && b[i].fly == 0 && b[i].cr <= max_cr){
            cout << b[i].name << ", " << b[i].size << endl;
            printf("HP: %d,  AC: %d,  CR: %f,  Swim Speed: %d, Fly Speed: %d\n\n", b[i].hp, b[i].ac, b[i].cr, b[i].swim, b[i].fly);
        }
        //if the character can choose swimming creatures but not flying creatures, don't print anything with a nonzero fly speed
        else if(swim && !fly && b[i].fly == 0 && b[i].cr <= max_cr){
            cout << b[i].name << ", " << b[i].size << endl;
            printf("HP: %d,  AC: %d,  CR: %f,  Swim Speed: %d, Fly Speed: %d\n\n", b[i].hp, b[i].ac, b[i].cr, b[i].swim, b[i].fly);
        }
        //at level 8 and up the only required check is the challenge rating (max_cr)
        else if(swim && fly && b[i].cr <= max_cr) {
            cout << b[i].name << ", " << b[i].size << endl;
            printf("HP: %d,  AC: %d,  CR: %f,  Swim Speed: %d, Fly Speed: %d\n\n", b[i].hp, b[i].ac, b[i].cr, b[i].swim, b[i].fly);
        }
    }
    return 0;
}

int levelUp(druid &character, bool load) {
    if(!load) {
        printf("Error: no druid loaded. Please create a new druid or load an existing one.\n\n");
        return 0;
    }
    cout << "Current level: " << character.level << endl;
    printf("Enter your new level: ");
    cin >> character.level;
    printf("Level updated! You are now level %d\n\n", character.level);
    return 0;
}

int main() {
    int choice;
    bool exit = false, loaded = false;
    vector<vector<string>> csvData;
    druid character;

    printf("Welcome to the D&D 5e Beast Shape assistant!\n\n");
    // Generate wild shape data
    loadcsv(csvData);
    beast allBeasts[csvData.size()];
    for(int i = 1; i < csvData.size(); i++){
        allBeasts[i].name = csvData[i][0];
        allBeasts[i].size = csvData[i][1];
        allBeasts[i].hp = stoi(csvData[i][2]);
        allBeasts[i].ac = stoi(csvData[i][3]);
        allBeasts[i].cr = stof(csvData[i][4]);
        allBeasts[i].swim = stoi(csvData[i][5]);
        allBeasts[i].fly = stoi(csvData[i][6]);
    }
    //Begin UI Loop
    while(!exit) {
        system("pause");
        printMainMenu();
        cin >> choice;
        if(choice == 1) { loaded = newDruid(character); }
        else if (choice == 2) { loaded = loadDruid(character); }
        else if (choice == 3) { saveDruid(character); }
        else if (choice == 4) { generateBeastList(character, loaded, allBeasts, csvData.size()); }
        else if (choice == 5) { levelUp(character, loaded); }
        else if (choice == 6) { break;}
        else {
            printf("Error, invalid entry. Choose an option\n\n"); 
        }
    }
    printf("Exiting program.\n\n");
    system("pause");
    return 0;
}

