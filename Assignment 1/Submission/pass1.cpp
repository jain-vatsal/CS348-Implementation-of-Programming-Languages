// This is the pass 1 of the 2 pass assembler
// RUN THIS BEFORE pass2.cpp

// including necessary header files
#include <bits/stdc++.h>
#include <vector>
using namespace std;

// necessary macros
typedef long long li;
#define vi vector<li>
#define vvi vector<vi>
#define pii pair<li, li>
#define mmp map<li, li> mp
#define rep(i, a, b) for (li i = a; i < b; i++)
const li N = 1e5 + 2, MOD = 1e9 + 7;

// this function is used remove whitespaces from a string
string whiteSpaceRemover(string str)
{
    string ans = ""; // empty string

    for (auto i : str) // iterating through the loop
        if (i != ' ')
            ans = ans + i;

    // returning the string without spaces
    return ans;
}

// this function is used to read the operation table from the "inputFiles/operationTable.txt" file and store it in a map
void operationTableReader(map<string, string> &operationTable)
{
    // reading file using ifstream class
    ifstream opTab("inputFiles/operationTable.txt");

    // check if the file is not opened
    if (!opTab.is_open())
    {
        cout << "Error in opening the operation table file" << endl;
        exit(0);
    }

    string line = ""; // string for iterating through the contents of the operationTable.txt file

    while (getline(opTab, line)) // reading the file sentence by sentence
    {
        // Extracting the mnemonic and code, remove whitespaces, and store in the map
        string mnemonic = whiteSpaceRemover(line.substr(0, 7));
        string code = whiteSpaceRemover(line.substr(7, 2));
        operationTable[mnemonic] = code;
    }
    // closing the file pointer
    opTab.close();
}

// this is a function to format a number with a specified width and option for hexadecimal
string format_number(string input, int width, bool hex)
{
    int num;
    // Convert the input string to an integer, either decimal or hexadecimal
    if (hex)
        num = stoi(input, NULL, 16);
    else
        num = stoi(input);

    // Formatting the name and converting it back to a string
    stringstream temp;
    temp << std::hex << std::uppercase << std::setfill('0') << std::setw(width) << num;
    return temp.str();
}

// this is a function to format a name with a specified width
string format_name(string name, int width)
{
    // Format the name and convert it back to a string
    stringstream temp;
    temp << std::left << std::setfill(' ') << std::setw(width) << name;
    return temp.str();
}
int main()
{
    // Initialize the operation table
    map<string, string> operationTable;
    operationTableReader(operationTable);

    // Initialize the symbol table
    map<string, int> symbolTable;

    // reading the input File
    ifstream inputFile("inputFiles/input.txt");

    // PLEASE MAKE SURE THE EMPTY DIRECTORY "pass1_outputFiles" IS CREATED.
    // declaring pointers for the output of pass1 files
    ofstream intermediateFile("pass1_outputFiles/intermediate.txt");
    ofstream symbolTableFile("pass1_outputFiles/symbolTable.txt");
    ofstream lengthFile("pass1_outputFiles/lengthFile.txt");

    // integers for storing length and addresses
    int locationCounter = 0, startingAddress = 0, LENGTH;

    // for checking error conditions
    bool error = false;

    // for reading the input file
    string line = "";

    while (getline(inputFile, line)) // reading the input file sentence by sentence
    {                               
        string label = "", opcode = "", operand = ""; // Extract label, opcode, and operand from the input line, remove whitespaces
        label = whiteSpaceRemover(line.substr(0, 10));
        if (label != ".")
        {
            opcode = whiteSpaceRemover(line.substr(10, 10));
            operand = whiteSpaceRemover(line.substr(20, 10));
        }

        // Check for comments and process the rest of the line if not a comment
        if (label != ".")
        {
            // Process START, END, and other instructions
            if (opcode == "START")
            {
                startingAddress = stoi(operand, NULL, 16);
                locationCounter = startingAddress;
                // Write the formatted line to the intermediate file
                intermediateFile << format_name(format_number(to_string(locationCounter), 4, false), 10) << line << '\n';
            }
            else if (opcode == "END")
            {
                LENGTH = locationCounter - startingAddress;
                intermediateFile << format_name("", 10) << line;
                break;
            }
            else
            {
                // Process labels, increment locationCounter based on the opcode
                if (label != "")
                {
                    if (symbolTable.find(label) == symbolTable.end())
                        symbolTable[label] = locationCounter;
                    else
                    {
                        error = true; // duplicate symbol
                        cout << "duplicate symbol\n";
                    }
                }

                // Write the formatted line to the intermediate file
                intermediateFile << format_name(format_number(to_string(locationCounter), 4, false), 10) << line << '\n';

               // Updating the locationCounter based on the opcode
                // handling special cases
                if (operationTable.find(opcode) != operationTable.end())
                    locationCounter += 3;
                else if (opcode == "WORD")
                    locationCounter += 3;
                else if (opcode == "RESW")
                    locationCounter += 3 * stoi(operand);
                else if (opcode == "RESB")
                    locationCounter += stoi(operand);
                else if (opcode == "BYTE")
                {
                    if (operand[0] == 'C')
                        locationCounter += (operand.length() - 3);
                    if (operand[0] == 'X')
                        locationCounter += (operand.length() - 3) / 2;
                }
                else
                {
                    error = true; // invalid operation code
                    cout << "invalid operation code\n";
                }
            }
        }
        else
            intermediateFile << line << '\n';
    }

    // writing the length of the program to an output file
    lengthFile << to_string(LENGTH) << endl;

    // writing onto the symbolTable file
    for (auto i : symbolTable)
    {
        string str = i.first;
        while (str.length() < 11)
        {
            str = str + ' ';
        }
        str = str + to_string(i.second);
        symbolTableFile << str << endl;
    }

    // Close the input and intermediate files
    inputFile.close();
    intermediateFile.close();
    lengthFile.close();
    symbolTableFile.close();
    
    return 0;
}