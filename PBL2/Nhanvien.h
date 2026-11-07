#ifndef NHANVIEN_H
#define NHANVIEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "Person.h"
#include "Input_util.h"

using namespace std;

class NhanVien : public Person
{
protected:
    string id;
    string khoa; // khoa

public:
    virtual ~NhanVien() {}

    // ---------- Moi chuc vu BAT BUOC phai tu khai bao ----------
    virtual string getTag() const = 0;    // dong dau cua ban ghi trong file, vd "[YTA]"
    virtual string getChucVu() const = 0; // ten hien thi
    virtual bool duocKeDon() const = 0;   // quyen ke don thuoc
    virtual string getGhiChu() = 0;       // cot "Ghi chu" trong bang danh sach
    virtual NhanVien *clone() const = 0;  // nhan ban (dung de hoan tac khi sua)

    // ---------- Quyen mac dinh la khong; chuc vu nao co thi ghi de ----------
    virtual bool duocQuanLyKhoa() const { return false; }

    // Du lieu rieng cua chuc vu (dung khi doi chuc vu). Mac dinh khong co.
    virtual void nhapThongTinRieng() {}

    string getid() { return id; }
    string getkhoa() { return khoa; }
    string getchuyennganh() { return khoa; } // giu ten cu cho code cu

    // Chep phan chung (Person + id + khoa) tu nhan vien khac - dung khi doi chuc vu
    void saoChepChung(const NhanVien &o)
    {
        Person::operator=(o);
        id = o.id;
        khoa = o.khoa;
    }

    virtual void input()
    {
        Person::input();

        cout << "Nhap ID: ";
        cin >> id;
        cin.ignore();

        cout << "Nhap khoa / chuyen nganh: ";
        getline(cin, khoa);
    }

    virtual void display()
    {
        Person::display();
        cout << "Chuc vu: " << getChucVu() << endl;
        cout << "ID: " << id << endl;
        cout << "Khoa: " << khoa << endl;
    }

    virtual void change_in4()
    {
        string s;
        int a;

        if (!docDong("Nhap ho ten moi: ", s))
            return;
        Person::setname(s);

        if (!docDong("Nhap CCCD moi: ", s))
            return;
        Person::setcccd(s);

        if (!docDong("Nhap so dien thoai moi: ", s))
            return;
        Person::setnumber(s);

        while (true)
        {
            if (!docSoNguyen("Nhap gioi tinh moi (1 = Nu / 0 = Nam): ", a))
                return;
            if (a == 0 || a == 1)
                break;
        }
        Person::setgender(a == 1);

        while (true)
        {
            if (!docSoNguyen("Nhap tuoi moi: ", a))
                return;
            if (Person::setage(a))
                break;
            cout << "Tuoi khong hop le!\n";
        }

        if (!docDong("Nhap khoa / chuyen nganh moi: ", khoa))
            return;
        if (!docDong("Nhap ID moi: ", id))
            return;
    }

    virtual void readFile(ifstream &file)
    {
        Person::readFile(file);
        getline(file, khoa);
        getline(file, id);
    }

    virtual void writeFile(ofstream &file)
    {
        Person::writeFile(file);
        file << khoa << endl;
        file << id << endl;
    }
};

#endif