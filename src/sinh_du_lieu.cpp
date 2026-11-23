#include "sinh_du_lieu.h"
#include <random>
#include <stdexcept>
#include <vector>

using namespace std;

const int SEED_MAC_DINH_HISTOGRAM = 42;

void sinhDuLieuNgauNhien(long long kichThuoc, double giaTriNhoNhat, double giaTriLonNhat, unsigned int maDinhDanh, vector<double>& vectorKetQua) {
    if (kichThuoc <= 0) {
        throw invalid_argument("Loi sinh du lieu: kich thuoc vector phai lon hon 0.");
    }
    if (giaTriNhoNhat > giaTriLonNhat) {
        throw invalid_argument("Loi sinh du lieu: gia tri nho nhat khong duoc lon hon gia tri lon nhat.");
    }
    vectorKetQua.resize((size_t)kichThuoc);
    // Dùng maDinhDanh để cố định vector: cùng seed thì sinh ra cùng dữ liệu.
    mt19937_64 boSinhNgauNhien(maDinhDanh);
    uniform_real_distribution<double> phanPhoiDeu(giaTriNhoNhat, giaTriLonNhat);
    for (long long i = 0; i < kichThuoc; ++i) {
        vectorKetQua[(size_t)i] = phanPhoiDeu(boSinhNgauNhien);
    }
}

void sinhDuLieuHistogram(long long kichThuocDuLieu, const string& loaiPhanPhoi, unsigned int maDinhDanh, vector<int>& duLieuDauRa) {
    if (kichThuocDuLieu <= 0) {
        throw invalid_argument("Loi sinh du lieu: kich thuoc du lieu phai lon hon 0.");
    }
    if (loaiPhanPhoi != "uniform" && loaiPhanPhoi != "skewed") {
        throw invalid_argument("Loi sinh du lieu: loaiPhanPhoi chi nhan uniform hoac skewed.");
    }
    duLieuDauRa.resize((size_t)kichThuocDuLieu);
    mt19937_64 boSinhNgauNhien(maDinhDanh);
    if (loaiPhanPhoi == "skewed") {
        // Lệch: 80% giá trị dồn vào 0..31 để làm rõ tranh chấp atomic,
        // 20% còn lại đều trong 0..255.
        uniform_int_distribution<int> phanPhoiHep(0, 31);
        uniform_int_distribution<int> phanPhoiRong(0, 255);
        uniform_real_distribution<double> xacSuat(0.0, 1.0);
        for (long long i = 0; i < kichThuocDuLieu; ++i) {
            duLieuDauRa[(size_t)i] = (xacSuat(boSinhNgauNhien) < 0.8) ? phanPhoiHep(boSinhNgauNhien) : phanPhoiRong(boSinhNgauNhien);
        }
    } else {
        uniform_int_distribution<int> phanPhoiDeu(0, 255);
        for (long long i = 0; i < kichThuocDuLieu; ++i) {
            duLieuDauRa[(size_t)i] = phanPhoiDeu(boSinhNgauNhien);
        }
    }
}

void sinhDuLieuHistogram(long long kichThuocDuLieu, const string& loaiPhanPhoi, vector<int>& duLieuDauRa) {
    sinhDuLieuHistogram(kichThuocDuLieu, loaiPhanPhoi, (unsigned int)SEED_MAC_DINH_HISTOGRAM, duLieuDauRa);
}
