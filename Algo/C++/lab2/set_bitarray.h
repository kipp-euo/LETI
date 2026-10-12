#ifndef SET_BITARRAY_H
#define SET_BITARRAY_H

#include <iostream>
#include <chrono>


class SetBitArray
{
private:
	static const int M = 33;
	bool bits[M];

	int to_index(wchar_t ch) {
		if (L'а' <= ch && ch <= L'я') {
			return ch - L'а';
		}
		if (ch == L'ё') {
			return 32;
		}
		return 0;
	}

	wchar_t from_index(int i) {
		if (i >= 0 && i < 32) {
			return L'а' + i;
		}
		if (i == 32) {
			return L'ё';
		}
		return L'\0';
	}

public:
	SetBitArray() {
		for (int i = 0; i < M; i++) {
			bits[i] = false;
		}
	}

	SetBitArray(const wchar_t (&arr)[33]) {
		for (int i = 0; i < M; i++) {
			bits[i] = false;
		}
		for (int i = 0; i < 33 && arr[i] != 0; i++) {
			bits[to_index(arr[i])] = true;
		}
	}

	void print_set() {
		for (int i = 0; i < M; i++) {
			if (bits[i]) {
				std::wcout << from_index(i);
			}
		}
		std::wcout << '\n';
	}

	void calc_set(const SetBitArray& A, const SetBitArray& B, const SetBitArray& C, const SetBitArray& D) {
		for (int i = 0; i < M; i++) {
			bits[i] = A.bits[i] && (B.bits[i] && C.bits[i]) && !D.bits[i];
		}
	}

	void benchmark(const SetBitArray& A, const SetBitArray& B, const SetBitArray& C, const SetBitArray& D) {
		auto t_start = std::chrono::high_resolution_clock::now();

		this->calc_set(A, B, C, D);

		auto t_end = std::chrono::high_resolution_clock::now();
		double time_elapsed = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		std::wcout << L"Time elapsed (SetBitArray): " << time_elapsed << L" ms\n";
		std::wcout << L"Resulting set E: ";
		this->print_set();
	}
};


#endif
