#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Patient {
    string id, name, age, gender, symptoms;
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void registerPatient() {
    Patient p;
    cout << "\n--- Patient Registration ---\n";
    cout << "Patient ID: "; getline(cin, p.id);
    cout << "Name: "; getline(cin, p.name);
    cout << "Age: "; getline(cin, p.age);
    cout << "Gender: "; getline(cin, p.gender);
    cout << "Symptoms: "; getline(cin, p.symptoms);

    cout << "\nPatient registered in this session successfully.\n";
    cout << "ID: " << p.id << "\nName: " << p.name
         << "\nAge: " << p.age << "\nGender: " << p.gender
         << "\nSymptoms: " << p.symptoms << "\n";
}

void symptomInformation() {
    string symptom;
    cout << "\nEnter a symptom (fever/cough/headache): ";
    getline(cin, symptom);

    if (symptom == "fever" || symptom == "Fever")
        cout << "General information: Fever can occur with many conditions. Monitor temperature and seek professional advice when concerned.\n";
    else if (symptom == "cough" || symptom == "Cough")
        cout << "General information: Cough can have many causes. Note its duration and any accompanying symptoms.\n";
    else if (symptom == "headache" || symptom == "Headache")
        cout << "General information: Headache can have many causes. Note severity, duration and associated symptoms.\n";
    else
        cout << "No basic information available for this keyword in the prototype.\n";

    cout << "This educational prototype does not diagnose or recommend treatment.\n";
}

int main() {
    int choice = 0;
    cout << "========================================\n";
    cout << " CLINICAL DECISION SUPPORT SYSTEM (CDSS)\n";
    cout << " Evolution 1 - Basic C++ Prototype\n";
    cout << "========================================\n";

    do {
        cout << "\n1. Patient Registration\n";
        cout << "2. Basic Symptom Information\n";
        cout << "3. About Project\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        clearInput();

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: symptomInformation(); break;
            case 3:
                cout << "\nCDSS Evolution 1 is a console-based educational prototype.\n";
                cout << "It demonstrates a menu, patient data entry and basic symptom information.\n";
                break;
            case 0: cout << "Exiting CDSS. Thank you.\n"; break;
            default: cout << "Please select a valid option.\n";
        }
    } while (choice != 0);

    return 0;
}
