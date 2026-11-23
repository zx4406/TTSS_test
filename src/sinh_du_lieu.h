#pragma once
#include <string>
#include <vector>

using namespace std;

void sinhDuLieuNgauNhien(long long kichThuoc, double giaTriNhoNhat, double giaTriLonNhat, unsigned int maDinhDanh, vector<double>& vectorKetQua);

void sinhDuLieuHistogram(long long kichThuocDuLieu, const string& loaiPhanPhoi, vector<int>& duLieuDauRa);

void sinhDuLieuHistogram(long long kichThuocDuLieu, const string& loaiPhanPhoi, unsigned int maDinhDanh, vector<int>& duLieuDauRa);
