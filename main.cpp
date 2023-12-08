#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <utility>

class ScheduleItem {
public:
    ScheduleItem(std::string subject = "", std::string catalog = "", std::string section = "",
                 std::string component = "", std::string session = "", int units = 0,
                 int totEnrl = 0, int capEnrl = 0, std::string instructor = "")
        : subject(subject), catalog(catalog), section(section), component(component),
          session(session), units(units), totEnrl(totEnrl), capEnrl(capEnrl),
          instructor(instructor) {}

    void print() const {
        std::cout << subject << " " << catalog << " " << section << " " << component << " "
                  << session << " " << units << " " << totEnrl << " " << capEnrl << " " << instructor << std::endl;
    }

    std::string getSubject() const { return subject; }
    std::string getCatalog() const { return catalog; }
    std::string getSection() const { return section; }
    std::string getInstructor() const { return instructor; }

private:
    std::string subject;
    std::string catalog;
    std::string section;
    std::string component;
    std::string session;
    int units;
    int totEnrl;
    int capEnrl;
    std::string instructor;
};

class Schedule {
public:
    void initSchedule(std::ifstream& inputFile) {
        std::string line;
        std::getline(inputFile, line); // Skip the header line

        while (std::getline(inputFile, line)) {
            ScheduleItem newItem = parseCSVLine(line);
            addEntry(newItem);
        }
    }

    void print() const {
        for (const auto& entry : scheduleMap) {
            entry.second.print();
        }
    }

    void findBySubject(const std::string& targetSubject) const {
        for (const auto& entry : scheduleMap) {
            if (entry.second.getSubject() == targetSubject) {
                entry.second.print();
            }
        }
    }

    void findBySubjectAndCatalog(const std::string& targetSubject, const std::string& targetCatalog) const {
        for (const auto& entry : scheduleMap) {
            if (entry.second.getSubject() == targetSubject && entry.second.getCatalog() == targetCatalog) {
                entry.second.print();
            }
        }
    }

    void findByInstructorLastName(const std::string& instructorLastName) const {
        for (const auto& entry : scheduleMap) {
            size_t lastSpace = entry.second.getInstructor().find_last_of(' ');
            std::string lastName = entry.second.getInstructor().substr(lastSpace + 1);
            if (lastName == instructorLastName) {
                entry.second.print();
            }
        }
    }

private:
    std::map<std::string, ScheduleItem> scheduleMap;

    ScheduleItem parseCSVLine(const std::string& line) const {
        std::istringstream ss(line);
        std::string subject, catalog, section, component, session, instructor;
        int units, totEnrl, capEnrl;

        std::getline(ss, subject, ',');
        std::getline(ss, catalog, ',');
        std::getline(ss, section, ',');
        std::getline(ss, component, ',');
        std::getline(ss, session, ',');
        ss >> units; ss.ignore();
        ss >> totEnrl; ss.ignore();
        ss >> capEnrl; ss.ignore();
        std::getline(ss, instructor, ',');

        return ScheduleItem(subject, catalog, section, component, session, units, totEnrl, capEnrl, instructor);
    }

    void addEntry(const ScheduleItem& newItem) {
        std::string key = newItem.getSubject() + "_" + newItem.getCatalog() + "_" + newItem.getSection();
        scheduleMap[key] = newItem;
    }
};

int main() {
    Schedule schedule;
    std::ifstream inputFile("Summer2022.csv");

    if (inputFile.is_open()) {
        schedule.initSchedule(inputFile);
        inputFile.close();

        int choice;
        do {
            std::cout << "\n1. Find records with target subject\n";
            std::cout << "2. Find records with target subject and catalog number\n";
            std::cout << "3. Find records given the instructor's last name\n";
            std::cout << "4. Print Schedule\n";
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::string targetSubject;
                    std::cout << "Enter target subject: ";
                    std::cin >> targetSubject;
                    schedule.findBySubject(targetSubject);
                    break;
                }
                case 2: {
                    std::string targetSubject, targetCatalog;
                    std::cout << "Enter target subject: ";
                    std::cin >> targetSubject;
                    std::cout << "Enter target catalog: ";
                    std::cin >> targetCatalog;
                    schedule.findBySubjectAndCatalog(targetSubject, targetCatalog);
                    break;
                }
                case 3: {
                    std::string instructorLastName;
                    std::cout << "Enter instructor's last name: ";
                    std::cin >> instructorLastName;
                    schedule.findByInstructorLastName(instructorLastName);
                    break;
                }
                case 4:
                    schedule.print();
                    break;
                case 0:
                    std::cout << "Exiting program.\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        } while (choice != 0);
    } else {
        std::cerr << "Error opening the file.\n";
    }

    return 0;
}
