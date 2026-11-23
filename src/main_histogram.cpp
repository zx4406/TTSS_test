#include "histogram.h"
#include "sinh_du_lieu.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <omp.h>

using namespace std;

double tinhTrungVi(vector<double> cacGiaTri) {
    sort(cacGiaTri.begin(), cacGiaTri.end());
    size_t soLuongGiaTri = cacGiaTri.size();
    if (soLuongGiaTri % 2 == 1) {
        return cacGiaTri[soLuongGiaTri / 2];
    } else {
        return (cacGiaTri[soLuongGiaTri / 2 - 1] + cacGiaTri[soLuongGiaTri / 2]) / 2.0;
    }
}

void kiemTraKetQua(const vector<long long>& ketQuaChuan, const vector<long long>& ketQuaKiemTra, long long kichThuocDuLieu) {
    if (ketQuaChuan.size() != ketQuaKiemTra.size()) {
        throw runtime_error("Loi: kich thuoc histogram khac nhau.");
    }
    long long tong = 0;
    for (size_t i = 0; i < ketQuaKiemTra.size(); ++i) {
        if (ketQuaChuan[i] != ketQuaKiemTra[i]) {
            throw runtime_error("Loi: ket qua song song khac ban tuan tu.");
        }
        tong += ketQuaKiemTra[i];
    }
    if (tong != kichThuocDuLieu) {
        throw runtime_error("Loi: tong histogram khac kichThuocDuLieu.");
    }
}

int main() {
    try {
        if (!freopen("input.txt", "r", stdin)) {
            throw runtime_error("Loi: khong mo duoc file input.txt.");
        }
        if (!freopen("output.txt", "w", stdout)) {
            throw runtime_error("Loi: khong tao duoc file output.txt.");
        }
        long long kichThuocDuLieu = 0;
        int soBin = 0;
        int soLuong = 0;
        int soLanLap = 0;
        string loaiPhanPhoi;
        if (!(cin >> kichThuocDuLieu >> soBin >> soLuong >> soLanLap)) {
            throw invalid_argument("Loi: input.txt thieu dong tham so.");
        }
        if (!(cin >> loaiPhanPhoi)) {
            throw invalid_argument("Loi: input.txt thieu loaiPhanPhoi.");
        }
        double thoiGianBatDau = omp_get_wtime();
        vector<int> duLieuDauVao;
        sinhDuLieuHistogram(kichThuocDuLieu, loaiPhanPhoi, duLieuDauVao);
        double thoiGianKhoiTao = omp_get_wtime() - thoiGianBatDau;
        vector<long long> ketQuaTuanTu;
        vector<long long> ketQuaAtomic;
        vector<long long> ketQuaCucBo;
        vector<double> thoiGianTuanTu;
        vector<double> thoiGianAtomic;
        vector<double> thoiGianCucBo;
        for (int lanLap = 0; lanLap < soLanLap; ++lanLap) {
            double thoiGianBatDauTinh = omp_get_wtime();
            tinhHistogramTuanTu(duLieuDauVao, soBin, ketQuaTuanTu);
            thoiGianTuanTu.push_back(omp_get_wtime() - thoiGianBatDauTinh);
        }
        for (int lanLap = 0; lanLap < soLanLap; ++lanLap) {
            double thoiGianBatDauTinh = omp_get_wtime();
            tinhHistogramAtomic(duLieuDauVao, soBin, soLuong, ketQuaAtomic);
            thoiGianAtomic.push_back(omp_get_wtime() - thoiGianBatDauTinh);
        }
        for (int lanLap = 0; lanLap < soLanLap; ++lanLap) {
            double thoiGianBatDauTinh = omp_get_wtime();
            tinhHistogramCucBo(duLieuDauVao, soBin, soLuong, ketQuaCucBo);
            thoiGianCucBo.push_back(omp_get_wtime() - thoiGianBatDauTinh);
        }
        kiemTraKetQua(ketQuaTuanTu, ketQuaAtomic, kichThuocDuLieu);
        kiemTraKetQua(ketQuaTuanTu, ketQuaCucBo, kichThuocDuLieu);
        double trungViTuanTu = tinhTrungVi(thoiGianTuanTu);
        double trungViAtomic = tinhTrungVi(thoiGianAtomic);
        double trungViCucBo = tinhTrungVi(thoiGianCucBo);
        if (!isfinite(trungViTuanTu) || !isfinite(trungViAtomic) || !isfinite(trungViCucBo)) {
            throw overflow_error("Loi: thoi gian do duoc bi Inf hoac NaN.");
        }
        cout << fixed << setprecision(6);
        cout << "kichThuocDuLieu=" << kichThuocDuLieu << " soBin=" << soBin << " soLuong=" << soLuong << " loaiPhanPhoi=" << loaiPhanPhoi << " soLanLap=" << soLanLap << "\n";
        cout << "thoiGianKhoiTao_s=" << thoiGianKhoiTao << "\n";
        cout << "tuanTu_median_s=" << trungViTuanTu << "\n";
        cout << "atomic_median_s=" << trungViAtomic << " speedup=" << trungViTuanTu / trungViAtomic << "\n";
        cout << "cucBo_median_s=" << trungViCucBo << " speedup=" << trungViTuanTu / trungViCucBo << " efficiency=" << (trungViTuanTu / trungViCucBo) / soLuong << "\n";
        cout << "kiemTraDungDan=PASS\n";
    } catch (const exception& loi) {
        cerr << loi.what() << "\n";
        return 1;
    }
    return 0;
}