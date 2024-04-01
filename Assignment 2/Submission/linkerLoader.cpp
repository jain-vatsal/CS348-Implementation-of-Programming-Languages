#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
using namespace std;
typedef long long int li;
#define rep(i, a, b) for (int i = a; i < b; i++)

int programAddress, lastAddress;        // program and last address
vector<string> memory((1 << 18), "xx"); // memory vector initialized with xx
map<string, int> externalSymbolTable;   // external symbol table

void passOneOfLinkerLoader(string input)
{
    ifstream inputFileReader(input);

    // check if file is successfully opened
    if (!inputFileReader.is_open())
    {
        perror("error in reading the input file in passOneOfLinkerLoader");
        exit(1);
    }

    // ask user for the address of the program
    cout << "Enter programAddress for the program: ";
    string addressForProgram;
    cin >> addressForProgram;

    programAddress = stoi(addressForProgram, nullptr, 16); // set programAddress to the input hexadecimal value
    int controlSectionAddress = programAddress, controlSectionLength = 0;
    string record = "";

    // read all records till the end of file
    while (getline(inputFileReader, record))
    {
        // break loop on reaching end of file
        if (record.empty())
            break;

        // Process Header records
        if (record.front() == 'H')
        {
            controlSectionAddress = controlSectionAddress + controlSectionLength;
            string controlSection = record.substr(1, 6);
            controlSectionLength = stoi(record.substr(13, 6), nullptr, 16);

            if (externalSymbolTable.find(controlSection) == externalSymbolTable.end())
                externalSymbolTable.insert({controlSection, controlSectionAddress}); // Insert controlSection and controlSectionAddress into ESTAB if not already present
            else
            {
                perror("duplicate external symbol"); // error message
                exit(1);
            }
        }

        // Process Define records
        if (record.front() == 'D')
        {
            vector<pair<string, int>> symbols;
            int n = record.length(), ite = 1;
            while (ite < n)
            {
                symbols.push_back(make_pair(record.substr(ite, 6), stoi(record.substr(ite + 6, 6), nullptr, 16)));
                ite = ite + 12;
            }
            for (int ite = 0; ite < symbols.size(); ite++) // iterating through all the symbols
            {
                auto symbol = symbols[ite];
                // symbols from Define records are added them to ESTAB
                if (externalSymbolTable.find(symbol.first) != externalSymbolTable.end())
                {
                    perror("The external symbol is a duplicate.");
                    exit(1);
                }
                else
                    externalSymbolTable.insert({symbol.first, symbol.second + controlSectionAddress});
            }
        }
    }
    lastAddress = controlSectionAddress + controlSectionLength; // Update LAST with the end address of the program
}

// Function to remove blank spaces from a string
string removeBlankSpaces(string s)
{
    string ans = "";
    li n = s.length();
    rep(i, 0, n) if (s[i] != ' ')
        ans += s[i];
    return ans;
}
// Function to convert a number to hexadecimal string
string numberToHexaDecimal(int num)
{
    stringstream ss;
    ss << hex << num;
    return ss.str();
}

// Function to format a number to a desired width with a specified filler character
string numberFormatter(int actualNumber, int desiredWidth, char filler = '0')
{
    stringstream t;
    t << hex << uppercase;
    string str = numberToHexaDecimal(actualNumber);
    int k = str.length();
    rep(i, 0, desiredWidth - k) t << filler;
    t << actualNumber;
    string desiredString = t.str();
    return desiredString;
}

// Function to format a string to a desired width with a specified padding character
string stringFormatter(string name, int desiredWidth, char pad = ' ')
{
    string ans = "";
    int n = name.length();
    int k = desiredWidth - n;
    ans += name;
    while (k > 0)
    {
        ans += pad;
        k--;
    }
    return ans;
}

void passTwoOfLinkerLoader(string input)
{
    ifstream inputFileReader(input);
    // inputFileReader.open(input, ios::in);

    // Check if input file is successfully opened
    if (!inputFileReader.is_open())
    {
        perror("can't open the input file in passTwoOfLinkerLoader"); //  error message
        exit(1);                                                      // terminate program
    }
    int controlSectionAddress = programAddress, EXECADDR = programAddress, controlSectionLength = 0;
    string record = "";

    // Read records from the input file until end of file
    while (getline(inputFileReader, record))
    {
        // Break loop if end of file is reached
        if (record.empty())
            break;

        // process header records
        if (record.front() == 'H')
            controlSectionLength = stoi(record.substr(13, 6), nullptr, 16);
        // process text records
        if (record.front() == 'T')
        {
            int startingAddress = stoi(record.substr(1, 6), nullptr, 16) + controlSectionAddress, ind = 0, ite = 9;
            // store object code in memory
            while (ite < (int)record.length())
            {
                memory[startingAddress + ind++] = record.substr(ite, 2);
                ite += 2;
            }
        }
        // process modification records
        else if (record.front() == 'M')
        {
            string symbol = record.substr(10, 6);

            // check if symbol is defined in ESTAB
            if (externalSymbolTable.find(symbol) != externalSymbolTable.end())
            {
                string val = "";
                int address = stoi(record.substr(1, 6), nullptr, 16) + controlSectionAddress, length = stoi(record.substr(7, 2), nullptr, 16);
                char hByte;
                if (length & 1)
                    hByte = memory[address][0];

                // Extract the value to be modified from memory
                int ite = 0;
                while (ite < (length + 1) / 2)
                {
                    val += memory[address + ite];
                    ite++;
                }
                int value = stoi(val, nullptr, 16), modification = externalSymbolTable[symbol];
                cout << "value          = " << val << '\n';

                // Apply modification based on the sign
                if (record[9] == '+')
                    value += modification;
                else
                    value -= modification;
                int mask = 0, ji = 0;

                // Apply bitmask to the modified value
                int alpha = length + (length % 2);
                while (ji < 4 * alpha)
                {
                    mask |= 1;
                    mask = mask << 1;
                    ji++;
                }
                mask = mask >> 1;
                value = value & mask;
                val = numberFormatter(value, alpha);

                ji = 0;

                // printing to the console
                cout << "modification   = " << numberFormatter(modification, 6) << endl
                     << "modified value = " << val << '\n';

                // Store modified value back in memory
                while (ji < length)
                {
                    memory[address + ji / 2] = val.substr(ji, 2);
                    ji += 2;
                }
                // Restore the half byte if the length is odd
                if (length & 1)
                    memory[address][0] = hByte;
            }
            else
            {
                cout << symbol << '\n';            // Print undefined symbol
                perror("The symbol is undefined"); // ERROR MESSAGE
                exit(1);
            }
        }

        // Process End records
        if (record.front() == 'E')
        {
            if (record != "E")
            {
                int FIRST = stoi(record.substr(1, 6), nullptr, 16);
                EXECADDR = controlSectionAddress + FIRST;
            }
            controlSectionAddress = controlSectionLength + controlSectionAddress; // Move to the next control section
        }
    }
    cout << "Starting execution at: " << numberFormatter(EXECADDR, 4) << '\n';
}
// Function to format memory content and write it to a file
void formatmemory()
{
    ofstream memoryFile("outputFromLinkerLoader/memory.txt");               // File stream object for output
    int i = (programAddress / 16) * 16, n = ((lastAddress + 16) / 16) * 16; // Calculate starting and ending addresses to format memory content
    // Loop through memory content in blocks of 16 bytes
    while (i < n)
    {
        memoryFile << numberFormatter(i, 4) << ' '; // Write formatted memory address to file
        // Loop through 16 bytes of memory content
        for (int j = 0; j < 4; j++)
        {
            // Loop through each byte within a block
            for (int k = 0; k < 4; k++)
                memoryFile << memory[i++]; // Write formatted memory content byte to file
            memoryFile << ' ';             // Write space between bytes
        }
        memoryFile << '\n'; // Write newline character to move to the next line
    }
    memoryFile.close(); // Close the output file
}

// Function to print memory content line by line with addresses and instructions
void printmemory()
{

    ofstream lineByLine("outputFromLinkerLoader/lineByLine.txt"); // Open output file named "lineByLine.txt" in write mode
    ifstream listing("outputFromPass2/listing.txt");       // Open input file named "listing.txt" in read mode
    // Check if input file is successfully opened
    if (!listing.is_open())
    {
        perror("listing.txt"); // Print error message
        exit(1);               // Terminate program
    }
    string line="";                 // Variable to store lines read from input file
    vector<string> instructions; // Vector to store instructions
    // Read lines from the input file until end of file
    while (getline(listing, line)) // Read a line from the file
    {
        // Break loop if end of file is reached
        if (line.empty())
            break;
        // Extract object code from listing and add to instructions vector
        string objectCode = removeBlankSpaces(line.substr(62, 10));
        if (!objectCode.empty())
            instructions.push_back(objectCode);
    }
    vector<string> answer, address; // Vectors to store formatted memory content and addresses
    int j = 0, i = 0;               // Variables for inding
    // Construct memory content line by line with addresses and instructions
    while (i < (int)(memory.size()))
    {
        if (memory[i] != "xx") // Check if memory location is not empty
        {
            string temp = "";                         // Initialize string to store memory content
            address.push_back(numberFormatter(i, 6)); // Format memory address and add to vector
            int size = instructions[j].length() / 2;  // Calculate size of instruction
            // Construct instruction by appending memory content
            int k = i;
            while (k < i + size)
            {
                temp += memory[k];
                k++;
            }                       // Append memory content to instruction
            answer.push_back(temp); // Add instruction to answer vector
            j = j + 1;              // Increment instruction ind
            i = i + size;           // Move to next memory location
        }
        else
            i++; // Move to next memory location
    }
    lineByLine << stringFormatter("address", 10) << stringFormatter("instruction", 10) << '\n'; // Write header to file
    // Write memory content line by line with addresses and instructions to file
    for (int i = 0; i < (int)answer.size(); i++)
        lineByLine << stringFormatter(address[i], 10) << stringFormatter(answer[i], 10) << '\n';
    lineByLine.close(); // Close the output file
    listing.close();    // Close the input file
}

int main()
{
    passOneOfLinkerLoader("outputFromPass2/output.txt");
    for (auto i : externalSymbolTable)
    {
        string p = i.first;
        string q = numberFormatter(i.second, 4);
        cout << p << " " << q << endl;
    }
    passTwoOfLinkerLoader("outputFromPass2/output.txt");
    formatmemory();
    printmemory();
    return 0;
}