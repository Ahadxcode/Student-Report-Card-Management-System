#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

class Student {
    int id;
    string name;
    vector<int> marks;
    float percentage;
    char grade;

public:
    Student(int i, string n, vector<int> m) {
        id = i;
        name = n;
        marks = m;
        calculate();
    }

    void calculate() {
        int total = 0;
        for (int m : marks) total += m;
        percentage = (float) total / marks.size();
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 75) grade = 'B';
        else if (percentage >= 60) grade = 'C';
        else if (percentage >= 40) grade = 'D';
        else grade = 'F';
    }

    void display() const {
        cout << setw(5) << id << setw(15) << name
             << setw(10) << fixed << setprecision(2) << percentage
             << setw(8) << grade << endl;
    }

    int getID() const { return id; }
    float getPercentage() const { return percentage; }
    char getGrade() const { return grade; }
    string getName() const { return name; }

    void saveToFile(ofstream &out) const {
        out << id << "," << name;
        for (int m : marks) out << "," << m;
        out << "," << percentage << "," << grade << "\n";
    }
};

// ---- Main Program ----
int main() {
    vector<Student> students;
    int choice;

    do {
        cout << "\n===== Student Report Card System =====\n";
        cout << "1. Add Student\n2. Display Students\n3. Search by ID\n"
             << "4. Sort by Percentage\n5. Save to File (report.txt)\n"
             << "6. Show Class Report\n7. Export Ranked Report (CSV)\n"
             << "0. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Try again.\n";
            continue;
        }

        if (choice == 1) {
            int id, nSubjects;
            string name;

            cout << "Enter Student ID: ";
            while (!(cin >> id)) {
                cout << "Invalid ID! Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Enter Student Name: ";
            cin.ignore(); // clear newline from buffer
            getline(cin, name);

            cout << "Enter number of subjects: ";
            while (!(cin >> nSubjects) || nSubjects <= 0) {
                cout << "Invalid number! Enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            vector<int> marks(nSubjects);
            for (int i = 0; i < nSubjects; i++) {
                cout << "Enter marks for subject " << i + 1 << ": ";
                while (!(cin >> marks[i]) || marks[i] < 0 || marks[i] > 100) {
                    cout << "Invalid marks! Enter between 0–100: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            students.push_back(Student(id, name, marks));
        }
        else if (choice == 2) {
            if (students.empty()) cout << "No students available!\n";
            else {
                cout << "\nID   Name            %        Grade\n";
                for (auto &s : students) s.display();
            }
        }
        else if (choice == 3) {
            int searchID;
            cout << "Enter Student ID to search: ";
            cin >> searchID;
            bool found = false;
            for (auto &s : students) {
                if (s.getID() == searchID) {
                    cout << "\nID   Name            %        Grade\n";
                    s.display();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Student not found!\n";
        }
        else if (choice == 4) {
            sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
                return a.getPercentage() > b.getPercentage();
            });
            cout << "Students sorted by Percentage!\n";
        }
        else if (choice == 5) {
            ofstream out("report.txt");
            for (auto &s : students) s.saveToFile(out);
            out.close();
            cout << "Data saved to report.txt\n";
        }
        else if (choice == 6) {
            if (students.empty()) {
                cout << "No students available!\n";
            } else {
                auto topper = max_element(students.begin(), students.end(),
                    [](const Student &a, const Student &b) { return a.getPercentage() < b.getPercentage(); });

                auto lowest = min_element(students.begin(), students.end(),
                    [](const Student &a, const Student &b) { return a.getPercentage() < b.getPercentage(); });

                float avg = 0;
                for (auto &s : students) avg += s.getPercentage();
                avg /= students.size();

                cout << "\n===== Class Report =====\n";
                cout << "Topper: "; topper->display();
                cout << "Lowest Scorer: "; lowest->display();
                cout << "Class Average: " << fixed << setprecision(2) << avg << "%\n";
            }
        }
        else if (choice == 7) {
            if (students.empty()) {
                cout << "No students available!\n";
            } else {
                // Sort by percentage
                sort(students.begin(), students.end(), [](const Student &a, const Student &b) {
                    return a.getPercentage() > b.getPercentage();
                });

                ofstream out("ranked_report.csv");
                out << "ID,Name,Percentage,Grade,Rank\n";

                int rank = 1;
                for (auto &s : students) {
                    out << s.getID() << "," << s.getName() << ","
                        << fixed << setprecision(2) << s.getPercentage() << ","
                        << s.getGrade() << "," << rank++ << "\n";
                }

                out.close();
                cout << "Ranked report saved to ranked_report.csv\n";
            }
        }

    } while (choice != 0);

    return 0;
}
