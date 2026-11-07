// xac_thuc.h - Dang nhap / dang ky / quen mat khau. Chi 1 file, chi can #include.
//
//   #include "xac_thuc.h"
//   DangNhap dn;                                  // dung file tai_khoan.txt
//   int kq = dn.dangNhap(tdn, mk);                // 1 = admin, 2 = user, 3 = bac si, 0 = khong ton tai
#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
using namespace std;
enum { KHONG_TON_TAI = 0, ADMIN = 1, USER = 2, BAC_SI = 3 };

// ---------------------------------------------------------------- ham phu
namespace xt {

inline string catTrang(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    //substr(vi_tri_bat_dau, so_luong_ky_tu)
    return s.substr(a, s.find_last_not_of(" \t\r\n") - a + 1);
}

// bo khoang trang 2 dau + chu thuong (dung cho ten dang nhap va dap an)
inline string chuanHoa(const string& s) {
    string r = catTrang(s);
    for (auto& c : r) 
    // c - A + a la de lay ky tu cua c theo chu thuong 
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return r;
}

inline void ve_bang(const vector<string>& dong) {
    size_t w = 0;

    for (const auto& d : dong)
        w = max(w, d.size());

    string vien = "+" + string(w + 4, '-') + "+";

    cout << "\n" << vien << "\n";

    for (const auto& d : dong) {
        cout << "|  " << d
                  << string(w - d.size(), ' ')
                  << "  |\n";
    }

    cout << vien << "\n";
}

inline bool hoi(const string& loiNhac, string& kq) {   // false neu het du lieu nhap
    cout << loiNhac;
    if (!getline(cin, kq)) return false;
    if (!kq.empty() && kq.back() == '\r') kq.pop_back();
    return true;
}

// Nhap mat khau: hien dau * thay cho ky tu (ho tro Backspace). false neu het du lieu nhap.
// Neu dau vao khong phai ban phim (vd. chuyen huong tu file) thi doc binh thuong.
inline bool hoiMatKhau(const string& loiNhac, string& kq) {
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) return hoi(loiNhac, kq);
    cout << loiNhac << flush;
    kq.clear();
    for (;;) {
        int c = _getch();
        if (c == '\r' || c == '\n') break;
        if (c == 0 || c == 224) { _getch(); continue; }     // phim mui ten / chuc nang
        if (c == 3) { cout << "\n"; return false; }        // Ctrl+C
        if (c == '\b' || c == 127) {
            if (!kq.empty()) { kq.erase(kq.size() - 1); cout << "\b \b" << flush; }
        } else if (c >= 32) {
            kq += static_cast<char>(c);
            cout << '*' << flush;
        }
    }
    cout << "\n";
    return true;
#else
    if (!isatty(STDIN_FILENO)) return hoi(loiNhac, kq);
    cout << loiNhac << flush;
    termios cu, moi;
    tcgetattr(STDIN_FILENO, &cu);
    moi = cu;
    moi.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &moi);
    kq.clear();
    bool ok = true;
    for (;;) {
        int c = getchar();
        if (c == EOF) { ok = false; break; }
        if (c == '\n' || c == '\r') break;
        if (c == 127 || c == '\b') {
            if (!kq.empty()) { kq.erase(kq.size() - 1); cout << "\b \b" << flush; }
        } else if (c >= 32) {
            kq += static_cast<char>(c);
            cout << '*' << flush;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &cu);
    cout << "\n";
    return ok;
#endif
}

}  // namespace xt

// ---------------------------------------------------------------- Class 1: quan ly (du lieu + file)
class QuanLyDangNhap {
public:
    explicit QuanLyDangNhap(string file = "tai_khoan.txt") : file_(file) {
        docFile();
        if (ds_.empty()) dangKy("admin", "admin123", "admin", "Mau yeu thich?", "xanh");  // admin mac dinh
    }

    // Doc file: moi dong 1 tai khoan, cac cot cach nhau bang tab:
    //   ten <tab> vai tro <tab> mat khau <tab> cau hoi <tab> dap an
    void docFile() {
        ds_.clear();
        ifstream f(file_);
        string dong;
        while (getline(f, dong)) {
            if (!dong.empty() && dong.back() == '\r') dong.pop_back();
            if (dong.empty()) continue;
            vector<string> c;
            stringstream ss(dong);
            for (string x; getline(ss, x, '\t');) c.push_back(x);
            if (c.size() != 5) throw runtime_error("File " + file_ + " bi hong.");
            ds_[c[0]] = {c[1], c[2], c[3], c[4]};
        }
    }

    void luuFile() const {
        ofstream f(file_);
        for (map<string, TaiKhoan>::const_iterator it = ds_.begin(); it != ds_.end(); ++it)
            f << it->first << '\t' << it->second.vaiTro << '\t' << it->second.mk << '\t'
              << it->second.cauHoi << '\t' << it->second.dapAn << '\n';
        if (!f) throw runtime_error("Khong ghi duoc file " + file_);
    }

    bool tonTai(const string& tdn) const { return ds_.count(xt::chuanHoa(tdn)) > 0; }

    // Tra ve "" neu thanh cong, nguoc lai la cau bao loi. vaiTro: "admin" | "user" | "bacsi"
    string dangKy(const string& tdn, const string& mk, const string& vaiTro,string cauHoi, string dapAn) {
        string ten = xt::chuanHoa(tdn);
        bool tenDep = ten.size() >= 3 && ten.size() <= 30 &&
                      all_of(ten.begin(), ten.end(), [](unsigned char c) { return isalnum(c)/*chu cai hay chu so*/ || c == '_' || c == '.'; });
        if (!tenDep) return "Ten dang nhap 3-30 ky tu, chi gom chu khong dau, so, '_' hoac '.'.";
        if (ds_.count(ten)) return "Ten dang nhap da ton tai.";
        if (mk.size() < 6) return "Mat khau phai tu 6 ky tu.";
        if (mk.find_first_of("\t\r\n") != string::npos) return "Mat khau khong duoc chua tab hoac xuong dong.";
        if (vaiTro != "admin" && vaiTro != "user" && vaiTro != "bacsi") return "Vai tro khong hop le.";
        for (auto* s : {&cauHoi, &dapAn}) {
            for (auto& c : *s) if (c == '\t' || c == '\n' || c == '\r') c = ' ';
            *s = xt::catTrang(*s);
        }
        if (cauHoi.empty() || dapAn.empty()) return "Can nhap cau hoi bao mat va dap an.";

        ds_[ten] = {vaiTro, mk, cauHoi, dapAn};
        luuFile();
        return "";
    }

    bool kiemTraMatKhau(const string& tdn, const string& mk) const {
        auto it = ds_.find(xt::chuanHoa(tdn));
        return it != ds_.end() && it->second.mk == mk;
    }

    int layVaiTro(const string& tdn) const {   // 1 / 2 / 3 / 0
        auto it = ds_.find(xt::chuanHoa(tdn));
        if (it == ds_.end()) return KHONG_TON_TAI;
        return it->second.vaiTro == "admin" ? ADMIN : it->second.vaiTro == "user" ? USER : BAC_SI;
    }

    string layCauHoi(const string& tdn) const {
        auto it = ds_.find(xt::chuanHoa(tdn));
        return it == ds_.end() ? "" : it->second.cauHoi;
    }

    bool kiemTraDapAn(const string& tdn, const string& dapAn) const {
        auto it = ds_.find(xt::chuanHoa(tdn));
        return it != ds_.end() && xt::chuanHoa(it->second.dapAn) == xt::chuanHoa(dapAn);
    }

    // Quen mat khau: dung dap an thi dat mat khau moi. Tra ve "" neu OK, nguoc lai la cau bao loi.
    string datLaiMatKhau(const string& tdn, const string& dapAn, const string& mkMoi) {
        if (!kiemTraDapAn(tdn, dapAn)) return "Dap an khong dung.";
        if (mkMoi.size() < 6) return "Mat khau phai tu 6 ky tu.";
        if (mkMoi.find_first_of("\t\r\n") != string::npos) return "Mat khau khong duoc chua tab hoac xuong dong.";
        ds_[xt::chuanHoa(tdn)].mk = mkMoi;
        luuFile();
        return "";
    }

private:
    struct TaiKhoan { string vaiTro, mk, cauHoi, dapAn; };
    string file_;
    map<string, TaiKhoan> ds_;
};

// ---------------------------------------------------------------- Class 2: dang nhap (luong + hop thoai)
class DangNhap {
public:
    QuanLyDangNhap ql;

    explicit DangNhap(const string& file = "tai_khoan.txt") : ql(file) {}

    // Tra ve 1 (admin) / 2 (user) / 3 (bac si) / 0 (khong ton tai hoac nguoi dung huy)
    int dangNhap(const string& tdn, string mk) {
        if (!ql.tonTai(tdn)) {
            xt::ve_bang({"Tai khoan '" + tdn + "' khong ton tai."});
            return KHONG_TON_TAI;
        }
        while (!ql.kiemTraMatKhau(tdn, mk)) {
            xt::ve_bang({"SAI MAT KHAU", "", "[1] Thu lai", "[2] Quen mat khau", "[0] Huy"});
            string chon;
            if (!xt::hoi("Chon: ", chon)) return KHONG_TON_TAI;
            chon = xt::catTrang(chon);
            if (chon == "0") return KHONG_TON_TAI;
            if (chon == "1") {
                if (!xt::hoiMatKhau("Nhap lai mat khau: ", mk) || mk.empty()) return KHONG_TON_TAI;
            } else if (chon == "2") {
                if (quenMatKhau(tdn)) {
                    if (!xt::hoiMatKhau("Dang nhap lai bang mat khau moi: ", mk) || mk.empty()) return KHONG_TON_TAI;
                }
            } else {
                cout << "Hay nhap 1, 2 hoac 0.\n";
            }
        }
        return ql.layVaiTro(tdn);
    }

    // Tra loi cau hoi bao mat (3 lan) roi dat mat khau moi. true neu doi thanh cong.
    bool quenMatKhau(const string& tdn) {
        xt::ve_bang({"QUEN MAT KHAU", "Cau hoi bao mat: " + ql.layCauHoi(tdn)});
        string dapAn;
        bool dung = false;
        for (int lan = 1; lan <= 3 && !dung; ++lan) {
            if (!xt::hoi("Dap an (de trong = huy): ", dapAn) || dapAn.empty()) return false;
            dung = ql.kiemTraDapAn(tdn, dapAn);
            if (!dung) cout << "Dap an khong dung (" << lan << "/3).\n";
        }
        if (!dung) return false;

        for (;;) {
            string m1, m2;
            if (!xt::hoiMatKhau("Mat khau moi (de trong = huy): ", m1) || m1.empty()) return false;
            if (!xt::hoiMatKhau("Nhap lai mat khau moi: ", m2)) return false;
            if (m1 != m2) { cout << "Hai mat khau khong khop.\n"; continue; }
            string loi = ql.datLaiMatKhau(tdn, dapAn, m1);
            if (loi.empty()) { cout << "Doi mat khau thanh cong.\n"; return true; }
            cout << loi << "\n";
        }
    }

    // Nhap thong tin dang ky tu ban phim. true neu tao duoc tai khoan.
    bool dangKy() {
        for (;;) {
            string tdn, mk, mk2, vt, cauHoi, dapAn;
            cout << "\n--- DANG KY ---\n";
            if (!xt::hoi("Ten dang nhap (de trong = huy): ", tdn) || tdn.empty()) return false;
            if (!xt::hoiMatKhau("Mat khau: ", mk) || !xt::hoiMatKhau("Nhap lai mat khau: ", mk2)) return false;
            if (!xt::hoi("Vai tro [1] User  [2] Bac si: ", vt)) return false;
            if (!xt::hoi("Cau hoi bao mat: ", cauHoi) || !xt::hoi("Dap an: ", dapAn)) return false;
            if (mk != mk2) { cout << "Hai mat khau khong khop.\n"; continue; }
            string loi = ql.dangKy(tdn, mk, xt::catTrang(vt) == "2" ? "bacsi" : "user", cauHoi, dapAn);
            if (loi.empty()) { cout << "Dang ky thanh cong.\n"; return true; }
            cout << loi << "\n";
        }
    }
};