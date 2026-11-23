#pragma once
#include <vector>

using namespace std;

const long long GIA_TRI_TOI_DA = 255;
const int KICH_THUOC_CACHE_LINE = 64;
const int SO_PHAN_TU_TREN_CACHE_LINE = 8;

long long tinhBin(int giaTri, int soBin);
void tinhHistogramTuanTu(const vector<int>& duLieuDauVao, int soBin, vector<long long>& ketQuaHistogram);
void tinhHistogramAtomic(const vector<int>& duLieuDauVao, int soBin, int soLuong, vector<long long>& ketQuaHistogram);
void tinhHistogramCucBo(const vector<int>& duLieuDauVao, int soBin, int soLuong, vector<long long>& ketQuaHistogram);