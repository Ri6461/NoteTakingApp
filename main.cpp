#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class NoteTakingApp {
public:
    void displayMenu();
    void createNote();
    void viewNotes();
    void deleteNote();
    void run();

private:
    vector<string> loadNotes();
    void saveNotes(const vector<string>& notes);
    string getNoteFileName();
    void printDivider();
    void clearScreen();
};

void NoteTakingApp::displayMenu() {
    clearScreen();  // Clears the console for a cleaner UI
    printDivider();
    cout << "  Note Taking App" << endl;
    printDivider();
    cout << "1. Create Note" << endl;
    cout << "2. View Notes" << endl;
    cout << "3. Delete Note" << endl;
    cout << "4. Exit" << endl;
    printDivider();
    cout << "Choose an option: ";
}

void NoteTakingApp::createNote() {
    string note;
    clearScreen();
    printDivider();
    cout << "  Create a New Note" << endl;
    printDivider();
    cout << "Enter your note: ";
    cin.ignore(); // To clear the newline left by previous input
    getline(cin, note);

    vector<string> notes = loadNotes();
    notes.push_back(note);
    saveNotes(notes);
    cout << "Note added successfully!" << endl;
    printDivider();
    cout << "Press Enter to return to the menu...";
    cin.get();
}

void NoteTakingApp::viewNotes() {
    clearScreen();
    vector<string> notes = loadNotes();
    printDivider();
    cout << "  Your Notes" << endl;
    printDivider();

    if (notes.empty()) {
        cout << "No notes available." << endl;
    } else {
        for (size_t i = 0; i < notes.size(); ++i) {
            cout << i + 1 << ". " << notes[i] << endl;
        }
    }
    printDivider();
    cout << "Press Enter to return to the menu...";
    cin.ignore();
    cin.get();
}

void NoteTakingApp::deleteNote() {
    clearScreen();
    vector<string> notes = loadNotes();
    if (notes.empty()) {
        cout << "No notes to delete." << endl;
        printDivider();
        cout << "Press Enter to return to the menu...";
        cin.ignore();
        cin.get();
        return;
    }

    viewNotes();  // Display the notes first
    size_t index;
    cout << "Enter the number of the note to delete: ";
    cin >> index;

    if (index < 1 || index > notes.size()) {
        cout << "Invalid index." << endl;
    } else {
        notes.erase(notes.begin() + index - 1);
        saveNotes(notes);
        cout << "Note deleted successfully!" << endl;
    }

    printDivider();
    cout << "Press Enter to return to the menu...";
    cin.ignore();
    cin.get();
}

vector<string> NoteTakingApp::loadNotes() {
    vector<string> notes;
    ifstream file(getNoteFileName());

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            notes.push_back(line);
        }
        file.close();
    }

    return notes;
}

void NoteTakingApp::saveNotes(const vector<string>& notes) {
    ofstream file(getNoteFileName());

    if (file.is_open()) {
        for (const auto& note : notes) {
            file << note << endl;
        }
        file.close();
    }
}

string NoteTakingApp::getNoteFileName() {
    return "notes.txt";
}

// Helper function to print a visual divider
void NoteTakingApp::printDivider() {
    cout << "=================================" << endl;
}

// Helper function to clear the console screen for better UI
void NoteTakingApp::clearScreen() {
#ifdef _WIN32
    system("cls");  // Windows specific command to clear the console
#else
    system("clear"); // Unix-based systems
#endif
}

void NoteTakingApp::run() {
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: createNote(); break;
            case 2: viewNotes(); break;
            case 3: deleteNote(); break;
            case 4: 
                clearScreen();
                cout << "Exiting... Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
                break;
        }
    } while (choice != 4);
}

int main() {
    NoteTakingApp app;
    app.run();
    return 0;
}
