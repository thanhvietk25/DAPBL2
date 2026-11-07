#ifndef YTA_H
#define YTA_H

#include "Nhanvien.h"

using namespace std;

// Y TA: KHONG duoc ke don. Co them ca truc.
class YTa : public NhanVien
{
private:
    string catruc; // Sang / Chieu / Dem

public:
    string getTag() const override { return "[YTA]"; }
    string getChucVu() const override { return "Y ta"; }
    bool duocKeDon() const override { return false; }
    string getGhiChu() override { return "Ca: " + catruc; }
    NhanVien *clone() const override { return new YTa(*this); }

    void input() override
    {
        NhanVien::input();

        cout << "Nhap ca truc (Sang/Chieu/Dem): ";
        cin >> catruc;
    }

    void nhapThongTinRieng() override
    {
        docDong("Nhap ca truc (Sang/Chieu/Dem): ", catruc);
    }

    void change_in4() override
    {
        NhanVien::change_in4();
        docDong("Nhap ca truc moi (Sang/Chieu/Dem): ", catruc);
    }

    void display() override
    {
        NhanVien::display();
        cout << "Ca truc: " << catruc << endl;
    }

    void readFile(ifstream &file) override
    {
        NhanVien::readFile(file);
        getline(file, catruc);
    }

    void writeFile(ofstream &file) override
    {
        NhanVien::writeFile(file);
        file << catruc << endl;
    }
};

#endif