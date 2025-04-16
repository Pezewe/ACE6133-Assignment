/* This code demonstrate a simple student management system
  User can add new students in the list by entering their roll number or ID
  and the list of students can be saved in a binary file
There are 3 classes in this program
Student class - storing information about one student
StudentManager class - store information about many students and managing these students objects
App class - the class that has manage the running of the whole program and interact with user
All data stored in students_data.dat

*/

#include <iostream>
#include <fstream>
#include <stdlib.h> // For system("cls")
#include <string>
#include <limits> // For numeric_limits

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
        while (!(cin >> roll)) // Validate integer input
        {
            cout << "Invalid input. Please enter a valid roll number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

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

        wf.write(reinterpret_cast<char *>(&sm.count), sizeof(sm.count)); // Save count first
        wf.write(reinterpret_cast<char *>(sm.students), sizeof(Student) * sm.count); // Save only valid students
        wf.close();
        cout << "\nSaving all students data into file done." << endl;
    }

    void loadData()
    {
        ifstream rf(filename, ios::in | ios::binary);
        if (!rf)
        {
            cout << "Cannot open file for reading!" << endl;
            return;
        }

        rf.read(reinterpret_cast<char *>(&sm.count), sizeof(sm.count)); // Load count first
        if (sm.count > MAX_STUDENTS) // Validate count
        {
            cout << "Data corrupted or exceeds maximum capacity!" << endl;
            sm.count = 0;
            rf.close();
            return;
        }

        rf.read(reinterpret_cast<char *>(sm.students), sizeof(Student) * sm.count); // Load only valid students
        rf.close();
        cout << "\nLoading all students data from file done." << endl;
    }

    void displayData()
    {
        cout << "\nDisplaying student data..." << endl;
        sm.displayStudents();
    }

    void showMenu()
    {
        int choice = 0;

        while (choice != 5)
        {
            cout << "\n===== Student Management System =====" << endl;
            cout << "1. Display all students" << endl;
            cout << "2. Add new student" << endl;
            cout << "3. Save StudentsManager to file" << endl;
            cout << "4. Load StudentsManager from file" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";
            while (!(cin >> choice)) // Validate integer input
            {
                cout << "Invalid input. Please enter a valid choice: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            // system("CLS") replaced with cross-platform alternative
            cout << string(50, '\n'); // Clear screen by printing newlines

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