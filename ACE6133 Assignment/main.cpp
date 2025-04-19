#include <iostream>
#include <fstream>
#include <stdlib.h> // For system("cls")
#include <string>

using namespace std;

// Maximum limits
const int MAX_ANSWER_LENGTH = 200;
const int MAX_QUESTION_LENGTH = 200;
const int MAX_FLASHCARDS = 20;

class Flashcard
{
public:
    string question;
    string answer;
    int difficulty;


    Flashcard():question(""), answer(""),difficulty(1){}


    Flashcard(string question_id, string a_answer) : question(question_id)
    {
        answer = a_answer;
        difficulty = 1;
    }
};

class FlashcardsManager
{
public:
    Flashcard flash_card[MAX_FLASHCARDS]; // Fixed array instead of vector
    int count;                      // Number of cards currently stored

    FlashcardsManager() : count(0) {}

    void addFlashcard(string question, string answer)
    {
        if (count < MAX_FLASHCARDS)
        {
            flash_card[count] = Flashcard(question, answer);
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
                cout << "Flashcard "<<i+1 << endl;
                cout << "Question: " << flash_card[i].question << endl;
                cout << "Answer: " << flash_card[i].answer << endl;
                cout << "Difficulty: "<< flash_card[i].difficulty << endl;
                cout << endl;
            }
        }
    }
};


class ReviewSession {
public:
    FlashcardsManager &fm;

    ReviewSession(FlashcardsManager &flash):fm(flash) {}

    void run(){
        if(fm.count==0){
            cout<<"No flashcards available for review. Please add some first."<<endl;
            return;
        }

        else{
            for(int q_Num=0;q_Num<fm.count;q_Num++){
                cout<<"Question "<<q_Num+1<<" = "<<fm.flash_card[q_Num].question<<endl;
                cout<<"Press Enter to see the answer...";
                cin.ignore();
                cin.get();
                cout<<"Answer ="<<fm.flash_card[q_Num].answer<<endl;
                cout<<"Please rate the difficulty of the question(1 to 3): ";
                cin>>fm.flash_card[q_Num].difficulty;
                cout<<"Question difficulty marked as "<<fm.flash_card[q_Num].difficulty;
                cout<<endl<<endl;

            }

            return;
        }
    }



};

class App
{
    FlashcardsManager fm;
    string filename;

public:
    App(const string &file = "flashcard_data.dat") : filename(file) {}

    void addNewFlashcard()
    {
        string question_id;
        string answer;

        cout << "Enter question: ";
        cin.ignore();
        getline(cin, question_id);


        cout << "Enter answer: ";
        getline(cin, answer);

        fm.addFlashcard(question_id, answer);
    }

    void saveData()
    {
        ofstream wf(filename, ios::out | ios::binary);
        if (!wf)
        {
            cout << "Cannot open file for writing!" << endl;
            return;
        }

        wf.write((char *)&fm, sizeof(FlashcardsManager));
        wf.close();
        cout << "\n Saving all flashcards data into file done " << endl;
    }

    void loadData()
    {

        ifstream rf(filename, ios::in | ios::binary);
        if (!rf)
        {
            cout << "Cannot open file for reading!" << endl;
            return;
        }

        rf.read((char *)&fm, sizeof(FlashcardsManager));
        rf.close();
        cout << "\n Loading all flashcards data from file done" << endl;
    }
    void reviewFlashcards() {
        ReviewSession session(fm);
        session.run();
    }

    void displayData()
    {
        cout << "\nDisplaying Flashcard data..." << endl;
        fm.displayStudents();
    }

    void showMenu()
    {
        int choice = 0;

        while (choice != 6)
        {
            cout << "\n===== Flashcard Management System =====" << endl;
            cout << "1. Display all flashcards" << endl;
            cout << "2. Add new flashcard" << endl;
            cout << "3. Review flashcards" << endl;
            cout << "4. Save flashcards to file" << endl;
            cout << "5. Load flashcards from file" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            system("CLS");

            switch (choice)
            {
            case 1:
                displayData();
                break;
            case 2:
                addNewFlashcard();
                break;
            case 3:
                reviewFlashcards();
                cout<<"Review Session Ended"<<endl;
                break;
            case 4:
                saveData();
                break;
            case 5:
                loadData();
                break;
            case 6:
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
