struct PacketNode {
    int data;
    PacketNode* next;
    PacketNode(int data = 0) : data(data), next(nullptr) {}
};

PacketNode* reverseKGroup(PacketNode* head, int k) {
    if (!head || k <= 1) return head;
    PacketNode* current = head;
    PacketNode* prevGroup = nullptr;
    PacketNode* newHead = nullptr;
    while (current) 
    {
        PacketNode* temp = current;
        int count = 0;
        while (temp && count < k) { temp = temp->next; count++; }
        if (count < k) break;
        PacketNode* prev = temp;
        PacketNode* groupHead = current;
        for (int i = 0; i < k; i++) 
        {
            PacketNode* next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        if (!newHead) { newHead = prev; }
        if (prevGroup) { prevGroup->next = prev; }
        prevGroup = groupHead;
    }
    if (!newHead) return head;
    return newHead;
}