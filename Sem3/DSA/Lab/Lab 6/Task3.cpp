struct NetworkNode{
	int nodeID;
	NetworkNode* next;
	NetworkNode(int nodeID=0) : nodeID(nodeID), next(nullptr){}
};

int findSurvivor(NetworkNode*& tail, int k) {
    if (!tail || k <= 0) return -1;
    NetworkNode* current = tail->next;
    NetworkNode* prev = tail;
    while (tail->next != tail) 
    {
        for (int i = 1; i < k; i++) 
        {
            prev = current;
            current = current->next;
        }
        NetworkNode* target = current;
        prev->next = current->next;
        if (current == tail) { tail = prev; }
        current = current->next;
        delete target;
    }
    return tail->nodeID;
}