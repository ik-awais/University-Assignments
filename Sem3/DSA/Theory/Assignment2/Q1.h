#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifndef Q1_H
#define Q1_H

#define MAX_FUNCTIONS 50
#define MAX_TOP_CALLS 50
#define ALIGNMENT 4

// One nested-call position inside a function body
// Plain call     -> isTernary = false, first holds the function name
// Ternary call   -> isTernary = true, condition decides between first and second
struct NestedCall {
    bool isTernary;
    bool condition;
    string first;
    string second;
};

struct FunctionDef {
    string name;
    bool hasRecursion;
    string recursionExpr;
    int recursionCount;
    NestedCall* calls;
    int callCount;
    long long requestedMemory;
    long long allocatedMemory;
    int successCount;
};

struct Frame {
    string name;
    long long allocated;
};

// Call stack backed by a dynamic array
struct CallStack {
    Frame* data;
    int top;
    int capacity;
    long long usedMemory;
    long long totalMemory;
};

struct Stats {
    int attempts;
    int successes;
    int overflows;
    int maxDepth;
    long long maxMemory;
};

struct Simulator {
    FunctionDef* defs;
    int defCount;
    CallStack stack;
    Stats stats;
};

// Helpers
string trimText(const string& s);
bool isAlphaChar(char c);
bool isDigitChar(char c);
bool isBlankLine(const string& s);
long long alignMemory(long long requested);

// Expression evaluator (+, -, *, / and parentheses)
long long parseFactor(const string& e, int& pos, bool& ok);
long long parseTerm(const string& e, int& pos, bool& ok);
long long parseExpr(const string& e, int& pos, bool& ok);
bool evaluateExpression(const string& e, long long& result);

// Call stack
void initStack(CallStack& s, long long totalMemory);
void growStack(CallStack& s);
bool isStackEmpty(const CallStack& s);
void pushFrame(CallStack& s, const string& name, long long allocated);
void popFrame(CallStack& s);
void displayStack(const CallStack& s);
void destroyStack(CallStack& s);

// Parsing
int countNestedItems(const string& body);
void parseNestedItem(const string& item, NestedCall& call);
bool parseDefinition(const string& line, FunctionDef& def);
void destroyFunctions(FunctionDef* defs, int count);

// Validation
bool isValidFunctionName(const string& name);
int findFunction(const Simulator& sim, const string& name);
int validateDefinitions(Simulator& sim);
int validateReferences(const Simulator& sim, const string* topCalls, int topCount);
bool findCycle(const Simulator& sim, int node, int* state, int* path, int& depth);
int checkCircularDependency(const Simulator& sim);

// Execution
void runInvocation(Simulator& sim, int fIdx, int remaining);
void executeBody(Simulator& sim, int fIdx);
void printSummary(const Simulator& sim);

// Test case handling
void processTestCase(const string* lines, int lineCount, int caseNumber);
int readAllLines(const char* fileName, string*& lines);

#endif