#include <iostream>
using namespace std;

struct node{
	int data;
	node* next;
};

int count = 0;
int limit = 10;

bool isEmpty(node* head){
	if (!head)
	{
		cout << "List is empty!\n";
		return true;
	}
	return false;
}

bool isFull(){
	if (count >= limit)
	{
		cout << "List is full!\n";
		return true;
	}
	return false;
}

void insertFront(node*& head, int data){
	if (isFull()) return;
	node* newNode = new (nothrow) node;
	if (!newNode)
	{
		cout << "Memory allocation failed!\n";
		return;
	}
	newNode->data = data;
	newNode->next = head;
	head = newNode;
	count++;
}

void insertEnd(node*& head, int data){
	if (isFull()) return;
	node* newNode = new (nothrow) node;
	if (!newNode)
	{
		cout << "Memory allocation failed!\n";
		return;
	}
	newNode->data = data;
	newNode->next = nullptr;
	if (!head)
	{
		head = newNode;
		count++;
		return;
	}
	node* temp = head;
	while (temp->next) temp = temp->next;
	temp->next = newNode;
	count++;
}

void insertByNumber(node*& head, int afterValue, int data){
	if (isEmpty(head)) return;
	if (isFull()) return;
	node* temp = head;
	while (temp && temp->data != afterValue) temp = temp->next;
	if (!temp)
	{
		cout << "Value not found in list!\n";
		return;
	}
	node* newNode = new (nothrow) node;
	if (!newNode)
	{
		cout << "Memory allocation failed!\n";
		return;
	}
	newNode->data = data;
	newNode->next = temp->next;
	temp->next = newNode;
	count++;
}

void display(node* head){
	if (isEmpty(head)) return;
	node* temp = head;
	while (temp)
	{
		cout << temp->data << "->";
		temp = temp->next;
	}
	cout << "NULL\n";
}

void sort(node* head){
	if (isEmpty(head)) return;
	node* outer = head;
	while (outer)
	{
		node* inner = outer->next;
		while(inner)
		{
			if (inner->data < outer->data)
			{
				int temp = outer->data;
				outer->data = inner->data;
				inner->data = temp;
			}
			inner = inner->next;
		}
		outer = outer->next;
	}
}

void deleteNode(node*& head, int val){
	if (isEmpty(head)) return;
	if (head->data == val)
	{
		node* target = head;
		head = head->next;
		delete target;
		count--;
		return;
	}
	node* temp = head;
	while (temp->next && temp->next->data != val) temp = temp->next;
	if (!temp->next)
	{
		cout << "Value not found in list!\n";
		return;
	}
	node* target = temp->next;
	temp->next = target->next;
	delete target;
	count--;
}

void changeLimit(int newLimit){
	limit = newLimit;
	cout << "Limit updated to " << limit << ".\n";
}

int main(){
	node* head = nullptr;
	bool running = true;
	while (running)
	{
		cout << "\nEnter choice:\n1. Insert Node at Front"
			<< "\n2. Insert Node at End\n3. Insert Node after a Number\n"
			<< "4. Check if List is Full\n5. Check if List is Empty\n"
			<< "6. Display List\n7. Sort the List\n8. Put new limit\n"
			<< "9. Delete a node by value\n0. Exit\n";
		int choice;
		cin >> choice;
		switch (choice)
		{
			case 0:
				running = false;
				break;
			case 1:
			{
				int num;
				cout << "Enter value: "; cin >> num;
				insertFront(head, num);
				break;
			}
			case 2:
			{
				int num;
				cout << "Enter value: "; cin >> num;
				insertEnd(head, num);
				break;
			}
			case 3:
			{
				int afterValue, num;
				cout << "Enter value to insert after: "; cin >> afterValue;
				cout << "Enter value to insert: "; cin >> num;
				insertByNumber(head, afterValue, num);
				break;
			}
			case 4:
				isFull();
				break;
			case 5:
				isEmpty(head);
				break;
			case 6:
				display(head);
				break;
			case 7:
				sort(head);
				cout << "List sorted.\n";
				break;
			case 8:
			{
				int newLimit;
				cout << "Enter new limit: "; cin >> newLimit;
				changeLimit(newLimit);
				break;
			}
			case 9:
			{
				int val;
				cout << "Enter value to delete: "; cin >> val;
				deleteNode(head, val);
				break;
			}
			default:
				cout << "Enter a valid choice!\n";
		}
	}
	return 0;
}