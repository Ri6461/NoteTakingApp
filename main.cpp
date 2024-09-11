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
};

void NoteTakingApp::displayMenu() {
    cout << "Note Taking App" << endl;
    cout << "1. Create Note" << endl;
    cout << "2. View Notes" << endl;
    cout << "3. Delete Note" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose an option: ";
}

void NoteTakingApp::createNote() {
    string note;
    cout << "Enter your note: ";
    cin.ignore(); // to clear the newline left by previous input
    getline(cin, note);

    vector<string> notes = loadNotes();
    notes.push_back(note);
    saveNotes(notes);
    cout << "Note added successfully!" << endl;
}

void NoteTakingApp::viewNotes() {
    vector<string> notes = loadNotes();
    if (notes.empty()) {
        cout << "No notes available." << endl;
        return;
    }

    for (size_t i = 0; i < notes.size(); ++i) {
        cout << i + 1 << ". " << notes[i] << endl;
    }
}

void NoteTakingApp::deleteNote() {
    vector<string> notes = loadNotes();
    if (notes.empty()) {
        cout << "No notes to delete." << endl;
        return;
    }

    viewNotes();
    size_t index;
    cout << "Enter the number of the note to delete: ";
    cin >> index;

    if (index < 1 || index > notes.size()) {
        cout << "Invalid index." << endl;
        return;
    }

    notes.erase(notes.begin() + index - 1);
    saveNotes(notes);
    cout << "Note deleted successfully!" << endl;
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

void NoteTakingApp::run() {
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: createNote(); break;
            case 2: viewNotes(); break;
            case 3: deleteNote(); break;
            case 4: cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 4);
}

int main() {
    NoteTakingApp app;
    app.run();
    return 0;
}
