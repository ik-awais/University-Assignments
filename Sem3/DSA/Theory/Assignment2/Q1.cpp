#include "Q1.h"

string trimText(const string& s) {
    int start = 0;
    int end = (int)s.length() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) { start++; }
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) { end--; }
    return s.substr(start, end - start + 1);
}
bool isAlphaChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isDigitChar(char c) {
    return (c >= '0' && c <= '9');
}
bool isBlankLine(const string& s) {
    return trimText(s).empty();
}
// Smallest multiple of 4 that is >= requested
long long alignMemory(long long requested) {
    return ((requested + 4 - 1) / 4) * 4;
}

bool readNumber(const string& text, int& pos, long long& number) {
    while (pos < (int)text.length() && (text[pos] == ' ' || text[pos] == '\t')) {
        pos++;
    }
    if (pos >= (int)text.length() || !isDigitChar(text[pos])) {
        return false;
    }
    number = 0;
    while (pos < (int)text.length() && isDigitChar(text[pos])) {
        number = number * 10 + (text[pos] - '0');
        pos++;
    }
    return true;
}

bool parseHeader(const string& line, int& n, int& m, long long& s) {
    int pos = 0;
    long long first = 0, second = 0, third = 0;

    if (!readNumber(line, pos, first)) { return false; }
    if (!readNumber(line, pos, second)) { return false; }
    if (!readNumber(line, pos, third)) { return false; }

    while (pos < (int)line.length() && (line[pos] == ' ' || line[pos] == '\t')) {
        pos++;
    }

    if (pos != (int)line.length()) { return false; }

    n = (int)first;
    m = (int)second;
    s = third;
    return true;
}

// ========================== Expression Evaluator ==========================

long long parseFactor(const string& e, int& pos, bool& ok) {
    int len = (int)e.length();
    while (pos < len && e[pos] == ' ') { pos++; }
    if (pos >= len) 
    {
        ok = false;
        return 0;
    }
    if (e[pos] == '(') 
    {
        pos++;
        long long value = parseExpr(e, pos, ok);
        while (pos < len && e[pos] == ' ') { pos++; }
        if (pos < len && e[pos] == ')') { pos++; }
        else { ok = false; }
        return value;
    }
    if (e[pos] == '-') 
    {
        pos++;
        return -parseFactor(e, pos, ok);
    }
    if (!isDigitChar(e[pos])) 
    {
        ok = false;
        return 0;
    }
    long long value = 0;
    while (pos < len && isDigitChar(e[pos])) 
    {
        value = value * 10 + (e[pos] - '0');
        pos++;
    }
    return value;
}
long long parseTerm(const string& e, int& pos, bool& ok) {
    int len = (int)e.length();
    long long value = parseFactor(e, pos, ok);
    while (ok) 
    {
        while (pos < len && e[pos] == ' ') { pos++; }
        if (pos >= len || (e[pos] != '*' && e[pos] != '/')) { break; }
        char op = e[pos];
        pos++;
        long long right = parseFactor(e, pos, ok);
        if (op == '*') { value = value * right; }
        else 
        {
            if (right == 0) 
            {
                ok = false;
                return 0;
            }
            value = value / right;
        }
    }
    return value;
}
long long parseExpr(const string& e, int& pos, bool& ok) {
    int len = (int)e.length();
    long long value = parseTerm(e, pos, ok);
    while (ok) 
    {
        while (pos < len && e[pos] == ' ') { pos++; }
        if (pos >= len || (e[pos] != '+' && e[pos] != '-')) { break; }
        char op = e[pos];
        pos++;
        long long right = parseTerm(e, pos, ok);
        if (op == '+') { value = value + right; }
        else { value = value - right; }
    }
    return value;
}
bool evaluateExpression(const string& e, long long& result) {
    int pos = 0;
    bool ok = true;
    result = parseExpr(e, pos, ok);
    while (pos < (int)e.length() && e[pos] == ' ') { pos++; }
    if (pos != (int)e.length()) { ok = false; }
    return ok;
}

// =============================== Call Stack ===============================

void initStack(CallStack& s, long long totalMemory) {
    s.capacity = 4;
    s.data = new Frame[s.capacity];
    s.top = 0;
    s.usedMemory = 0;
    s.totalMemory = totalMemory;
}
void growStack(CallStack& s) {
    int newCapacity = s.capacity * 2;
    Frame* bigger = new Frame[newCapacity];
    for (int i = 0; i < s.top; i++) 
    {
        bigger[i].name = s.data[i].name;
        bigger[i].allocated = s.data[i].allocated;
    }
    delete[] s.data;
    s.data = bigger;
    s.capacity = newCapacity;
}
bool isStackEmpty(const CallStack& s) {
    return s.top == 0;
}
void pushFrame(CallStack& s, const string& name, long long allocated) {
    if (s.top == s.capacity) { growStack(s); }
    s.data[s.top].name = name;
    s.data[s.top].allocated = allocated;
    s.top++;
    s.usedMemory += allocated;
}
void popFrame(CallStack& s) {
    if (isStackEmpty(s)) { return; }
    s.top--;
    s.usedMemory -= s.data[s.top].allocated;
}
void displayStack(const CallStack& s) {
    cout << "Stack: ";
    if (isStackEmpty(s)) { cout << "EMPTY\n"; }
    else 
    {
        for (int i = 0; i < s.top; i++) 
        {
            cout << "[" << s.data[i].name << ":" << s.data[i].allocated << "]";
            if (i < s.top - 1) { cout << " -> "; }
        }
        cout << " <- TOP\n";
    }
    cout << "Memory: " << s.usedMemory << "/" << s.totalMemory << " B\n";
}
void destroyStack(CallStack& s) {
    delete[] s.data;
    s.data = nullptr;
    s.top = 0;
    s.capacity = 0;
    s.usedMemory = 0;
}

// ================================ Parsing ================================

// Number of comma separated items in a nested-call body
int countNestedItems(const string& body) {
    if (isBlankLine(body)) { return 0; }
    int count = 1;
    for (int i = 0; i < (int)body.length(); i++) 
    {
        if (body[i] == ',') { count++; }
    }
    return count;
}
void parseNestedItem(const string& rawItem, NestedCall& call) {
    string item = trimText(rawItem);
    call.isTernary = false;
    call.condition = false;
    call.first = item;
    call.second = "";
    if (item.empty() || item[0] != '(') { return; }

    // (condition ? first : second)
    string inner = item.substr(1, item.length() - 2);
    int q = -1;
    int c = -1;
    for (int i = 0; i < (int)inner.length(); i++) {
        if (inner[i] == '?' && q == -1) { q = i; }
        else if (inner[i] == ':' && c == -1) { c = i; }
    }
    if (q == -1 || c == -1 || c < q) 
    {
        call.first = trimText(inner);
        return;
    }
    call.isTernary = true;
    call.condition = (trimText(inner.substr(0, q)) == "true");
    call.first = trimText(inner.substr(q + 1, c - q - 1));
    call.second = trimText(inner.substr(c + 1));
}
// Format: name(recursion){nested calls} memory  -> recursion and nested calls are optional
bool parseDefinition(const string& line, FunctionDef& def) {
    def.name = "";
    def.hasRecursion = false;
    def.recursionExpr = "";
    def.recursionCount = 1;
    def.calls = nullptr;
    def.callCount = 0;
    def.requestedMemory = 0;
    def.allocatedMemory = 0;
    def.successCount = 0;

    string s = trimText(line);
    int len = (int)s.length();
    int p = 0;
    while (p < len && s[p] != '(' && s[p] != '{' && s[p] != ' ' && s[p] != '\t') { p++; }
    def.name = s.substr(0, p);
    while (p < len && (s[p] == ' ' || s[p] == '\t')) { p++; }

    if (p < len && s[p] == '(') 
    {
        int depth = 0;
        int start = p + 1;
        while (p < len) 
        {
            if (s[p] == '(') { depth++; }
            else if (s[p] == ')') 
            {
                depth--;
                if (depth == 0) { break; }
            }
            p++;
        }
        if (p >= len) { return false; }
        def.recursionExpr = trimText(s.substr(start, p - start));
        def.hasRecursion = !def.recursionExpr.empty();
        p++;
    }
    while (p < len && (s[p] == ' ' || s[p] == '\t')) { p++; }

    string body = "";
    if (p < len && s[p] == '{') 
    {
        int depth = 0;
        int start = p + 1;
        while (p < len) 
        {
            if (s[p] == '{') { depth++; }
            else if (s[p] == '}') 
            {
                depth--;
                if (depth == 0) { break; }
            }
            p++;
        }
        if (p >= len) { return false; }
        body = s.substr(start, p - start);
        p++;
    }

    string rest = trimText(s.substr(p));
    bool digitsOnly = !rest.empty() && rest.length() <= 18;
    for (int i = 0; i < (int)rest.length(); i++) 
    {
        if (!isDigitChar(rest[i])) { digitsOnly = false; }
    }
    if (digitsOnly) 
    {
        long long value = 0;
        for (int i = 0; i < (int)rest.length(); i++) { value = value * 10 + (rest[i] - '0'); }
        def.requestedMemory = value;
    }

    def.callCount = countNestedItems(body);
    if (def.callCount > 0) 
    {
        def.calls = new NestedCall[def.callCount];
        int index = 0;
        int itemStart = 0;
        for (int i = 0; i <= (int)body.length(); i++) 
        {
            if (i == (int)body.length() || body[i] == ',') 
            {
                parseNestedItem(body.substr(itemStart, i - itemStart), def.calls[index]);
                index++;
                itemStart = i + 1;
            }
        }
    }
    return true;
}
void destroyFunctions(FunctionDef* defs, int count) {
    if (!defs) { return; }
    for (int i = 0; i < count; i++) 
    {
        delete[] defs[i].calls;
        defs[i].calls = nullptr;
    }
    delete[] defs;
}

// ============================= Static Validation =============================

bool isValidFunctionName(const string& name) {
    if (name.empty() || !isAlphaChar(name[0])) { return false; }
    for (int i = 1; i < (int)name.length(); i++) 
    {
        if (!isAlphaChar(name[i]) && !isDigitChar(name[i])) { return false; }
    }
    return true;
}
// First definition wins, -1 when the name is not defined
int findFunction(const Simulator& sim, const string& name) {
    for (int i = 0; i < sim.defCount; i++) 
    {
        if (sim.defs[i].name == name) { return i; }
    }
    return -1;
}
// Name rules, duplicate definitions, memory and recursion-count checks
int validateDefinitions(Simulator& sim) {
    int errors = 0;
    for (int i = 0; i < sim.defCount; i++) 
    {
        FunctionDef& f = sim.defs[i];
        if (!isValidFunctionName(f.name)) 
        {
            cout << "Error: Invalid function name " << f.name << "\n";
            errors++;
        }
        if (findFunction(sim, f.name) != i) 
        {
            cout << "Error: Duplicate definition of " << f.name << "\n";
            errors++;
        }
        if (f.requestedMemory < 1) 
        {
            cout << "Error: Invalid requested memory for " << f.name << "\n";
            errors++;
        }
        else { f.allocatedMemory = alignMemory(f.requestedMemory); }
        if (f.hasRecursion) 
        {
            long long count = 0;
            if (!evaluateExpression(f.recursionExpr, count) || count < 1 || count > 1000000) 
            {
                cout << "Error: Invalid recursion count for " << f.name << "\n";
                errors++;
            }
            else { f.recursionCount = (int)count; }
        }
    }
    return errors;
}
// Every nested reference (both ternary branches) and every top-level call must be defined
int validateReferences(const Simulator& sim, const string* topCalls, int topCount) {
    int errors = 0;
    for (int i = 0; i < sim.defCount; i++) 
    {
        const FunctionDef& f = sim.defs[i];
        for (int j = 0; j < f.callCount; j++) 
        {
            int branches = f.calls[j].isTernary ? 2 : 1;
            for (int b = 0; b < branches; b++) 
            {
                const string& target = (b == 0) ? f.calls[j].first : f.calls[j].second;
                if (findFunction(sim, target) < 0) 
                {
                    cout << "Error: Undefined function " << target << " called by " << f.name << "\n";
                    errors++;
                }
            }
        }
    }
    for (int i = 0; i < topCount; i++) 
    {
        if (findFunction(sim, topCalls[i]) < 0) 
        {
            cout << "Error: Undefined top-level function " << topCalls[i] << "\n";
            errors++;
        }
    }
    return errors;
}
// state: 0 = unvisited, 1 = on current path, 2 = fully explored
bool findCycle(const Simulator& sim, int node, int* state, int* path, int& depth) {
    state[node] = 1;
    path[depth] = node;
    depth++;
    const FunctionDef& f = sim.defs[node];
    for (int j = 0; j < f.callCount; j++) 
    {
        int branches = f.calls[j].isTernary ? 2 : 1;
        for (int b = 0; b < branches; b++) 
        {
            const string& target = (b == 0) ? f.calls[j].first : f.calls[j].second;
            int next = findFunction(sim, target);
            if (next < 0) { continue; }
            if (state[next] == 1) 
            {
                int start = 0;
                while (path[start] != next) { start++; }
                cout << "Error: Circular Dependency: ";
                for (int k = start; k < depth; k++) { cout << sim.defs[path[k]].name << " -> "; }
                cout << sim.defs[next].name << "\n";
                return true;
            }
            if (state[next] == 0 && findCycle(sim, next, state, path, depth)) { return true; }
        }
    }
    state[node] = 2;
    depth--;
    return false;
}
// Global check over all definitions, reports only the first cycle found
int checkCircularDependency(const Simulator& sim) {
    int* state = new int[sim.defCount];
    int* path = new int[sim.defCount + 1];
    for (int i = 0; i < sim.defCount; i++) { state[i] = 0; }
    int found = 0;
    for (int i = 0; i < sim.defCount && !found; i++) 
    {
        if (findFunction(sim, sim.defs[i].name) != i || state[i] != 0) { continue; }
        int depth = 0;
        if (findCycle(sim, i, state, path, depth)) { found = 1; }
    }
    delete[] state;
    delete[] path;
    return found;
}

// ================================ Execution ================================

// One invocation: recursion first, then the nested-call body of this invocation
void runInvocation(Simulator& sim, int fIdx, int remaining) {
    FunctionDef& f = sim.defs[fIdx];
    sim.stats.attempts++;
    long long available = sim.stack.totalMemory - sim.stack.usedMemory;
    if (f.allocatedMemory > available) 
    {
        sim.stats.overflows++;
        cout << "Error: Stack overflow while calling " << f.name << "\n";
        return;
    }

    pushFrame(sim.stack, f.name, f.allocatedMemory);
    sim.stats.successes++;
    f.successCount++;
    if (sim.stack.top > sim.stats.maxDepth) { sim.stats.maxDepth = sim.stack.top; }
    if (sim.stack.usedMemory > sim.stats.maxMemory) { sim.stats.maxMemory = sim.stack.usedMemory; }
    cout << f.name << " called\n";
    displayStack(sim.stack);

    if (remaining > 1) { runInvocation(sim, fIdx, remaining - 1); }
    executeBody(sim, fIdx);

    popFrame(sim.stack);
    cout << f.name << " finished\n";
    displayStack(sim.stack);
}
void executeBody(Simulator& sim, int fIdx) {
    const FunctionDef& f = sim.defs[fIdx];
    for (int j = 0; j < f.callCount; j++) 
    {
        const NestedCall& c = f.calls[j];
        const string& target = (!c.isTernary || c.condition) ? c.first : c.second;
        int next = findFunction(sim, target);
        if (next >= 0) { runInvocation(sim, next, sim.defs[next].recursionCount); }
    }
}
void printSummary(const Simulator& sim) {
    int best = -1;
    for (int i = 0; i < sim.defCount; i++) 
    {
        int bestCount = (best < 0) ? 0 : sim.defs[best].successCount;
        if (sim.defs[i].successCount > bestCount) { best = i; }
    }
    cout << "\nExecution Summary\n";
    cout << "-----------------\n";
    cout << "Total call attempts: " << sim.stats.attempts << "\n";
    cout << "Successful calls: " << sim.stats.successes << "\n";
    cout << "Skipped due to stack overflow: " << sim.stats.overflows << "\n";
    cout << "Maximum stack depth reached: " << sim.stats.maxDepth << "\n";
    cout << "Maximum stack memory used: " << sim.stats.maxMemory << " B\n";
    cout << "Total stack capacity: " << sim.stack.totalMemory << " B\n";
    cout << "Most frequently called function: " << ((best < 0) ? "None" : sim.defs[best].name) << "\n";
}

// ============================= Test Case Handling =============================

// lines holds only the non-blank lines of one test case
void processTestCase(const string* lines, int lineCount, int caseNumber) {
    cout << "========== Test Case " << caseNumber << " ==========\n";

    int n = 0;
    int m = 0;
    long long s = 0;
    if (!parseHeader(lines[0], n, m, s)) 
    {
        cout << "Error: Invalid test case header\n";
        return;
    }
    bool valid = true;
    if (n < 1 || n > 50) 
    {
        cout << "Error: Number of definitions must be between 1 and " << 50 << "\n";
        valid = false;
    }
    if (m < 1 || m > 50) 
    {
        cout << "Error: Number of top-level calls must be between 1 and " << 50 << "\n";
        valid = false;
    }
    if (s < 1) 
    {
        cout << "Error: Stack capacity must be a positive integer\n";
        valid = false;
    }
    if (!valid) { return; }
    if (lineCount < 1 + n + m) 
    {
        cout << "Error: Test case has fewer lines than declared\n";
        return;
    }

    Simulator sim;
    sim.defCount = n;
    sim.defs = new FunctionDef[n];
    sim.stack.data = nullptr;
    sim.stack.top = 0;
    sim.stack.capacity = 0;
    sim.stack.usedMemory = 0;
    sim.stack.totalMemory = s;
    sim.stats.attempts = 0;
    sim.stats.successes = 0;
    sim.stats.overflows = 0;
    sim.stats.maxDepth = 0;
    sim.stats.maxMemory = 0;

    bool malformed = false;
    for (int i = 0; i < n; i++) 
    {
        if (!parseDefinition(lines[1 + i], sim.defs[i])) 
        {
            cout << "Error: Malformed definition: " << trimText(lines[1 + i]) << "\n";
            malformed = true;
        }
    }
    string* topCalls = new string[m];
    for (int i = 0; i < m; i++) { topCalls[i] = trimText(lines[1 + n + i]); }

    int errors = 0;
    if (malformed) { errors++; }
    else 
    {
        errors += validateDefinitions(sim);
        errors += validateReferences(sim, topCalls, m);
        errors += checkCircularDependency(sim);
    }

    if (errors == 0) 
    {
        initStack(sim.stack, s);
        for (int i = 0; i < m; i++) 
        {
            int idx = findFunction(sim, topCalls[i]);
            runInvocation(sim, idx, sim.defs[idx].recursionCount);
        }
        printSummary(sim);
        destroyStack(sim.stack);
    }
    delete[] topCalls;
    destroyFunctions(sim.defs, n);
}
// Reads the whole file into a dynamic array of lines, returns -1 if the file cannot be opened
int readAllLines(const char* fileName, string*& lines) {
    ifstream file(fileName);
    if (!file.is_open()) { return -1; }
    int capacity = 64;
    int count = 0;
    lines = new string[capacity];
    string line;
    while (getline(file, line)) 
    {
        if (!line.empty() && line[line.length() - 1] == '\r') { line.erase(line.length() - 1); }
        if (count == capacity) 
        {
            string* bigger = new string[capacity * 2];
            for (int i = 0; i < count; i++) { bigger[i] = lines[i]; }
            delete[] lines;
            lines = bigger;
            capacity *= 2;
        }
        lines[count] = line;
        count++;
    }
    file.close();
    return count;
}

int main(int argc, char* argv[]) {
    const char* fileName = "input.txt";
    if (argc > 1) { fileName = argv[1]; }

    string* lines = nullptr;
    int total = readAllLines(fileName, lines);
    if (total < 0) 
    {
        cout << "Error: Could not open file " << fileName << "\n";
        return 1;
    }

    int caseNumber = 0;
    int start = 0;
    for (int i = 0; i <= total; i++) 
    {
        if (i == total || trimText(lines[i]) == "###") 
        {
            int count = 0;
            for (int j = start; j < i; j++) 
            {
                if (!isBlankLine(lines[j])) { count++; }
            }
            if (count > 0) 
            {
                string* caseLines = new string[count];
                int k = 0;
                for (int j = start; j < i; j++) 
                {
                    if (!isBlankLine(lines[j])) 
                    {
                        caseLines[k] = lines[j];
                        k++;
                    }
                }
                caseNumber++;
                if (caseNumber > 1) { cout << "\n"; }
                processTestCase(caseLines, count, caseNumber);
                delete[] caseLines;
            }
            start = i + 1;
        }
    }
    delete[] lines;
    return 0;
}