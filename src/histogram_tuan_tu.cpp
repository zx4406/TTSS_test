#include "histogram.h"
#include <stdexcept>

using namespace std;

long long tinhBin(int giaTri, int soBin) {
    if (soBin <= 0) {
        throw invalid_argument("Loi: soBin phai lon hon 0.");
    }
    if (giaTri < 0 || giaTri > GIA_TRI_TOI_DA) {
        throw invalid_argument("Loi: giaTri nam ngoai khoang 0->" + to_string(GIA_TRI_TOI_DA) + ".");
    }
    return (long long)giaTri * soBin / (GIA_TRI_TOI_DA + 1);
}

void tinhHistogramTuanTu(const vector<int>& duLieuDauVao, int soBin, vector<long long>& ketQuaHistogram) {
    if (duLieuDauVao.empty()) {
        throw invalid_argument("Loi: duLieuDauVao rong.");
    }
    if (soBin <= 0) {
        throw invalid_argument("Loi: soBin phai lon hon 0.");
    }
    ketQuaHistogram.assign((size_t)soBin, 0);
    for (long long i = 0; i < duLieuDauVao.size(); i++) {
        ketQuaHistogram[tinhBin(duLieuDauVao[i], soBin)]++;
    }
}