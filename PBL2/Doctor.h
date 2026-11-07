#ifndef DOCTOR_H
#define DOCTOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include "Manager_Medicine.h"
#include "Manager_patient.h"
#include "Nhanvien.h"

using namespace std;

class Doctor : public NhanVien
{
protected:
    int soca;
public:
    Doctor() : soca(0) {}

    string getTag() const override { return "[BACSI]"; }
    string getChucVu() const override { return "Bac si"; }
    bool duocKeDon() const override { return true; }
    string getGhiChu() override { return "So ca: " + to_string(soca); }
    NhanVien *clone() const override { return new Doctor(*this); }

    int getSoca() { return soca; }
    void setSoca(int s) { soca = s; }

    void input() override
    {
        NhanVien::input();

        cout << "Nhap so ca kham: ";
        while (!(cin >> soca) || soca < 0)
        {
            cout << "So ca khong hop le! Vui long nhap lai: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    void nhapThongTinRieng() override
    {
        while (true)
        {
            if (!docSoNguyen("Nhap so ca kham: ", soca))
                return;
            if (soca >= 0)
                break;
            cout << "So ca khong hop le!\n";
        }
    }

    void display() override
    {
        NhanVien::display();
        cout << "So ca: " << soca << endl;
    }

    void readFile(ifstream &file) override
    {
        NhanVien::readFile(file);
        file >> soca;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void writeFile(ofstream &file) override
    {
        NhanVien::writeFile(file);
        file << soca << endl;
    }
};

#endif