#ifndef BHYT_H
#define BHYT_H

#include <iostream>
#include <fstream>
#include <string>

#include "Date.h"

using namespace std;

class BHYT {
private:
    string so;
    Date A;

public:
    // ================= GETTER =================
    string getso() const {
        return so;
    }
    Date getngaymua() const {
        return A;
    }

    // ================= SETTER =================
    void setso(const string &s) {
        so = s;
    }
    // Tra ve false neu ngay khong hop le
    bool setngaymua(int n, int t, int nm) {
        return A.setDate(n, t, nm);
    }

    void input() {
        cout << "Nhap so BHYT: ";
        cin >> so;

        cout << "Nhap ngay mua BHYT: ";
        A.nhap();
    }

    void display() {
        cout << "So BHYT: " << so << endl;
        cout << "Ngay mua BHYT: ";
        A.xuat();
    }

    void readFile(ifstream &file) {
        getline(file, so);
        A.readFile(file);
    }

    void writeFile(ofstream &file) {
        file << so << endl;
        A.writeFile(file);
    }
};

#endif
