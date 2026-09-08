#include <iostream>
#include <string>
using namespace std;

struct Course
{
	string Code;
	string Name;
	int CHs;
	bool status;
	Course* next;
};

bool isEmpty(Course* path)
{
	if (!path)
	{
		cout << "No Academic Path specified!\n";
		return true;
	}
	return false;
}

void createAcademicPath(Course*& path, int& total)
{
	cout << "How many courses do you have: ";
	cin >> total;
	Course* tail = nullptr;
	for (int i = 0; i < total; i++)
	{
		Course* temp = new (nothrow) Course;
		if (!temp)
		{
			cout << "Memory allocation failed!\n";
			return;
		}
		cout << "Enter code of course " << i + 1 << ": "; cin >> temp->Code;
		cout << "Enter name of course " << i + 1 << ": "; cin >> temp->Name;
		cout << "Enter credit hours of course " << i + 1 << ": "; cin >> temp->CHs;
		temp->status = false;
		temp->next = nullptr;
		if (!path)
		{
			path = temp;
			tail = temp;
		}
		else
		{
			tail->next = temp;
			tail = temp;
		}
	}
}

void markCourseStatus(Course* path)
{
	if (isEmpty(path)) return;
	string code;
	cout << "Enter Course Code for editing course status: ";
	cin >> code;
	Course* temp = path;
	while (temp && temp->Code != code) temp = temp->next;
	if (!temp)
	{
		cout << "Course not found!\n";
		return;
	}
	if (temp->status)
	{
		cout << "Course already marked as completed!\n";
		return;
	}
	temp->status = true;
	cout << "Course is marked completed!\n";
}

void findEligibleCourse(Course* path)
{
	if (isEmpty(path)) return;
	Course* temp = path;
	while (temp && temp->status) temp = temp->next;
	if (!temp)
	{
		cout << "All courses completed, no course pending!\n";
		return;
	}
	cout << "You are currently eligible for:\n";
	cout << temp->Code << " - " << temp->Name << "\n";
}

void checkProgress(Course* path)
{
	if (isEmpty(path)) return;
	int completed = 0, total = 0;
	Course* temp = path;
	while (temp)
	{
		if (temp->status) completed++;
		total++;
		temp = temp->next;
	}
	int remaining = total - completed;
	double progress = (double)completed / total * 100;
	cout << "Courses Completed: " << completed << "\n";
	cout << "Courses Remaining: " << remaining << "\n";
	printf("Progress: %.0f%%\n", progress);
}

void displayPath(Course* path)
{
	if (isEmpty(path)) return;
	Course* temp = path;
	while (temp)
	{
		cout << temp->Code << " -> " << temp->Name << " -> " << (temp->status ? "Completed" : "Not Completed") << "\n";
		temp = temp->next;
	}
}

void checkGraduation(Course* path)
{
	if (isEmpty(path)) return;
	int remaining = 0;
	Course* temp = path;
	while (temp)
	{
		if (!temp->status) remaining++;
		temp = temp->next;
	}
	if (!remaining)
	{
		cout << "All courses have been completed.\n";
		cout << "Student has completed the academic path.\n";
	}
	else
	{
		cout << "Student cannot complete the academic path yet.\n";
		cout << "Courses remaining: " << remaining << "\n";
	}
}

int main()
{
	Course* path = nullptr;
	int total = 0;
	bool running = true;
	while (running)
	{
		cout << "\n===COURSE PATH ANALYZER===\n";
		cout << "1. Create Academic Path\n2. Mark Course as Completed\n";
		cout << "3. Find Current Eligible Course\n4. Show Academic Progress\n";
		cout << "5. Display Complete Course Path\n6. Check Graduation Status\n";
		cout << "7. Exit\nEnter your choice: ";
		int choice;
		cin >> choice;
		switch (choice)
		{
			case 1:
				createAcademicPath(path, total);
				break;
			case 2:
				markCourseStatus(path);
				break;
			case 3:
				findEligibleCourse(path);
				break;
			case 4:
				checkProgress(path);
				break;
			case 5:
				displayPath(path);
				break;
			case 6:
				checkGraduation(path);
				break;
			case 7:
				running = false;
				break;
			default:
				cout << "Enter a valid choice!\n";
		}
	}
	return 0;
}