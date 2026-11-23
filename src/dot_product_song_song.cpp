#include "dot_product.h"
#include <cmath>
#include <omp.h>
#include <stdexcept>

using namespace std;

double tinhTichVoHuongSongSong(const vector<double>& vectorA, const vector<double>& vectorB, int soLuong) {
    if (vectorA.empty() || vectorB.empty()) {
        throw invalid_argument("Loi: vector rong.");
    }
    if (vectorA.size() != vectorB.size()) {
        throw invalid_argument("Loi: kich thuoc hai vector khong bang nhau.");
    }
    if (soLuong <= 0 || soLuong > omp_get_num_procs()) {
        throw invalid_argument("Loi: soLuong phai lon hon 0 va khong vuot qua so core.");
    }
    double tongTich = 0.0;
    #pragma omp parallel for reduction(+:tongTich) schedule(static) num_threads(soLuong)
    for (long long i = 0; i < vectorA.size(); i++) {
        tongTich += vectorA[i] * vectorB[i];
    }
    if (!isfinite(tongTich)) {
        throw overflow_error("Loi: ket qua bi Inf hoac NaN.");
    }
    return tongTich;
}