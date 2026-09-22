#include <chrono>
#include <cstdint>
#include <ctime>
#include <clocale>
#include <iostream>


const wchar_t alphabet[] = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";

struct List {
	wchar_t el;
	List * next;

	List(wchar_t e, List *n = nullptr) : el(e), next(n) {}
	~List() { delete next; }
};

void input_set(wchar_t (&result)[33], const wchar_t* name);

void print_set(const wchar_t (&set)[33]);
void print_set(const List* set);
void print_set(uint64_t set);

void set_fill_random(wchar_t (&result)[33]);

List* arr_to_list(const wchar_t (&arr)[33]);
uint64_t arr_to_qword(const wchar_t (&arr)[33]);

bool contains(const wchar_t (&arr)[33], wchar_t elem);
bool contains(const List* l, wchar_t elem);

void calc_set(const wchar_t (&A)[33], const wchar_t (&B)[33],
		const wchar_t (&C)[33], const wchar_t (&D)[33],
		wchar_t (&out)[33]);
List* calc_set(const List* A, const List* B, const List* C, const List* D);
uint64_t calc_set(uint64_t A, uint64_t B, uint64_t C, uint64_t D);

void benchmark(const wchar_t (&A)[33], const wchar_t (&B)[33],
		const wchar_t (&C)[33], const wchar_t (&D)[33]);
void benchmark(const List* A, const List* B, const List* C, const List* D);
void benchmark(uint64_t A, uint64_t B, uint64_t C, uint64_t D);


int main()
{
	// For cyrillic letters
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale(""));

	srand((unsigned)time(NULL));

	// // Example
	// wchar_t A[] = L"бвгде";
	// wchar_t B[] = L"авгде";
	// wchar_t C[] = L"абгде";
	// wchar_t D[] = L"абвде";

	// // Example from input
	// wchar_t A[33], B[33], C[33], D[33];
	// input_set(A, L"A");
	// input_set(B, L"B");
	// input_set(C, L"C");
	// input_set(D, L"D");

	// Random generated examples
	wchar_t A[33], B[33], C[33], D[33];
	set_fill_random(A);
	set_fill_random(B);
	set_fill_random(C);
	set_fill_random(D);

	std::wcout << "Sets:\n";
	std::wcout << "A = ";
	print_set(A);
	std::wcout << "B = ";
	print_set(B);
	std::wcout << "C = ";
	print_set(C);
	std::wcout << "D = ";
	print_set(D);

	// Arrays
	benchmark(A, B, C, D);

	// Linked lists
	const List* lA = arr_to_list(A);
	const List* lB = arr_to_list(B);
	const List* lC = arr_to_list(C);
	const List* lD = arr_to_list(D);

	benchmark(lA, lB, lC, lD);

	// Machine words
	const uint64_t bA = arr_to_qword(A);
	const uint64_t bB = arr_to_qword(B);
	const uint64_t bC = arr_to_qword(C);
	const uint64_t bD = arr_to_qword(D);

	benchmark(bA, bB, bC, bD);
}

void input_set(wchar_t (&result)[33], const wchar_t* name)
{
	std::wcout << L"Enter set " << name << L": ";
	std::wcin.getline(result, 33);
}

void print_set(const wchar_t (&set)[33])
{
	for (int i = 0; set[i] != '\0'; i++) {
		std::wcout << set[i];
	}
	std::wcout << '\n';
}
void print_set(const List* set)
{
	for (; set != nullptr; set = set->next) {
		std::wcout << set->el;
	}
	std::wcout << '\n';
}
void print_set(uint64_t set)
{
	for (int i = 0; i < 33; i++) {
		if ((set >> i) & 1) {
			std::wcout << alphabet[i];
		}
	}
	std::wcout << '\n';
}

void set_fill_random(wchar_t (&result)[33])
{
	int i = 0;
	for (const wchar_t& letter : alphabet) {
		if (rand() % 2 == 1) {
			result[i++] = letter;
		}
	}
	result[i] = '\0';
}

List* arr_to_list(const wchar_t (&arr)[33])
{
	List* head = nullptr;
	List* tail = nullptr;

	for (int i = 0; arr[i] != L'\0'; i++) {
		List* node = new List(arr[i]);
		if (!head) {
			head = node;
			tail = node;
		} else {
			tail->next = node;
			tail = node;
		}
	}
	return head;
}
uint64_t arr_to_qword(const wchar_t (&arr)[33])
{
	uint64_t out = 0;
	for (size_t i = 0; arr[i] != '\0'; i++) {
		out = out | (1 << (arr[i] - alphabet[0]));
	}
	return out;
}

bool contains(const wchar_t (&arr)[33], wchar_t elem)
{
	for (int i = 0; arr[i] != '\0'; i++) {
		if (arr[i] == elem) {
			return true;
		}
	}
	return false;
}
bool contains(const List* l, wchar_t elem)
{
	for (; l != nullptr; l = l->next) {
		if (l->el == elem) {
			return true;
		}
	}
	return false;
}

void calc_set(const wchar_t (&A)[33], const wchar_t (&B)[33],
		const wchar_t (&C)[33], const wchar_t (&D)[33],
		wchar_t (&out)[33])
{
	int i = 0;
	for (int j = 0; C[j] != '\0'; j++) {
		if ((contains(A, C[j]) || contains(B, C[j])) && !contains(D, C[j])) {
			out[i++] = C[j];
		}
	}
	out[i] = '\0';
}
List* calc_set(const List* A, const List* B, const List* C, const List* D) 
{
	List* head = nullptr;
	List* tail = nullptr;

	for (const List* p = C; p != nullptr; p = p->next) {
		wchar_t ch = p->el;
		if ((contains(A, ch) || contains(B, ch)) && !contains(D, ch)) {
			List* node = new List(ch);
			if (!head) {
				head = node;
				tail = node;
			} else {
				tail->next = node;
				tail = node;
			}
		}
	}
	return head;
}
uint64_t calc_set(uint64_t A, uint64_t B, uint64_t C, uint64_t D)
{
	return (A | B) & C & (~D);
}

void benchmark(const wchar_t (&A)[33], const wchar_t (&B)[33],
		const wchar_t (&C)[33], const wchar_t (&D)[33])
{
	auto t_start = std::chrono::high_resolution_clock::now();

	wchar_t E[33];
	calc_set(A, B, C, D, E);

	auto t_end = std::chrono::high_resolution_clock::now();
	double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::wcout << L"Time elapsed (array): " << time_elapsed << '\n';

	std::wcout << L"Resulting set E: ";
	print_set(E);
}
void benchmark(const List* A, const List* B, const List* C, const List* D)
{
	auto t_start = std::chrono::high_resolution_clock::now();

	List* E = calc_set(A, B, C, D);

	auto t_end = std::chrono::high_resolution_clock::now();
	double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::wcout << L"Time elapsed (list): " << time_elapsed << '\n';

	std::wcout << L"Resulting set E: ";
	print_set(E);
}
void benchmark(uint64_t A, uint64_t B, uint64_t C, uint64_t D)
{
	auto t_start = std::chrono::high_resolution_clock::now();

	uint64_t E = calc_set(A, B, C, D);

	auto t_end = std::chrono::high_resolution_clock::now();
	double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::wcout << L"Time elapsed (quad word): " << time_elapsed << '\n';

	std::wcout << L"Resulting set E: ";
	print_set(E);
}
