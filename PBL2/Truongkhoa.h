#ifndef TRUONGKHOA_H
#define TRUONGKHOA_H

#include "Doctor.h"

using namespace std;
class TruongKhoa : public Doctor
{
public:
    string getTag() const override { return "[TRUONGKHOA]"; }
    string getChucVu() const override { return "Truong khoa"; }
    bool duocQuanLyKhoa() const override { return true; }
    NhanVien *clone() const override { return new TruongKhoa(*this); }
};

#endif