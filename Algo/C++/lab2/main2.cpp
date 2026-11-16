#include <clocale>
// Uncomment one

// #include "set_array.h"
// #define Set SetArray

#include "set_array_2.h"
#define Set SetArray

// #include "set_list.h"
// #define Set SetList

// #include "set_bitarray.h"
// #define Set SetBitArray

// #include "set_word.h" 
// #define Set SetWord

int main()
{
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale(""));
	setlocale(LC_ALL, "Russian");

	srand((unsigned)time(NULL));

	// wchar_t A[33] = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";
	// wchar_t B[33] = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";
	// wchar_t C[33] = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";
	// wchar_t D[33] = L"гдежзийклмнопрстуфхцчшщъыьэюяаб";

	// Example from input
	// wchar_t A[33], B[33], C[33], D[33];
	// input_set(A, L"A");
	// input_set(B, L"B");
	// input_set(C, L"C");
	// input_set(D, L"D");

	// Random generated examples
	// wchar_t A[33], B[33], C[33], D[33];
	// set_fill_random(A);
	// set_fill_random(B);
	// set_fill_random(C);
	// set_fill_random(D);

    wchar_t A[33] = L"абв";
    wchar_t B[33] = L"аб";
    wchar_t C[33] = L"ав";
    wchar_t D[33] = L"г";

    SetArray setA(A);
    SetArray setB(B);
    SetArray setC(C);
    SetArray setD(D);
    SetArray setE;

    std::wcout << L"\nВычисление множеств:\n";
    setE.benchmark(setA, setB, setC, setD);
    std::wcout << L"\n";

    return 0;
}