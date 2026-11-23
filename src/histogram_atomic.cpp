#include "histogram.h"
#include <omp.h>
#include <stdexcept>

using namespace std;

void tinhHistogramAtomic(const vector<int>& duLieuDauVao, int soBin, int soLuong, vector<long long>& ketQuaHistogram) {
    if (duLieuDauVao.empty()) {
        throw invalid_argument("Loi: duLieuDauVao rong.");
    }
    if (soBin <= 0) {
        throw invalid_argument("Loi: soBin phai lon hon 0.");
    }
    if (soLuong <= 0 || soLuong > omp_get_num_procs()) {
        throw invalid_argument("Loi: soLuong phai lon hon 0 va khong vuot qua so core.");
    }
    ketQuaHistogram.assign(soBin, 0);
    // Mỗi phần tử tăng trực tiếp vào mảng chung bằng atomic.
    #pragma omp parallel for schedule(static) num_threads(soLuong)
    for (long long i = 0; i < (long long)duLieuDauVao.size(); i++) {
        long long chiSoBin = duLieuDauVao[i] * soBin / (GIA_TRI_TOI_DA + 1);
        #pragma omp atomic
        ketQuaHistogram[chiSoBin]++;
    }
}