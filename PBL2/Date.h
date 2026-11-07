#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <string>

using namespace std;

class Date {
private:
    int ngay;
    int thang;
    int nam;

public:
    bool check(int n, int t, int nm) {
        int i[] = {0, 31, 28, 31, 30, 31, 30,
                   31, 31, 30, 31, 30, 31};

        if ((nm % 4 == 0 && nm % 100 != 0) || nm % 400 == 0)
            i[2] = 29;

        if (t < 1 || t > 12)
            return false;

        return n >= 1 && n <= i[t];
    }

    // ================= GETTER =================
    int getngay() const {
        return ngay;
    }
    int getthang() const {
        return thang;
    }
    int getnam() const {
        return nam;
    }

    bool setDate(int n, int t, int nm) {
        if (!check(n, t, nm))
            return false;
        ngay = n;
        thang = t;
        nam = nm;
        return true;
    }

    void nhap() {
        do {
            cout << "Nhap ngay: ";
            cin >> ngay;

            cout << "Nhap thang: ";
            cin >> thang;

            cout << "Nhap nam: ";
            cin >> nam;

            if (!check(ngay, thang, nam))
                cout << "Ngay khong hop le! Nhap lai.\n";

        } while (!check(ngay, thang, nam));
    }

    void tangngay() {
        ngay++;

        if (!check(ngay, thang, nam)) {
            ngay = 1;
            thang++;

            if (thang > 12) {
                thang = 1;
                nam++;
            }
        }
    }

    void xuat() {
        cout << ngay << "/" << thang << "/" << nam;
    }

    void readFile(ifstream &file) {
        string s;
        getline(file, s);

        sscanf(s.c_str(), "%d/%d/%d",
               &ngay, &thang, &nam);
    }

    void writeFile(ofstream &file) {
        file << setfill('0')
             << setw(2) << ngay << "/"
             << setw(2) << thang << "/"
             << setw(4) << nam << endl;
    }
};

#endif
