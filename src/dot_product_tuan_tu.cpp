#include "dot_product.h"
#include <cmath>
#include <stdexcept>

using namespace std;

double tinhTichVoHuongTuanTu(const vector<double>& vectorA, const vector<double>& vectorB) {
    if (vectorA.empty() || vectorB.empty()) {
        throw invalid_argument("Loi: vector rong.");
    }
    if (vectorA.size() != vectorB.size()) {
        throw invalid_argument("Loi: kich thuoc hai vector khong bang nhau.");
    }
    double tichVoHuong = 0.0;
    for (long long i = 0; i < vectorA.size(); ++i) {
        tichVoHuong += vectorA[i] * vectorB[i];
    }
    if (!isfinite(tichVoHuong)) {
        throw overflow_error("Loi: ket qua bi Inf hoac NaN.");
    }
    return tichVoHuong;
}