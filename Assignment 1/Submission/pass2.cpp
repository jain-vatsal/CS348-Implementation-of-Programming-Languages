// this is the pass 2 of the two pass assembler
// RUN THIS PROGRAM AFTER RUNNING pass1.cpp

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


// this is a function to remove whitespaces from a string
int stringtoInteger(string str)
{
    int ans = 0; // integer

    for (auto i : str) // iterating through the string and updating the number
        ans = ans * 10 + (i - '0');

    // returning the number
    return ans;
}
// this is a function to read the length of the program given as input in pass 1 and written to an output file of pass 1
int lengthReader()
{
    // opening the length file using ifstream class
    ifstream lengthFile("pass1_outputFiles/lengthFile.txt");

    // checking if the length file is opened correctly
    if (!lengthFile.is_open())
    {
        cout << "Error in opening the length file" << endl;
        exit(0);
    }

    // integer
    int ans = 0;
    string str = "";                 // for extracting the strings
    while (getline(lengthFile, str)) // reading sentences line by line
    {
        ans = stringtoInteger(str); // conversion into integer
        break;                      // breaking after one iteration as only one number has to be read
    }
    // returning the length of the program as an integer
    return ans;
}

// Function to remove whitespaces from a string
string whiteSpaceRemover(string str)
{
    string ans = "";
    for (auto i : str)
        if (i != ' ')
            ans = ans + i;
    return ans;
}



// Function to read the operation table from a file and store it in a map
void operationTableReader(map<string, string> &operationTable)
{
    ifstream opTab("inputFiles/operationTable.txt");
    if (!opTab.is_open())
    {
        cout << "Error in opening the operation table file" << endl;
        exit(0);
    }

    string line = "";
    while (getline(opTab, line))
    {
        // Extract mnemonic and code, remove whitespaces, and store in the map
        string mnemonic = whiteSpaceRemover(line.substr(0, 7));
        string code = whiteSpaceRemover(line.substr(7, 2));
        operationTable[mnemonic] = code;
    }

    opTab.close();
}

// this is a function to read the symbol table from the file "pass1_outputFiles/symbolTable.txt" and store it in a map
void symbolTableReader(map<string, int> &symbolTable)
{

    // opening the symbol table file using ifstream class
    ifstream symTab("pass1_outputFiles/symbolTable.txt");

    // checking if the symbol table file is opened correctly
    if (!symTab.is_open())
    {
        cout << "Error in opening the symbol table file" << endl;
        exit(0);
    }

    string line = "";             // for extracting the strings
    while (getline(symTab, line)) // reading sentences line by line
    {
        // Extracting mnemonic and code, remove whitespaces, and store in the map
        string mnemonic = whiteSpaceRemover(line.substr(0, 10));
        string code = whiteSpaceRemover(line.substr(11, 4));
        symbolTable[mnemonic] = stringtoInteger(code);
    }

    // closing the symbol table file pointer
    symTab.close();
}


// Function to format a number with a specified width and option for hexadecimal
string format_number(string input, int width, bool hex)
{
    int num;
    // Convert the input string to an integer, either decimal or hexadecimal
    if (hex)
        num = stoi(input, NULL, 16);
    else
        num = stoi(input);

    // Format the number and convert it back to a string
    stringstream temp;
    temp << std::hex << std::uppercase << std::setfill('0') << std::setw(width) << num;
    return temp.str();
}

// Function to format a name with a specified width
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
    symbolTableReader(symbolTable);

    // reading the length of the program given as input to pass 1
    int LENGTH = lengthReader();

    // reading the intermediate file which is the output of pass 1 and input to pass 2
    ifstream intermediateFile("pass1_outputFiles/intermediate.txt");

    // declaring the pointers to the output files
    ofstream outputFile("pass2_outputFiles/output.txt");
    ofstream listingFile("pass2_outputFiles/listing.txt");

    string textRecord = "", startingAddress = "";
    // declaring empty strings for convenience

    // error flag
    bool error = false;

    // for reading the sentences in input file(intermediate)
    string line = "";

    // Process each line in the intermediate file
    while (getline(intermediateFile, line))
    {
        // extracting label,opcode,operand
        string label = "", opcode = "", operand = "", locationCounter = "";
        locationCounter = whiteSpaceRemover(line.substr(0, 10));

        // Check if the line is a comment
        if (locationCounter == ".")
            label = whiteSpaceRemover(line.substr(0, 10));
        else
        {
            // Extract label, opcode, and operand from the line, remove whitespaces
            label = whiteSpaceRemover(line.substr(10, 10));
            opcode = whiteSpaceRemover(line.substr(20, 10));
            operand = whiteSpaceRemover(line.substr(30, 10));
        }

        // Check for comments and process the rest of the line if not a comment
        if (label != ".")
        {
            // Process START, END, and other instructions
            if (opcode == "START")
            {
                // Write the header record to the output file
                stringstream header;
                header << "H";
                header << format_name(label, 6) << format_number(operand, 6, true) << format_number(to_string(LENGTH), 6, false);
                outputFile << header.str() << '\n';
                listingFile << line << '\n';
            }
            else if (opcode == "END")
            {
                // Write the end record to the output file
                if (textRecord.length())
                {
                    int length = (textRecord.length()) / 2;
                    outputFile << "T" << format_number(startingAddress, 6, true) << format_number(to_string(length), 2, false) << textRecord << '\n';
                }
                stringstream end;
                int first = symbolTable[operand];
                end << "E";
                end << format_number(to_string(first), 6, false);
                outputFile << end.str() << '\n';
                listingFile << line;
            }
            else
            {
                string obcode = "";
                if (operationTable.find(opcode) != operationTable.end())
                {
                    // Process standard instructions, handle operands and symbols
                    obcode = operationTable[opcode];
                    if (operand != "")
                    {
                        int r1 = 0;
                        if (operand.find(",") != string::npos)
                        {
                            operand.resize(operand.find(","));
                            r1 += 32768;
                        }
                        if (symbolTable.find(operand) != symbolTable.end())
                        {
                            r1 += symbolTable[operand];
                            obcode += format_number(to_string(r1), 4, false);
                        }
                        else
                        {
                            obcode += format_number("0", 4, false);
                            error = true; // undefined symbol
                            cout << "undefined symbol\n";
                        }
                    }
                    else
                        obcode += format_number("0", 4, false);
                    listingFile << line << format_name(obcode, 10) << '\n';
                }
                else
                {
                    // Process BYTE and WORD instructions
                    if (opcode == "BYTE")
                    {
                        if (operand[0] == 'C')
                        {
                            string constant = operand.substr(2, operand.length() - 3);
                            for (char ch : constant)
                            {
                                int x = ch;
                                obcode += format_number(to_string(x), 2, false);
                            }
                        }
                        else
                        {
                            string constant = operand.substr(2, operand.length() - 3);
                            obcode += format_number(constant, constant.length(), true);
                        }
                    }
                    else if (opcode == "WORD")
                        obcode += format_number(operand, 6, false);

                    listingFile << line << format_name(obcode, 10) << '\n';
                }

                // Build the text record for object code
                if (textRecord.length() == 0)
                    startingAddress = locationCounter;

                if (textRecord.length() + obcode.length() <= 60 && opcode != "RESW" && opcode != "RESB")
                    textRecord += obcode;
                else
                {
                    // Write the text record to the output file
                    if (textRecord.length())
                    {
                        int length = (textRecord.length()) / 2;
                        outputFile << "T" << format_number(startingAddress, 6, true) << format_number(to_string(length), 2, false) << textRecord << '\n';
                        textRecord = obcode;
                        startingAddress = locationCounter;
                    }
                }
            }
        }
        else
            listingFile << line << '\n';
    }

    // closing all the opened file pointers
    intermediateFile.close();
    outputFile.close();
    listingFile.close();
    return 0;
}