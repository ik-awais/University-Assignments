#include <iostream>
#include <cstdio>
#include "Q1.h"

int myStrLen(const char* s) {
    if (!s) { return 0; }
    int i = 0;
    while (*(s + i)) { i++; }
    return i;
}
void myStrCopy(char* dest, const char* src) {
    if (!dest || !src) { return; }
    int i = 0;
    while (*(src + i)) 
    {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}
int myStrCompare(const char* a, const char* b) {
    if (!a || !b) { return 0; }
    int i = 0;
    while (*(a + i) && *(b + i) && *(a + i) == *(b + i)) { i++; }
    return ((int)(unsigned char)*(a + i) - (int)(unsigned char)*(b + i));
}
char* cloneCString(const char* src) {
    if (!src) { return nullptr; }
    int len = myStrLen(src);
    char* copy = new char[len + 1];
    myStrCopy(copy, src);
    return copy;
}
void reportSizes() {
    std::cout << "SIZEOF char=" << (int)sizeof(char)
    << " int=" << (int)sizeof(int) << " float=" << (int)sizeof(float)
    << " ptr=" << (int)sizeof(char*) << "\n";
    int readingPayload = (int)sizeof(char[9]) + (int)sizeof(float) + 
                          (int)sizeof(char);
    int readingSize = (int)sizeof(Reading);
    std::cout << "SIZEOF Reading=" << readingSize
    << " payload=" << readingPayload
    << " padding=" << readingSize - readingPayload
    << "\n";
    int probePayload = (int)sizeof(int) + (int)sizeof(char*) + 
                        (int)sizeof(Reading*) + (int)sizeof(int) + 
                        (int)sizeof(int);
    int probeSize = (int)sizeof(Probe);
    std::cout << "SIZEOF Probe=" << probeSize
    << " payload=" << probePayload
    << " padding=" << probeSize - probePayload << "\n";
    int fleetPayload = (int)sizeof(Probe**) + (int)sizeof(int) + (int)sizeof(int);
    int fleetSize = (int)sizeof(Fleet);
    std::cout << "SIZEOF Fleet=" << fleetSize
    << " payload=" << fleetPayload
    << " padding=" << fleetSize - fleetPayload << "\n";
}

void initFleet(Fleet& f, int initialCapacity) {
    if (initialCapacity < 1) { initialCapacity = 1; }
    f.probes = new Probe*[initialCapacity];
    for (int i = 0; i < initialCapacity; i++){*(f.probes + i) = nullptr;}
    f.count = 0;
    f.capacity = initialCapacity;
}
bool growFleet(Fleet& f) {
    int resize = (!f.capacity) ? P1 : f.capacity*2;
    if (resize > MAX_PROBES) 
    {
        std::cout << "ERR FLEET_FULL\n";
        return false;
    }
    Probe** newProbe = new Probe*[resize];
    for (int i = 0; i < resize; i++) { *(newProbe + i) = nullptr; }
    for (int i = 0; i < f.count; i++) {*(newProbe + i) = *(f.probes + i);}
    delete[] f.probes;
    f.probes = newProbe;
    f.capacity = resize;
    return true;
}
bool addProbe(Fleet& f, int probeId, const char* callSign) {
    if (probeId < ID_MIN || probeId > ID_MAX) 
    {
        std::cout << "ERR BAD_ID\n";
        return false;
    }
    int signLen = myStrLen(callSign);
    if (signLen < 1 || signLen > SIGN_LIMIT) 
    {
        std::cout << "ERR BAD_SIGN\n";
        return false;
    }
    if (findSlot(f, probeId)) 
    {
        std::cout << "ERR DUP_ID\n";
        return false;
    }
    if (f.count == f.capacity) 
    {
        if (!growFleet(f)) { return false; }
    }
    Probe* p = new Probe;
    p->probeId = probeId;
    p->callSign = cloneCString(callSign);
    p->readings = nullptr;
    p->readingCount = 0;
    p->readingCapacity = 0;
    *(f.probes + f.count) = p;
    f.count++;
    return true;
}
bool growReadings(Probe* p) {
    if (!p) { return false; }
    int resize = (!p->readingCapacity) ? P2 : p->readingCapacity + P3;
    if (resize > MAX_READINGS) 
    {
        std::cout << "ERR LOG_FULL\n";
        return false;
    }
    Reading* newRead = new Reading[resize];
    for (int i = 0; i < p->readingCount; i++) 
    {
        myStrCopy((newRead + i)->sensor, (p->readings + i)->sensor);
        (newRead + i)->value = (p->readings + i)->value;
        (newRead + i)->status = (p->readings + i)->status;
    }
    delete[] p->readings;
    p->readings = newRead;
    p->readingCapacity = resize;
    return true;
}
bool addReading(Probe* p, const char* sensor, float value, char status) {
    if (!p) 
    {
        std::cout << "ERR NO_PROBE\n";
        return false;
    }
    int sensorLen = myStrLen(sensor);
    if (sensorLen < 1 || sensorLen > SENSOR_LIMIT) 
    {
        std::cout << "ERR BAD_SENSOR\n";
        return false;
    }
    if (status != 'N' && status != 'W' && status != 'C') 
    {
        std::cout << "ERR BAD_STATUS\n";
        return false;
    }
    for (int i = 0; i < p->readingCount; i++) 
    {
        if (!myStrCompare((p->readings + i)->sensor, sensor)) {
            std::cout << "ERR DUP_SENSOR\n";
            return false;
        }
    }
    if (p->readingCount == p->readingCapacity) 
    {
        if (!growReadings(p)) { return false; }
    }
    myStrCopy((p->readings + p->readingCount)->sensor, sensor);
    (p->readings + p->readingCount)->value = value;
    (p->readings + p->readingCount)->status = status;
    p->readingCount++;
    return true;
}
Probe** findSlot(const Fleet& f, int probeId) {
    if (!f.probes || !f.count) { return nullptr; }
    for (int i = 0; i < f.count; i++) 
    {
        if (*(f.probes + i) && (*(f.probes + i))->probeId == probeId) 
        {
            return f.probes + i;
        }
    }
    return nullptr;
}
Probe* findProbe(const Fleet& f, int probeId) {
    Probe** slot = findSlot(f, probeId);
    if (!slot) { return nullptr; }
    return *slot;
}
float probeHealth(const Probe* p) {
    if (!p || !p->readingCount) { return 0.0f; }
    float sum = 0.0f;
    for (int i = 0; i < p->readingCount; i++) 
    {
        char status = (p->readings + i)->status;
        if (status == 'N') { sum += 1.0f; } 
        else if (status == 'W') { sum += 0.5f; }
    }
    return (sum / p->readingCount);
}
void printProbe(const Probe* p) {
    if (!p) 
    {
        printf("ERR NO_PROBE\n");
        return;
    }
    printf("PROBE %04d | %-20s | LOGS=%d | HEALTH=%.2f\n",
           p->probeId, p->callSign, p->readingCount, probeHealth(p));
    if (!p->readingCount) { printf(" (no telemetry)\n"); } 
    else 
    {
        for (int i = 0; i < p->readingCount; i++) 
        {
            printf(" <%02d> %-8s VAL=%8.2f ST=%c\n",
                   i, (p->readings + i)->sensor, (p->readings + i)->value, (p->readings + i)->status);
        }
    }
}
void printFleet(const Fleet& f) {
    printf("FLEET count=%d capacity=%d\n", f.count, f.capacity);
    if (!f.count) { printf(" (empty)\n"); } 
    else 
    {
        for (int i = 0; i < f.count; i++) 
        {
            printProbe(*(f.probes + i));
        }
    }
    printf("END FLEET\n");
}

void destroyProbe(Probe*& p) {
    if (!p) { return; }
    delete[] p->callSign;
    delete[] p->readings;
    delete p;
    p = nullptr;
}
void compactFleet(Fleet& f, int removedIndex) {
    if (removedIndex < 0 || removedIndex >= f.count) { return; }
    for (int i = removedIndex; i < f.count - 1; i++) { *(f.probes + i) = *(f.probes + i + 1); }
    *(f.probes + f.count - 1) = nullptr;
    f.count--;
}
bool removeProbe(Fleet& f, int probeId) {
    Probe** slot = findSlot(f, probeId);
    if (!slot) 
    {
        std::cout << "ERR NOT_FOUND\n";
        return false;
    }
    int index = (int)(slot - f.probes);
    destroyProbe(*slot);
    compactFleet(f, index);
    return true;
}
void deepCopyProbe(const Probe* src, Probe*& dest) {
    if (!src) 
    {
        dest = nullptr;
        return;
    }
    Probe* p = new Probe;
    p->probeId = src->probeId;
    p->callSign = cloneCString(src->callSign);
    if (src->readingCapacity) 
    {
        p->readings = new Reading[src->readingCapacity];
        for (int i = 0; i < src->readingCount; i++) 
        {
            myStrCopy((p->readings + i)->sensor, (src->readings + i)->sensor);
            (p->readings + i)->value = (src->readings + i)->value;
            (p->readings + i)->status = (src->readings + i)->status;
        }
    } 
    else { p->readings = nullptr; }
    p->readingCount = src->readingCount;
    p->readingCapacity = src->readingCapacity;
    dest = p;
}
void aliasCopyProbe(Probe* src, Probe* dest) {
    if (!src || !dest) { return; }
    dest->probeId = src->probeId;
    dest->callSign = src->callSign;
    dest->readings = src->readings;
    dest->readingCount = src->readingCount;
    dest->readingCapacity = src->readingCapacity;
}
bool mergeFleets(Fleet& target, const Fleet& source) {
    for (int i = 0; i < source.count; i++) 
    {
        Probe* srcProbe = *(source.probes + i);
        if (!srcProbe) { continue; }
        if (findSlot(target, srcProbe->probeId)) { continue; }
        if (target.count == target.capacity) 
        {
            if (!growFleet(target)) { return false; }
        }
        Probe* copy = nullptr;
        deepCopyProbe(srcProbe, copy);
        *(target.probes + target.count) = copy;
        target.count++;
    }
    return true;
}
void destroyFleet(Fleet& f) {
    if (!f.probes) 
    {
        f.count = 0;
        f.capacity = 0;
        return;
    }
    for (int i = 0; i < f.count; i++) { destroyProbe(*(f.probes + i)); }
    delete[] f.probes;
    f.probes = nullptr;
    f.count = 0;
    f.capacity = 0;
}
bool addProbeByValue(Fleet f, int probeId, const char* callSign) {
    if (probeId < ID_MIN || probeId > ID_MAX) 
    {
        std::cout << "ERR BAD_ID\n";
        return false;
    }
    int signLen = myStrLen(callSign);
    if (signLen < 1 || signLen > SIGN_LIMIT) 
    {
        std::cout << "ERR BAD_SIGN\n";
        return false;
    }
    if (findSlot(f, probeId)) 
    {
        std::cout << "ERR DUP_ID\n";
        return false;
    }
    if (f.count == f.capacity) 
    {
        if (!growFleet(f)) { return false; }
    }
    Probe* p = new Probe;
    p->probeId = probeId;
    p->callSign = cloneCString(callSign);
    p->readings = nullptr;
    p->readingCount = 0;
    p->readingCapacity = 0;
    *(f.probes + f.count) = p;
    f.count++;
    return true;
}
void loadFleetA(Fleet& f) {
    destroyFleet(f);
    initFleet(f, P1);
    const char* SIGNS[8] = {"Voyager","Pathfinder","Odyssey","Horizon","Sentinel","Aurora","Vanguard","Meridian"};
    const char* SENSORS[6] = {"TEMP-A","PWR-BUS","RAD-CNT","GYRO-X","COMMS-1","FUEL-P"};
    const char* STATUS = "NWC";
    int total = (SEED % 3) + 3;
    for (int i = 0; i < total; i++) 
    {
        int id = 1000 + SEED + 11 * i;
        const char* sign = *(SIGNS + (SEED + i) % 8);
        addProbe(f, id, sign);
        Probe* p = findProbe(f, id);
        int logs = (SEED + i) % 4;
        for (int j = 0; j < logs; j++) 
        {
            const char* sensor = *(SENSORS + (SEED + i + j) % 6);
            float value = (float)(SEED + 10 * i + 3 * j) + 0.5f;
            char status = *(STATUS + (SEED + 2 * i + j) % 3);
            addReading(p, sensor, value, status);
        }
    }
}
void loadFleetB(Fleet& f) {
    destroyFleet(f);
    initFleet(f, 2);
    const char* SENSORS[6] = {"TEMP-A","PWR-BUS","RAD-CNT","GYRO-X","COMMS-1","FUEL-P"};
    const char* STATUS = "NWC";
    int idAlpha = 1000 + SEED;
    addProbe(f, idAlpha, "Relay-Alpha");
    Probe* alpha = findProbe(f, idAlpha);
    addReading(alpha, *(SENSORS + SEED % 6), (float)SEED + 0.5f, *(STATUS + SEED % 3));
    int idBeta = 8000 + SEED;
    addProbe(f, idBeta, "Relay-Beta");
    Probe* beta = findProbe(f, idBeta);
    addReading(beta, *(SENSORS + (SEED + 1) % 6), (float)SEED + 20.5f, *(STATUS + (SEED + 1) % 3));
    addReading(beta, *(SENSORS + (SEED + 2) % 6), (float)SEED + 40.5f, *(STATUS + (SEED + 2) % 3));
}

void printMenu() {
    std::cerr << "---- MISSION CONTROL MENU ----\n";
    std::cerr << " 0  EXIT\n";
    std::cerr << " 1  ADD id sign\n";
    std::cerr << " 2  LOG id sensor value status\n";
    std::cerr << " 3  REMOVE id\n";
    std::cerr << " 4  FIND id\n";
    std::cerr << " 5  PRINT_A\n";
    std::cerr << " 6  GROW\n";
    std::cerr << " 7  ADD_BYVAL id sign\n";
    std::cerr << " 8  CLONE id\n";
    std::cerr << " 9  PRINT_CLONE\n";
    std::cerr << "10  ALIAS id\n";
    std::cerr << "11  SEED_A\n";
    std::cerr << "12  SEED_B\n";
    std::cerr << "13  PRINT_B\n";
    std::cerr << "14  MERGE_B\n";
    std::cerr << "15  MERGE_SELF\n";
    std::cerr << "16  DESTROY\n";
    std::cerr << "17  SIZES\n";
    std::cerr << "-------------------------------\n";
    std::cerr << "Enter a command number (and its arguments, space-separated):\n";
}

int main() {
    printf("=== MISSION CONTROL ===\n");
    printf("ROLL_N=%d P1=%d P2=%d P3=%d\n", ROLL_N, P1, P2, P3);
    printMenu();
    Fleet A, B;
    initFleet(A, P1);
    initFleet(B, 2);
    Probe* clone = nullptr;
    int choice;
    bool running = true;
    while (running)
    {
        std::cerr << "> ";
        if (!(std::cin >> choice)) { break; }
        switch (choice) 
        {
            case 0: 
            {
                running = false;
                break;
            }
            case 1: 
            {
                int id;
                char sign[64];
                std::cin >> id;
                std::cin.width(sizeof(sign));
                std::cin >> sign;
                if (addProbe(A, id, sign)) 
                {
                    printf("OK PROBE_ADDED %04d\n", id);
                }
                break;
            }
            case 2: 
            {
                int id;
                char sensor[64];
                float value;
                char status[64];
                std::cin >> id;
                std::cin.width(sizeof(sensor));
                std::cin >> sensor;
                std::cin >> value;
                std::cin.width(sizeof(status));
                std::cin >> status;
                Probe* p = findProbe(A, id);
                if (!p) 
                {
                    printf("ERR NOT_FOUND\n");
                } 
                else 
                {
                    if (addReading(p, sensor, value, status[0])) 
                    {
                        printf("OK LOG_ADDED %04d %s\n", id, sensor);
                    }
                }
                break;
            }
            case 3: 
            {
                int id;
                std::cin >> id;
                if (removeProbe(A, id)) 
                {
                    printf("OK PROBE_REMOVED %04d\n", id);
                }
                break;
            }
            case 4: 
            {
                int id;
                std::cin >> id;
                Probe* p = findProbe(A, id);
                if (!p) 
                {
                    printf("ERR NOT_FOUND\n");
                } 
                else 
                {
                    printf("OK FOUND %04d\n", id);
                    printProbe(p);
                }
                break;
            }
            case 5: 
            {
                printf("A:\n");
                printFleet(A);
                break;
            }
            case 6: 
            {
                if (growFleet(A)) 
                {
                    printf("OK FLEET_GROWN capacity=%d\n", A.capacity);
                }
                break;
            }
            case 7: 
            {
                int id;
                char sign[64];
                std::cin >> id;
                std::cin.width(sizeof(sign));
                std::cin >> sign;
                printf("BYVAL pre count=%d capacity=%d\n", A.count, A.capacity);
                bool result = addProbeByValue(A, id, sign);
                printf("BYVAL returned=%d\n", result ? 1 : 0);
                printf("BYVAL post count=%d capacity=%d\n", A.count, A.capacity);
                break;
            }
            case 8: 
            {
                int id;
                std::cin >> id;
                Probe* p = findProbe(A, id);
                if (!p) 
                {
                    printf("ERR NOT_FOUND\n");
                } 
                else 
                {
                    destroyProbe(clone);
                    deepCopyProbe(p, clone);
                    printf("OK CLONED %04d\n", id);
                }
                break;
            }
            case 9: 
            {
                if (!clone) 
                {
                    printf("ERR NO_CLONE\n");
                } 
                else 
                {
                    printf("CLONE:\n");
                    printProbe(clone);
                }
                break;
            }
            case 10: 
            {
                int id;
                std::cin >> id;
                Probe* found = findProbe(A, id);
                if (!found) 
                {
                    printf("ERR NOT_FOUND\n");
                } 
                else 
                {
                    Probe stackProbe;
                    stackProbe.probeId = 0;
                    stackProbe.callSign = nullptr;
                    stackProbe.readings = nullptr;
                    stackProbe.readingCount = 0;
                    stackProbe.readingCapacity = 0;
                    aliasCopyProbe(found, &stackProbe);
                    int signSame = (stackProbe.callSign == found->callSign) ? 1 : 0;
                    int logsSame = (stackProbe.readings == found->readings) ? 1 : 0;
                    printf("ALIAS sign=%d logs=%d\n", signSame, logsSame);
                    if (stackProbe.callSign && *stackProbe.callSign) 
                    {
                        *stackProbe.callSign = 'Z';
                    }
                    char st;
                    if (stackProbe.readings && stackProbe.readingCount > 0) 
                    {
                        st = found->readings[0].status;
                        stackProbe.readings[0].status = 'C';
                    } 
                    else 
                    {
                        st = '-';
                    }
                    printf("ALIAS after sign=%s log0=%c\n", found->callSign, st);
                }
                break;
            }
            case 11: 
            {
                loadFleetA(A);
                printf("OK SEED_A count=%d\n", A.count);
                break;
            }
            case 12: 
            {
                loadFleetB(B);
                printf("OK SEED_B count=%d\n", B.count);
                break;
            }
            case 13: 
            {
                printf("B:\n");
                printFleet(B);
                break;
            }
            case 14: 
            {
                int before = A.count;
                bool ok = mergeFleets(A, B);
                if (ok) 
                {
                    printf("OK MERGED added=%d count=%d capacity=%d\n", A.count - before, A.count, A.capacity);
                } 
                else 
                {
                    printf("ERR MERGE_FAILED\n");
                }
                break;
            }
            case 15: 
            {
                int before = A.count;
                bool ok = mergeFleets(A, A);
                if (ok) 
                {
                    printf("OK MERGED added=%d count=%d capacity=%d\n", A.count - before, A.count, A.capacity);
                } 
                else 
                {
                    printf("ERR MERGE_FAILED\n");
                }
                break;
            }
            case 16: 
            {
                destroyFleet(A);
                printf("OK FLEET_DESTROYED\n");
                break;
            }
            case 17: 
            {
                reportSizes();
                break;
            }
            default: 
            {
                printf("ERR BAD_choice\n");
                break;
            }
        }
    }
    destroyProbe(clone);
    destroyFleet(A);
    destroyFleet(B);
    printf("BYE\n");
    return 0;
}