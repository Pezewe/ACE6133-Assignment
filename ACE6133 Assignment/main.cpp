#include <iostream>
#include <fstream>
#include <stdlib.h> // For system("CLS")
#include <string>
#include <vector>

using namespace std;

// Maximum limits
const int MAX_ANSWER_LENGTH = 200;
const int MAX_QUESTION_LENGTH = 200;
const int MAX_FLASHCARDS = 20;


class Flashcard {
public:
    string question;
    string answer;
    int difficulty;

    Flashcard() : question(""), answer(""), difficulty(1) {}
    Flashcard(string question_id, string a_answer)
        : question(question_id), answer(a_answer), difficulty(1) {}
};


class FlashcardsManager {
public:
    Flashcard flash_card[MAX_FLASHCARDS];
    int count;

    FlashcardsManager() : count(0) {}

    void addFlashcard(string question, string answer) {
        if (count < MAX_FLASHCARDS) {
            flash_card[count] = Flashcard(question, answer);
            count++;
            cout << "Flashcard added successfully!" << endl;
        } else {
            cout << "Cannot add more flashcard. Maximum capacity reached." << endl;
        }
    }

    void displayStudents() {
        if (count == 0) {
            cout << "No flashcard to display." << endl;
            return;
        }
        cout << "\nTotal flashcards = " << count << endl;
        for (int i = 0; i < count; i++) {
            cout << "Flashcard " << i + 1 << endl;
            cout << "Question: " << flash_card[i].question << endl;
            cout << "Answer: " << flash_card[i].answer << endl;
            cout << "Difficulty: " << flash_card[i].difficulty << endl;
            cout << endl;
        }
    }

    void editFlashcard(int index, const string &newQuestion, const string &newAnswer) {
        if (index >= 0 && index < count) {
            flash_card[index].question = newQuestion;
            flash_card[index].answer = newAnswer;
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
        if (fm.count == 0) {
            cout << "No flashcards available for review. Please add some first." << endl;
            return;
        }

        vector<int> reviewIndices;
        for (size_t i = 0; i < fm.count; i++) {
            for (int j = 0; j < fm.flash_card[i].difficulty; j++) {
                reviewIndices.push_back(i);
            }
        }

        for (int idx : reviewIndices) {
            cout << "Question = " << fm.flash_card[idx].question << endl;
            cout << "Press Enter to see the answer...";
            cin.ignore();
            cin.get();
            cout << "Answer = " << fm.flash_card[idx].answer << endl;
            cout << "Please rate the difficulty of the question (1 to 3): ";
            cin >> fm.flash_card[idx].difficulty;
            cout << "Question difficulty marked as " << fm.flash_card[idx].difficulty << endl << endl;
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
        cout << "\nSaving all flashcards data into file done" << endl;
    }

    void loadData() {
        ifstream rf(filename, ios::in | ios::binary);
        if (!rf) {
            cout << "Cannot open file for reading!" << endl;
            return;
        }
        rf.read((char *)&fm, sizeof(FlashcardsManager));
        rf.close();
        cout << "\nLoading all flashcards data from file done" << endl;
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
        string question_id, answer;
        cout << "Enter question: ";
        cin.ignore();
        getline(cin, question_id);
        cout << "Enter answer: ";
        getline(cin, answer);
        fm.addFlashcard(question_id, answer);
    }

    void editFlashcard() {
        int index;
        string newQuestion, newAnswer;
        fm.displayStudents();
        cout << "Enter flashcard number to edit (1 to " << fm.count << "): ";
        cin >> index;
        cin.ignore();
        cout << "Enter new question: ";
        getline(cin, newQuestion);
        cout << "Enter new answer: ";
        getline(cin, newAnswer);
        fm.editFlashcard(index - 1, newQuestion, newAnswer);
    }

    void deleteFlashcard() {
        int index;
        fm.displayStudents();
        cout << "Enter flashcard number to delete (1 to " << fm.count << "): ";
        cin >> index;
        fm.deleteFlashcard(index - 1);
    }

    void reviewFlashcards() {
        ReviewSession session(fm);
        session.run();
    }

    void displayData() {
        cout << "\nDisplaying Flashcard data..." << endl;
        fm.displayStudents();
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
                case 1: displayData(); break;
                case 2: addNewFlashcard(); break;
                case 3: reviewFlashcards(); cout << "Review Session Ended" << endl; break;
                case 4: fileHandler.saveData(); break;
                case 5: fileHandler.loadData(); break;
                case 6: editFlashcard(); break;
                case 7: deleteFlashcard(); break;
                case 8: cout << "Exiting program." << endl; break;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
};

int main() {
    App app;
    app.showMenu();
    return 0;
}
