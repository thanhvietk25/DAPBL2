#ifndef MANAGER_DOCTOR_H
#define MANAGER_DOCTOR_H
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "Doctor.h"
#include "Yta.h"
#include "Truongkhoa.h"
#include "Display.h"
#include "Input_util.h"
using namespace std;

const int RONG_BANG_NS = 80;

class Manager_Doctor
{
private:
    // luu theo con tro lop cha => 1 danh sach chua duoc ca 3 chuc vu
    vector<unique_ptr<NhanVien>> doctors;

    string toLower(string str)
    {
        transform(str.begin(), str.end(), str.begin(),
                  [](unsigned char c)
                  {
                      return static_cast<char>(tolower(c));
                  });
        return str;
    }

    // Hoi chuc vu va tao doi tuong rong tuong ung
    unique_ptr<NhanVien> taoTheoChucVu()
    {
        cout << "1. Bac si\n";
        cout << "2. Y ta\n";
        cout << "3. Truong khoa\n";

        int c;
        while (true)
        {
            if (!docSoNguyen("Chon chuc vu: ", c))
                return nullptr;
            if (c >= 1 && c <= 3)
                break;
            cout << "Vui long chon 1, 2 hoac 3!\n";
        }

        if (c == 1)
            return unique_ptr<NhanVien>(new Doctor);
        if (c == 2)
            return unique_ptr<NhanVien>(new YTa);
        return unique_ptr<NhanVien>(new TruongKhoa);
    }

    // Khoa nay da co truong khoa chua? (boQua: bo qua chinh nguoi dang xet)
    bool coTruongKhoa(const string &khoa, const NhanVien *boQua)
    {
        string k = toLower(khoa);
        for (size_t i = 0; i < doctors.size(); i++)
            if (doctors[i].get() != boQua && doctors[i]->duocQuanLyKhoa() &&
                toLower(doctors[i]->getkhoa()) == k)
                return true;
        return false;
    }

    // tag rong = in tat ca; STT luon la STT that trong danh sach
    void inBang(const string &tag, const string &tieuDe)
    {
        printgiua(tieuDe, "=", RONG_BANG_NS);
        cout << left
             << setw(5) << "STT"
             << setw(22) << "Ho ten"
             << setw(14) << "CCCD"
             << setw(13) << "Chuc vu"
             << setw(14) << "Khoa"
             << "Ghi chu" << endl;
        printline("=", RONG_BANG_NS);

        bool coDong = false;
        for (size_t i = 0; i < doctors.size(); i++)
        {
            if (!tag.empty() && doctors[i]->getTag() != tag)
                continue;

            cout << left
                 << setw(5) << i + 1
                 << setw(22) << doctors[i]->getname()
                 << setw(14) << doctors[i]->getcccd()
                 << setw(13) << doctors[i]->getChucVu()
                 << setw(14) << doctors[i]->getkhoa()
                 << doctors[i]->getGhiChu() << endl;
            printline("-", RONG_BANG_NS);
            coDong = true;
        }

        if (!coDong)
            cout << "(Khong co nhan su nao)\n";
        printline("=", RONG_BANG_NS);
    }

public:
    NhanVien *findByCCCD(const string &cccd)
    {
        for (size_t i = 0; i < doctors.size(); i++)
            if (doctors[i]->getcccd() == cccd)
                return doctors[i].get();
        return nullptr;
    }

    void display()
    {
        inBang("", " DANH SACH NHAN SU Y TE ");
    }

    void displayTheoChucVu()
    {
        cout << "1. Bac si\n2. Y ta\n3. Truong khoa\n";
        int c;
        while (true)
        {
            if (!docSoNguyen("Chon chuc vu can xem: ", c))
                return;
            if (c >= 1 && c <= 3)
                break;
            cout << "Vui long chon 1, 2 hoac 3!\n";
        }

        if (c == 1)
            inBang("[BACSI]", " DANH SACH BAC SI ");
        else if (c == 2)
            inBang("[YTA]", " DANH SACH Y TA ");
        else
            inBang("[TRUONGKHOA]", " DANH SACH TRUONG KHOA ");
    }

    // Tra ve true neu them thanh cong
    bool input()
    {
        unique_ptr<NhanVien> nv = taoTheoChucVu();
        if (!nv)
            return false;

        nv->input();

        if (findByCCCD(nv->getcccd()))
        {
            cout << "-> Loi: CCCD " << nv->getcccd()
                 << " da ton tai trong he thong!\n";
            return false;
        }
        if (nv->duocQuanLyKhoa() && coTruongKhoa(nv->getkhoa(), nullptr))
        {
            cout << "-> Loi: Khoa " << nv->getkhoa()
                 << " da co truong khoa!\n";
            return false;
        }

        doctors.push_back(std::move(nv));
        return true;
    }

    void readFile(ifstream &file)
    {
        doctors.clear();

        while (file.peek() != EOF)
        {
            streampos vitri = file.tellg();
            string dong;
            getline(file, dong);
            if (!dong.empty() && dong[dong.size() - 1] == '\r')
                dong.erase(dong.size() - 1);

            if (dong.empty())
                continue; // dong trong ngan cach

            unique_ptr<NhanVien> nv;
            if (dong == "[YTA]")
                nv.reset(new YTa);
            else if (dong == "[TRUONGKHOA]")
                nv.reset(new TruongKhoa);
            else if (dong == "[BACSI]")
                nv.reset(new Doctor);
            else
            {
                file.seekg(vitri); // file cu: dong nay la ten bac si
                nv.reset(new Doctor);
            }

            nv->readFile(file);
            doctors.push_back(std::move(nv));
        }
    }

    void delete_cccddoctor(string cccd_delete, string file)
    {
        for (size_t i = 0; i < doctors.size(); i++)
        {
            if (doctors[i]->getcccd() == cccd_delete)
            {
                doctors.erase(doctors.begin() + i);
                cout << "Da xoa thanh cong nhan su co cccd: " << cccd_delete << endl;
                saveToFile(file);
                return;
            }
        }
        cout << "Khong tim thay nhan su co cccd:" << cccd_delete << endl;
    }

    void saveToFile(string &filePath)
    {
        ofstream file(filePath);

        if (!file.is_open())
        {
            cout << "Loi khi mo file de ghi!\n";
            return;
        }

        writeFile(file);

        if (!file)
        {
            cout << "Loi khi ghi du lieu vao file!\n";
            return;
        }

        file.close();

        cout << "-> Da dong bo du lieu vao file "
             << filePath << " thanh cong!\n";
    }

    void change_doctor(string &filePath)
    {
        if (doctors.empty())
        {
            cout << "Danh sach nhan su rong!\n";
            return;
        }

        display();
        int i;
        while (true)
        {
            if (!docSoNguyen("Chon stt de thay doi thong tin: ", i))
                return;
            if (i >= 1 && i <= (int)doctors.size())
                break;
            cout << "Vui long nhap stt phu hop!!!\n";
        }

        NhanVien *nv = doctors[i - 1].get();
        unique_ptr<NhanVien> backup(nv->clone()); // de hoan tac neu sai

        nv->change_in4();

        for (size_t j = 0; j < doctors.size(); j++)
        {
            if (j != (size_t)(i - 1) && doctors[j]->getcccd() == nv->getcccd())
            {
                cout << "-> Loi: CCCD moi bi trung voi nhan su khac, huy thay doi!\n";
                doctors[i - 1] = std::move(backup);
                return;
            }
        }
        if (nv->duocQuanLyKhoa() && coTruongKhoa(nv->getkhoa(), nv))
        {
            cout << "-> Loi: Khoa " << nv->getkhoa()
                 << " da co truong khoa khac, huy thay doi!\n";
            doctors[i - 1] = std::move(backup);
            return;
        }

        saveToFile(filePath);
    }

    // Doi chuc vu: Bac si <-> Y ta <-> Truong khoa (vd: thang chuc / ha chuc)
    void doi_chucvu(string &filePath)
    {
        if (doctors.empty())
        {
            cout << "Danh sach nhan su rong!\n";
            return;
        }

        display();
        int stt;
        while (true)
        {
            if (!docSoNguyen("Chon stt nhan su can doi chuc vu: ", stt))
                return;
            if (stt >= 1 && stt <= (int)doctors.size())
                break;
            cout << "Vui long nhap stt phu hop!!!\n";
        }

        NhanVien *cu = doctors[stt - 1].get();
        cout << "Chuc vu hien tai cua " << cu->getname() << ": " << cu->getChucVu() << endl;
        cout << "Chon chuc vu MOI:\n";

        unique_ptr<NhanVien> moi = taoTheoChucVu();
        if (!moi)
            return;

        if (moi->getTag() == cu->getTag())
        {
            cout << "Chuc vu khong thay doi!\n";
            return;
        }
        if (moi->duocQuanLyKhoa() && coTruongKhoa(cu->getkhoa(), cu))
        {
            cout << "-> Loi: Khoa " << cu->getkhoa()
                 << " da co truong khoa! Hay ha chuc truong khoa hien tai truoc.\n";
            return;
        }

        moi->saoChepChung(*cu);

        // Giua cac chuc vu cung la bac si thi giu nguyen so ca; con lai hoi du lieu rieng
        Doctor *dcu = dynamic_cast<Doctor *>(cu);
        Doctor *dmoi = dynamic_cast<Doctor *>(moi.get());
        if (dcu && dmoi)
            dmoi->setSoca(dcu->getSoca());
        else
            moi->nhapThongTinRieng();

        cout << "-> Da doi " << cu->getname() << " tu " << cu->getChucVu()
             << " thanh " << moi->getChucVu() << endl;

        doctors[stt - 1] = std::move(moi);
        saveToFile(filePath);
    }

    void search_namedoctor()
    {
        string namesearch;

        cout << "Nhap ten nhan su muon tim kiem: ";

        if (!getline(cin, namesearch))
            return;
        bool found = false;
        string key = toLower(namesearch);

        for (size_t i = 0; i < doctors.size(); i++)
        {
            string currentName = toLower(doctors[i]->getname());

            if (currentName.find(key) != string::npos)
            {
                printgiua(" Nhan su " + to_string(i + 1) + " ", "=");

                doctors[i]->display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "Khong tim thay nhan su co ten: "
                 << namesearch << endl;
        }
    }

    void search_chuyennganhdoctor()
    {
        string chuyennganh;

        cout << "Nhap khoa / chuyen nganh muon tim: ";
        if (!getline(cin, chuyennganh))
            return;

        string key = toLower(chuyennganh);
        bool found = false;

        for (size_t i = 0; i < doctors.size(); i++)
        {
            string current = toLower(doctors[i]->getkhoa());

            if (current.find(key) != string::npos)
            {
                printgiua(" Nhan su " + to_string(i + 1) + " ", "=");
                doctors[i]->display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "Khong tim thay nhan su thuoc khoa / chuyen nganh: "
                 << chuyennganh << endl;
        }
    }

    // Moi ban ghi: dong tag + du lieu; cac ban ghi cach nhau 1 dong trong
    void writeFile(ofstream &file)
    {
        for (size_t i = 0; i < doctors.size(); i++)
        {
            file << doctors[i]->getTag() << "\n";
            doctors[i]->writeFile(file);

            if (i + 1 < doctors.size())
                file << "\n";
        }
    }
};
#endif