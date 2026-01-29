#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Contact
{
private:
    int contactID;
    string firstName, lastName, category;
    vector<string> phoneNumbers;
    vector<string> emails;

public:
    Contact() {}
    Contact(int id, string fn, string ln, string cat)
        : contactID(id), firstName(fn), lastName(ln), category(cat) {}

    int getID() { return contactID; }
    string getLastName() { return lastName; }

    void addPhone(string phone) { phoneNumbers.push_back(phone); }
    void addEmail(string email) { emails.push_back(email); }

    void display()
    {
        cout << "ID: " << contactID
             << " | Name: " << firstName << " " << lastName
             << " | Category: " << category << endl;

        cout << "Phones: ";
        for (string p : phoneNumbers)
            cout << p << " ";
        cout << "\nEmails: ";
        for (string e : emails)
            cout << e << " ";
        cout << endl;
    }

    string serialize()
    {
        // Save data in CSV style for file storage
        ostringstream oss;
        oss << contactID << "," << firstName << "," << lastName << "," << category;
        oss << ",Phones:";
        for (string p : phoneNumbers)
            oss << p << "|";
        oss << ",Emails:";
        for (string e : emails)
            oss << e << "|";
        return oss.str();
    }

    void deserialize(string line)
    {
        phoneNumbers.clear();
        emails.clear();
        stringstream ss(line);
        string token;

        getline(ss, token, ',');
        contactID = stoi(token);
        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, category, ',');

        getline(ss, token, ','); // Phones:
        if (token.find("Phones:") == 0)
        {
            string phones = token.substr(7);
            stringstream sp(phones);
            while (getline(sp, token, '|'))
            {
                if (!token.empty())
                    phoneNumbers.push_back(token);
            }
        }

        getline(ss, token, ','); // Emails:
        if (token.find("Emails:") == 0)
        {
            string emailsStr = token.substr(7);
            stringstream se(emailsStr);
            while (getline(se, token, '|'))
            {
                if (!token.empty())
                    emails.push_back(token);
            }
        }
    }
};

vector<Contact> contacts;
int nextID = 1;

void loadContacts()
{
    ifstream fin("contacts.dat");
    string line;
    while (getline(fin, line))
    {
        Contact c;
        c.deserialize(line);
        contacts.push_back(c);
        if (c.getID() >= nextID)
            nextID = c.getID() + 1;
    }
    fin.close();
}

void saveContacts()
{
    ofstream fout("contacts.dat");
    for (auto &c : contacts)
    {
        fout << c.serialize() << endl;
    }
    fout.close();
}

void addContact()
{
    string fn, ln, cat, phone, email;
    cout << "Enter First Name: ";
    cin >> fn;
    cout << "Enter Last Name: ";
    cin >> ln;
    cout << "Enter Category: ";
    cin >> cat;

    Contact c(nextID++, fn, ln, cat);

    cout << "Enter Phone Numbers (type 'done' to stop): ";
    while (true)
    {
        cin >> phone;
        if (phone == "done")
            break;
        c.addPhone(phone);
    }

    cout << "Enter Emails (type 'done' to stop): ";
    while (true)
    {
        cin >> email;
        if (email == "done")
            break;
        c.addEmail(email);
    }

    contacts.push_back(c);
    cout << "Contact added successfully.\n";
}

void searchContact()
{
    int id;
    string lname;
    cout << "Search by 1.ID or 2.Last Name? ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        cout << "Enter Contact ID: ";
        cin >> id;
        for (auto &c : contacts)
        {
            if (c.getID() == id)
            {
                c.display();
                return;
            }
        }
    }
    else
    {
        cout << "Enter Last Name: ";
        cin >> lname;
        for (auto &c : contacts)
        {
            if (c.getLastName() == lname)
            {
                c.display();
                return;
            }
        }
    }
    cout << "Contact not found.\n";
}

void deleteContact()
{
    int id;
    cout << "Enter Contact ID to delete: ";
    cin >> id;
    for (auto it = contacts.begin(); it != contacts.end(); ++it)
    {
        if (it->getID() == id)
        {
            contacts.erase(it);
            cout << "Contact deleted.\n";
            return;
        }
    }
    cout << "Contact not found.\n";
}

void updateContact()
{
    int id;
    cout << "Enter Contact ID to update: ";
    cin >> id;
    for (auto &c : contacts)
    {
        if (c.getID() == id)
        {
            cout << "Updating contact...\n";
            c.display();
            cout << "Enter new First Name: ";
            string fn;
            cin >> fn;
            cout << "Enter new Last Name: ";
            string ln;
            cin >> ln;
            cout << "Enter new Category: ";
            string cat;
            cin >> cat;
            Contact updated(id, fn, ln, cat);
            string phone, email;

            cout << "Enter new Phone Numbers (done to stop): ";
            while (true)
            {
                cin >> phone;
                if (phone == "done")
                    break;
                updated.addPhone(phone);
            }

            cout << "Enter new Emails (done to stop): ";
            while (true)
            {
                cin >> email;
                if (email == "done")
                    break;
                updated.addEmail(email);
            }

            c = updated;
            cout << "Contact updated successfully.\n";
            return;
        }
    }
    cout << "Contact not found.\n";
}

void displayAll()
{
    for (auto &c : contacts)
    {
        c.display();
    }
}

int main()
{
    loadContacts();
    int choice;

    do
    {
        cout << "\n==== Contact Management System ====\n";
        cout << "1. Add New Contact\n";
        cout << "2. Search for a Contact\n";
        cout << "3. Delete a Contact\n";
        cout << "4. Update a Contact\n";
        cout << "5. Display All Contacts\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addContact();
            break;
        case 2:
            searchContact();
            break;
        case 3:
            deleteContact();
            break;
        case 4:
            updateContact();
            break;
        case 5:
            displayAll();
            break;
        case 6:
            saveContacts();
            cout << "Exiting... Data saved.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
