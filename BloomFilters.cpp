#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <stdexcept>
using namespace std;

const int FILTER_SIZE = 1000;
const int NUM_HASH_FUNCTIONS = 3;

class BloomFilter
{
private:
    bitset<FILTER_SIZE> filter;

    unsigned int hash1(string& str)
    {
        unsigned int hash = 0;
        for(char c : str)
        {
            hash = hash * 31 + c;
        }
        return hash % FILTER_SIZE;
    }

    unsigned int hash2(string& str)
    {
        unsigned int hash = 0;
        for (char c : str)
        {
            hash = hash * 37 + c;
        }
        return hash % FILTER_SIZE;
    }
    unsigned int hash3(string& str)
    {
        unsigned int hash = 0;
        for (char c : str)
        {
            hash = hash * 41 + c;
        }
        return hash % FILTER_SIZE;
    }

public:
    BloomFilter() {}

    void insertInFile(string& str)
    {
        filter.set(hash1(str));
        filter.set(hash2(str));
        filter.set(hash3(str));
    }
    bool mightContain(string& str)
    {
        return filter.test(hash1(str)) && filter.test(hash2(str)) && filter.test(hash3(str));
    }
};

bool signUp(string& username, string& password) {
    try {
        while (true) {
            ifstream file("user_details.txt");
            if (!file) {
                ofstream createFile("user_details.txt");
                if (!createFile) {
                    throw runtime_error("File cannot be opened");
                }
                createFile.close();
            }
            file.close();

            ifstream inFile("user_details.txt");
            if (!inFile.is_open())
            {
                throw runtime_error("File cannot be opened!");
            }

            string storedUsername, storedPassword;
            while (inFile >> storedUsername >> storedPassword)
            {
                if (storedUsername == username || storedPassword == password)
                {
                    inFile.close();
                    throw runtime_error("\nUsername or password already exists! Please choose another!");
                }
            }
            inFile.close();

            ofstream outFile("user_details.txt", ios::app);
            if (!outFile.is_open())
            {
                throw runtime_error("\nFile cannot be opened!");
            }

            outFile << username << " " << password << endl;
            outFile.close();

            cout << "\nSign up successful!" << endl;
            return true;
        }
    }
    catch(exception& error)
    {
        string choice;
        cout << error.what() << endl;
        cout << "\n 0 - Enter to exit sign up\n Enter any key to try signing up again" << endl;
        cout << "\nEnter your choice : ";
        cin >> choice;
        if (choice == "0")
        {
            return false;
        }
        else
        {
            cout << "\nEnter username : ";
            cin >> username;
            cout << "Enter password : ";
            cin >> password;
            return signUp(username, password);
        }
    }
}

bool login(string& username, string& password)
{
    string storedUsername, storedPassword;
    try
    {
        ifstream file("user_details.txt");
        if (!file)
        {
            throw runtime_error("File cannot be opened");
        }

        while(file >> storedUsername >> storedPassword)
        {
            if(storedUsername == username && storedPassword == password)
            {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
    catch (const exception& error)
    {
        cout << error.what() << endl;
        return false;
    }
}

int main()
{
    BloomFilter filter;
    int choice;
    string username, password;

    while(true)
    {
        cout << "-------------------" << endl;
        cout << "      MENU" << endl;
        cout << "-------------------" << endl;
        cout << "  1   -   LOG IN" << endl;
        cout << "  2   -   SIGN UP" << endl;
        cout << "  3   -   EXIT" << endl;
        cout << "-------------------" << endl << endl;
        cout << "Enter your choice : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "\nEnter the login details : \n";
            cout << "\nEnter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if(login(username, password))
            {
                cout << "\nLogin successful!" << endl;
                filter.insertInFile(username);
            }
            else
            {
                cout << "\nLogin failed! Invalid username or password!" << endl;
                string signupOption;
                cout << "\nDo you want to sign up? (Y/N): ";
                cin >> signupOption;
                if (signupOption == "Y" || signupOption == "y")
                {
                    cout << "\nEnter the sign up details :\n";
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    signUp(username, password);
                }
                else if (signupOption == "N" || signupOption == "n")
                {
                    continue;
                }
            }
            break;

        case 2:
            cout << "\nEnter the sign up details :\n";
            cout << "\nEnter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            signUp(username, password);
            break;

        case 3:
            cout << "\nExiting program." << endl;
            return 0;

        default:
            cout << "\nInvalid Input! Enter again!" << endl;
        }
    }
    return 0;
}
