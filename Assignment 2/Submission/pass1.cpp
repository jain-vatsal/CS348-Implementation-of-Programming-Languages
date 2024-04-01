// Including necessary header files
#include <bits/stdc++.h> // For including all standard libraries
#include <vector>
using namespace std;

// Defining aliases for data types
typedef long long int li;
#define rep(i, a, b) for (int i = a; i < b; i++) // Macro for iteration with specified limits
#define pii pair<int, int>                       // Alias for pair of integers

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

// Function to check if a string is a number
bool is_number(string s)
{
    int n = s.length();
    rep(i, 0, n) if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

// Function to apply a mask to a value based on the number of bits
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

// Function to map opcode to its corresponding format and code
pii OPTAB(string opcode)
{
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
    return make_pair(-1, -1); // Return invalid opcode
}

// Structure to represent a symbol
struct SYMBOL
{
    string CSECT, name;
    SYMBOL(string CSECT, string name)
    {
        this->CSECT = CSECT, this->name = name;
    }
    bool operator<(const SYMBOL &s) const
    {
        return make_pair(this->CSECT, this->name) < make_pair(s.CSECT, s.name);
    }
};

// Structure to represent a value associated with a symbol
struct VALUE
{
    int value;
    bool type;
    int length;
    VALUE(int value = 0, int length = 3, bool type = 1)
    {
        this->value = value;
        this->length = length;
        this->type = type;
    }
};

// Map to store symbol table
map<SYMBOL, VALUE> symbolTable;

// Map to store literal table
map<SYMBOL, VALUE> literalTable;

// Set to store external references
set<string> EXTREF;

// Function to determine precedence of operators
int precedence(string op)
{
    vector<string> operations = {"+", "-", "*", "/"};
    vector<int> prec = {1, 1, 2, 2};
    rep(i, 0, 4) if (operations[i] == op) return prec[i];
    return 0;
}

// Function to apply an operator on two operands
pii apply_operator(pii a, pii b, string op)
{
    pii p = make_pair(0, 0);
    if (op == "+")
        p = make_pair(a.first + b.first, a.second + b.second);
    if (op == "-")
        p = make_pair(a.first - b.first, a.second - b.second);
    if (op == "*")
        p = make_pair(a.first * b.first, a.second * b.second);
    if (op == "/")
        p = make_pair(a.first / b.first, a.second / b.second);
    return p;
}

// Function to tokenize an expression
vector<string> tokenize(string exp)
{
    int n = exp.length();
    vector<string> tokens;
    int i = 0;
    while (i < n)
    {
        if (exp[i] == '*' || exp[i] == '+' || exp[i] == '-' || exp[i] == '/' || exp[i] == ')' || exp[i] == '(')
        {
            tokens.push_back(exp.substr(i, 1));
            i++;
        }
        else
        {
            int j = i;
            while (j < n && !(exp[j] == '*' || exp[j] == '+' || exp[j] == '-' || exp[j] == '/' || exp[j] == '(' || exp[j] == ')'))
                j++;
            tokens.push_back(exp.substr(i, j - i));
            i = j;
        }
    }
    return tokens;
}

// Function to evaluate an expression
pii evaluate(string exp, string CSECT)
{
    vector<string> tokens = tokenize(exp);
    stack<pii> operands;
    stack<int> type;
    stack<string> operators;
    int n = tokens.size();
    for (int i = 0; i < n; i++)
    {
        if (tokens[i] == "(")
            operators.push("(");
        else if (is_number(tokens[i]))
        {
            operands.push(make_pair(stoi(tokens[i]), stoi(tokens[i])));
        }
        else if (tokens[i] == ")")
        {
            while (!operators.empty() && operators.top() != "(")
            {
                pii operand_2 = operands.top();
                operands.pop();
                pii operand_1 = operands.top();
                operands.pop();
                string op = operators.top();
                operators.pop();
                operands.push(apply_operator(operand_1, operand_2, op));
            }
            if (!operators.empty())
                operators.pop();
        }
        else if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/")
        {
            while (!operators.empty() && precedence(operators.top()) >= precedence(tokens[i]))
            {
                pii operand_2 = operands.top();
                operands.pop();
                pii operand_1 = operands.top();
                operands.pop();
                string op = operators.top();
                operators.pop();
                operands.push(apply_operator(operand_1, operand_2, op));
            }
            operators.push(tokens[i]);
        }
        else
        {
            if (symbolTable.find(SYMBOL(CSECT, tokens[i])) != symbolTable.end())
            {
                int value = symbolTable[SYMBOL(CSECT, tokens[i])].value;
                int type = symbolTable[SYMBOL(CSECT, tokens[i])].type;
                operands.push(make_pair(type, value));
            }
            else
                operands.push(make_pair(0, 0));
        }
    }
    while (!operators.empty())
    {
        pii operand_2 = operands.top();
        operands.pop();
        pii operand_1 = operands.top();
        operands.pop();
        string op = operators.top();
        operators.pop();
        operands.push(apply_operator(operand_1, operand_2, op));
    }
    return operands.top();
}

// Main function
int main()
{

    // Open the input file for reading
    ifstream inputFile("inputFiles/input.txt");

    // Check if the input file is successfully opened
    if (!inputFile.is_open())
    {
        // Print an error message and exit the program if the file failed to open
        perror("Error in opening inputFiles/input.txt");
        exit(1);
    }

    // Open the output file for writing intermediateFile results
    ofstream intermediateFile("outputFromPass1/intermediateFile.txt");

    // Open the file to write the symbol table
    ofstream symbolTableFile("outputFromPass1/symbolTable.txt");

    // Open the file to write the literal table
    ofstream literalTableFile("outputFromPass1/literalTable.txt");

    // Initialize location counter, starting address, and LENGTH variables
    int locationCounter = 0, startingAddress = 0, LENGTH = 0;

    // Initialize CSECT string
    string CSECT = "";

    string line = "";
    // Loop through each line of the input file
    while (getline(inputFile, line)) // Read a line from the input file
    {

        // Exit the loop if the line is empty
        if (line.empty())
            break;

        // Initialize variables for label, opcode, and operands
        string label = "";
        string opcode = "";
        string operands = "";

        // Default length of instruction
        int length = 3;

        // Default addressing modes
        bool i = 1, n = 1, p = 1, b = 0, e = 0, x = 0;

        // Default value for literal
        bool literal = 0;

        // Offset for each field in the line
        int offset = 0;

        // Extract label, opcode, and operands from the line
        label = removeBlankSpaces(line.substr(offset, 10));
        if (line[offset + 10] == '+')
            e = 1, p = 0, b = 0, n = 1, i = 1;
        opcode = removeBlankSpaces(line.substr(offset + 10 + 1, 10));
        operands = removeBlankSpaces(line.substr(offset + 2 * 10 + 2, 30));

        // Determine the addressing mode based on the presence of '#', '@', or '='
        if (line[offset + 2 * 10 + 1] != ' ')
        {
            char x = line[offset + 2 * 10 + 1];
            if (x == '#')
            {
                i = 1, n = 0;
                if (is_number(operands))
                    p = 0, b = 0;
            }
            else if (x == '@')
                i = 0, n = 1;
            else if (x == '=')
                literal = true;
        }

        // Process the line based on the opcode and label
        if (label != ".")
        {
            if (opcode == "START" || opcode == "CSECT")
            {
                // Store current program's length in symbol table
                if (!CSECT.empty())
                {
                    LENGTH = locationCounter - startingAddress;
                    symbolTable.insert({SYMBOL(CSECT, CSECT), VALUE(startingAddress, LENGTH)});
                }
                // Set starting address and initialize location counter
                if (operands != "")
                    startingAddress = stoi(operands, NULL, 16);
                else
                    startingAddress = 0;
                locationCounter = startingAddress;
                // Update CSECT and insert predefined symbols
                CSECT = label;
                symbolTable.insert({SYMBOL(CSECT, "A"), VALUE(0, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "X"), VALUE(1, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "L"), VALUE(2, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "PC"), VALUE(8, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "SW"), VALUE(9, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "B"), VALUE(3, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "S"), VALUE(4, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "T"), VALUE(5, 3, 0)});
                symbolTable.insert({SYMBOL(CSECT, "F"), VALUE(6, 3, 0)});
                intermediateFile << stringFormatter(numberFormatter(locationCounter, 4), 10) << line << '\n';
            }
            else if (opcode == "END" || opcode == "LTORG")
            {
                // Write the line to the output file
                intermediateFile << stringFormatter("", 10) << line << '\n';
                // Process literals
                for (auto &x : literalTable)
                {
                    if (x.second.value == -1)
                    {
                        intermediateFile << stringFormatter(numberFormatter(locationCounter, 4), 10) << stringFormatter("*", 10) << "=" << stringFormatter(x.first.name, 10) << " " << stringFormatter("", 30) << '\n';
                        x.second.value = locationCounter;
                        if (x.first.name.front() == 'C')
                            locationCounter += (x.first.name.length() - 3);
                        else if (x.first.name.front() == 'X')
                            locationCounter += ((x.first.name.length() - 3) / 2);
                        else
                            locationCounter += 3;
                    }
                }
                // Update program length and store in symbol table if END opcode encountered
                if (opcode == "END")
                {
                    LENGTH = locationCounter - startingAddress;
                    symbolTable.insert({SYMBOL(CSECT, CSECT), VALUE(startingAddress, LENGTH)});
                }
            }
            else if (opcode == "BASE")
                // Write the line to the output file
                intermediateFile << stringFormatter("", 10) << line << '\n';
            else if (opcode == "EQU")
            {
                // Process EQU directive
                if (label != "")
                {
                    if (symbolTable.find(SYMBOL(CSECT, label)) == symbolTable.end())
                    {
                        if (operands == "*")
                        {
                            // Assign current location counter to label
                            symbolTable.insert({SYMBOL(CSECT, label), VALUE(locationCounter)});
                            intermediateFile << stringFormatter(numberFormatter(locationCounter, 4), 10) << line << '\n';
                        }
                        else if (is_number(operands)) // assumption that number is in decimal format only
                        {
                            // Assign constant value to label
                            symbolTable.insert({SYMBOL(CSECT, label), VALUE(stoi(operands), 3, 0)});
                            intermediateFile << stringFormatter(numberFormatter(stoi(operands), 4), 10) << line << '\n';
                        }
                        else
                        {
                            // Evaluate expression and assign value to label
                            int value = evaluate(operands, CSECT).second;
                            int type = evaluate(operands, CSECT).first;
                            if (type != 0 && type != -1 && type != 1)
                            {
                                // Error: Invalid expression
                                perror("Invalid Expression");
                                exit(1);
                            }
                            // Apply mask to value and write to output file
                            apply_mask(value, 16);
                            intermediateFile << stringFormatter(numberFormatter(value, 4), 10) << line << '\n';
                        }
                    }
                    else
                    {
                        // Error: Duplicate symbol
                        perror("duplicate symbol");
                        exit(1);
                    }
                }
            }
            else if (opcode == "EXTDEF" || opcode == "EXTREF")
                // Write the line to the output file
                intermediateFile << stringFormatter("", 10) << line << '\n';
            else
            {
                // Process other opcodes
                intermediateFile << stringFormatter(numberFormatter(locationCounter, 4), 10) << line << '\n';
                if (literal)
                {
                    // Check for literals and add to literal table if not already present
                    if (literalTable.find(SYMBOL(CSECT, operands)) == literalTable.end())
                    {
                        literalTable.insert({SYMBOL(CSECT, operands), VALUE(-1)});
                    }
                }
                // Insert label and corresponding address into symbol table
                if (label != "")
                {
                    if (symbolTable.find(SYMBOL(CSECT, label)) == symbolTable.end())
                        symbolTable.insert({SYMBOL(CSECT, label), VALUE(locationCounter)});
                    else
                    {
                        // Error: Duplicate symbol
                        perror("duplicate symbol");
                        exit(1);
                    }
                }
                // Update location counter based on opcode
                if (OPTAB(opcode) != make_pair(-1, -1))
                {
                    pii format = OPTAB(opcode);
                    if (e)
                        format.first++;

                    length = format.first;
                    locationCounter += length;
                }
                // updating locationCounter based on different values
                else if (opcode == "WORD")
                    locationCounter += 3;
                else if (opcode == "RESW")
                    locationCounter += 3 * stoi(operands);
                else if (opcode == "RESB")
                    locationCounter += stoi(operands);
                else if (opcode == "BYTE")
                {
                    if (operands.front() == 'C')
                        locationCounter += (operands.length() - 3);
                    if (operands.front() == 'X')
                        locationCounter += ((operands.length() - 3) / 2);
                }
                else
                {
                    // Error: Invalid Operation Code
                    perror("Invalid Operation Code\n");
                    exit(1);
                }
            }
        }
        else
            // Write the line to the output file
            intermediateFile << line << '\n';
    }

    // Write symbol table entries to symbolTable.txt
    for (auto i : symbolTable)
    {
        SYMBOL sym = i.first;
        VALUE val = i.second;
        string str = sym.CSECT + " " + sym.name + " " + to_string(val.length) + " " + to_string(val.type) + " " + to_string(val.value) + " ";
        symbolTableFile << str << '\n';
    }

    // Write literal table entries to literalTable.txt
    for (auto i : literalTable)
    {
        SYMBOL sym = i.first;
        VALUE val = i.second;
        string str = sym.CSECT + " " + sym.name + " " + to_string(val.length) + " " + to_string(val.type) + " " + to_string(val.value) + " ";
        literalTableFile << str << '\n';
    }

    // Close all file streams
    inputFile.close();
    intermediateFile.close();
    symbolTableFile.close();
    literalTableFile.close();

    return 0;
}
