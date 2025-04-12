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

using namespace std;

// Maximum limits
const int MAX_NAME_LENGTH = 50;
const int MAX_STUDENTS = 10;

class Flashcard
{
public:
    string question;
    string answer;


    Flashcard() : question("")
    {
        answer = " ";
    }

    Flashcard(string question_id, string a_answer) : question(question_id)
    {
        answer = a_answer;
    }
};

class StudentsManager
{
public:
    Flashcard students[MAX_STUDENTS]; // Fixed array instead of vector
    int count;                      // Number of students currently stored

    StudentsManager() : count(0) {}

    void addStudent(string question, string answer)
    {
        if (count < MAX_STUDENTS)
        {
            students[count] = Flashcard(question, answer);
            count++;
            cout << "Flashcard added successfully!" << endl;
        }
        else
        {
            cout << "Cannot add more flashcard. Maximum capacity reached." << endl;
        }
    }

    void displayStudents()
    {
        if (count == 0)
        {
            cout << "No flashcard to display." << endl;
            return;
        }

        else
        {
            cout << endl
                 << "Total flashcards = " << count << endl;
            for (int i = 0; i < count; i++)
            {
                cout << "Question: " << students[i].question << endl;
                cout << "Answer: " << students[i].answer << endl;
                cout << "Difficulty: "<<endl;
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
    App(const string &file = "flashcard_data.dat") : filename(file) {}

    void addNewStudent()
    {
        string question_id;
        string answer;

        cout << "Enter question: ";
        cin.ignore();
        getline(cin, question_id);


        cout << "Enter answer: ";
        getline(cin, answer);

        sm.addStudent(question_id, answer);
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
        cout << "\nDisplaying Flashcard data..." << endl;
        sm.displayStudents();
    }

    void showMenu()
    {
        int choice = 0;

        while (choice != 5) // Changed from 6 to 5
        {
            cout << "\n===== Flashcard Management System =====" << endl;
            cout << "1. Display all students" << endl;
            cout << "2. Add new Flashcard" << endl;
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
