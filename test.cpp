#include <iostream>
#include <fstream>
#include <stdlib.h> // For system("cls")
#include <string>

using namespace std;

// Maximum limits
const int MAX_NAME_LENGTH = 50;
const int MAX_STUDENTS = 10;

class Student
{
public:
    int roll_no;
    string name;

    Student() : roll_no(0)
    {
        name = " ";
    }

    Student(int id, string a_name) : roll_no(id)
    {
        name = a_name;
    }
};

class StudentsManager
{
public:
    Student students[MAX_STUDENTS]; // Fixed array instead of vector
    int count;                      // Number of students currently stored

    StudentsManager() : count(0) {}

    void addStudent(int roll_no, string name)
    {
        if (count < MAX_STUDENTS)
        {
            students[count] = Student(roll_no, name);
            count++;
            cout << "Student added successfully!" << endl;
        }
        else
        {
            cout << "Cannot add more students. Maximum capacity reached." << endl;
        }
    }

    void displayStudents()
    {
        if (count == 0)
        {
            cout << "No students to display." << endl;
            return;
        }

        else
        {
            cout << endl
                 << "Total students = " << count << endl;
            for (int i = 0; i < count; i++)
            {
                cout << "Roll No: " << students[i].roll_no << endl;
                cout << "Name: " << students[i].name << endl;
                cout << endl;
            }
        }
    }
};

class App
{
    StudentsManager sm;
    string filename;

public:
    App(const string &file = "students_data.dat") : filename(file) {}

    void addNewStudent()
    {
        int roll;
        string name;

        cout << "Enter Roll No: ";
        cin >> roll;
        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        sm.addStudent(roll, name);
    }

    void saveData()
    {
        ofstream wf(filename, ios::out | ios::binary);
        if (!wf)
        {
            cout << "Cannot open file for writing!" << endl;
            return;
        }

        wf.write((char *)&sm, sizeof(StudentsManager));
        wf.close();
        cout << "\n Saving all students data into file done " << endl;
    }

    void loadData()
    {

        ifstream rf(filename, ios::in | ios::binary);
        if (!rf)
        {
            cout << "Cannot open file for reading!" << endl;
            return;
        }

        rf.read((char *)&sm, sizeof(StudentsManager));
        rf.close();
        cout << "\n Loading all students data from file done" << endl;
    }

    void displayData()
    {
        cout << "\nDisplaying student data..." << endl;
        sm.displayStudents();
    }

    void showMenu()
    {
        int choice = 0;

        while (choice != 5) // Changed from 6 to 5
        {
            cout << "\n===== Student Management System =====" << endl;
            cout << "1. Display all students" << endl;
            cout << "2. Add new student" << endl;
            cout << "3. Save StudentsManager to file" << endl;
            cout << "4. Load StudentsManager from file" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            system("CLS");

            switch (choice)
            {
            case 1:
                displayData();
                break;
            case 2:
                addNewStudent();
                break;
            case 3:
                saveData();
                break;
            case 4:
                loadData();
                break;
            case 5:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
};

int main()
{
    App app;
    app.showMenu();
    return 0;
}