#include "histogram.h"
#include <omp.h>
#include <stdexcept>

using namespace std;

void tinhHistogramCucBo(const vector<int>& duLieuDauVao, int soBin, int soLuong, vector<long long>& ketQuaHistogram) {
    if (duLieuDauVao.empty()) {
        throw invalid_argument("Loi: duLieuDauVao rong.");
    }
    if (soBin <= 0) {
        throw invalid_argument("Loi: soBin phai lon hon 0.");
    }
    if (soLuong <= 0 || soLuong > omp_get_num_procs()) {
        throw invalid_argument("Loi: soLuong phai lon hon 0 va khong vuot qua so core.");
    }
    ketQuaHistogram.assign(soBin, 0); // Xóa và đặt lại vector giá trị 0
    // Pad mỗi histogram cục bộ lên bội số 64 byte để tránh false sharing:
    // mỗi ô long long 8 byte nên cần bội số của 8 ô mới đủ 1 cache line.
    int soBinPad = ((soBin + SO_PHAN_TU_TREN_CACHE_LINE - 1) / SO_PHAN_TU_TREN_CACHE_LINE) * SO_PHAN_TU_TREN_CACHE_LINE;
    vector<vector<long long>> histRieng(soLuong, vector<long long>(soBinPad, 0));
    #pragma omp parallel num_threads(soLuong)
    {
        int maLuong = omp_get_thread_num();
        vector<long long>& hist = histRieng[maLuong];
        #pragma omp for schedule(static)
        for (size_t i = 0; i < duLieuDauVao.size(); ++i) {
            size_t bin = (size_t)((long long)duLieuDauVao[i] * soBin / (GIA_TRI_TOI_DA + 1));
            hist[bin]++;
        }
    }
    // Reduction: gộp các histogram cục bộ, chi phí O(soLuong * soBin).
    for (int maLuong = 0; maLuong < soLuong; ++maLuong) {
        for (int bin = 0; bin < soBin; ++bin) {
            ketQuaHistogram[bin] += histRieng[maLuong][bin];
        }
    }
}