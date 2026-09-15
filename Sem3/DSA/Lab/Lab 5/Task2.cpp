#include<iostream>
using namespace std;
class Node{
public:
	int val;
	Node *next;
	Node(int val){
		this->val=val;
		next=NULL;
	}
};
class Stack{
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
		if(top==NULL){return true;}
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
	void push(int val){
		if(!isFull())
		{
			Node *temp=new Node(val);
			temp->next=top;
			top=temp;
			length++;
		}
	}
	int pop(){
		Node *temp=top;
		int val=top->val;
		top=top->next;
		delete temp;
		temp=NULL;
		length--;
		return val;
	}
	int getTop(){
		return top->val;
	}
};
bool isOperator(char val){
	if(val=='+'||val=='-'||val=='*'||val=='/'||val=='%'){return true;}
	return false;
}
int calculate(int oper1, int oper2, char op){
	if(op=='+')return oper1+oper2;
	if(op=='-')return oper1-oper2;
	if(op=='*')return oper1*oper2;
	if(op=='/')return oper1/oper2;
	return oper1%oper2;
}
int convertInt(string tok){
	int result=0;
	for(int i=0; i<tok.size(); i++)
	{
		result = result*10 + (tok[i]-'0');
	}
	return result;
}
int main(){
	Stack s(15);
	cout << "Enter postfix expression: ";
	string input;
	int oper2,oper1;
	while(cin >> input)
	{
		if(input.size()==1 && isOperator(input[0]))
		{
			oper2=s.pop();
			oper1=s.pop();
			s.push(calculate(oper1,oper2,input[0]));
		}
		else
		{
			s.push(convertInt(input));
		}
	}
	cout << "Result = " << s.getTop() << endl;
return 0;
}