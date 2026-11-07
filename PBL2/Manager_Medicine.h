#ifndef MANAGER_THUOC_H
#define MANAGER_THUOC_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

#include "Medicine.h"
#include "Display.h"

using namespace std;

class Manager_Thuoc
{
private:
    vector<Thuoc> thuocs;
    int indexOf(const string &ma)
    {
        for (size_t i = 0; i < thuocs.size(); i++)
            if (thuocs[i].getmathuoc() == ma)
                return (int)i;
        return -1;
    }

public:
    Thuoc *find(const string &ma)
    {
        int i = indexOf(ma);
        return i < 0 ? nullptr : &thuocs[i];
    }

    bool checkMaThuoc(string ma)
    {
        for (size_t i = 0; i < thuocs.size(); i++)
        {
            if (thuocs[i].getmathuoc() == ma)
                return true;
        }
        return false;
    }
    bool checkTenThuoc(string tenthuoc)
    {
        for (size_t i = 0; i < thuocs.size(); i++)
        {
            if (thuocs[i].gettenthuoc() == tenthuoc)
                return true;
        }
        return false;
    }
    //Nếu True -> lớn hơn hoặc bằng so, nếu false bé hơn
    void display(int so = 0, bool c = true)
    {
        printgiua(" DANH SACH THUOC ", "=",72);

        cout << left
             << setw(13) << "Ma thuoc"
             << setw(27) << "Ten thuoc"
             << setw(11) << "Don vi"
             << setw(11) << "Don gia"
             << setw(12) << "So luong"
             << endl;

        printline("=",72);

        for (size_t i = 0; i < thuocs.size(); i++)
        {
            if(c ? thuocs[i].getsoluong() >= so : thuocs[i].getsoluong() < so){
            cout << left << " "
                 << setw(12)  << thuocs[i].getmathuoc()
                 << setw(27) << thuocs[i].gettenthuoc()
                 << setw(11) << thuocs[i].getdonvitinh()
                 << setw(11) << thuocs[i].getdongia()
                 << setw(12) << thuocs[i].getsoluong()
                 << endl;
            printline("-",72);
            }

        }
        printline("=",72);
    }

    void addThuoc(const string &filePath)
    {
        Thuoc thuoc;
        thuoc.input();

        if (checkMaThuoc(thuoc.getmathuoc()))
        {
            cout << "Ma thuoc da ton tai!\n";
            return;
        }
        if (checkTenThuoc(thuoc.gettenthuoc()))
        {
            cout << "Ten thuoc da ton tai!\n";
            return;
        }

        thuocs.push_back(thuoc);
        saveToFile(filePath);

        cout << "Them thuoc thanh cong!\n";
    }

    void searchThuoc()
    {
        string ten;
        cout << "Nhap ten thuoc can tim: ";
        getline(cin, ten);

        for (size_t i = 0; i < thuocs.size(); i++)
        {
            if (thuocs[i].gettenthuoc().find(ten) != string::npos)
            {
                thuocs[i].display();
                printline("-");
                return;
            }
        }

            cout << "Khong tim thay thuoc!\n";
            return;
        }

    void deleteThuoc(const string &filePath)
    {
        string temp;
            cout << "Nhap ma thuoc can xoa: \n";
            getline(cin,temp);
        for (size_t i = 0; i < thuocs.size(); i++)
        {
            if (thuocs[i].getmathuoc() == temp)
            {
                thuocs.erase(thuocs.begin() + i);
                cout << "Da xoa thuoc thanh cong!\n";
                saveToFile(filePath);
                return;
            }
        }
        cout << "Khong tim thay ma thuoc!\n";
    }

    void updateMedicine(const string &filePath)
    {
        string ma;

        cout << "Nhap ma thuoc can cap nhat: ";
        getline(cin, ma);

        int i = indexOf(ma);

        if (i == -1)
        {
            cout << "Khong tim thay ma thuoc!\n";
            return;
        }

        printgiua(" THONG TIN THUOC HIEN TAI ", "-");
        thuocs[i].display();

        int chon;
        printgiua(" CHON THONG TIN CAN CAP NHAT ", "=");
        cout << "1. Cap nhat don gia\n";
        cout << "2. Cap nhat so luong ton kho\n";
        cout << "3. Cap nhat ca don gia va so luong\n";
        cout << "0. Huy cap nhat\n";

        cout << "Lua chon cua ban: ";

        while (!(cin >> chon) || chon < 0 || chon > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Lua chon khong hop le! Nhap lai: ";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (chon == 0)
        {
            cout << "Da huy cap nhat!\n";
            return;
        }

        double gia;
        int sl;

        if (chon == 1 || chon == 3)
        {
            cout << "Nhap don gia moi: ";

            while (!(cin >> gia) || gia < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Don gia khong hop le! Nhap lai: ";
            }

            thuocs[i].setdongia(gia);
        }

        if (chon == 2 || chon == 3)
        {
            cout << "Nhap so luong ton kho moi: ";

            while (!(cin >> sl) || sl < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "So luong khong hop le! Nhap lai: ";
            }

            thuocs[i].setsoluong(sl);
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        saveToFile(filePath);

        cout << "Cap nhat thuoc thanh cong!\n";
        printgiua(" THONG TIN SAU KHI CAP NHAT ", "-");
        thuocs[i].display();
    }

    void saveToFile(const string &filePath)
    {
        ofstream file(filePath);

        if (!file.is_open())
        {
            cout << "Loi khi mo file de ghi!\n";
            return;
        }

        for (size_t i = 0; i < thuocs.size(); i++)
        {
            thuocs[i].writeFile(file);

            if (i + 1 < thuocs.size())
                file << '\n';
        }

        if (!file)
        {
            cout << "Loi khi ghi du lieu vao file!\n";
            return;
        }

        file.close();
        cout << "Da luu danh sach thuoc!\n";
    }

    void readFile(ifstream &file)
    {

        if (!file.is_open())
        {
            cout << "Khong mo duoc file thuoc!\n";
            return;
        }

        thuocs.clear();

        while (file.peek() != EOF)
        {
            if (file.peek() == '\n' || file.peek() == '\r')
            {
                file.ignore(1000, '\n');
                continue;
            }

            Thuoc thuoc;
            thuoc.readFile(file);

            if (!file)
                break;

            thuocs.push_back(thuoc);

            if (file.peek() == '\r')
                file.get();

            if (file.peek() == '\n')
                file.get();
        }

        file.close();
    }
};

#endif