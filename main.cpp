#include <iostream>
#include <string>
#include<iomanip>
using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    int priority;
    int arrival;
};


const int MAX = 100;
Patient room[MAX];
int count = 0;
int nextId = 1;
int arrivalNumber = 0;


bool isMoreUrgent(Patient a, Patient b) {
    if (a.priority != b.priority) {
        return a.priority > b.priority;
    }
    return a.arrival < b.arrival;
}


void swapPatients(int i, int j) {
    Patient temp = room[i];
    room[i] = room[j];
    room[j] = temp;
}


void moveUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (isMoreUrgent(room[i], room[parent])) {
            swapPatients(i, parent);
            i = parent;
        } else {
            break;
        }
    }
}


void moveDown(int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int best = i;
        if (left < count && isMoreUrgent(room[left], room[best])) {
            best = left;
        }
        if (right < count && isMoreUrgent(room[right], room[best])) {
            best = right;
        }
        if (best == i) {
            break;
        }
        swapPatients(i, best);
        i = best;
    }
}


string priorityName(int p) {
    if (p == 1) return "Routine";
    if (p == 2) return "Serious";
    if (p == 3) return "Critical";
    if (p == 4) return "Emergency";
    if (p == 5) return "Life-threatening";
    return "Unknown";
}



// 1. Register a new patient
int registerPatient(string name, int age, int priority) {
    Patient p;
    p.id = nextId;
    nextId = nextId + 1;
    p.name = name;
    p.age = age;
    p.priority = priority;
    arrivalNumber = arrivalNumber + 1;
    p.arrival = arrivalNumber;

    room[count] = p;
    count = count + 1;
    moveUp(count - 1);
    return p.id;
}

// 2. Change a patient's priority
bool changePriority(int id, int newPriority) {
    for (int i = 0; i < count; i++) {
        if (room[i].id == id) {
            int oldPriority = room[i].priority;
            room[i].priority = newPriority;
            if (newPriority > oldPriority) {
                moveUp(i);
            } else if (newPriority < oldPriority) {
                moveDown(i);
            }
            return true;
        }
    }
    return false;
}

// 3. Look at the most urgent patient (do not remove)
bool viewTop(Patient &result) {
    if (count == 0) return false;
    result = room[0];
    return true;
}

// 4. Serve the most urgent patient (remove them)
bool servePatient(Patient &result) {
    if (count == 0) return false;
    result = room[0];
    room[0] = room[count - 1];
    count = count - 1;
    if (count > 0) {
        moveDown(0);
    }
    return true;
}

// 5. Show everyone, most urgent first
void displayWaiting() {
    if (count == 0) {
        cout << "\nNo patients are waiting.\n";
        return;
    }
    Patient copy[MAX];
    for (int i = 0; i < count; i++) {
        copy[i] = room[i];
    }
    // simple bubble sort: most urgent goes first
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (isMoreUrgent(copy[j + 1], copy[j])) {
                Patient temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }
    /*cout << "\n--- WAITING PATIENTS (most urgent first) ---\n";
    cout << "ID\tName\t\tAge\tPriority\n";
    for (int i = 0; i < count; i++) {
        cout << copy[i].id << "\t" << copy[i].name << "\t\t"
             << copy[i].age << "\t" << copy[i].priority
             << " (" << priorityName(copy[i].priority) << ")\n";
    }
    cout << "Total waiting: " << count << "\n";
}*/

cout << "\n--- WAITING PATIENTS (most urgent first) ---\n";

    cout << left << setw(6) << "ID" << setw(18) << "Name" << setw(6) << "Age" << "Priority\n";
    for (int i = 0; i < count; i++) {
        string shortName = copy[i].name;
        if (shortName.length() > 16) {
            shortName = shortName.substr(0, 16);
        }
        cout << setw(6) << copy[i].id << setw(18) << shortName << setw(6) << copy[i].age
             << copy[i].priority << " (" << priorityName(copy[i].priority) << ")\n";
    }
    cout << "Total waiting: " << count << "\n";
}



int readNumber(string message, int low, int high) {
    int value;
    while (true) {
        cout << message;
        cin >> value;
        if (!cin.fail() && value >= low && value <= high) {
            cin.ignore(1000, '\n');
            return value;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please type a number from " << low << " to " << high << ".\n";
    }
}

void showPriorityGuide() {
    cout << "  1 = Routine\n";
    cout << "  2 = Serious\n";
    cout << "  3 = Critical\n";
    cout << "  4 = Emergency\n";
    cout << "  5 = Life-threatening\n";
}

void printPatient(Patient p) {
    cout << "ID " << p.id << " | " << p.name << " | Age " << p.age
         << " | Priority " << p.priority << " (" << priorityName(p.priority) << ")\n";
}

// ---------- The main menu ----------
int main() {
    int choice = 0;
    do {
        cout << "\n===== EMERGENCY RESPONSE PRIORITY SYSTEM =====\n";
        cout << "1. Register patient\n";
        cout << "2. Assign (change) priority\n";
        cout << "3. View highest-priority patient\n";
        cout << "4. Serve patient\n";
        cout << "5. Display waiting patients\n";
        cout << "0. Exit\n";
        choice = readNumber("Choose an option: ", 0, 5);

        if (choice == 1) {
            if (count == MAX) {
                cout << "The waiting room is full.\n";
            } else {
                string name;
                do {
    cout << "Patient name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Name cannot be empty. Try again.\n";
    }

} while (name.empty());
                int age = readNumber("Age (0-120): ", 0, 120);
                cout << "Priority levels:\n";
                showPriorityGuide();
                int pr = readNumber("Priority (1-5): ", 1, 5);
                int id = registerPatient(name, age, pr);
                cout << "Registered " << name << " with ID " << id << ".\n";
            }
        } else if (choice == 2) {
            if (count == 0) {
                cout << "No patients are waiting.\n";
            } else {
                int id = readNumber("Patient ID: ", 1, 1000000);
                showPriorityGuide();
                int pr = readNumber("New priority (1-5): ", 1, 5);
                if (changePriority(id, pr)) {
                    cout << "Priority updated.\n";
                } else {
                    cout << "No waiting patient has that ID.\n";
                }
            }
        } else if (choice == 3) {
            Patient p;
            if (viewTop(p)) {
                cout << "Next to be served: ";
                printPatient(p);
            } else {
                cout << "No patients are waiting.\n";
            }
        } else if (choice == 4) {
            Patient p;
            if (servePatient(p)) {
                cout << "Now serving: ";
                printPatient(p);
            } else {
                cout << "No patients are waiting.\n";
            }
        } else if (choice == 5) {
            displayWaiting();
        }
    } while (choice != 0);

    cout << "Goodbye! Stay safe.\n";
    return 0;
}
