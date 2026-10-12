#ifndef SET_WORD_H
#define SET_WORD_H

#include <iostream>
#include <chrono>


class SetWord
{
private:
	unsigned long long word;

	int to_index(wchar_t ch) const {
		if (ch >= L'а' && ch <= L'я') {
			return ch - L'а';
		}
		if (ch == L'ё') {
			return 32;
		}
		return 0;
	}

	wchar_t from_index(int i) const {
		if (i >= 0 && i < 32) {
			return L'а' + i;
		}
		if (i == 32) {
			return L'ё';
		}
		return L'\0';
	}

public:
	SetWord() : word(0) {}

	SetWord(const wchar_t (&arr)[33]) : word(0) {
		for (int i = 0; i < 33 && arr[i] != 0; i++) {
			word |= (1ULL << to_index(arr[i]));
		}
	}

	void print_set() {
		for (int i = 0; i < 33; i++) {
			if ((word >> i) & 1ULL) {
				std::wcout << from_index(i);
			}
		}
		std::wcout << '\n';
	}

	void calc_set(const SetWord& A, const SetWord& B, const SetWord& C, const SetWord& D) {
		word = A.word & (B.word & C.word) & ~D.word;
	}

	void benchmark(const SetWord& A, const SetWord& B, const SetWord& C, const SetWord& D) {
		auto t_start = std::chrono::high_resolution_clock::now();

		this->calc_set(A, B, C, D);

		auto t_end = std::chrono::high_resolution_clock::now();
		double time_elapsed = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		std::wcout << L"Time elapsed (SetWord): " << time_elapsed << L" ms\n";
		std::wcout << L"Resulting set E: ";
		this->print_set();
	}
};


#endif
