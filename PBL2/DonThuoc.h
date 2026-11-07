#ifndef DONTHUOC_H
#define DONTHUOC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "Date.h"
#include "Input_util.h"
#include "Display.h"

using namespace std;

struct ChiTietDon
{
    string mathuoc;
    string tenthuoc;
    int soluong;
    double dongia;
    string lieudung;
};

class DonThuoc
{
private:
    string madon;
    string cccdbn, tenbn; // benh nhan
    string cccdbs, tenbs; // bac si ke don
    Date ngay;
    vector<ChiTietDon> ds;

public:
    // ================= GETTER =================
    string getmadon() const { return madon; }
    string getcccdbn() const { return cccdbn; }
    string getcccdbs() const { return cccdbs; }
    const vector<ChiTietDon> &getItems() const { return ds; }
    bool empty() const { return ds.empty(); }

    // ================= SETTER =================
    void setmadon(const string &m) { madon = m; }
    void setBenhNhan(const string &cccd, const string &ten)
    {
        cccdbn = cccd;
        tenbn = ten;
    }
    void setBacSi(const string &cccd, const string &ten)
    {
        cccdbs = cccd;
        tenbs = ten;
    }
    void setngay(const Date &d) { ngay = d; }

    void them(const ChiTietDon &c) { ds.push_back(c); }

    bool coThuoc(const string &ma) const
    {
        for (size_t i = 0; i < ds.size(); i++)
            if (ds[i].mathuoc == ma)
                return true;
        return false;
    }

    double tongTien() const
    {
        double tong = 0;
        for (size_t i = 0; i < ds.size(); i++)
            tong += ds[i].dongia * ds[i].soluong;
        return tong;
    }

    void display()
    {
        printgiua(" DON THUOC " + madon + " ", "=", 65);
        cout << "Ngay ke   : ";
        ngay.xuat();
        cout << endl;
        cout << "Benh nhan : " << tenbn << " (CCCD: " << cccdbn << ")\n";
        cout << "Bac si ke : " << tenbs << " (CCCD: " << cccdbs << ")\n";
        printline("-", 65);
        cout << left
             << setw(5) << "STT"
             << setw(24) << "Ten thuoc"
             << setw(6) << "SL"
             << setw(12) << "Don gia"
             << setw(14) << "Thanh tien"
             << "Lieu dung" << endl;
        printline("-", 65);

        for (size_t i = 0; i < ds.size(); i++)
        {
            cout << left
                 << setw(5) << i + 1
                 << setw(24) << ds[i].tenthuoc
                 << setw(6) << ds[i].soluong
                 << setw(12) << dinhDangTien(ds[i].dongia)
                 << setw(14) << dinhDangTien(ds[i].dongia * ds[i].soluong)
                 << ds[i].lieudung << endl;
        }

        printline("-", 65);
        cout << "TONG TIEN: " << dinhDangTien(tongTien()) << " VND\n";
        printline("=", 65);
    }

    // Co the nem ngoai le (stoi/stod) neu file sai dinh dang -> Manager bat
    void readFile(ifstream &file)
    {
        getline(file, madon);
        getline(file, cccdbn);
        getline(file, tenbn);
        getline(file, cccdbs);
        getline(file, tenbs);
        ngay.readFile(file);

        string s;
        getline(file, s);
        int n = stoi(s);

        ds.clear();
        for (int i = 0; i < n; i++)
        {
            ChiTietDon c;
            getline(file, c.mathuoc);
            getline(file, c.tenthuoc);
            getline(file, s);
            c.soluong = stoi(s);
            getline(file, s);
            c.dongia = stod(s);
            getline(file, c.lieudung);
            ds.push_back(c);
        }
    }

    void writeFile(ofstream &file)
    {
        file << madon << "\n"
             << cccdbn << "\n"
             << tenbn << "\n"
             << cccdbs << "\n"
             << tenbs << "\n";
        ngay.writeFile(file);
        file << ds.size() << "\n";

        ios_base::fmtflags f = file.flags();
        streamsize p = file.precision();
        file << fixed << setprecision(2);

        for (size_t i = 0; i < ds.size(); i++)
        {
            file << ds[i].mathuoc << "\n"
                 << ds[i].tenthuoc << "\n"
                 << ds[i].soluong << "\n"
                 << ds[i].dongia << "\n"
                 << ds[i].lieudung << "\n";
        }

        file.flags(f);
        file.precision(p);
    }
};

#endif