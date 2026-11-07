#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

class Person
{
protected:
    string name;
    string cccd;
    string number;
    bool gender;
    int age;

public:
    void setname(string &s)
    {
        name = s;
    }
    void setcccd(string &s)
    {
        bool valid = false;
        while (!valid)
        {
            valid = (s.length() == 12);

            for (char c : s)
            {
                if (!isdigit(c))
                {
                    valid = false;
                    break;
                }
            }

            if (!valid)
            {
                cout << "Vui long nhap CCCD gom 12 chu so: ";
                cin >> s;
            }
        }

        cccd = s;
    }
    void setnumber(string &s)
    {
        number = s;
    }
    void setgender(bool g)
    {
        gender = g;
    }
    bool setage(int a)
    {
        if (a < 1 || a > 120)
            return false;
        age = a;
        return true;
    }
    string getcccd()
    {
        return cccd;
    }
    string getname()
    {
        return name;
    }
    string getnumber()
    {
        return number;
    }
    bool getgender()
    {
        return gender;
    }
    int getage()
    {
        return age;
    }
    void input()
    {
        string temp;
        cout << "Nhap CCCD: ";
        cin >> temp;
        setcccd(temp);
        cin.ignore();

        cout << "Nhap ho ten: ";
        getline(cin, name);

        cout << "Nhap tuoi: ";

        while (!(cin >> age) || age < 1 || age > 120)
        {
            cout << "Tuoi khong hop le! Vui long nhap lai: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << "Nhap gioi tinh : 1 = Nu / 0 = Nam : ";

        while (!(cin >> gender))
        {
            cout << "Vui long nhap 0 hoac 1: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << "Nhap so dien thoai: ";
        cin >> number;
    }

    void display()
    {
        cout << "CCCD: " << cccd << endl;
        cout << "Ho ten: " << name << endl;
        cout << "Tuoi: " << age << endl;
        cout << "Gioi tinh: " << (gender ? "Nu" : "Nam") << endl;
        cout << "So dien thoai: " << number << endl;
    }

    void readFile(ifstream &file)
    {
        getline(file, name);
        getline(file, cccd);
        getline(file, number);

        string genStr, ageStr;
        getline(file, genStr);
        gender = (genStr == "1" || genStr == "true");

        getline(file, ageStr);
        if (!ageStr.empty())
        {
            age = stoi(ageStr);
        }
    }

    void writeFile(ofstream &file)
    {
        file << name << "\n"
             << cccd << "\n"
             << number << "\n"
             << gender << "\n"
             << age << "\n";
    }
};

#endif