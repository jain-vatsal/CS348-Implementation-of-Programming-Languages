// Include libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
// Avoid using this - it includes many unnecessary headers
// #include <bits/stdc++.h>

// Define type alias for long long integer
typedef long long int li;

// Define macro for loop iteration
#define rep(i, a, b) for (int i = a; i < b; i++)
// Define macro for pushing elements into a vector
#define pb push_back
// Define pair of integers
#define pii pair<int, int>

// Use standard namespace
using namespace std;

// Function to remove blank spaces from a string
string removeBlankSpaces(string s)
{
    string ans = "";
    li n = s.length();
    rep(i, 0, n)
    {
        if (s[i] != ' ')
            ans += s[i];
    }
    return ans;
}

// Function to convert a number to hexadecimal string
string numberToHexaDecimal(int num)
{
    stringstream ss;
    ss << hex << num;
    return ss.str();
}

// Function to format a number with desired width and filler character
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

// Function to format a string with desired width and padding character
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

// Function to check if a string represents a number
bool is_number(string s)
{
    int n = s.length();
    rep(i, 0, n)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return false;
        }
    }
    return true;
}

// Function to apply a mask to an integer value
void apply_mask(int &value, int bits)
{
    if (value < 0)
    {
        int mask = 0;
        rep(i, 0, bits)
        {
            mask |= 1;
            mask = mask << 1;
        }
        mask = mask >> 1;
        value = value & mask;
    }
}

// Function to map opcodes to their respective opcode format and machine code
pii OPTAB(string opcode)
{
    // Mapping opcodes to their respective opcode format and machine code
    if (opcode == "LDA")
        return make_pair(3, 0x00);
    if (opcode == "LDX")
        return make_pair(3, 0x04);
    if (opcode == "LDL")
        return make_pair(3, 0x08);
    if (opcode == "LDB")
        return make_pair(3, 0x68);
    if (opcode == "LDT")
        return make_pair(3, 0x74);
    if (opcode == "STA")
        return make_pair(3, 0x0C);
    if (opcode == "STX")
        return make_pair(3, 0x10);
    if (opcode == "STL")
        return make_pair(3, 0x14);
    if (opcode == "LDCH")
        return make_pair(3, 0x50);
    if (opcode == "STCH")
        return make_pair(3, 0x54);
    if (opcode == "ADD")
        return make_pair(3, 0x18);
    if (opcode == "SUB")
        return make_pair(3, 0x1C);
    if (opcode == "MUL")
        return make_pair(3, 0x20);
    if (opcode == "DIV")
        return make_pair(3, 0x24);
    if (opcode == "COMP")
        return make_pair(3, 0x28);
    if (opcode == "COMPR")
        return make_pair(2, 0xA0);
    if (opcode == "CLEAR")
        return make_pair(2, 0xB4);
    if (opcode == "J")
        return make_pair(3, 0x3C);
    if (opcode == "JLT")
        return make_pair(3, 0x38);
    if (opcode == "JEQ")
        return make_pair(3, 0x30);
    if (opcode == "JGT")
        return make_pair(3, 0x34);
    if (opcode == "JSUB")
        return make_pair(3, 0x48);
    if (opcode == "RSUB")
        return make_pair(3, 0x4C);
    if (opcode == "TIX")
        return make_pair(3, 0x2C);
    if (opcode == "TIXR")
        return make_pair(2, 0xB8);
    if (opcode == "TD")
        return make_pair(3, 0xE0);
    if (opcode == "RD")
        return make_pair(3, 0xD8);
    if (opcode == "WD")
        return make_pair(3, 0xDC);
    return make_pair(-1, -1); // Default case for unknown opcodes
}
// Struct for representing a symbol
struct SYMBOL
{
    string CSECT; // Control Section
    string name;  // Name of the symbol
    SYMBOL(string CSECT, string name)
    {
        this->CSECT = CSECT, this->name = name;
    }
    // Overloading < operator for sorting purposes
    bool operator<(const SYMBOL &s) const
    {
        return make_pair(this->CSECT, this->name) < make_pair(s.CSECT, s.name);
    }
};

// Struct for representing a value
struct VALUE
{
    int value, length; // Value and length
    bool type;         // Type of the value
    VALUE(int value = 0, int length = 3, bool type = 1)
    {
        this->value = value, this->length = length, this->type = type;
    }
};
// Map for storing symbol table
map<SYMBOL, VALUE> symbolTable;

// function to convert a string to an integer
int convertStringToInteger(string s)
{
    int sum = 0;
    int n = s.length();
    int i = 0;
    while (i < n)
    {
        sum = sum * 10 + (s[i] - '0');
        i++;
    }
    return sum;
}

// function to extract all strings from a line, consisting of multiple strings
vector<string> extractAllStrings(string line)
{
    vector<string> ans;
    int n = line.length(), i = 0;
    while (i < n)
    {
        string k = "";
        int j = i;
        while (j < n && line[j] != ' ')
        {
            k += line[j];
            j++;
        }
        ans.pb(k);
        i = j + 1;
    }
    return ans;
}

// read the symbolTable from the symbolTable.txt file
void symbolTableReader()
{
    ifstream symbolTableFile("outputFromPass1/symbolTable.txt");
    string line = "";

    while (getline(symbolTableFile, line)) // extract each line
    {
        vector<string> allStrings = extractAllStrings(line);
        // insert the strings into objects
        SYMBOL s(allStrings[0], allStrings[1]);
        VALUE v(convertStringToInteger(allStrings[4]), convertStringToInteger(allStrings[2]), convertStringToInteger(allStrings[3]));

        // storing in symbolTable
        symbolTable.insert({s, v});
        line = "";
    }
    symbolTableFile.close();
}

// Map for storing literal table
map<SYMBOL, VALUE> literalTable;

// read the literalTable from the literalTable.txt file
void literalTableReader()
{
    ifstream literalTableFile("outputFromPass1/literalTable.txt");
    string line = "";
    while (getline(literalTableFile, line)) // extract each line
    {
        vector<string> allStrings = extractAllStrings(line);

        // insert the strings into objects
        SYMBOL s(allStrings[0], allStrings[1]);
        VALUE v(convertStringToInteger(allStrings[4]), convertStringToInteger(allStrings[2]), convertStringToInteger(allStrings[3]));

        // storing in symbolTable
        literalTable.insert({s, v});
        line = "";
    }
    literalTableFile.close();
}

set<string> EXTREF;

// This function determines the precedence of an operator.
// Operators with higher precedence will be evaluated first.
int precedence(string op)
{
    // If the operator is addition or subtraction, return 1.
    if (op == "+" || op == "-")
        return 1;
    // If the operator is multiplication or division, return 2.
    if (op == "*" || op == "/")
        return 2;
    // Otherwise, return 0.
    return 0;
}

// This function applies the given operator on two pairs of integers.
// It returns a pair representing the result of the operation.
pii apply_operator(pii a, pii b, string op)
{
    // Perform addition if the operator is addition.
    if (op == "+")
        return make_pair(a.first + b.first, a.second + b.second);
    // Perform subtraction if the operator is subtraction.
    if (op == "-")
        return make_pair(a.first - b.first, a.second - b.second);
    // Perform multiplication if the operator is multiplication.
    if (op == "*")
        return make_pair(a.first * b.first, a.second * b.second);
    // Perform division if the operator is division.
    if (op == "/")
        return make_pair(a.first / b.first, a.second / b.second);
    // If none of the above cases match, return a pair of zeros.
    return make_pair(0, 0);
}

// This function tokenizes the given expression string and returns a vector of tokens.
vector<string> tokenize(string exp)
{
    int n = exp.length();
    vector<string> allTokens;
    int i = 0;
    // Loop through each character in the expression.
    while (i < n)
    {
        // If the character is an operator or parenthesis, add it as a token.
        if (exp[i] == '*' || exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == ')' || exp[i] == '(')
        {
            allTokens.pb(exp.substr(i, 1));
            i++;
        }
        // If the character is not an operator or parenthesis,
        // find the substring representing the number and add it as a token.
        else
        {
            int j = i;
            while (j < n && !(exp[j] == '*' || exp[j] == '+' || exp[j] == '-' || exp[j] == '/' || exp[j] == '(' || exp[j] == ')'))
                j++;
            allTokens.pb(exp.substr(i, j - i));
            i = j;
        }
    }
    // Return the vector containing all tokens.
    return allTokens;
}

// This function evaluates the given arithmetic expression using stacks.
pii evaluate(string exp, string CSECT)
{
    // Tokenize the expression string.
    vector<string> allTokens = tokenize(exp);
    // Initialize stacks for operands, types, and operators.
    stack<pii> operands;
    stack<int> type;
    stack<string> operators;
    int n = allTokens.size();
    // Iterate through all tokens in the expression.
    for (int i = 0; i < n; i++)
    {
        // If the token is an opening parenthesis, push it onto the operators stack.
        if (allTokens[i] == "(")
            operators.push("(");
        // If the token is a number, convert it to an integer pair and push onto the operands stack.
        else if (is_number(allTokens[i]))
        {
            operands.push(make_pair(stoi(allTokens[i]), stoi(allTokens[i])));
        }
        // If the token is a closing parenthesis, evaluate the expression until reaching the corresponding opening parenthesis.
        else if (allTokens[i] == ")")
        {
            while (!operators.empty() && operators.top() != "(")
            {
                // Pop operands and operator, apply the operator, and push the result back onto operands stack.
                pii operand_2 = operands.top();
                operands.pop();
                pii operand_1 = operands.top();
                operands.pop();
                string op = operators.top();
                operators.pop();
                operands.push(apply_operator(operand_1, operand_2, op));
            }
            // Pop the opening parenthesis.
            if (!operators.empty())
                operators.pop();
        }
        // If the token is an operator, evaluate the expression until the top operator has lower precedence.
        else if (allTokens[i] == "+" || allTokens[i] == "-" || allTokens[i] == "*" || allTokens[i] == "/")
        {
            while (!operators.empty() && precedence(operators.top()) >= precedence(allTokens[i]))
            {
                // Pop operands and operator, apply the operator, and push the result back onto operands stack.
                pii operand_2 = operands.top();
                operands.pop();
                pii operand_1 = operands.top();
                operands.pop();
                string op = operators.top();
                operators.pop();
                operands.push(apply_operator(operand_1, operand_2, op));
            }
            // Push the current operator onto operators stack.
            operators.push(allTokens[i]);
        }
        // If the token is a variable, push its value onto operands stack.
        else
        {
            // Search for the variable in the symbol table.
            if (symbolTable.find(SYMBOL(CSECT, allTokens[i])) != symbolTable.end())
                // If found, push its type and value onto operands stack.
                operands.push(make_pair(symbolTable[SYMBOL(CSECT, allTokens[i])].type, symbolTable[SYMBOL(CSECT, allTokens[i])].value));
            // If not found, push zeros onto operands stack.
            else
                operands.push(make_pair(0, 0));
        }
    }
    // Evaluate any remaining operators.
    while (!operators.empty())
    {
        // Pop operands and operator, apply the operator, and push the result back onto operands stack.
        pii operand_2 = operands.top();
        operands.pop();
        pii operand_1 = operands.top();
        operands.pop();
        string op = operators.top();
        operators.pop();
        operands.push(apply_operator(operand_1, operand_2, op));
    }
    // Return the final result.
    return operands.top();
}
// This struct represents an instruction with various attributes such as label, opcode, operands, length, and flags.
struct instruction
{
    string label;    // Label of the instruction.
    string opcode;   // Opcode mnemonic.
    string operands; // Operands of the instruction.
    int length;      // Length of the instruction.
    bool i;          // Immediate addressing mode flag.
    bool n;          // Indirect addressing mode flag.
    bool p;          // Program-counter-relative addressing mode flag.
    bool b;          // Base-relative addressing mode flag.
    int e;           // Extended addressing mode flag.
    int x;           // Indexing flag.
    int literal;     // Literal flag.

    // Constructor to initialize instruction attributes.
    instruction(string label = "", string opcode = "", string operands = "", int length = 3, bool i = 1, bool n = 1, bool p = 1, bool b = 0, int e = 0, int literal = 0, int x = 0)
    {
        // Initialize instruction attributes with provided or default values.
        this->label = label;
        this->opcode = opcode;
        this->operands = operands;
        this->length = length;
        this->i = i;
        this->n = n;
        this->p = p;
        this->b = b;
        this->e = e;
        this->x = x;
        this->literal = literal;
    }

    // Function to parse operands of the instruction and return them as a vector of strings.
    vector<string> get_operands()
    {
        // Initialize a vector to store parsed operands.
        vector<string> m_operands;
        // Initialize index variables for parsing.
        int i = 0;
        int l = operands.length();
        // Loop through the operands string.
        while (i < l)
        {
            // Find the end of the current operand.
            int j = i + 1;
            while (j < l && operands[j] != ',')
                j++;
            // Extract the current operand and add it to the vector.
            m_operands.pb(operands.substr(i, j - i));
            // Move to the next operand.
            i = j + 1;
        }
        // Return the vector containing parsed operands.
        return m_operands;
    }

    // Function to format operand value based on instruction attributes.
    void format_operand(int &operand)
    {
        // If the instruction length is 3 or 4 (i.e., standard or extended format).
        if (length == 3 || length == 4)
        {
            // Create a mask to apply on the operand based on instruction length.
            int mask = 0;
            for (int i = 0; i < ((length == 3) ? 12 : 20); i++)
            {
                mask |= 1;
                mask = mask << 1;
            }
            mask = mask >> 1;
            // Apply the mask on the operand.
            operand = operand & mask;

            // Apply flags to the operand based on addressing mode.
            if (!e) // If not in extended addressing mode.
            {
                operand |= (p << 13); // Set program-counter-relative flag.
                operand |= (b << 14); // Set base-relative flag.
                operand |= (x << 15); // Set indexing flag.
            }
            else // If in extended addressing mode.
            {
                operand |= (e << 20); // Set extended addressing mode flag.
                operand |= (p << 21); // Set program-counter-relative flag.
                operand |= (b << 22); // Set base-relative flag.
                operand |= (x << 23); // Set indexing flag.
            }
        }
    }
};

int main()
{
    symbolTableReader();
    literalTableReader();

    // Declare file streams for input and output operations.
    // fstream fin, fout, fout2;

    // Open the intermediateFile file for reading.
    ifstream intermediateFile("outputFromPass1/intermediateFile.txt");

    // Check if the file is successfully opened.
    if (!intermediateFile.is_open())
    {
        // If file opening fails, print an error message and exit the program.
        perror("intermediateFile.txt");
        exit(1);
    }

    // Open the output file for writing.
    ofstream outputFile("outputFromPass2/output.txt");

    // Open the listing file for writing.
    ofstream listingFile("outputFromPass2/listing.txt");

    // Initialize variables for program execution.
    int LOCCTR = 0;       // Location counter.
    int BASE = 0;         // Base register.
    int PC = 0;           // Program counter.
    int START = 0;        // Start address.
    string CSECT = "";    // Control section name.
    string PROGNAME = ""; // Program name.
    string text = "";     // Text record.

    // Initialize data structures to store various lists.
    map<string, vector<string>> textList;          // Text records list.
    map<string, vector<string>> modification_list; // Modification records list.
    map<string, string> headerList;                // Header records list.
    map<string, string> endList;                   // End records list.
    map<string, string> definingList;              // Defining symbols list.
    map<string, string> referList;                 // Referenced symbols list.
    vector<string> CSECTS;                         // List of control sections.
    string line = "";                              // Reading the lines of the file

    // Process each line in the input file
    while (getline(intermediateFile, line))
    {
        if (line.empty()) // Break loop if an empty line is encountered
            break;

        // Initialize an instruction object and LOCCTR1 string
        instruction instr;
        string LOCCTR1 = "";

        // Set offset for parsing the line
        int offset = 10;

        // Extract label, opcode, and operands from the line
        instr.label = removeBlankSpaces(line.substr(offset, 10));
        if (line[offset + 10] == '+')
            instr.e = 1, instr.p = 0, instr.b = 0, instr.n = 1, instr.i = 1;
        instr.opcode = removeBlankSpaces(line.substr(offset + 10 + 1, 10));
        instr.operands = removeBlankSpaces(line.substr(offset + 2 * 10 + 2, 30));

        // Check for extended addressing mode or special characters in operands
        if (line[offset + 2 * 10 + 1] != ' ')
        {
            char x = line[offset + 2 * 10 + 1];
            if (x == '#')
            {
                instr.i = 1, instr.n = 0;
                if (is_number(instr.operands))
                    instr.p = 0, instr.b = 0;
            }
            else if (x == '@')
                instr.i = 0, instr.n = 1;
            else if (x == '=')
                instr.literal = true;
        }

        // Extract LOCCTR1 from the line
        LOCCTR1 = removeBlankSpaces(line.substr(0, 10));
        if (!LOCCTR1.empty())
            LOCCTR = stoi(LOCCTR1, nullptr, 16);

        // Process the instruction based on opcode
        if (instr.label != ".")
        {
            if (instr.opcode == "START") // Process START directive
            {
                listingFile << line << '\n';
                CSECT = instr.label;
                PROGNAME = CSECT;
                CSECTS.pb(CSECT);
                int STADDR = 0;
                if (instr.operands != "")
                    STADDR = stoi(instr.operands, NULL, 16);
                else
                    STADDR = 0;
                int LENGTH = 0;
                if (symbolTable.find(SYMBOL(CSECT, CSECT)) != symbolTable.end())
                    LENGTH = symbolTable[SYMBOL(CSECT, CSECT)].length;
                stringstream header;
                header << "H";
                header << stringFormatter(CSECT, 6);
                header << numberFormatter(STADDR, 6);
                header << numberFormatter(LENGTH, 6);
                headerList.insert({CSECT, header.str()});
            }
            else if (instr.opcode == "END") // Process END directive
            {
                listingFile<< line << '\n';
                stringstream end;
                int first = symbolTable[SYMBOL(CSECT, instr.operands)].value;
                end << "E";
                endList.insert({CSECT, end.str()});
                end << numberFormatter(first, 6);
                endList[PROGNAME] = end.str();
            }
            else if (instr.opcode == "CSECT") // Process CSECT directive
            {
                listingFile<< line << '\n';
                if (text.length())
                {
                    int LENGTH = (text.length()) / 2;
                    stringstream textRecord;
                    textRecord << "T";
                    textRecord << numberFormatter(START, 6);
                    textRecord << numberFormatter(LENGTH, 2);
                    textRecord << text;

                    textList[CSECT].pb(textRecord.str());

                    text = "";
                    START = 0;
                }
                stringstream end;
                end << "E";
                endList.insert({CSECT, end.str()});
                int STADDR = 0;
                CSECT = instr.label;
                CSECTS.pb(CSECT);
                int LENGTH = 0;
                if (symbolTable.find(SYMBOL(CSECT, CSECT)) != symbolTable.end())
                    LENGTH = symbolTable[SYMBOL(CSECT, CSECT)].length;
                stringstream header;
                header << "H";
                header << stringFormatter(CSECT, 6);
                header << numberFormatter(STADDR, 6);
                header << numberFormatter(LENGTH, 6);
                headerList.insert({CSECT, header.str()});
            }
            else if (instr.opcode == "EXTDEF" || instr.opcode == "EXTREF") // Process EXTDEF or EXTREF directive
            {
                listingFile<< line << '\n';
                vector<string> operands = instr.get_operands();
                if (instr.opcode == "EXTDEF")
                {
                    stringstream def;
                    def << "D";
                    for (string operand : operands)
                    {
                        def << stringFormatter(operand, 6);
                        int address = 0;
                        if (symbolTable.find(SYMBOL(CSECT, operand)) != symbolTable.end())
                            address = symbolTable[SYMBOL(CSECT, operand)].value;
                        def << numberFormatter(address, 6);
                    }
                    definingList.insert({CSECT, def.str()});
                }
                else
                {
                    EXTREF.clear();
                    for (auto x : operands)
                        EXTREF.insert(x);
                    stringstream refer;
                    refer << "R";
                    for (string operand : operands)
                        refer << stringFormatter(operand, 6);
                    referList.insert({CSECT, refer.str()});
                }
            }
            else if (instr.opcode == "BASE") // Process BASE directive
            {
                listingFile << line << '\n';
                if (symbolTable.find(SYMBOL(CSECT, instr.operands)) != symbolTable.end())
                    BASE = symbolTable[SYMBOL(CSECT, instr.operands)].value;
            }
            else if (instr.opcode == "LTORG") // Process LTORG or EQU directive
                listingFile << line << '\n';
            else if (instr.opcode == "EQU")
                listingFile << line << '\n';
            else // Process other instructions
            {
                string obcode = "";
                if (OPTAB(instr.opcode) != make_pair(-1, -1))
                {
                    pii format = OPTAB(instr.opcode);
                    if (instr.e)
                        format.first++;
                    instr.length = format.first;
                    int op = format.second;
                    if (instr.length == 3 || instr.length == 4)
                    {
                        op |= instr.i;
                        op |= (instr.n << 1);
                    }
                    PC = LOCCTR + instr.length;
                    int operandcode = 0;
                    if (instr.operands != "")
                    {
                        if (instr.operands.find(",") != string::npos)
                        {
                            vector<string> m_operands = instr.get_operands();
                            if (instr.length == 2)
                            {
                                for (string op : m_operands)
                                {
                                    if (symbolTable.find(SYMBOL(CSECT, op)) != symbolTable.end())
                                    {
                                        int x = symbolTable[SYMBOL(CSECT, op)].value;
                                        operandcode = operandcode << 4;
                                        operandcode += x;
                                    }
                                    else
                                    {
                                        perror("register not found");
                                        exit(1);
                                    }
                                }
                            }
                            else
                            {
                                int x = 0;
                                if (symbolTable.find(SYMBOL(CSECT, m_operands.front())) != symbolTable.end())
                                {
                                    x = symbolTable[SYMBOL(CSECT, m_operands.front())].value;
                                    operandcode += x;
                                    if (instr.p)
                                    {
                                        if (operandcode - PC >= -2048 && operandcode - PC <= 2047)
                                            operandcode -= PC;
                                        else if (operandcode - BASE >= 0 && operandcode - BASE <= 4095)
                                            operandcode -= BASE, instr.p = 0, instr.b = 1;
                                        else
                                        {
                                            perror("can't fit PC or BASE");
                                            exit(1);
                                        }
                                    }

                                    if (instr.e)
                                    {
                                        stringstream modification;
                                        modification << "M" << numberFormatter(LOCCTR + 1, 6) << "05+" << stringFormatter(CSECT, 6);
                                        modification_list[CSECT].pb(modification.str());
                                    }
                                }
                                else
                                {
                                    operandcode += x;
                                    if (EXTREF.find(m_operands.front()) != EXTREF.end())
                                    {
                                        stringstream modification;
                                        modification << "M" << numberFormatter(LOCCTR + 1, 6) << "05+" << stringFormatter(m_operands.front(), 6);
                                        modification_list[CSECT].pb(modification.str());
                                    }
                                }
                                instr.x = 1;
                            }
                        }
                        else
                        {
                            if (instr.literal)
                            {
                                if (literalTable.find(SYMBOL(CSECT, instr.operands)) != literalTable.end())
                                {
                                    operandcode = literalTable[SYMBOL(CSECT, instr.operands)].value;
                                    if (instr.p)
                                    {
                                        if (operandcode - PC >= -2048 && operandcode - PC <= 2047)
                                            operandcode -= PC;
                                        else if (operandcode - BASE >= 0 && operandcode - BASE <= 4095)
                                        {
                                            operandcode -= BASE;
                                            instr.p = 0;
                                            instr.b = 1;
                                        }
                                        else
                                        {
                                            perror("can't fit PC or BASE");
                                            exit(1);
                                        }
                                    }
                                }
                                else
                                {
                                    perror("my error");
                                    exit(1);
                                }
                            }
                            else if (symbolTable.find(SYMBOL(CSECT, instr.operands)) != symbolTable.end())
                            {
                                operandcode = symbolTable[SYMBOL(CSECT, instr.operands)].value;
                                if (instr.length == 2)
                                    operandcode = operandcode << 4;
                                else
                                {
                                    if (instr.p)
                                    {
                                        if (operandcode - PC >= -2048 && operandcode - PC <= 2047)
                                            operandcode -= PC;
                                        else if (operandcode - BASE >= 0 && operandcode - BASE <= 4095)
                                        {
                                            operandcode -= BASE;
                                            instr.p = 0;
                                            instr.b = 1;
                                        }
                                        else
                                        {
                                            perror("can't fit PC or BASE");
                                            exit(1);
                                        }
                                    }

                                    if (instr.e)
                                    {
                                        stringstream modification;
                                        modification << "M" << numberFormatter(LOCCTR + 1, 6) << "05+" << stringFormatter(CSECT, 6);
                                        modification_list[CSECT].pb(modification.str());
                                    }
                                }
                            }
                            else
                            {
                                if (EXTREF.find(instr.operands) != EXTREF.end())
                                {
                                    stringstream modification;
                                    modification << "M" << numberFormatter(LOCCTR + 1, 6) << "05+" << stringFormatter(instr.operands, 6);
                                    modification_list[CSECT].pb(modification.str());
                                }
                                if (instr.i)
                                {
                                    if (is_number(instr.operands))
                                        operandcode = stoi(instr.operands);
                                }
                            }
                        }
                        instr.format_operand(operandcode);
                    }
                    obcode = numberFormatter(op, 2) + numberFormatter(operandcode, 2 * (instr.length - 1));
                    listingFile << line << stringFormatter(obcode, 10) << '\n';
                }
                else
                {
                    if (instr.label == "*")
                    {
                        string constant = instr.opcode.substr(2, instr.opcode.length() - 3);
                        if (instr.opcode.front() == 'X')
                        {
                            int x = stoi(constant, nullptr, 16);
                            obcode = numberFormatter(x, constant.length());
                        }
                        else if (instr.opcode.front() == 'C')
                        {
                            for (char ch : constant)
                            {
                                int x = ch;
                                obcode += numberFormatter(x, 2, false);
                            }
                        }
                        else
                        {
                            int word = stoi(instr.operands);
                            obcode = numberFormatter(word, 6, false);
                        }
                    }
                    if (instr.opcode == "WORD")
                    {
                        if (is_number(instr.operands))
                        {
                            int word = stoi(instr.operands);
                            obcode = numberFormatter(word, 6);
                        }
                        else
                        {
                            int word = evaluate(instr.operands, CSECT).second;
                            apply_mask(word, 24);
                            obcode = numberFormatter(word, 6);
                            vector<string> m_operands = tokenize(instr.operands);
                            for (int i = 0; i < (int)(m_operands.size()); i++)
                            {
                                if (EXTREF.find(m_operands[i]) != EXTREF.end())
                                {
                                    stringstream modification;
                                    modification << "M" << numberFormatter(LOCCTR, 6) << "06";
                                    if (i > 0 && m_operands[i - 1] == "-")
                                        modification << "-";
                                    else
                                        modification << "+";
                                    modification << stringFormatter(m_operands[i], 6);
                                    modification_list[CSECT].pb(modification.str());
                                }
                            }
                        }
                    }
                    if (instr.opcode == "BYTE")
                    {
                        string constant = instr.operands.substr(2, instr.operands.length() - 3);
                        if (instr.operands.front() == 'X')
                        {
                            int x = stoi(constant, nullptr, 16);
                            obcode = numberFormatter(x, constant.length());
                        }
                        if (instr.operands.front() == 'C')
                            for (char ch : constant)
                            {
                                int x = ch;
                                obcode += numberFormatter(x, 2, false);
                            }
                    }
                    listingFile<< line << stringFormatter(obcode, 10) << '\n';
                }
                if (text.length() == 0)
                    START = LOCCTR;
                if (text.length() + obcode.length() <= 60 && instr.opcode != "RESW" && instr.opcode != "RESB")
                    text += obcode;
                else
                {
                    if (text.length())
                    {
                        int LENGTH = (text.length()) / 2;
                        stringstream textRecord;
                        textRecord << "T" << numberFormatter(START, 6) << numberFormatter(LENGTH, 2) << text;

                        textList[CSECT].pb(textRecord.str());

                        text = obcode;
                        START = LOCCTR;
                    }
                }
            }
        }
        else
            listingFile<< line << '\n';
    }
    if (text.length()) // Process remaining text records and generate output files
    {
        int LENGTH = (text.length()) / 2;
        stringstream textRecord;
        textRecord << "T" << numberFormatter(START, 6) << numberFormatter(LENGTH, 2) << text;
        textList[CSECT].pb(textRecord.str());
        text = "";
        START = 0;
    }
    for (string CSECT : CSECTS)
    {
        outputFile << headerList[CSECT] << '\n';
        if (!definingList[CSECT].empty())
            outputFile << definingList[CSECT] << '\n';
        if (!referList[CSECT].empty())
            outputFile << referList[CSECT] << '\n';
        for (string textRecord : textList[CSECT])
            outputFile << textRecord << '\n';
        for (string modificationRecord : modification_list[CSECT])
            outputFile << modificationRecord << '\n';
        outputFile << endList[CSECT] << '\n';
    }
    // Close input and output files
    intermediateFile.close();
    outputFile.close();
    listingFile.close();

    // Return 0 to indicate successful execution
    return 0;
}
