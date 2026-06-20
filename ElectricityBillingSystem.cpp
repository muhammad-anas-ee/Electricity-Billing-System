#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

class Person {
protected:
    string name;
    string address;

public:
    Person() : name(""), address("") {}

    void setPersonalInfo() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
    }

    void displayPersonalInfo() const {
        cout << "\nName: " << name << "\nAddress: " << address;
    }

    void saveToFile(ofstream& file) const {
        file << name << endl << address << endl;
    }

    void loadFromFile(ifstream& file) {
        file.ignore();
        getline(file, name);
        getline(file, address);
    }
};

class Consumer : public Person {
private:
    int id;
    int unitsconsumed;
    double billAmount;

public:
    Consumer() : id(0), unitsconsumed(0), billAmount(0.0) {}

    void setConsumerInfo() {
        setPersonalInfo();
        cout << "Enter Consumer ID: ";
        cin >> id;
        cout << "Enter Units Consumed: ";
        cin >> unitsconsumed;
        calculateBill();
    }

    void calculateBill() {
        if (unitsconsumed <= 100)
            billAmount = unitsconsumed * 5;
        else if (unitsconsumed <= 300)
            billAmount = 100 * 5 + (unitsconsumed - 100) * 7;
        else
            billAmount = 100 * 5 + 200 * 7 + (unitsconsumed - 300) * 10;
    }

    void display() const {
        displayPersonalInfo();
        cout << "\nID: " << id
             << "\nUnits Consumed: " << unitsconsumed
             << "\nBill Amount: Rs. " << billAmount
             << endl << endl;
    }

    void saveToFile(ofstream& file) const {
        Person::saveToFile(file);
        file << id << endl << unitsconsumed << endl << billAmount << endl;
    }

    void loadFromFile(ifstream& file) {
        Person::loadFromFile(file);
        file >> id >> unitsconsumed >> billAmount;
    }

    int getId() const {
        return id;
    }

    double getBillAmount() const {
        return billAmount;
    }
};

class ElectricityBillingSystem {
private:
    vector<Consumer> consumers;
    const string filename = "consumers_data.txt";

public:
    ElectricityBillingSystem() {
        loadFromFile();
    }

    ~ElectricityBillingSystem() {
        saveToFile();
    }

    void addConsumer() {
        Consumer c;
        c.setConsumerInfo();
        consumers.push_back(c);
        cout << "Consumer added successfully!" << endl;
        saveToFile();
    }

    void viewConsumers() const {
        if (consumers.empty()) {
            cout << "\nNo consumers found!" << endl;
            return;
        }
        cout << "\n--- All Consumers ---" << endl;
        for (const auto& c : consumers)
            c.display();
    }

    void searchConsumer(int id) const {
        for (const auto& c : consumers) {
            if (c.getId() == id) {
                cout << "\n--- Consumer Found ---\n";
                c.display();
                return;
            }
        }
        cout << "\nConsumer not found!\n";
    }

    void deleteConsumer(int id) {
        for (auto it = consumers.begin(); it != consumers.end(); ++it) {
            if (it->getId() == id) {
                consumers.erase(it);
                cout << "\nConsumer deleted successfully!\n";
                saveToFile();
                return;
            }
        }
        cout << "\nConsumer not found!\n";
    }

    void calculateTotalRevenue() const {
        double total = 0;
        for (const auto& c : consumers)
            total += c.getBillAmount();
        cout << "\nTotal Revenue: Rs. " << total << endl;
    }

    void saveToFile() const {
        ofstream file(filename);
        for (const auto& c : consumers)
            c.saveToFile(file);
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;
        while (file.peek() != EOF) {
            Consumer c;
            c.loadFromFile(file);
            if (file)
                consumers.push_back(c);
        }
    }
};

int main() {
    ElectricityBillingSystem system;
    int choice;

    do {
        cout << "\n1. Add Consumer" << endl;
        cout << "2. View All Consumers" << endl;
        cout << "3. Search Consumer" << endl;
        cout << "4. Delete Consumer" << endl;
        cout << "5.Calculate Total Revenue" << endl;
        cout << "6.Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.addConsumer();
                break;
            case 2:
                system.viewConsumers();
                break;
            case 3: {
                int id;
                cout << "Enter Consumer ID to search: ";
                cin >> id;
                system.searchConsumer(id);
                break;
            }
            case 4: {
                int id;
                cout << "Enter Consumer ID to delete: ";
                cin >> id;
                system.deleteConsumer(id);
                break;
            }
            case 5:
                system.calculateTotalRevenue();
                break;
            case 6:
                cout << "Exiting Program..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 6);

    return 0;
}
