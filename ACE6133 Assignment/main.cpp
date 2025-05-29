#include <iostream>
#include <fstream>
#include <stdlib.h>   // For system("CLS")
#include <string>
#include <vector>

using namespace std;

const int MAX_ANSWER_LENGTH   = 200;
const int MAX_QUESTION_LENGTH = 200;
const int MAX_FLASHCARDS      = 20;

class Flashcard {
public:
    string question;
    string answer;
    int mark;

    Flashcard() : question(""), answer(""), mark(0) {}
    Flashcard(string q, string a)
        : question(q), answer(a), mark(0) {}
};

class FlashcardsManager {
public:
    Flashcard flash_card[MAX_FLASHCARDS];
    int count;

    FlashcardsManager() : count(0) {}

    void addFlashcard(const string &question, const string &answer) {
        if (count < MAX_FLASHCARDS) {
            flash_card[count] = Flashcard(question, answer);
            count++;
            cout << "Flashcard added successfully!" << endl;
        } else {
            cout << "Cannot add more flashcards. Maximum capacity reached." << endl;
        }
    }

    void displayFlashcards() {
        if (count == 0) {
            cout << "No flashcards to display." << endl;
            return;
        }
        cout << "\nTotal flashcards = " << count << endl;
        for (int i = 0; i < count; i++) {
            cout << "Flashcard " << (i + 1) << ":" << endl;
            cout << "  Question: " << flash_card[i].question << endl;
            cout << "  Answer:   " << flash_card[i].answer << endl;
            cout << "  Mark:     " << flash_card[i].mark << endl;
            cout << endl;
        }
    }

    void editFlashcard(int index, const string &newQuestion, const string &newAnswer, const int &newMark) {
        if (index >= 0 && index < count) {
            flash_card[index].question = newQuestion;
            flash_card[index].answer   = newAnswer;
            flash_card[index].mark     = newMark;
            cout << "Flashcard updated successfully." << endl;
        } else {
            cout << "Invalid flashcard index!" << endl;
        }
    }

    void deleteFlashcard(int index) {
        if (index >= 0 && index < count) {
            for (int i = index; i < count - 1; i++) {
                flash_card[i] = flash_card[i + 1];
            }
            count--;
            cout << "Flashcard deleted successfully." << endl;
        } else {
            cout << "Invalid flashcard index!" << endl;
        }
    }
};

class ReviewSession {
public:
    FlashcardsManager &fm;

    ReviewSession(FlashcardsManager &flash) : fm(flash) {}

    void run() {
        vector<int> toReview;
        for (int i = 0; i < fm.count; i++) {
            if (fm.flash_card[i].mark < 3) {
                toReview.push_back(i);
            }
        }

        if (toReview.empty()) {
            cout << "All flashcards have mark >= 3. No cards to review right now." << endl;
            return;
        }

        for (int idx : toReview) {
            cout << "Question: " << fm.flash_card[idx].question << endl;
            cout << "Press Enter to see the answer...";
            cin.get();

            cout << "Answer: " << fm.flash_card[idx].answer << endl;

            cout << "Did you remember the answer? (y/n): ";
            char c;
            cin >> c;
            c = tolower(c);

            if (c == 'y') {
                fm.flash_card[idx].mark += 1;
                cout << "Marked as remembered. New mark = " << fm.flash_card[idx].mark << endl;
            } else if (c == 'n') {
                fm.flash_card[idx].mark -= 1;
                cout << "Marked as forgotten. New mark = " << fm.flash_card[idx].mark << endl;
            } else {
                cout << "Invalid input. No change to mark." << endl;
            }


            cin.ignore(10000, '\n');

            cout << endl;
        }
    }
};

class FileHandler {
public:
    FileHandler(const string &file, FlashcardsManager &manager)
        : filename(file), fm(manager) {}

    void saveData() {
        ofstream wf(filename, ios::out | ios::binary);
        if (!wf) {
            cout << "Cannot open file for writing!" << endl;
            return;
        }
        wf.write((char *)&fm, sizeof(FlashcardsManager));
        wf.close();
        cout << "\nSaving all flashcards data into file done." << endl;
    }

    void loadData() {
        ifstream rf(filename, ios::in | ios::binary);
        if (!rf) {
            cout << "Cannot open file for reading!" << endl;
            return;
        }
        rf.read((char *)&fm, sizeof(FlashcardsManager));
        rf.close();
        cout << "\nLoading all flashcards data from file done." << endl;
    }

private:
    string filename;
    FlashcardsManager &fm;
};

class App {
    FlashcardsManager fm;
    FileHandler fileHandler;
    string filename;

public:
    App(const string &file = "flashcard_data.dat")
        : filename(file), fileHandler(file, fm) {}

    void addNewFlashcard() {
        string question, answer;
        cout << "Enter question: ";
        cin.ignore(10000, '\n');
        getline(cin, question);
        cout << "Enter answer: ";
        getline(cin, answer);
        fm.addFlashcard(question, answer);
    }

    void editFlashcard() {
        if (fm.count == 0) {
            cout << "No flashcards to edit." << endl;
            return;
        }
        fm.displayFlashcards();
        cout << "Enter flashcard number to edit (1 to " << fm.count << "): ";
        int index;
        cin >> index;
        cin.ignore(10000, '\n');

        if (index < 1 || index > fm.count) {
            cout << "Invalid index." << endl;
            return;
        }

        char edit_mark;
        int newMark = fm.flash_card[index - 1].mark;
        string newQuestion, newAnswer;
        cout << "Enter new question: ";
        getline(cin, newQuestion);
        cout << "Enter new answer: ";
        getline(cin, newAnswer);
        cout << "Do you want to edit mark? (y/n): ";
        cin >> edit_mark;
        edit_mark = tolower(edit_mark);
        if (edit_mark == 'y') {
            cout << "Enter new mark: ";
            cin >> newMark;
        } else {
            cout << "Mark kept as before." << endl;
        }

        fm.editFlashcard(index - 1, newQuestion, newAnswer, newMark);
    }

    void deleteFlashcard() {
        if (fm.count == 0) {
            cout << "No flashcards to delete." << endl;
            return;
        }
        fm.displayFlashcards();
        cout << "Enter flashcard number to delete (1 to " << fm.count << "): ";
        int index;
        cin >> index;
        cin.ignore(10000, '\n');

        if (index < 1 || index > fm.count) {
            cout << "Invalid index." << endl;
            return;
        }
        fm.deleteFlashcard(index - 1);
    }

    void reviewFlashcards() {
        char again;


        cin.ignore(10000, '\n');

        do {


            system("CLS");

            ReviewSession session(fm);
            session.run();
            cout << "Review Session Ended." << endl;


            cout << "Do you want to review again? (y/n): ";
            cin >> again;
            cin.ignore(10000, '\n');
            cout << endl;
            if(again=='y'){
                cout<< "Press enter to continue..."<<endl;
            }
        } while (tolower(again) == 'y');
    }

    void displayData() {
        cout << "\nDisplaying Flashcard data..." << endl;
        fm.displayFlashcards();
    }

    void showMenu() {
        int choice = 0;
        while (choice != 8) {
            cout << "\n===== Flashcard Management System =====" << endl;
            cout << "1. Display all flashcards" << endl;
            cout << "2. Add new flashcard" << endl;
            cout << "3. Review flashcards" << endl;
            cout << "4. Save flashcards to file" << endl;
            cout << "5. Load flashcards from file" << endl;
            cout << "6. Edit flashcard" << endl;
            cout << "7. Delete flashcard" << endl;
            cout << "8. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            system("CLS");

            switch (choice) {
                case 1:
                    displayData();
                    break;
                case 2:
                    addNewFlashcard();
                    break;
                case 3:
                    reviewFlashcards();
                    break;
                case 4:
                    fileHandler.saveData();
                    break;
                case 5:
                    fileHandler.loadData();
                    break;
                case 6:
                    editFlashcard();
                    break;
                case 7:
                    deleteFlashcard();
                    break;
                case 8:
                    cout << "Exiting program." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
};

int main() {
    App app;
    app.showMenu();
    return 0;
}
