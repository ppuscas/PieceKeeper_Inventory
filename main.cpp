#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

bool goodLogin;

struct Location {
    int unitNum;
    int shelfNum;

    // constructor
    Location(int u, int s) :
    unitNum(u), shelfNum(s) { }
};

struct Item {
    string name;
    Location location;
    int buyPrice;
    int sellPrice;
    int partNum;
    int amount;

    Item* next;

    // constructor
    Item(string n, Location l, int b, int s, int p, int a) :
            name(n), location(l), buyPrice(b), sellPrice(s), partNum(p) , amount(a), next(nullptr) { }
};

void loadFile(const string& fname, fstream& file)
{
    file.open(fname.c_str());
    if (file.fail())
    {
        cout << "Cannot open file " << fname << endl;
    }
}

void login() {
    string line;
    string usr;
    string pw;
    string test_u, test_p;
    char splitter = ':';		//format for file will be: each line is one set of credentials, separated by colon. e.g. USERNAME:PASSWORD
    goodLogin = false;

    string credFileName = "credentials.txt";
    fstream credential;

    loadFile(credFileName, credential);

    cout << "Enter username: ";
    cin >> usr;
    cout << endl << "Enter password: ";
    cin >> pw;
    cout << endl;

    while (!goodLogin) {
        while (getline(credential, line)) {
            test_u = line.substr(0, line.find(splitter));
            test_p = line.substr(line.find(splitter) + 1);

            if (test_u == usr) {
                if (test_p == pw) {
                    cout << "good" << endl;
                    goodLogin = true;
                }
            }
        }
    }
    credential.close();
}

void addToInv(const Item& data, Item* head) {
    Item* newItem = new Item(data.name, data.location, data.buyPrice, data.sellPrice, data.partNum, data.amount );

    // Assign to head
    if (head == nullptr) {
        head = newItem;
        return;
    }

    // Traverse till end of list
    Item* temp = head;
    while (temp->next != nullptr) {

        // Update temp
        temp = temp->next;
    }

    // Insert at the last.
    temp->next = newItem;
}

Item* find(int part, Item* head) {

    Item* temp = head;
    while (temp->next != nullptr) {

        if(temp->partNum == part)
        {
            return temp;
        }
        // Update temp
        temp = temp->next;
    }
    temp = nullptr;
    return temp;
}


void addNumParts(int id, int amt, Item* head) {

    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        currentItem->amount += amt;
    }
}

void removeNumParts(int id, int amt, Item* head) {
    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        currentItem->amount -= amt;
    }
}

void changeBuyPrice(int id, int price, Item* head) {
    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        currentItem->buyPrice = price;
    }
}

void changeSellPrice(int id, int price, Item* head) {
    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        currentItem->sellPrice = price;
    }
}

void changeLocation(int id, int shelf, int unit, Item* head) {
    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        currentItem->location.shelfNum = shelf;
        currentItem->location.unitNum = unit;
    }

}

void printOneItem(int id, Item* head)  {
    Item* currentItem = find(id, head);

    if(currentItem != nullptr) {
        cout << "Part Name: " << currentItem->name << endl;
        cout << "Part Number: "<< currentItem->partNum << endl;
        cout << "Shelf Number: "<< currentItem->location.shelfNum << " ";
        cout << "Unit Number: "<< currentItem->location.unitNum << endl;
        cout << "Buy Price: $"<< currentItem->buyPrice << endl;
        cout << "Sell Price: $"<< currentItem->sellPrice << endl;
        cout << "Amount in Inventory: "<< currentItem->amount << endl;
    }
}

void printAllInv(Item* head)  {
    Item* currentItem = head;

    // Check for empty list.
    if (head == nullptr) {
        cout << "List empty" << endl;
        return;
    }

    // Traverse the list.
    while (currentItem != nullptr) {
        cout << "Part Name: " << currentItem->name << endl;
        cout << "Part Number: "<< currentItem->partNum << endl;
        cout << "Shelf Number: "<< currentItem->location.shelfNum << " ";
        cout << "Unit Number: "<< currentItem->location.unitNum << endl;
        cout << "Buy Price: $"<< currentItem->buyPrice << endl;
        cout << "Sell Price: $"<< currentItem->sellPrice << endl;
        cout << "Amount in Inventory: "<< currentItem->amount << endl;
        currentItem = currentItem->next;
    }
}


int main() {
    string inputFilename = "input.txt";
    string line;

    string input;
    // parse user input
    stringstream inputSS(input);
    vector<string> commands;

    Item *head = nullptr;

    // open input file
    fstream inputFile;
    loadFile(inputFilename, inputFile);

    while (getline(inputFile, line)) {
        // parse file input
        stringstream lineSS(line);
        string token;

        // store tokens in a vector
        vector<string> tokens;

        while (getline(lineSS, token, ' ')) {
            token.erase(token.find_last_not_of(" \n\r\t") + 1);
            tokens.push_back(token);
        }
        Location l(stoi(tokens.at(1)), stoi(tokens.at(2)));

        Item newItem(tokens.at(0), l, stoi(tokens.at(3)), stoi(tokens.at(4)), stoi(tokens.at(5)), stoi(tokens.at(6)));

        addToInv(newItem, head);
    }

    while(input != "quit") {

        cout << "Welcome to the Piece Keeper Inventory Management System." << endl;
        cout << "To access administrator privileges please authenticate using the login command: 'login'" << endl;
        cout << "To see a list of commands for Inventory Management please enter the help command: 'help'" << endl;
        cout << "To exit the inventory management system please enter the quit command: 'quit'" << endl << endl;

        cout << "Enter a command: ";
        cin >> input;

        if(input == "help") {
            cout << "Piece Keeper Inventory Commands:" << endl;
            cout << "list - Displays a list of all parts in inventory with all of their info" << endl;
            cout << "find 'part#' - Using the part number, finds a part and displays its info" << endl;
            cout << "-----------------------------------------------------------------------" << endl;
            cout << "Piece Keeper Inventory Administrator Commands:" << endl;
            cout << "add 'part#' 'amount' - Using the part number, adds amount to the available inventory" << endl;
            cout << "remove 'part#' 'amount' - Using the part number, removes amount from the available inventory" << endl;
            cout << "bprice 'part#' 'amount' - Using the part number, changes the buy price of an item" << endl;
            cout << "sprice 'part#' 'amount' - Using the part number, changes the sell price of an item" << endl;
            cout << "location 'part#' 'shelf#' 'unit#' - Using the part number, changes the location of an item" << endl;
        }

        if(input == "login") {
            login();
        }

        if(input == "list") {
            printAllInv(head);
        }

        if(commands.at(0) == "find") {
            printOneItem(stoi(commands.at(1)), head);
        }

        if(commands.at(0) == "add") {
            if(goodLogin) {
                addNumParts(stoi(commands.at(1)), stoi(commands.at(2)), head);
                printOneItem(stoi(commands.at(1)), head);
            }
            else {
                cout << "ERROR: Administrator commands require authentication. Please login to use this command." << endl;
            }
        }

        if(commands.at(0) == "remove") {
            if(goodLogin) {
                removeNumParts(stoi(commands.at(1)), stoi(commands.at(2)), head);
                printOneItem(stoi(commands.at(1)), head);
            }
            else {
                cout << "ERROR: Administrator commands require authentication. Please login to use this command." << endl;
            }
        }

        if(commands.at(0) == "bprice") {
            if(goodLogin) {
                changeBuyPrice(stoi(commands.at(1)), stoi(commands.at(2)), head);
                printOneItem(stoi(commands.at(1)), head);
            }
            else {
                cout << "ERROR: Administrator commands require authentication. Please login to use this command." << endl;
            }
        }

        if(commands.at(0) == "sprice") {
            if(goodLogin) {
                changeSellPrice(stoi(commands.at(1)), stoi(commands.at(2)), head);
                printOneItem(stoi(commands.at(1)), head);
            }
            else {
                cout << "ERROR: Administrator commands require authentication. Please login to use this command." << endl;
            }
        }

        if(commands.at(0) == "location") {
            if(goodLogin) {
                changeLocation(stoi(commands.at(1)), stoi(commands.at(2)), stoi(commands.at(3)), head);
                printOneItem(stoi(commands.at(1)), head);
            }
            else {
                cout << "ERROR: Administrator commands require authentication. Please login to use this command." << endl;
            }
        }

    }
    inputFile.close();
    system("pause");
    return EXIT_SUCCESS;
}
