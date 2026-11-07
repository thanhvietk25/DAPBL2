#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Person.h"
#include "BHYT.h"
#include "Date.h"
#include "Medicine.h"
#include "Manager_Medicine.h"
#include "Doctor.h"
#include "Display.h"

using namespace std;

class Patient : public Person, public BHYT
{
private:
    string bacsikham;
    string chuandoan;
    string sophong;
    Date date;
    vector<Thuoc> thuocBN;

public:
    string getname()
    {
        return Person::getname();
    }
    vector<Thuoc>& getthuoc()
    {
        return thuocBN;
    }
    string getcccd()
    {
        return Person::getcccd();
    }

    string getnumber()
    {
        return Person::getnumber();
    }

    bool getgender()
    {
        return Person::getgender();
    }

    int getage()
    {
        return Person::getage();
    }

    string getbacsikham()
    {
        return bacsikham;
    }
    string getchuandoan()
    {
        return chuandoan;
    }
    string getsophong()
    {
        return sophong;
    }
    Date getngaykham()
    {
        return date;
    }

    void setname(string &s)
    {
        Person::setname(s);
    }

    void setcccd(string &s)
    {
        Person::setcccd(s);
    }

    void setnumber(string &s)
    {
        Person::setnumber(s);
    }

    void setgender(bool g)
    {
        Person::setgender(g);
    }

    bool setage(int a)
    {
        return Person::setage(a);
    }

    void setbacsikham(string &bs)
    {
        bacsikham = bs;
    }
    void setchuandoan(string &cd)
    {
        chuandoan = cd;
    }
    void setsophong(string &sp)
    {
        sophong = sp;
    }
    bool setngaykham(int n, int t, int nm)
    {
        return date.setDate(n, t, nm);
    }
    void setthuoc(vector<Thuoc> thuoctemp)
    {
        thuocBN = thuoctemp;
        return;
    }

    void input()
    {
        Person::input();

        BHYT::input();

        cin.ignore();

        cout << "Nhap bac si kham: ";
        getline(cin, bacsikham);

        cout << "Nhap chuan doan: ";
        getline(cin, chuandoan);

        cout << "Nhap so phong: ";
        getline(cin, sophong);

        cout << "Nhap ngay kham: ";
        date.nhap();

        
    }

    void display()
    {
        Person::display();
        BHYT::display();
        cout << endl;
        cout << "Bac si kham: " << bacsikham << endl;
        cout << "Chuan doan: " << chuandoan << endl;
        cout << "So phong: " << sophong << endl;
        cout << "Ngay kham: ";
        date.xuat();
        cout << endl;
        
        // --- Thêm phần hiển thị danh sách thuốc ở đây ---
        cout << "Danh sach thuoc ke don (" << thuocBN.size() << "):" << endl;
        if (thuocBN.empty())
        {
            cout << "  (Khong co thuoc)" << endl;
        }
        else
        {
            for (size_t i = 0; i < thuocBN.size(); i++)
            {
                cout << "  --- Thuoc " << i + 1 << " ---" << endl;
                thuocBN[i].display(); // Gọi hàm display của class Thuoc
            }
        }
        cout << "\n";
        printline("-");
    }

    void readFile(ifstream &file)
    {
        Person::readFile(file);
        BHYT::readFile(file);

        getline(file, bacsikham);
        getline(file, chuandoan);
        getline(file, sophong);

        date.readFile(file);

        size_t n;
        file >> n;
        file.ignore(1000, '\n');

        thuocBN.clear();
        for (size_t i = 0; i < n; i++)
        {
            Thuoc thuoc;
            thuoc.readFile(file);
            thuocBN.push_back(thuoc);
        }
    }

    void writeFile(ofstream &file)
    {
        Person::writeFile(file);
        BHYT::writeFile(file);

        file << bacsikham << "\n";
        file << chuandoan << "\n";
        file << sophong << "\n";

        date.writeFile(file);

        file << thuocBN.size() << "\n";
        for (size_t i = 0; i < thuocBN.size(); i++)
        {
            thuocBN[i].writeFile(file);
        }
    }
};

#endif