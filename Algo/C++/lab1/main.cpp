#include <chrono>
#include <iostream>
#include <cstdlib>


struct List {
	char el;
	List * next;

	List(char e, List *n = nullptr) : el(e), next(n) {}
	~List() { delete next; }
};

// Function to generate a random set of characters
void set_fill_random(wchar_t (&result)[34]);

bool contains(const wchar_t (&arr)[34], wchar_t elem);
bool contains(const List* l, wchar_t elem);

void print_set(const wchar_t (&set)[34]);

void calc_set(const wchar_t (&A)[34], const wchar_t (&B)[34],
		const wchar_t (&C)[34], const wchar_t (&D)[34],
		wchar_t (&out)[34]);

int main()
{
	// For cyrillic letters
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale(""));
	// Random generation
	srand((unsigned)time(NULL));
	// Time benchmarking
	const auto t_start = std::chrono::high_resolution_clock::now();

	// MAIN LOGIC
	wchar_t E[34];
	// Example
	wchar_t A[34] = L"бвгде";
	wchar_t B[34] = L"авгде";
	wchar_t C[34] = L"абгде";
	wchar_t D[34] = L"абвде";
	// Example from input
	
	// Random generated examples
	// wchar_t A[34], B[34], C[34], D[34];
	// set_fill_random(A);
	// set_fill_random(B);
	// set_fill_random(C);
	// set_fill_random(D);

	std::wcout << "Generated sets:\n";
	print_set(A);
	print_set(B);
	print_set(C);
	print_set(D);

	calc_set(A, B, C, D, E);

	std::wcout << "Resulting set E: ";
	print_set(E);
	// END

	const auto t_end = std::chrono::high_resolution_clock::now();
	const double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::wcout << "Time elapsed: " << time_elapsed << '\n'; 
}

// Function to generate a random set of characters
void set_fill_random(wchar_t (&result)[34])
{
	const wchar_t alphabet[] = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
	int i = 0;
	for (const wchar_t& letter : alphabet) {
		if (rand() % 2 == 1) {
			result[i++] = letter;
		}
	}
	result[i] = '\0';
}

bool contains(const wchar_t (&arr)[34], wchar_t elem)
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

void print_set(const wchar_t (&set)[34])
{
	for (int i = 0; set[i] != '\0'; i++) {
		std::wcout << set[i];
	}
	std::wcout << '\n';
}

void calc_set(const wchar_t (&A)[34], const wchar_t (&B)[34],
		const wchar_t (&C)[34], const wchar_t (&D)[34],
		wchar_t (&out)[34])
{
	int i = 0;
	for (int j = 0; A[j] != '\0'; j++) {
		if (contains(B, A[j]) && contains(C, A[j]) && !contains(D, A[j])) {
			out[i++] = A[j];
		}
	}
	out[i] = '\0';
}
