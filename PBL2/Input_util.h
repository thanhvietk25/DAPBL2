#ifndef INPUT_UTIL_H
#define INPUT_UTIL_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstdio>
#include <ctime>

#include "Date.h"

using namespace std;

// Cat khoang trang (ca '\r' neu file/console kieu Windows)
inline string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Doc 1 dong. Tra ve false neu het du lieu nhap (EOF)
inline bool docDong(const string &tb, string &s)
{
    cout << tb;
    if (!getline(cin, s))
        return false;
    s = trim(s);
    return true;
}

inline bool docSoNguyen(const string &tb, int &x)
{
    string dong;
    while (true)
    {
        cout << tb;
        if (!getline(cin, dong))
            return false;

        stringstream ss(dong);
        char thua;
        if ((ss >> x) && !(ss >> thua))
            return true;

        cout << "Vui long nhap mot so nguyen hop le!\n";
    }
}

inline bool docSoThuc(const string &tb, double &x)
{
    string dong;
    while (true)
    {
        cout << tb;
        if (!getline(cin, dong))
            return false;

        stringstream ss(dong);
        char thua;
        if ((ss >> x) && !(ss >> thua))
            return true;

        cout << "Vui long nhap mot so hop le!\n";
    }
}

// Nhap ngay dang dd/mm/yyyy. Neu choPhepHomNay = true thi de trong = lay ngay hom nay
inline bool docNgay(const string &tb, Date &d, bool choPhepHomNay = false)
{
    while (true)
    {
        cout << tb;
        string s;
        if (!getline(cin, s))
            return false;
        s = trim(s);

        int n, t, nm;

        if (s.empty() && choPhepHomNay)
        {
            time_t now = time(nullptr);
            tm *lt = localtime(&now);
            return d.setDate(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
        }

        char thua;
        if (sscanf(s.c_str(), "%d/%d/%d%c", &n, &t, &nm, &thua) == 3 &&
            d.setDate(n, t, nm))
            return true;

        cout << "Ngay khong hop le! Nhap theo dang dd/mm/yyyy.\n";
    }
}

// 1500000 -> "1.500.000"
inline string dinhDangTien(double v)
{
    ostringstream os;
    os << fixed << setprecision(0) << v;
    string s = os.str(), out;
    int dem = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--)
    {
        out.insert(out.begin(), s[i]);
        if (++dem % 3 == 0 && i > 0 && isdigit((unsigned char)s[i - 1]))
            out.insert(out.begin(), '.');
    }
    return out;
}

#endif
