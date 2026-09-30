struct ProcessNode {
    int processID;
    int remainingTime;
    ProcessNode* next;
    ProcessNode(int processID = 0, int remainingTime = 0)
        : processID(processID), remainingTime(remainingTime), next(nullptr) {}
};

void executeCycle(ProcessNode*& tail, int timeQuantum) {
    if (!tail || timeQuantum <= 0) return;
    ProcessNode* current = tail->next;
    ProcessNode* previous = tail;
    int count = 0;
    ProcessNode* temp = tail->next;
    do 
    {
        count++;
        temp = temp->next;
    } while (temp != tail->next);
    for (int i = 0; i < count && tail; i++) 
    {
        current->remainingTime -= timeQuantum;
        if (current->remainingTime <= 0) 
        {
            ProcessNode* target = current;
            if (current == tail && current->next == current) 
            {
                delete target;
                tail = nullptr;
                break;
            }
            previous->next = current->next;
            if (current == tail) {tail = previous; }
            current = current->next;
            delete target;
        }
        else { previous = current; current = current->next; }
    }
}