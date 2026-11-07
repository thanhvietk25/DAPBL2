#ifndef MANAGER_DONTHUOC_H
#define MANAGER_DONTHUOC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "DonThuoc.h"
#include "Manager_doctor.h"
#include "Manager_patient.h"
#include "Manager_Medicine.h"
#include "Input_util.h"
#include "Display.h"

using namespace std;

class Manager_DonThuoc
{
private:
    vector<DonThuoc> ds;

    string taoMaDon()
    {
        int mx = 0;
        for (size_t i = 0; i < ds.size(); i++)
        {
            string m = ds[i].getmadon();
            if (m.size() > 2)
            {
                try
                {
                    mx = max(mx, stoi(m.substr(2)));
                }
                catch (...)
                {
                }
            }
        }
        ostringstream os;
        os << "DT" << setfill('0') << setw(4) << mx + 1;
        return os.str();
    }

public:
    void keDon(Manager_Doctor &bacsi, manager_patient &benhnhan, Manager_Thuoc &kho,
               const string &fileDon, const string &fileThuoc)
    {
        printgiua(" KE DON THUOC ", "-");

        string cccdbs, cccdbn;

        if (!docDong("Nhap CCCD bac si ke don: ", cccdbs))
            return;
        NhanVien *bs = bacsi.findByCCCD(cccdbs);
        if (!bs)
        {
            cout << "Khong tim thay nhan su co CCCD: " << cccdbs << endl;
            return;
        }
        if (!bs->duocKeDon())
        {
            cout << "-> " << bs->getChucVu() << " " << bs->getname()
                 << " KHONG co quyen ke don thuoc!\n";
            return;
        }
        cout << "-> " << bs->getChucVu() << ": " << bs->getname()
             << " - Khoa: " << bs->getkhoa() << endl;

        if (!docDong("Nhap CCCD benh nhan: ", cccdbn))
            return;
        Patient *bn = benhnhan.findByCCCD(cccdbn);
        if (!bn)
        {
            cout << "Khong tim thay benh nhan co CCCD: " << cccdbn << endl;
            return;
        }
        cout << "-> Benh nhan: " << bn->getname()
             << " - Chuan doan: " << bn->getchuandoan() << endl;

        Date ngay;
        if (!docNgay("Nhap ngay ke don (dd/mm/yyyy, Enter = hom nay): ", ngay, true))
            return;

        DonThuoc don;
        don.setmadon(taoMaDon());
        don.setBacSi(cccdbs, bs->getname());
        don.setBenhNhan(cccdbn, bn->getname());
        don.setngay(ngay);

        cout << "\nNhap tung thuoc vao don (de trong ma thuoc de ket thuc).\n";

        while (true)
        {
            string ma;
            if (!docDong("\nMa thuoc: ", ma))
                return;
            if (ma.empty())
                break;

            Thuoc *t = kho.find(ma);
            if (!t)
            {
                cout << "Khong tim thay thuoc co ma: " << ma << endl;
                continue;
            }
            if (don.coThuoc(ma))
            {
                cout << "Thuoc nay da co trong don!\n";
                continue;
            }

            cout << "   " << t->gettenthuoc() << " (" << t->getdonvitinh()
                 << ") - Don gia: " << dinhDangTien(t->getdongia())
                 << " - Ton kho: " << t->getsoluong() << endl;

            if (t->getsoluong() <= 0)
            {
                cout << "Thuoc nay da het hang!\n";
                continue;
            }

            int sl;
            while (true)
            {
                if (!docSoNguyen("So luong: ", sl))
                    return;
                if (sl >= 1 && sl <= t->getsoluong())
                    break;
                if (sl < 1)
                    cout << "So luong phai lon hon 0!\n";
                else
                    cout << "Khong du hang! Ton kho chi con " << t->getsoluong() << endl;
            }

            string lieu;
            if (!docDong("Lieu dung (vd: Ngay 2 lan, moi lan 1 vien): ", lieu))
                return;
            if (lieu.empty())
                lieu = "Theo chi dinh cua bac si";

            ChiTietDon c;
            c.mathuoc = t->getmathuoc();
            c.tenthuoc = t->gettenthuoc();
            c.soluong = sl;
            c.dongia = t->getdongia();
            c.lieudung = lieu;
            don.them(c);

            cout << "-> Da them: " << c.tenthuoc << " x" << sl << endl;
        }

        if (don.empty())
        {
            cout << "Don thuoc trong -> da huy.\n";
            return;
        }

        don.display();

        string xacNhan;
        if (!docDong("Xac nhan ke don nay? (y/n): ", xacNhan))
            return;

        if (xacNhan != "y" && xacNhan != "Y")
        {
            cout << "Da huy don thuoc. Kho thuoc khong thay doi.\n";
            return;
        }

        // Tru kho
        const vector<ChiTietDon> &items = don.getItems();
        for (size_t i = 0; i < items.size(); i++)
        {
            Thuoc *t = kho.find(items[i].mathuoc);
            if (t)
                t->setsoluong(t->getsoluong() - items[i].soluong);
        }

        ds.push_back(don);
        saveToFile(fileDon);
        kho.saveToFile(fileThuoc);
        cout << "-> Da ke don " << don.getmadon() << " thanh cong!\n";
    }

    void displayAll()
    {
        if (ds.empty())
        {
            cout << "Chua co don thuoc nao!\n";
            return;
        }
        for (size_t i = 0; i < ds.size(); i++)
            ds[i].display();
    }

    void displayByPatient()
    {
        string cccd;
        if (!docDong("Nhap CCCD benh nhan: ", cccd))
            return;

        bool found = false;
        for (size_t i = 0; i < ds.size(); i++)
        {
            if (ds[i].getcccdbn() == cccd)
            {
                ds[i].display();
                found = true;
            }
        }
        if (!found)
            cout << "Benh nhan co CCCD " << cccd << " chua co don thuoc nao!\n";
    }

    void displayByDoctor()
    {
        string cccd;
        if (!docDong("Nhap CCCD bac si: ", cccd))
            return;

        bool found = false;
        for (size_t i = 0; i < ds.size(); i++)
        {
            if (ds[i].getcccdbs() == cccd)
            {
                ds[i].display();
                found = true;
            }
        }
        if (!found)
            cout << "Bac si co CCCD " << cccd << " chua ke don nao!\n";
    }

    void readFile(ifstream &file)
    {
        ds.clear();

        if (!file.is_open())
            return; // chua co don nao -> file se duoc tao khi ke don dau tien

        try
        {
            while (file >> ws, file.peek() != EOF)
            {
                DonThuoc d;
                d.readFile(file);
                ds.push_back(d);

                string dongTrong;
                getline(file, dongTrong);
            }
        }
        catch (...)
        {
            cout << "Loi dinh dang file don thuoc! Da doc duoc " << ds.size() << " don.\n";
        }
    }

    void saveToFile(const string &filePath)
    {
        ofstream file(filePath);

        if (!file.is_open())
        {
            cout << "Loi khi mo file don thuoc de ghi!\n";
            return;
        }

        for (size_t i = 0; i < ds.size(); i++)
        {
            ds[i].writeFile(file);
            if (i + 1 < ds.size())
                file << "\n"; // dong trong ngan cach giua cac don
        }

        if (!file)
        {
            cout << "Loi khi ghi du lieu don thuoc vao file!\n";
            return;
        }

        file.close();
        cout << "-> Da dong bo du lieu vao file " << filePath << " thanh cong!\n";
    }
};

#endif