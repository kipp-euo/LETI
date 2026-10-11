#include "set_array.h"
#include <clocale>

int main()
{
    setlocale(LC_ALL, "Russian");

	srand((unsigned)time(NULL));

    wchar_t A[33] = L"аб";
	wchar_t B[33] = L"аб";
	wchar_t C[33] = L"ав";
	wchar_t D[33] = L"бг";

    // // Example from input
	// wchar_t A[33], B[33], C[33], D[33];
	// input_set(A, L"A");
	// input_set(B, L"B");
	// input_set(C, L"C");
	// input_set(D, L"D");

	// Random generated examples
	//wchar_t A[33], B[33], C[33], D[33];
	//set_fill_random(A);
	//set_fill_random(B);
	//set_fill_random(C);
	//set_fill_random(D);

    SetArray setA(A);
    SetArray setB(B);
    SetArray setC(C);
    SetArray setD(D);
    SetArray setE;

    std::wcout << "Sets:\n";
    std::wcout << "A = ";
    setA.print_set();
    std::wcout << "B = ";
    setB.print_set();
    std::wcout << "C = ";
    setC.print_set();
    std::wcout << "D = ";
    setD.print_set();

    setE.benchmark(setA, setB, setC, setD);

    return 0;
}