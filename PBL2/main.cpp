#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <memory>
#include <cstring>
#include <cstdlib>

#include "xac_thuc.h"
#include "Display.h"
#include "Input_util.h"
#include "Manager_doctor.h"
#include "Manager_patient.h"
#include "Manager_Medicine.h"
#include "Manager_DonThuoc.h"

using namespace std;
string FILE_PATIENT = "1.patient.txt";
string FILE_DOCTOR = "2.Danhsachbacsi.txt";
string FILE_THUOC = "3.Thuoc.txt";
string FILE_DON = "4.Donthuoc.txt";

int docLuaChon(const string &thongbao)
{
    int x;
    if (!docSoNguyen(thongbao, x))
        return 0;
    return x;
}

void tamDung()
{
    cout << "\nNhan Enter de tiep tuc...";
    string s;
    getline(cin, s);
}


// Phan quyen: moi menu chi hien va cho phep cac muc nam trong chuoi "cho_phep" (vd "1348")
static inline bool duocPhep(int chon, const char *cho_phep)
{
    return chon >= 0 && chon <= 9 && strchr(cho_phep, '0' + chon) != nullptr;
}

static inline void inMuc(int so, const char *cho_phep, const string &noidung)
{
    if (duocPhep(so, cho_phep))
        cout << so << ". " << noidung << "\n";
}

// Xoa man hinh console (de an ten dang nhap / mat khau vua nhap)
static inline void xoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static inline const char *tenVaiTro(int vt)
{
    return vt == ADMIN ? "Quan tri vien" : vt == BAC_SI ? "Bac si" : "Nguoi dung";
}

// ====================== MENU BAC SI ======================
void menuBacSi(Manager_Doctor &ds, int vaiTro)
{
    const char *cp = (vaiTro == ADMIN) ? "012345678" : "01348";
    int chon;
    do
    {
        printgiua(" QUAN LY NHAN SU Y TE ", "=");
        inMuc(1, cp, "Xem danh sach nhan su");
        inMuc(2, cp, "Them nhan su moi (bac si / y ta / truong khoa)");
        inMuc(3, cp, "Tim nhan su theo ten");
        inMuc(4, cp, "Tim nhan su theo khoa / chuyen nganh");
        inMuc(5, cp, "Sua thong tin nhan su");
        inMuc(6, cp, "Xoa nhan su theo CCCD");
        inMuc(7, cp, "Doi chuc vu (bac si / y ta / truong khoa)");
        inMuc(8, cp, "Xem danh sach theo chuc vu");
        cout << "0. Quay lai\n";
        printline("=");
        chon = docLuaChon("Lua chon cua ban: ");

        if (chon != 0 && !duocPhep(chon, cp))
        {
            cout << (chon >= 1 && chon <= 8 ? "Ban khong co quyen su dung chuc nang nay!\n"
                                            : "Lua chon khong hop le!\n");
            tamDung();
            continue;
        }

        switch (chon)
        {
        case 1:
            ds.display();
            break;
        case 2:
            if (ds.input())
                ds.saveToFile(FILE_DOCTOR);
            cin.ignore(1000, '\n'); 
            break;
        case 3:
            ds.search_namedoctor();
            break;
        case 4:
            ds.search_chuyennganhdoctor();
            break;
        case 5:
            ds.change_doctor(FILE_DOCTOR);
            break;
        case 6:
        {
            cout << "Nhap CCCD bac si can xoa: ";
            string cccd;
            getline(cin, cccd);
            ds.delete_cccddoctor(cccd, FILE_DOCTOR);
            break;
        }
        case 7:
            ds.doi_chucvu(FILE_DOCTOR);
            break;
        case 8:
            ds.displayTheoChucVu();
            break;
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
        if (chon != 0)
            tamDung();
    } while (chon != 0);
}

// ====================== MENU BENH NHAN ======================
void menuBenhNhan(manager_patient &ds, int vaiTro)
{
    const char *cp = (vaiTro == ADMIN) ? "0123456" : (vaiTro == BAC_SI) ? "012345" : "0134";
    int chon;
    do
    {
        printgiua(" QUAN LY BENH NHAN ", "=");
        inMuc(1, cp, "Xem danh sach benh nhan");
        inMuc(2, cp, "Them benh nhan moi");
        inMuc(3, cp, "Tim benh nhan theo ten");
        inMuc(4, cp, "Tim benh nhan theo CCCD");
        inMuc(5, cp, "Sua thong tin benh nhan");
        inMuc(6, cp, "Xoa benh nhan");
        cout << "0. Quay lai\n";
        printline("=");
        chon = docLuaChon("Lua chon cua ban: ");

        if (chon != 0 && !duocPhep(chon, cp))
        {
            cout << (chon >= 1 && chon <= 6 ? "Ban khong co quyen su dung chuc nang nay!\n"
                                            : "Lua chon khong hop le!\n");
            tamDung();
            continue;
        }

        switch (chon)
        {
        case 1:
            ds.display();
            break;
        case 2:
            ds.addPatient(FILE_PATIENT);
            cin.ignore(1000, '\n'); 
            break;
        case 3:
            ds.search_name();
            break;
        case 4:
            ds.search_cccd();
            break;
        case 5:
            ds.editPatient(FILE_PATIENT);
            break;
        case 6:
            ds.deletePatient(FILE_PATIENT);
            break;
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
        if (chon != 0)
            tamDung();
    } while (chon != 0);
}

void menuThuoc(Manager_Thuoc &kho, int vaiTro)
{
    const char *cp = (vaiTro == ADMIN) ? "0123456" : (vaiTro == BAC_SI) ? "01368" : "013";
    int chon;
    do
    {
        printgiua(" QUAN LY THUOC ", "=");
        inMuc(1, cp, "Xem danh sach thuoc");
        inMuc(2, cp, "Them thuoc moi");
        inMuc(3, cp, "Tim thuoc theo ten / ma");
        inMuc(4, cp, "Cap nhat don gia / so luong ton");
        inMuc(5, cp, "Xoa thuoc");
        inMuc(6, cp, "Thuoc sap het hang");
        cout << "0. Quay lai\n";
        printline("=");
        chon = docLuaChon("Lua chon cua ban: ");

        if (chon != 0 && !duocPhep(chon, cp))
        {
            cout << (chon >= 1 && chon <= 6 ? "Ban khong co quyen su dung chuc nang nay!\n"
                                            : "Lua chon khong hop le!\n");
            tamDung();
            continue;
        }

        switch (chon)
        {
        case 1:
            kho.display();
            break;
        case 2:
            kho.addThuoc(FILE_THUOC);
            break;
        case 3:
            kho.searchThuoc();
            break;
        case 4:
            kho.updateMedicine(FILE_THUOC);
            break;
        case 5:
            kho.deleteThuoc(FILE_THUOC);
            break;
        case 6:
        {
            int a = 0;
            if (!docSoNguyen("Nhap so luong ban muon tim thuoc co so luong be hon: ", a))
                break;
            kho.display(a, false);
            break;
        }
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
        if (chon != 0)
            tamDung();
    } while (chon != 0);
}

void menuDonThuoc(Manager_DonThuoc &don, Manager_Doctor &bacsi, manager_patient &benhnhan, Manager_Thuoc &kho, int vaiTro)
{
    const char *cp = (vaiTro == USER) ? "0234" : "01234";
    int chon;
    do
    {
        printgiua(" KE DON THUOC ", "=");
        inMuc(1, cp, "Ke don thuoc moi");
        inMuc(2, cp, "Xem tat ca don thuoc da ke");
        inMuc(3, cp, "Xem don thuoc theo CCCD benh nhan");
        inMuc(4, cp, "Xem don thuoc theo CCCD bac si");
        cout << "0. Quay lai\n";
        printline("=");
        chon = docLuaChon("Lua chon cua ban: ");

        if (chon != 0 && !duocPhep(chon, cp))
        {
            cout << (chon >= 1 && chon <= 4 ? "Ban khong co quyen su dung chuc nang nay!\n"
                                            : "Lua chon khong hop le!\n");
            tamDung();
            continue;
        }

        switch (chon)
        {
        case 1:
            don.keDon(bacsi, benhnhan, kho, FILE_DON, FILE_THUOC);
            break;
        case 2:
            don.displayAll();
            break;
        case 3:
            don.displayByPatient();
            break;
        case 4:
            don.displayByDoctor();
            break;
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
        if (chon != 0)
            tamDung();
    } while (chon != 0);
}

// Chay mot phien lam viec cho tai khoan da dang nhap (tra ve khi dang xuat)
void phienLamViec(int vaiTro, Manager_Doctor &doctors, manager_patient &patients,
                  Manager_Thuoc &kho, Manager_DonThuoc &donthuoc)
{
    int chon;
    do
    {
        printtb("QUAN LY BENH VIEN - PBL2", "#");
        cout << "  Vai tro: " << tenVaiTro(vaiTro) << "\n";
        cout << "  1. Quan ly nhan su y te\n";
        cout << "  2. Quan ly benh nhan\n";
        cout << "  3. Quan ly thuoc\n";
        cout << "  4. Ke don thuoc\n";
        cout << "  0. Dang xuat\n";
        printline("#");
        chon = docLuaChon("Lua chon cua ban: ");

        switch (chon)
        {
        case 1:
            menuBacSi(doctors, vaiTro);
            break;
        case 2:
            menuBenhNhan(patients, vaiTro);
            break;
        case 3:
            menuThuoc(kho, vaiTro);
            break;
        case 4:
            menuDonThuoc(donthuoc, doctors, patients, kho, vaiTro);
            break;
        case 0:
            cout << "Da dang xuat.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (chon != 0);
}

int main()
{
    Manager_Doctor doctors;
    manager_patient patients;
    Manager_Thuoc kho;
    Manager_DonThuoc donthuoc;

    ifstream fd(FILE_DOCTOR);
    doctors.readFile(fd);
    fd.close();

    ifstream fp(FILE_PATIENT);
    patients.readFile(fp);
    fp.close();

    ifstream fthuoc(FILE_THUOC);
    kho.readFile(fthuoc);
    fthuoc.close();

    ifstream fdon(FILE_DON);
    donthuoc.readFile(fdon);
    fdon.close();

    unique_ptr<DangNhap> dn;
    try
    {
        dn.reset(new DangNhap("tai_khoan.txt"));
    }
    catch (const exception &e)
    {
        cout << "Loi tai khoan: " << e.what() << "\n";
        return 1;
    }

    string chon;
    while (true)
    {
        printtb("HE THONG QUAN LY BENH VIEN", "#");
        cout << "  1. Dang nhap\n";
        cout << "  2. Dang ky\n";
        cout << "  0. Thoat chuong trinh\n";
        printline("#");

        if (!xt::hoi("Lua chon cua ban: ", chon))
            break;
        chon = xt::catTrang(chon);

        if (chon == "0")
        {
            cout << "\t\tCam on da ghe tham, Xin chao tam biet!\n";
            break;
        }
        else if (chon == "1")
        {
            string tdn, mk;
            if (!xt::hoi("Ten dang nhap: ", tdn) || !xt::hoiMatKhau("Mat khau: ", mk))
                break;

            int vaiTro = dn->dangNhap(tdn, mk);
            if (vaiTro == KHONG_TON_TAI)
                continue;

            xoaManHinh();
            cout << "\n\tDang nhap thanh cong!\n";
            cout << "\tNhan Enter de vao man hinh chinh!";
            string tam;
            getline(cin, tam);
            xoaManHinh();
            phienLamViec(vaiTro, doctors, patients, kho, donthuoc);
            xoaManHinh();
        }
        else if (chon == "2")
        {
            dn->dangKy();
        }
        else
        {
            cout << "Lua chon khong hop le!\n";
        }
    }

    return 0;
}
