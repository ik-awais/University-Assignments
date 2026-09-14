#ifndef Q2_H
#define Q2_H

#ifndef ROLL_N
#define ROLL_N 3207
#endif
#define P1 (((ROLL_N) % 4) + 3)
#define P2 (((ROLL_N) % 3) + 2)
#define P3 (((ROLL_N) % 5) + 1)
#define SEED ((ROLL_N) % 100)

struct Appointment {
    int clientId;
    char* clientName;
    char service[9];
    float price;
};

struct DaySchedule {
    Appointment* slots;
    int count;
    int capacity;
};

struct Week {
    DaySchedule* days;
    int dayCount;
};

#define DAYS_IN_WEEK 7
#define NAME_LIMIT 14
#define SERVICE_LIMIT 8
#define MAX_SLOTS 40
#define CLIENT_MIN 1
#define CLIENT_MAX 9999

int myStrLen(const char* s);
void myStrCopy(char* dest, const char* src);
int myStrCompare(const char* a, const char* b);
char* cloneCString(const char* src);

void fillPrices(float* begin, float* end, int n);
float sumRange(const float* begin, const float* end);
float* maxElementPtr(float* begin, float* end);
int countAbove(const float* begin, const float* end, float threshold);
void reverseInPlace(float* begin, float* end);
void reportSizes();

void initWeek(Week& w);
bool growDay(DaySchedule& d);
bool bookAppointment(Week& w, int day, int clientId, const char* name, const char* service, float price);
bool cancelAppointment(Week& w, int day, int slot);
Appointment* findAppointment(const Week& w, int clientId, int& outDay, int& outSlot);
float dayRevenue(const DaySchedule& d);
void printDay(const DaySchedule& d, int dayIndex);
void printWeek(const Week& w);

bool moveAppointment(Week& w, int fromDay, int fromSlot, int toDay);
Appointment** buildIndex(const Week& w, int& outCount);
void sortIndexByPrice(Appointment** index, int n);
void printIndex(Appointment** index, int n);
void destroyIndex(Appointment**& index, int& n);
void destroyWeek(Week& w);
bool bookByValue(DaySchedule day, int clientId, const char* name, const char* service, float price);
void loadSeedWeek(Week& w);

#endif
