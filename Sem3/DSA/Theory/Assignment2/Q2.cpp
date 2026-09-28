#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

bool isOpeningBracket(char c)
{
    return c == '(' || c == '{' || c == '[';
}

bool isClosingBracket(char c)
{
    return c == ')' || c == '}' || c == ']';
}

bool bracketsMatch(char opening, char closing)
{
    if (opening == '(' && closing == ')') return true;
    if (opening == '{' && closing == '}') return true;
    if (opening == '[' && closing == ']') return true;
    return false;
}

char getExpectedBracket(char opening)
{
    if (opening == '(') return ')';
    if (opening == '{') return '}';
    return ']';
}

string removeCarriageReturn(string line)
{
    if (!line.empty() && line[line.length() - 1] == '\r')
    {
        line.erase(line.length() - 1);
    }
    return line;
}

// First non-space character of the line
char getFirstNonSpace(const string& line)
{
    for (int i = 0; i < (int)line.length(); i++)
    {
        if (!isSpace(line[i]))
        {
            return line[i];
        }
    }

    return '\0';
}

// ============================================================
// Dynamic array stack
// ============================================================

struct BracketEntry
{
    char bracket;
    int line;
    int column;
};

struct BracketStack
{
    BracketEntry* data;
    int top;
    int capacity;
};

void initializeStack(BracketStack& stack)
{
    stack.capacity = 10;
    stack.top = 0;
    stack.data = new BracketEntry[stack.capacity];
}

bool isEmpty(const BracketStack& stack)
{
    return stack.top == 0;
}

void growStack(BracketStack& stack)
{
    int newCapacity = stack.capacity * 2;
    BracketEntry* bigger = new BracketEntry[newCapacity];

    for (int i = 0; i < stack.top; i++)
    {
        bigger[i] = stack.data[i];
    }

    delete[] stack.data;
    stack.data = bigger;
    stack.capacity = newCapacity;
}

void push(BracketStack& stack, char bracket, int line, int column)
{
    if (stack.top == stack.capacity)
    {
        growStack(stack);
    }

    stack.data[stack.top].bracket = bracket;
    stack.data[stack.top].line = line;
    stack.data[stack.top].column = column;
    stack.top++;
}

BracketEntry pop(BracketStack& stack)
{
    stack.top--;
    return stack.data[stack.top];
}

BracketEntry topEntry(const BracketStack& stack)
{
    return stack.data[stack.top - 1];
}

void clearStack(BracketStack& stack)
{
    stack.top = 0;
}

void destroyStack(BracketStack& stack)
{
    delete[] stack.data;
    stack.data = nullptr;
    stack.top = 0;
    stack.capacity = 0;
}

// ============================================================
// Lexical states
// ============================================================

enum State
{
    NORMAL_CODE,
    STRING_LITERAL,
    CHARACTER_LITERAL,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT
};

// ============================================================
// Test case analyzer
// ============================================================

struct Analyzer
{
    BracketStack stack;
    State state;
    int maximumDepth;
    int matchedPairs;
    bool invalid;
};

void initializeAnalyzer(Analyzer& analyzer)
{
    initializeStack(analyzer.stack);
    analyzer.state = NORMAL_CODE;
    analyzer.maximumDepth = 0;
    analyzer.matchedPairs = 0;
    analyzer.invalid = false;
}

void resetAnalyzer(Analyzer& analyzer)
{
    clearStack(analyzer.stack);
    analyzer.state = NORMAL_CODE;
    analyzer.maximumDepth = 0;
    analyzer.matchedPairs = 0;
    analyzer.invalid = false;
}

void reportUnexpectedClosing(Analyzer& analyzer, int line, int column, char closing)
{
    cout << "INVALID" << endl;
    cout << "Error at Line " << line << ", Column " << column
         << ": Unexpected '" << closing << "'" << endl;
    analyzer.invalid = true;
}

void reportMismatch(Analyzer& analyzer, int line, int column,
                    char expected, char found)
{
    cout << "INVALID" << endl;
    cout << "Error at Line " << line << ", Column " << column
         << ": Expected '" << expected << "' but found '"
         << found << "'" << endl;
    analyzer.invalid = true;
}

void processCharacter(Analyzer& analyzer, const string& line, int& i, int lineNumber)
{
    char current = line[i];
    int column = i + 1;

    // ========================================================
    // NORMAL CODE
    // ========================================================
    if (analyzer.state == NORMAL_CODE)
    {
        // Start of a string
        if (current == '"')
        {
            analyzer.state = STRING_LITERAL;
            return;
        }

        // Start of a character literal
        if (current == '\'')
        {
            analyzer.state = CHARACTER_LITERAL;
            return;
        }

        // Start of a single-line comment
        if (current == '/' && i + 1 < (int)line.length() && line[i + 1] == '/')
        {
            analyzer.state = SINGLE_LINE_COMMENT;
            i++;
            return;
        }

        // Start of a multi-line comment
        if (current == '/' && i + 1 < (int)line.length() && line[i + 1] == '*')
        {
            analyzer.state = MULTI_LINE_COMMENT;
            i++;
            return;
        }

        // Opening bracket
        if (isOpeningBracket(current))
        {
            push(analyzer.stack, current, lineNumber, column);

            if (analyzer.stack.top > analyzer.maximumDepth)
            {
                analyzer.maximumDepth = analyzer.stack.top;
            }

            return;
        }

        // Closing bracket
        if (isClosingBracket(current))
        {
            if (isEmpty(analyzer.stack))
            {
                reportUnexpectedClosing(analyzer, lineNumber, column, current);
                return;
            }

            BracketEntry opening = topEntry(analyzer.stack);
            char expected = getExpectedBracket(opening.bracket);

            if (!bracketsMatch(opening.bracket, current))
            {
                reportMismatch(analyzer, lineNumber, column, expected, current);
                return;
            }

            pop(analyzer.stack);
            analyzer.matchedPairs++;
        }

        return;
    }

    // ========================================================
    // STRING LITERAL
    // ========================================================
    if (analyzer.state == STRING_LITERAL)
    {
        // Backslash means the next character is escaped
        if (current == '\\' && i + 1 < (int)line.length())
        {
            i++;
            return;
        }

        if (current == '"')
        {
            analyzer.state = NORMAL_CODE;
        }

        return;
    }

    // ========================================================
    // CHARACTER LITERAL
    // ========================================================
    if (analyzer.state == CHARACTER_LITERAL)
    {
        // Backslash means the next character is escaped
        if (current == '\\' && i + 1 < (int)line.length())
        {
            i++;
            return;
        }

        if (current == '\'')
        {
            analyzer.state = NORMAL_CODE;
        }

        return;
    }

    // ========================================================
    // MULTI-LINE COMMENT
    // ========================================================
    if (analyzer.state == MULTI_LINE_COMMENT)
    {
        if (current == '*' && i + 1 < (int)line.length() && line[i + 1] == '/')
        {
            analyzer.state = NORMAL_CODE;
            i++;
        }

        return;
    }

    // ========================================================
    // SINGLE-LINE COMMENT
    // ========================================================
    if (analyzer.state == SINGLE_LINE_COMMENT)
    {
        // Nothing is processed inside a single-line comment.
        return;
    }
}

void processLine(Analyzer& analyzer, const string& originalLine, int lineNumber)
{
    string line = removeCarriageReturn(originalLine);

    if (analyzer.invalid)
    {
        return;
    }

    // A preprocessor line is ignored completely.
    // We only check this when we are in normal source code.
    if (analyzer.state == NORMAL_CODE)
    {
        if (getFirstNonSpace(line) == '#')
        {
            return;
        }
    }

    for (int i = 0; i < (int)line.length(); i++)
    {
        processCharacter(analyzer, line, i, lineNumber);

        if (analyzer.invalid)
        {
            return;
        }
    }

    // Single-line comment ends at the newline.
    if (analyzer.state == SINGLE_LINE_COMMENT)
    {
        analyzer.state = NORMAL_CODE;
    }
}

void finishTestCase(Analyzer& analyzer)
{
    if (analyzer.invalid)
    {
        return;
    }

    if (!isEmpty(analyzer.stack))
    {
        BracketEntry opening = topEntry(analyzer.stack);

        cout << "INVALID" << endl;
        cout << "Error: '" << opening.bracket << "' opened at Line "
             << opening.line << ", Column " << opening.column
             << " was never closed" << endl;
        analyzer.invalid = true;
        return;
    }

    cout << "VALID" << endl;
    cout << "Maximum Nesting Depth: " << analyzer.maximumDepth << endl;
    cout << "Total Matched Pairs: " << analyzer.matchedPairs << endl;
}

// ============================================================
// Main
// ============================================================

int main(int argc, char* argv[])
{
    const char* fileName = "input.txt";

    if (argc > 1)
    {
        fileName = argv[1];
    }

    ifstream file(fileName);

    if (!file.is_open())
    {
        cout << "Error: Could not open file " << fileName << endl;
        return 1;
    }

    Analyzer analyzer;
    initializeAnalyzer(analyzer);

    string line;
    int lineNumber = 0;
    int testCaseNumber = 1;
    bool hasTestCase = false;

    while (getline(file, line))
    {
        line = removeCarriageReturn(line);

        // ### separates test cases.
        if (line == "###")
        {
            if (hasTestCase)
            {
                finishTestCase(analyzer);
                cout << endl;

                testCaseNumber++;
                resetAnalyzer(analyzer);
                lineNumber = 0;
                hasTestCase = false;
            }

            continue;
        }

        // Print the heading as soon as the test case begins.
        if (!hasTestCase)
        {
            cout << "========== Test Case " << testCaseNumber << " ==========" << endl;
            hasTestCase = true;
        }

        // A line belongs to the current test case, including blank lines.
        lineNumber++;

        // Keep reading after an error, but stop analyzing this test case.
        if (!analyzer.invalid)
        {
            processLine(analyzer, line, lineNumber);
        }
    }

    // Process the last test case if the file does not end with ###.
    if (hasTestCase)
    {
        finishTestCase(analyzer);
    }

    destroyStack(analyzer.stack);
    file.close();

    return 0;
}
