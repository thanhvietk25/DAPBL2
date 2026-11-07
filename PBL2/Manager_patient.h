#ifndef MANAGER_PATIENT_H
#define MANAGER_PATIENT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>

#include "Patient.h"
#include "Display.h"

using namespace std;

class manager_patient
{
private:
    vector<Patient> patients;

    string toLower(string str)
    {
        transform(str.begin(), str.end(), str.begin(),
                  [](unsigned char c)
                  {
                      return static_cast<char>(tolower(c));
                  });
        return str;
    }

    bool docSo(const string &thongbao, int &x)
    {
        string dong;

        while (true)
        {
            cout << thongbao;

            if (!getline(cin, dong))
                return false;

            stringstream ss(dong);
            char kyTuThua;

            if ((ss >> x) && !(ss >> kyTuThua))
                return true;

            cout << "Vui long nhap mot so nguyen hop le!\n";
        }
    }

public:
    Patient *findByCCCD(const string &cccd)
    {
        for (size_t i = 0; i < patients.size(); i++)
            if (patients[i].getcccd() == cccd)
                return &patients[i];
        return nullptr;
    }

    Patient *getPatientByCCCD(const string &cccd)
    {
        for (size_t i = 0; i < patients.size(); i++)
        {
            if (patients[i].getcccd() == cccd)
            {
                return &patients[i];
            }
        }

        return nullptr;
    }

    void display()
    {
        if (patients.empty())
        {
            cout << "Danh sach benh nhan rong!\n";
            return;
        }

        for (size_t i = 0; i < patients.size(); i++)
        {
            printgiua(" BENH NHAN " + to_string(i + 1) + " ", "=");

            patients[i].display();
        }
    }

    void addPatient(const string &filePath)
    {
        printgiua(" NHAP THONG TIN BENH NHAN MOI ", "-");

        Patient p;
        p.input();

        for (Patient &item : patients)
        {
            if (item.getcccd() == p.getcccd())
            {
                cout << "-> Loi: CCCD " << p.getcccd()
                     << " da ton tai trong he thong!\n";
                return;
            }
        }

        patients.push_back(p);
        saveToFile(filePath);
    }

    void readFile(ifstream &file)
    {
        patients.clear();

        if (!file.is_open())
        {
            cout << "Loi: File khong mo duoc!\n";
            return;
        }

        while (file)
        {
            file >> ws;

            if (file.peek() == EOF)
                break;

            Patient patient;
            patient.readFile(file);

            if (file.fail())
            {
                cout << "Loi khi doc du lieu benh nhan!\n";
                break;
            }

            patients.push_back(patient);

            string dongTrong;
            getline(file, dongTrong);
        }
    }

    void saveToFile(const string &filePath)
    {
        ofstream file(filePath);

        if (!file.is_open())
        {
            cout << "Loi khi mo file de ghi!\n";
            return;
        }

        for (size_t i = 0; i < patients.size(); i++)
        {
            patients[i].writeFile(file);

            if (i + 1 < patients.size())
                file << "\n";
        }

        if (!file)
        {
            cout << "Loi khi ghi du lieu vao file!\n";
            return;
        }

        file.close();

        cout << "-> Da dong bo du lieu vao file "
             << filePath << " thanh cong!\n";
    }

    void search_name()
    {
        string namesearch;

        cout << "Nhap ten benh nhan muon tim kiem: ";

        if (!getline(cin, namesearch))
            return;

        bool found = false;
        string key = toLower(namesearch);

        for (size_t i = 0; i < patients.size(); i++)
        {
            string currentName = toLower(patients[i].getname());

            if (currentName.find(key) != string::npos)
            {
                printgiua(" BENH NHAN " + to_string(i + 1) + " ", "=");

                patients[i].display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "Khong tim thay benh nhan co ten: "
                 << namesearch << endl;
        }
    }

    void search_cccd()
    {
        string cccdsearch;

        cout << "Nhap CCCD benh nhan muon tim kiem: ";

        if (!getline(cin, cccdsearch))
            return;

        for (size_t i = 0; i < patients.size(); i++)
        {
            if (patients[i].getcccd() == cccdsearch)
            {
                printgiua(" BENH NHAN " + to_string(i + 1) + " ", "=");

                patients[i].display();
                return;
            }
        }

        cout << "Hien tai khong co thong tin benh nhan co "
             << "CCCD la: " << cccdsearch << endl;
    }

    void editPatient(string &filePath)
    {
        string cccdEdit;

        cout << "Nhap CCCD cua benh nhan can sua thong tin: ";

        if (!getline(cin, cccdEdit))
            return;

        for (size_t i = 0; i < patients.size(); i++)
        {
            if (patients[i].getcccd() != cccdEdit)
                continue;

            Patient &p = patients[i];
            bool daSua = false;

            int SO_MUC = 11; // so muc thong tin co the sua

            auto suaMuc = [&](int muc)
            {
                string s;
                int a, b, c;

                switch (muc)
                {
                case 1:
                    cout << "Nhap ho ten moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setname(s);
                    daSua = true;
                    break;

                case 2:
                {
                    cout << "Nhap CCCD moi: ";
                    if (!getline(cin, s))
                        return;

                    bool trung = false;

                    for (size_t j = 0; j < patients.size(); j++)
                    {
                        if (j != i && patients[j].getcccd() == s)
                        {
                            trung = true;
                            break;
                        }
                    }

                    if (trung)
                    {
                        cout << "CCCD nay da ton tai o benh nhan khac!\n";
                    }
                    else if (s.empty())
                    {
                        cout << "CCCD khong duoc de trong!\n";
                    }
                    else
                    {
                        p.setcccd(s);
                        daSua = true;
                    }

                    break;
                }

                case 3:
                    cout << "Nhap so dien thoai moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setnumber(s);
                    daSua = true;
                    break;

                case 4:
                    if (!docSo("Nhap gioi tinh moi (1 = Nu / 0 = Nam): ", a))
                        return;

                    while (a != 0 && a != 1)
                    {
                        if (!docSo("Vui long nhap 0 hoac 1: ", a))
                            return;
                    }

                    p.setgender(a == 1);
                    daSua = true;
                    break;

                case 5:
                    if (!docSo("Nhap tuoi moi: ", a))
                        return;

                    while (!p.setage(a))
                    {
                        if (!docSo("Tuoi khong hop le! Nhap lai: ", a))
                            return;
                    }

                    daSua = true;
                    break;

                case 6:
                    cout << "Nhap so BHYT moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setso(s);
                    daSua = true;
                    break;

                case 7:
                    cout << "Nhap ngay mua BHYT moi\n";

                    do
                    {
                        if (!docSo("Nhap ngay: ", a) ||
                            !docSo("Nhap thang: ", b) ||
                            !docSo("Nhap nam: ", c))
                            return;

                        if (p.setngaymua(a, b, c))
                            break;

                        cout << "Ngay khong hop le! Nhap lai.\n";
                    } while (true);

                    daSua = true;
                    break;

                case 8:
                    cout << "Nhap bac si kham moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setbacsikham(s);
                    daSua = true;
                    break;

                case 9:
                    cout << "Nhap chuan doan moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setchuandoan(s);
                    daSua = true;
                    break;

                case 10:
                    cout << "Nhap so phong moi: ";
                    if (!getline(cin, s))
                        return;

                    p.setsophong(s);
                    daSua = true;
                    break;

                case 11:
                    cout << "Nhap ngay kham moi\n";

                    do
                    {
                        if (!docSo("Nhap ngay: ", a) ||
                            !docSo("Nhap thang: ", b) ||
                            !docSo("Nhap nam: ", c))
                            return;

                        if (p.setngaykham(a, b, c))
                            break;

                        cout << "Ngay khong hop le! Nhap lai.\n";
                    } while (true);

                    daSua = true;
                    break;
                }
            };

            cout << "-> Da tim thay benh nhan.\n";

            int chon;

            do
            {
                p.display();

                cout << "\n";
                printgiua(" CHON THONG TIN MUON SUA ", "=");
                cout << "1. Ho va ten\n";
                cout << "2. CCCD\n";
                cout << "3. So dien thoai\n";
                cout << "4. Gioi tinh\n";
                cout << "5. Tuoi\n";
                cout << "6. So BHYT\n";
                cout << "7. Ngay mua BHYT\n";
                cout << "8. Bac si kham\n";
                cout << "9. Chuan doan\n";
                cout << "10. So phong\n";
                cout << "11. Ngay kham\n";
                cout << "12. Tat ca thong tin\n";
                cout << "0. Hoan tat\n";

                if (!docSo("Lua chon cua ban: ", chon))
                    return;

                if (chon >= 1 && chon <= SO_MUC)
                {
                    suaMuc(chon);
                }
                else if (chon == SO_MUC + 1)
                {
                    for (int muc = 1; muc <= SO_MUC; muc++)
                        suaMuc(muc);
                }
                else if (chon != 0)
                {
                    cout << "Lua chon khong hop le!\n";
                }

            } while (chon != 0);

            if (daSua)
            {
                saveToFile(filePath);
                cout << "-> Cap nhat thong tin thanh cong!\n";
            }
            else
            {
                cout << "-> Khong co thong tin nao duoc thay doi.\n";
            }

            return;
        }

        cout << "Khong tim thay benh nhan co CCCD: "
             << cccdEdit << endl;
    }
    void deletePatient(const string &filePath)
    {
        string cccdXoa;

        cout << "Nhap CCCD benh nhan can xoa: ";

        if (!getline(cin, cccdXoa))
            return;

        for (size_t i = 0; i < patients.size(); i++)
        {
            if (patients[i].getcccd() != cccdXoa)
                continue;

            printgiua(" BENH NHAN " + to_string(i + 1) + " ", "=");
            patients[i].display();

            string xacNhan;
            cout << "Ban co chac chan muon xoa benh nhan nay? (y/n): ";

            if (!getline(cin, xacNhan))
                return;

            if (xacNhan != "y" && xacNhan != "Y")
            {
                cout << "-> Da huy xoa.\n";
                return;
            }

            patients.erase(patients.begin() + i);
            saveToFile(filePath);

            cout << "-> Da xoa benh nhan co CCCD: " << cccdXoa << endl;
            return;
        }

        cout << "Khong tim thay benh nhan co CCCD: " << cccdXoa << endl;
    }
};

#endif