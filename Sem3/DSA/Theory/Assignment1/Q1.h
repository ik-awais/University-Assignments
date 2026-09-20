#ifndef Q1_H
#define Q1_H

#ifndef ROLL_N
#define ROLL_N 3012
#endif
#define P1 (((ROLL_N) % 4) + 3)
#define P2 (((ROLL_N) % 3) + 2)
#define P3 (((ROLL_N) % 5) + 1)
#define SEED ((ROLL_N) % 100)

struct Reading {
    char sensor[9];
    float value;
    char status;
};

struct Probe {
    int probeId;
    char* callSign;
    Reading* readings;
    int readingCount;
    int readingCapacity;
};

struct Fleet {
    Probe** probes;
    int count;
    int capacity;
};

#define SIGN_LIMIT 20
#define SENSOR_LIMIT 8
#define MAX_PROBES 4096
#define MAX_READINGS 64
#define ID_MIN 1
#define ID_MAX 9999

int myStrLen(const char* s);
void myStrCopy(char* dest, const char* src);
int myStrCompare(const char* a, const char* b);
char* cloneCString(const char* src);
void reportSizes();

void initFleet(Fleet& f, int initialCapacity);
bool growFleet(Fleet& f);
bool addProbe(Fleet& f, int probeId, const char* callSign);
bool growReadings(Probe* p);
bool addReading(Probe* p, const char* sensor, float value, char status);
Probe* findProbe(const Fleet& f, int probeId);
Probe** findSlot(const Fleet& f, int probeId);
float probeHealth(const Probe* p);
void printProbe(const Probe* p);
void printFleet(const Fleet& f);

void destroyProbe(Probe*& p);
void compactFleet(Fleet& f, int removedIndex);
bool removeProbe(Fleet& f, int probeId);
void deepCopyProbe(const Probe* src, Probe*& dest);
void aliasCopyProbe(Probe* src, Probe* dest);
bool mergeFleets(Fleet& target, const Fleet& source);
void destroyFleet(Fleet& f);
bool addProbeByValue(Fleet f, int probeId, const char* callSign);
void loadFleetA(Fleet& f);
void loadFleetB(Fleet& f);

#endif