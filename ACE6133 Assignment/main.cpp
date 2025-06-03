#include <iostream>
#include <fstream>
#include <stdlib.h>   // For system("CLS")
#include <string>
#include <vector>

using namespace std;

// Constants to define maximum limits
const int MAX_ANSWER_LENGTH   = 2000;
const int MAX_QUESTION_LENGTH = 2000;
const int MAX_FLASHCARDS      = 20;

// Flashcard class to hold question, answer, and review mark
class Flashcard {
public:
    string question;
    string answer;
    int mark;

    Flashcard() : question(""), answer(""), mark(0) {}
    Flashcard(string q, string a)
        : question(q), answer(a), mark(0) {}
};

// Manages a collection of flashcards
class FlashcardsManager {
public:
    Flashcard flash_card[MAX_FLASHCARDS];
    int count;

    FlashcardsManager() : count(0) {}

    // Adds a new flashcard if there's room
    void addFlashcard(const string &question, const string &answer) {
        if (count < MAX_FLASHCARDS) {
            flash_card[count] = Flashcard(question, answer);
            count++;
            cout << "Flashcard added successfully!" << endl;
        } else {
            cout << "Cannot add more flashcards. Maximum capacity reached." << endl;
        }
    }

    // Displays all flashcards
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

    // Edits an existing flashcard
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

    // Deletes a flashcard by shifting the array elements
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

// Handles the flashcard review session
class ReviewSession {
public:
    FlashcardsManager &fm;

    ReviewSession(FlashcardsManager &flash) : fm(flash) {}

    // Runs a review session for flashcards with mark < 3
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

            // Adjust mark based on user response
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

// Handles reading from and writing to a file
class FileHandler {
public:
    FileHandler(const string &file, FlashcardsManager &manager)
        : filename(file), fm(manager) {}

    // Save all flashcard data to file
    void saveData() {
        ofstream wf(filename);
        if (!wf) {
            cout << "Cannot open file for writing!" << endl;
            return;
        }

        for (int i = 0; i < fm.count; ++i) {
            wf << fm.flash_card[i].question << endl;
            wf << fm.flash_card[i].answer << endl;
            wf << fm.flash_card[i].mark << endl;
        }

        wf.close();
        cout << "\nSaving all flashcards data into file done." << endl;
    }

    // Load flashcard data from file
    void loadData() {
        ifstream rf(filename);
        if (!rf) {
            cout << "Cannot open file for reading!" << endl;
            return;
        }

        fm.count = 0;
        string question, answer, markStr;

        while (getline(rf, question) && getline(rf, answer) && getline(rf, markStr)) {
            if (fm.count < MAX_FLASHCARDS) {
                int mark = stoi(markStr);  // Convert string to integer
                fm.flash_card[fm.count] = Flashcard(question, answer);
                fm.flash_card[fm.count].mark = mark;
                fm.count++;
            }
        }

        rf.close();
        cout << "\nLoading all flashcards data from file done." << endl;
    }

private:
    string filename;
    FlashcardsManager &fm;
};

// Main application controller
class App {
    FlashcardsManager fm;
    FileHandler fileHandler;
    string filename;

public:
    App(const string &file = "flashcard_data.dat")
        : filename(file), fileHandler(file, fm) {}

    // Adds a new flashcard
    void addNewFlashcard() {
        string question, answer;
        cout << "Enter question: ";
        cin.ignore(10000, '\n');
        getline(cin, question);
        cout << "Enter answer: ";
        getline(cin, answer);
        fm.addFlashcard(question, answer);
    }

    // Edits an existing flashcard
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

    // Deletes a flashcard
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

    // Starts a flashcard review session
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
            if (again == 'y') {
                cout << "Press enter to continue..." << endl;
            }
        } while (tolower(again) == 'y');
    }

    // Displays all flashcards
    void displayData() {
        cout << "\nDisplaying Flashcard data..." << endl;
        fm.displayFlashcards();
    }

    // Displays the main menu and handles user input
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

// Entry point of the program
int main() {
    App app;
    app.showMenu();
    return 0;
}
