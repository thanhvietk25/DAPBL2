#ifndef THUOC_H
#define THUOC_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Thuoc
{
private:
    string mathuoc;
    string tenthuoc;
    string donvitinh;
    string congdung;
    double dongia;
    int soluong;

public:
    string getmathuoc()
    {
        return mathuoc;
    }

    string gettenthuoc()
    {
        return tenthuoc;
    }

    string getdonvitinh()
    {
        return donvitinh;
    }

    string getcongdung()
    {
        return congdung;
    }

    double getdongia()
    {
        return dongia;
    }

    int getsoluong()
    {
        return soluong;
    }

    void setmathuoc(string &s)
    {
        while (s.empty())
        {
            cout << "Ma thuoc khong duoc de trong, nhap lai: ";
            getline(cin, s);
        }

        mathuoc = s;
    }

    void settenthuoc(string &s)
    {
        while (s.empty())
        {
            cout << "Ten thuoc khong duoc de trong, nhap lai: ";
            getline(cin, s);
        }

        tenthuoc = s;
    }

    void setdonvitinh(string &s)
    {
        while (s.empty())
        {
            cout << "Don vi tinh khong duoc de trong, nhap lai: ";
            getline(cin, s);
        }

        donvitinh = s;
    }

    void setcongdung(string &s)
    {
        while (s.empty())
        {
            cout << "Cong dung khong duoc de trong, nhap lai: ";
            getline(cin, s);
        }

        congdung = s;
    }

    void setdongia(double gia)
    {
        while (gia < 0)
        {
            cout << "Don gia khong hop le, nhap lai: ";
            cin >> gia;
        }

        dongia = gia;
    }

    void setsoluong(int sl)
    {
        while (sl < 0)
        {
            cout << "So luong khong hop le, nhap lai: ";
            cin >> sl;
        }

        soluong = sl;
    }
    void input()
    {
        string s;
        double gia;
        int sl;

        cout << "Nhap ma thuoc: ";
        getline(cin, s);
        setmathuoc(s);

        cout << "Nhap ten thuoc: ";
        getline(cin, s);
        settenthuoc(s);

        cout << "Nhap don vi tinh: ";
        getline(cin, s);
        setdonvitinh(s);

        cout << "Nhap cong dung: ";
        getline(cin, s);
        setcongdung(s);

        cout << "Nhap don gia: ";
        cin >> gia;
        setdongia(gia);

        cout << "Nhap so luong ton kho: ";
        cin >> sl;
        setsoluong(sl);

        cin.ignore(1000, '\n');
    }

    void display()
    {
        cout << "Ma thuoc: " << mathuoc << endl;
        cout << "Ten thuoc: " << tenthuoc << endl;
        cout << "Don vi tinh: " << donvitinh << endl;
        cout << "Cong dung: " << congdung << endl;
        cout << "Don gia: " << dongia << endl;
        cout << "So luong ton kho: " << soluong << endl;
    }

    void writeFile(ofstream &file)
    {
        file << mathuoc << '\n';
        file << tenthuoc << '\n';
        file << donvitinh << '\n';
        file << congdung << '\n';
        file << dongia << '\n';
        file << soluong << '\n';
    }

    void readFile(ifstream &file)
    {
        string gia, sl;

        getline(file, mathuoc);
        getline(file, tenthuoc);
        getline(file, donvitinh);
        getline(file, congdung);
        getline(file, gia);
        getline(file, sl);

        dongia = stod(gia);
        soluong = stoi(sl);
    }
};
#endif