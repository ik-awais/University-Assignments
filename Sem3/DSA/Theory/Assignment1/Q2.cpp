#include <iostream>
#include <cstdio>
#include "Q2.h"

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
void fillPrices(float* begin, float* end, int n) {
    int count = (int)(end - begin);
    for (int i = 0; i < count; i++) { *(begin + i) = (float)((n + 613 * i) % 4000) / 2.0f + 250.0f; }
}
float sumRange(const float* begin, const float* end) {
    int count = (int)(end - begin);
    float sum = 0.0f;
    for (int i = 0; i < count; i++) { sum += *(begin + i); }
    return sum;
}
float* maxElementPtr(float* begin, float* end) {
    int count = (int)(end - begin);
    if (!count) { return nullptr; }
    float* best = begin;
    for (int i = 1; i < count; i++) 
    {
        if (*(begin + i) > *best) { best = begin + i; }
    }
    return best;
}
int countAbove(const float* begin, const float* end, float threshold) {
    int count = (int)(end - begin);
    int total = 0;
    for (int i = 0; i < count; i++) 
    {
        if (*(begin + i) > threshold) { total++; }
    }
    return total;
}
void reverseInPlace(float* begin, float* end) {
    float* left = begin;
    float* right = end - 1;
    while (left < right) 
    {
        float temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}
void reportSizes() {
    int appointmentPayload = (int)sizeof(int) + (int)sizeof(char*) + 
                              (int)sizeof(char[9]) + (int)sizeof(float);
    int appointmentSize = (int)sizeof(Appointment);
    std::cout << "SIZEOF Appointment=" << appointmentSize
    << " payload=" << appointmentPayload
    << " padding=" << appointmentSize - appointmentPayload << "\n";
    int dayPayload = (int)sizeof(Appointment*) + (int)sizeof(int) + (int)sizeof(int);
    int daySize = (int)sizeof(DaySchedule);
    std::cout << "SIZEOF DaySchedule=" << daySize
    << " payload=" << dayPayload
    << " padding=" << daySize - dayPayload << "\n";
    int weekPayload = (int)sizeof(DaySchedule*) + (int)sizeof(int);
    int weekSize = (int)sizeof(Week);
    std::cout << "SIZEOF Week=" << weekSize
    << " payload=" << weekPayload
    << " padding=" << weekSize - weekPayload << "\n";
}

void initWeek(Week& w) {
    w.days = new DaySchedule[DAYS_IN_WEEK];
    for (int i = 0; i < DAYS_IN_WEEK; i++) 
    {
        (w.days + i)->slots = nullptr;
        (w.days + i)->count = 0;
        (w.days + i)->capacity = 0;
    }
    w.dayCount = DAYS_IN_WEEK;
}
bool growDay(DaySchedule& d) {
    int newCapacity = (!d.capacity) ? P2 : d.capacity + P3;
    if (newCapacity > MAX_SLOTS) 
    {
        std::cout << "ERR DAY_FULL\n";
        return false;
    }
    Appointment* newArr = new Appointment[newCapacity];
    for (int i = 0; i < d.count; i++) 
    {
        (newArr + i)->clientId = (d.slots + i)->clientId;
        (newArr + i)->clientName = (d.slots + i)->clientName;
        myStrCopy((newArr + i)->service, (d.slots + i)->service);
        (newArr + i)->price = (d.slots + i)->price;
    }
    delete[] d.slots;
    d.slots = newArr;
    d.capacity = newCapacity;
    return true;
}
bool bookAppointment(Week& w, int day, int clientId, const char* name, 
    const char* service, float price) {
    if (!w.days) 
    {
        std::cout << "ERR WEEK_DESTROYED\n";
        return false;
    }
    if (day < 0 || day >= w.dayCount) 
    {
        std::cout << "ERR BAD_DAY\n";
        return false;
    }
    if (clientId < CLIENT_MIN || clientId > CLIENT_MAX) 
    {
        std::cout << "ERR BAD_CLIENT\n";
        return false;
    }
    int nameLen = myStrLen(name);
    if (nameLen < 1 || nameLen > NAME_LIMIT) 
    {
        std::cout << "ERR BAD_NAME\n";
        return false;
    }
    int serviceLen = myStrLen(service);
    if (serviceLen < 1 || serviceLen > SERVICE_LIMIT) 
    {
        std::cout << "ERR BAD_SERVICE\n";
        return false;
    }
    if (price < 100.0f || price > 20000.0f) 
    {
        std::cout << "ERR BAD_PRICE\n";
        return false;
    }
    DaySchedule* d = w.days + day;
    for (int i = 0; i < d->count; i++) 
    {
        if ((d->slots + i)->clientId == clientId) 
        {
            std::cout << "ERR DUP_BOOKING\n";
            return false;
        }
    }
    if (d->count == d->capacity) 
    {
        if (!growDay(*d)) { return false; }
    }
    (d->slots + d->count)->clientId = clientId;
    (d->slots + d->count)->clientName = cloneCString(name);
    myStrCopy((d->slots + d->count)->service, service);
    (d->slots + d->count)->price = price;
    d->count++;
    return true;
}
bool cancelAppointment(Week& w, int day, int slot) {
    if (!w.days) 
    {
        std::cout << "ERR WEEK_DESTROYED\n";
        return false;
    }
    if (day < 0 || day >= w.dayCount) 
    {
        std::cout << "ERR BAD_DAY\n";
        return false;
    }
    DaySchedule* d = w.days + day;
    if (slot < 0 || slot >= d->count) 
    {
        std::cout << "ERR BAD_SLOT\n";
        return false;
    }
    delete[] (d->slots + slot)->clientName;
    for (int i = slot; i < d->count - 1; i++) { *(d->slots + i) = *(d->slots + i + 1); }
    d->count--;
    return true;
}
Appointment* findAppointment(const Week& w, int clientId, int& outDay, int& outSlot) {
    if (!w.days) 
    {
        outDay = -1;
        outSlot = -1;
        return nullptr;
    }
    for (int day = 0; day < w.dayCount; day++) 
    {
        DaySchedule* d = w.days + day;
        for (int slot = 0; slot < d->count; slot++) 
        {
            if ((d->slots + slot)->clientId == clientId) 
            {
                outDay = day;
                outSlot = slot;
                return d->slots + slot;
            }
        }
    }
    outDay = -1;
    outSlot = -1;
    return nullptr;
}
float dayRevenue(const DaySchedule& d) {
    float sum = 0.0f;
    for (int i = 0; i < d.count; i++) { sum += (d.slots + i)->price; }
    return sum;
}
void printDay(const DaySchedule& d, int dayIndex) {
    const char* DAYNAMES[7] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
    printf("DAY %d %s | BOOKED=%d/%d | REVENUE=%.2f\n", 
        dayIndex, *(DAYNAMES + dayIndex), d.count, d.capacity, dayRevenue(d));
    if (!d.count) { printf("  (free)\n"); } 
    else 
    {
        for (int i = 0; i < d.count; i++) 
        {
            printf(" <%02d> %04d %-14s %-8s PKR %8.2f\n",
                   i, (d.slots + i)->clientId, (d.slots + i)->clientName, 
                   (d.slots + i)->service, (d.slots + i)->price);
        }
    }
}
void printWeek(const Week& w) {
    if (!w.days) 
    {
        printf("WEEK bookings=0 revenue=0.00\n");
        printf("  (destroyed)\n");
        printf("END WEEK\n");
        return;
    }
    int totalBookings = 0;
    float totalRevenue = 0.0f;
    for (int i = 0; i < w.dayCount; i++) 
    {
        totalBookings += (w.days + i)->count;
        totalRevenue += dayRevenue(*(w.days + i));
    }
    printf("WEEK bookings=%d revenue=%.2f\n", totalBookings, totalRevenue);
    for (int i = 0; i < w.dayCount; i++) { printDay(*(w.days + i), i); }
    printf("END WEEK\n");
}

bool moveAppointment(Week& w, int fromDay, int fromSlot, int toDay) {
    if (!w.days) 
    {
        std::cout << "ERR WEEK_DESTROYED\n";
        return false;
    }
    if (fromDay < 0 || fromDay >= w.dayCount || toDay < 0 || toDay >= w.dayCount) 
    {
        std::cout << "ERR BAD_DAY\n";
        return false;
    }
    if (fromDay == toDay) 
    {
        std::cout << "ERR SAME_DAY\n";
        return false;
    }
    DaySchedule* src = w.days + fromDay;
    if (fromSlot < 0 || fromSlot >= src->count) 
    {
        std::cout << "ERR BAD_SLOT\n";
        return false;
    }
    DaySchedule* dst = w.days + toDay;
    int movingClientId = (src->slots + fromSlot)->clientId;
    for (int i = 0; i < dst->count; i++) 
    {
        if ((dst->slots + i)->clientId == movingClientId) 
        {
            std::cout << "ERR DUP_BOOKING\n";
            return false;
        }
    }
    if (dst->count == dst->capacity) 
    {
        if (!growDay(*dst)) { return false; }
    }
    (dst->slots + dst->count)->clientId = (src->slots + fromSlot)->clientId;
    (dst->slots + dst->count)->clientName = (src->slots + fromSlot)->clientName;
    myStrCopy((dst->slots + dst->count)->service, (src->slots + fromSlot)->service);
    (dst->slots + dst->count)->price = (src->slots + fromSlot)->price;
    dst->count++;
    for (int i = fromSlot; i < src->count - 1; i++) { *(src->slots + i) = *(src->slots + i + 1); }
    src->count--;
    return true;
}
Appointment** buildIndex(const Week& w, int& outCount) {
    if (!w.days) 
    {
        outCount = 0;
        return nullptr;
    }
    int total = 0;
    for (int day = 0; day < w.dayCount; day++) { total += (w.days + day)->count; }
    if (!total) 
    {
        outCount = 0;
        return nullptr;
    }
    Appointment** index = new Appointment*[total];
    int idx = 0;
    for (int day = 0; day < w.dayCount; day++) 
    {
        DaySchedule* d = w.days + day;
        for (int slot = 0; slot < d->count; slot++) 
        {
            *(index + idx) = d->slots + slot;
            idx++;
        }
    }
    outCount = total;
    return index;
}
void sortIndexByPrice(Appointment** index, int n) {
    if (!index) { return; }
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - 1 - i; j++) 
        {
            if ((*(index + j))->price < (*(index + j + 1))->price) 
            {
                Appointment* temp = *(index + j);
                *(index + j) = *(index + j + 1);
                *(index + j + 1) = temp;
            }
        }
    }
}
void printIndex(Appointment** index, int n) {
    printf("INDEX size=%d\n", n);
    if (!index || !n) { printf("  (none)\n"); } 
    else 
    {
        for (int i = 0; i < n; i++) 
        {
            printf("  [%02d] %04d %-14s %-8s PKR %8.2f\n",
                   i, (*(index + i))->clientId, (*(index + i))->clientName, 
                   (*(index + i))->service, (*(index + i))->price);
        }
    }
    printf("END INDEX\n");
}
void destroyIndex(Appointment**& index, int& n) {
    delete[] index;
    index = nullptr;
    n = 0;
}
void destroyWeek(Week& w) {
    if (!w.days) { return; }
    for (int day = 0; day < w.dayCount; day++) 
    {
        DaySchedule* d = w.days + day;
        for (int i = 0; i < d->count; i++) { delete[] (d->slots + i)->clientName; }
        delete[] d->slots;
    }
    delete[] w.days;
    w.days = nullptr;
    w.dayCount = 0;
}
bool bookByValue(DaySchedule day, int clientId, const char* name, const char* service, float price) {
    if (clientId < CLIENT_MIN || clientId > CLIENT_MAX) 
    {
        std::cout << "ERR BAD_CLIENT\n";
        return false;
    }
    int nameLen = myStrLen(name);
    if (nameLen < 1 || nameLen > NAME_LIMIT) 
    {
        std::cout << "ERR BAD_NAME\n";
        return false;
    }
    int serviceLen = myStrLen(service);
    if (serviceLen < 1 || serviceLen > SERVICE_LIMIT) 
    {
        std::cout << "ERR BAD_SERVICE\n";
        return false;
    }
    if (price < 100.0f || price > 20000.0f) 
    {
        std::cout << "ERR BAD_PRICE\n";
        return false;
    }
    for (int i = 0; i < day.count; i++) 
    {
        if ((day.slots + i)->clientId == clientId) 
        {
            std::cout << "ERR DUP_BOOKING\n";
            return false;
        }
    }
    if (day.count == day.capacity) 
    {
        if (!growDay(day)) { return false; }
    }
    (day.slots + day.count)->clientId = clientId;
    (day.slots + day.count)->clientName = cloneCString(name);
    myStrCopy((day.slots + day.count)->service, service);
    (day.slots + day.count)->price = price;
    day.count++;
    return true;
}
void loadSeedWeek(Week& w) {
    destroyWeek(w);
    initWeek(w);
    const char* NAMES[8] = {"Ayesha","Hina","Sana","Mahnoor","Zara","Iqra","Nimra","Rabia"};
    const char* SERVICES[6] = {"HAIRCUT","FACIAL","MANI","PEDI","COLOR","MAKEUP"};
    float PRICES[6] = {1500.0f, 2500.0f, 800.0f, 4200.0f, 1200.0f, 3000.0f};
    int total = (SEED % 3) + 9;
    for (int k = 0; k < total; k++) 
    {
        int day = 5 + (k % 2);
        int clientId = 500 + SEED + 7 * k;
        const char* name = *(NAMES + (SEED + k) % 8);
        int which = (SEED + k) % 6;
        const char* service = *(SERVICES + which);
        float price = *(PRICES + which);
        bookAppointment(w, day, clientId, name, service, price);
    }
    int lastDay = SEED % 5;
    int lastClientId = 500 + SEED + 7 * total;
    const char* lastName = *(NAMES + (SEED + total) % 8);
    int lastWhich = (SEED + total) % 6;
    const char* lastService = *(SERVICES + lastWhich);
    float lastPrice = *(PRICES + lastWhich);
    bookAppointment(w, lastDay, lastClientId, lastName, lastService, lastPrice);
}

void printMenu() {
    std::cerr << "---- GLOW & GRACE SALON MENU ----\n";
    std::cerr << " 0  EXIT\n";
    std::cerr << " 1  BOOK day id name service price\n";
    std::cerr << " 2  CANCEL day slot\n";
    std::cerr << " 3  PRINT_DAY day\n";
    std::cerr << " 4  PRINT_WEEK\n";
    std::cerr << " 5  MOVE fromDay fromSlot toDay\n";
    std::cerr << " 6  BUILD_INDEX\n";
    std::cerr << " 7  PRINT_INDEX\n";
    std::cerr << " 8  SORT_INDEX\n";
    std::cerr << " 9  DROP_INDEX\n";
    std::cerr << "10  FIND id\n";
    std::cerr << "11  SEED\n";
    std::cerr << "12  BOOK_BYVAL day id name service price\n";
    std::cerr << "13  DESTROY_WEEK\n";
    std::cerr << "14  UTILS\n";
    std::cerr << "----------------------------------\n";
    std::cerr << "Enter a command number (and its arguments, space-separated):\n";
}

int main() {
    printf("=== GLOW & GRACE SALON ===\n");
    printf("ROLL_N=%d P2=%d P3=%d\n", ROLL_N, P2, P3);
    printMenu();
    Week w;
    initWeek(w);
    Appointment** index = nullptr;
    int count = 0;
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
                int day, id;
                char name[64], service[64];
                float price;
                std::cin >> day >> id;
                std::cin.width(sizeof(name));
                std::cin >> name;
                std::cin.width(sizeof(service));
                std::cin >> service;
                std::cin >> price;
                if (index) { destroyIndex(index, count); }
                if (bookAppointment(w, day, id, name, service, price)) 
                {
                    printf("OK BOOKED d%d %04d\n", day, id);
                }
                break;
            }
            case 2: 
            {
                int day, slot;
                std::cin >> day >> slot;
                if (index) { destroyIndex(index, count); }
                if (cancelAppointment(w, day, slot)) 
                {
                    printf("OK CANCELLED d%d s%d\n", day, slot);
                }
                break;
            }
            case 3: 
            {
                int day;
                std::cin >> day;
                if (!w.days) 
                {
                    printf("ERR WEEK_DESTROYED\n");
                } 
                else if (day < 0 || day >= w.dayCount) 
                {
                    printf("ERR BAD_DAY\n");
                } 
                else 
                {
                    printDay(w.days[day], day);
                }
                break;
            }
            case 4: 
            {
                printWeek(w);
                break;
            }
            case 5: 
            {
                int fromDay, fromSlot, toDay;
                std::cin >> fromDay >> fromSlot >> toDay;
                if (index) { destroyIndex(index, count); }
                if (moveAppointment(w, fromDay, fromSlot, toDay)) 
                {
                    printf("OK MOVED d%d s%d -> d%d\n", fromDay, fromSlot, toDay);
                }
                break;
            }
            case 6: 
            {
                if (index) { destroyIndex(index, count); }
                index = buildIndex(w, count);
                printf("OK INDEX_BUILT size=%d\n", count);
                break;
            }
            case 7: 
            {
                printIndex(index, count);
                break;
            }
            case 8: 
            {
                sortIndexByPrice(index, count);
                printf("OK INDEX_SORTED\n");
                break;
            }
            case 9: 
            {
                destroyIndex(index, count);
                printf("OK INDEX_DROPPED\n");
                break;
            }
            case 10: 
            {
                int id;
                std::cin >> id;
                int outDay, outSlot;
                Appointment* found = findAppointment(w, id, outDay, outSlot);
                if (!found) 
                {
                    printf("ERR NOT_FOUND\n");
                } 
                else 
                {
                    printf("OK FOUND %04d day=%d slot=%d %-8s PKR %8.2f\n",
                           id, outDay, outSlot, found->service, found->price);
                }
                break;
            }
            case 11: 
            {
                if (index) { destroyIndex(index, count); }
                loadSeedWeek(w);
                printf("OK SEED_LOADED\n");
                break;
            }
            case 12: 
            {
                int day, id;
                char name[64], service[64];
                float price;
                std::cin >> day >> id;
                std::cin.width(sizeof(name));
                std::cin >> name;
                std::cin.width(sizeof(service));
                std::cin >> service;
                std::cin >> price;
                if (day < 0 || day >= DAYS_IN_WEEK) 
                {
                    printf("ERR BAD_DAY\n");
                } 
                else if (!w.days) 
                {
                    printf("ERR WEEK_DESTROYED\n");
                } 
                else 
                {
                    DaySchedule dayCopy = w.days[day];
                    printf("BYVAL pre count=%d capacity=%d\n", dayCopy.count, dayCopy.capacity);
                    bool result = bookByValue(dayCopy, id, name, service, price);
                    printf("BYVAL returned=%d\n", result ? 1 : 0);
                    printf("BYVAL post count=%d capacity=%d\n", w.days[day].count, w.days[day].capacity);
                }
                break;
            }
            case 13: 
            {
                if (index) { destroyIndex(index, count); }
                destroyWeek(w);
                printf("OK WEEK_DESTROYED\n");
                break;
            }
            case 14: 
            {
                reportSizes();
                float list[10];
                fillPrices(list, list + 10, 10);
                printf("A2 LIST:");
                for (int i = 0; i < 10; i++) { printf(" %.1f", list[i]); }
                printf("\n");
                printf("A2 SUM=%.2f\n", sumRange(list, list + 10));
                float* maxPtr = maxElementPtr(list, list + 10);
                printf("A2 MAX=%.1f OFF=%d\n", *maxPtr, (int)(maxPtr - list));
                printf("A2 ABOVE=%d\n", countAbove(list, list + 10, 1000.0f));
                reverseInPlace(list, list + 10);
                printf("A2 REV:");
                for (int i = 0; i < 10; i++) { printf(" %.1f", list[i]); }
                printf("\n");
                break;
            }
            default: 
            {
                printf("ERR BAD_choice\n");
                break;
            }
        }
    }
    if (index) { destroyIndex(index, count); }
    destroyWeek(w);
    printf("BYE\n");
    return 0;
}