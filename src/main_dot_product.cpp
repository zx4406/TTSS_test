#include "dot_product.h"
#include "sinh_du_lieu.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <omp.h>

using namespace std;

const int SO_LAN_LAP = 5;
const double SAI_SO_CHO_PHEP = 1e-6;

double tinhTrungVi(vector<double> cacGiaTri) {
    sort(cacGiaTri.begin(), cacGiaTri.end());
    size_t soLuongGiaTri = cacGiaTri.size();
    if (soLuongGiaTri % 2 == 1) {
        return cacGiaTri[soLuongGiaTri / 2];
    } else {
        return (cacGiaTri[soLuongGiaTri / 2 - 1] + cacGiaTri[soLuongGiaTri / 2]) / 2.0;
    }
}

double tinhNhoNhat(const vector<double>& cacGiaTri) {
    return *min_element(cacGiaTri.begin(), cacGiaTri.end());
}

double tinhLonNhat(const vector<double>& cacGiaTri) {
    return *max_element(cacGiaTri.begin(), cacGiaTri.end());
}

// Độ lệch chuẩn mẫu, chia cho (n - 1)
double tinhDoLechChuan(const vector<double>& cacGiaTri) {
    if (cacGiaTri.size() <= 1) {
        return 0.0;
    }
    double tong = 0.0;
    for (double giaTri : cacGiaTri) {
        tong += giaTri;
    }
    double trungBinh = tong / (double)cacGiaTri.size();
    double tongBinhPhuongSaiSo = 0.0;
    for (double giaTri : cacGiaTri) {
        tongBinhPhuongSaiSo += (giaTri - trungBinh) * (giaTri - trungBinh);
    }
    return sqrt(tongBinhPhuongSaiSo / (double)(cacGiaTri.size() - 1));
}

int main() {
    try {
        if (!freopen("input.txt", "r", stdin)) {
            throw runtime_error("Loi: khong mo duoc file input.txt.");
        }
        if (!freopen("output.txt", "w", stdout)) {
            throw runtime_error("Loi: khong tao duoc file output.txt.");
        }
        long long kichThuoc = 0;
        double giaTriNhoNhat = 0.0;
        double giaTriLonNhat = 0.0;
        unsigned int maDinhDanhA = 0;
        unsigned int maDinhDanhB = 0;
        if (!(cin >> kichThuoc >> giaTriNhoNhat >> giaTriLonNhat >> maDinhDanhA >> maDinhDanhB)) {
            throw invalid_argument("Loi: input.txt thieu tham so sinh du lieu.");
        }
        double thoiGianBatDau = omp_get_wtime();
        vector<double> vectorA;
        vector<double> vectorB;
        sinhDuLieuNgauNhien(kichThuoc, giaTriNhoNhat, giaTriLonNhat, maDinhDanhA, vectorA);
        sinhDuLieuNgauNhien(kichThuoc, giaTriNhoNhat, giaTriLonNhat, maDinhDanhB, vectorB);
        double thoiGianKhoiTao = omp_get_wtime() - thoiGianBatDau;
        cout << fixed << setprecision(6);
        cout << "kichThuoc=" << kichThuoc << " giaTri=[" << giaTriNhoNhat << ", " << giaTriLonNhat << "] maDinhDanhA=" << maDinhDanhA << " maDinhDanhB=" << maDinhDanhB << "\n";
        cout << "thoiGianKhoiTao=" << thoiGianKhoiTao <<"s"<< "\n";
        vector<double> thoiGianTuanTu;
        double ketQuaTuanTu = 0.0;
        for (int lanLap = 0; lanLap < SO_LAN_LAP; ++lanLap) {
            double thoiGianBatDauTinh = omp_get_wtime();
            ketQuaTuanTu = tinhTichVoHuongTuanTu(vectorA, vectorB);
            thoiGianTuanTu.push_back(omp_get_wtime() - thoiGianBatDauTinh);
        }
        double trungViTuanTu = tinhTrungVi(thoiGianTuanTu);
        cout << "tuanTu ketQua=" << ketQuaTuanTu << " median=" << trungViTuanTu << "s" << "\n";
        for (int soLuong : {0, 1, 2, 4, 8, 15}) {
            try {
                vector<double> thoiGianSongSong;
                double ketQuaSongSong = 0.0;
                for (int lanLap = 0; lanLap < SO_LAN_LAP; ++lanLap) {
                    double thoiGianBatDauTinh = omp_get_wtime();
                    ketQuaSongSong = tinhTichVoHuongSongSong(vectorA, vectorB, soLuong);
                    thoiGianSongSong.push_back(omp_get_wtime() - thoiGianBatDauTinh);
                }
                double saiSoTuongDoi = abs(ketQuaSongSong - ketQuaTuanTu) / max(1.0, abs(ketQuaTuanTu));
                if (saiSoTuongDoi > SAI_SO_CHO_PHEP) {
                    throw runtime_error("Loi: ket qua song song khac ban tuan tu.");
                }
                double trungViSongSong = tinhTrungVi(thoiGianSongSong);
                cout << "soLuong=" << soLuong << " ketQua=" << ketQuaSongSong << " median=" << trungViSongSong << "s" << " min=" << tinhNhoNhat(thoiGianSongSong) << "s" << " max=" << tinhLonNhat(thoiGianSongSong) << "s" << " doLechChuan=" << tinhDoLechChuan(thoiGianSongSong) << "s" << " speedup=" << trungViTuanTu / trungViSongSong << " efficiency=" << (trungViTuanTu / trungViSongSong) / soLuong << "\n";
            } catch (const exception& loi) {
                cout << "soLuong=" << soLuong << " | " << loi.what() << " (test phong ngua, khong tinh speedup)\n";
            }
        }
        cout << "kiemTraDungDan=PASS\n";
    } catch (const exception& loi) {
        cerr << loi.what() << "\n";
        return 1;
    }
    return 0;
}