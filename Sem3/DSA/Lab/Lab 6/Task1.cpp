#include <iostream>
using namespace std;

struct NetworkNode{
	int nodeID;
	NetworkNode* next;
	NetworkNode(int nodeID=0) : nodeID(nodeID), next(nullptr){}
};

void addNodeToRing(NetworkNode*& tail, int id) {
    NetworkNode* temp = new NetworkNode(id);
    if (!tail)
	{
		tail = temp;
        temp->next = temp;
        return;
    }
    temp->next = tail->next;
    tail->next = temp;
    tail = temp;
}

void destroyRing(NetworkNode*& tail){
	if(!tail) return;
	NetworkNode* head = tail->next;
	tail->next = nullptr;
	while(head)
	{
		NetworkNode* target = head;
		head = head->next;
		delete target;
	}
	tail = nullptr;
}