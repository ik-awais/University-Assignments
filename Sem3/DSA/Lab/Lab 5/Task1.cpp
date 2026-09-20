#include<iostream>
using namespace std;
class Node{
public:
	char val;
	Node *next;
	Node(char val){
		this->val=val;
		next=NULL;
	}
};
class Stack {
	Node *top;
	int limit;
	int length;
public:
	Stack(int limit){
		this->limit=limit;
		top=NULL;
		length=0;
	}
	~Stack() {
		while (!isEmpty()) {pop();}
	}
	bool isEmpty(){
		if(top==NULL)
		{
			return true;
			cout<<"Stack is empty"<<endl;
		}
		else{return false;}
	}
	bool isFull(){
		if(length==limit)
		{
			cout<<"Stack is full"<<endl;
			return true;
		}
		else{return false;}
	}
	void push(char val){
		if(!isFull())
		{
			Node *temp=new Node(val);
			temp->next=top;
			top=temp;
			length++;
//			cout<<"Element: "<<val<<" is pushed"<<endl;
		}
	}
	void pop(){
		Node *temp=top;
		char val=top->val;
		top=top->next;
		delete temp;
		temp=NULL;
		length--;
//		cout<<"Element: "<<val<<" is popped"<<endl;
	}
	void peek(){if(!isEmpty()){cout<<"Top of the stack: "<<top->val<<endl;}}
	char getTop(){return top->val;}
	bool isMatch(char open, char close){
		if(open=='(' && close==')') return true;
		if(open=='[' && close==']') return true;
		if(open=='{' && close=='}') return true;
		return false;
	}
	void display(){
		if(!isEmpty())
		{
			cout<<"Stack Elements: "<<endl;
			Node *temp=top;
			while(temp!=NULL)
			{
				cout<<temp->val;
				temp=temp->next;
			}
			cout<<endl;
		}
	}
};
int main(){
	string expr;
	cout << "Enter expression: ";
	getline(cin, expr);
	Stack s(expr.length());
	bool valid = true;
	for(int i = 0; i < expr.length(); i++)
	{
		char val = expr[i];
		if(val=='('||val=='['||val=='{')
		{
			s.push(val);
		}
		else if(val==')'||val==']'||val=='}')
		{
			if(s.isEmpty()){valid=false;continue;}
			if(!s.isMatch(s.getTop(),val)){valid=false;continue;}
			s.pop();
		}
		else continue;
	}
	if(valid && s.isEmpty())cout<<"Balanced"<<endl;
	else cout<<"Not Balanced"<<endl;
return 0;
}

//({[]})
//({[}])
