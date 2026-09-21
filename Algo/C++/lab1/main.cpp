#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <clocale>

struct List {
	wchar_t el;
	List * next;

	List(wchar_t e, List *n = nullptr) : el(e), next(n) {}
	~List() { delete next; }
};

// Function to generate a random set of characters
void set_fill_random(wchar_t (&result)[34]);

// Function to get input from the user
void input_set(wchar_t (&result)[34], const wchar_t* name);
void print_set(const wchar_t (&set)[34]);

bool contains(const wchar_t (&arr)[34], wchar_t elem);
bool contains(const List* l, wchar_t elem);

void calc_set_arr(const wchar_t (&A)[34], const wchar_t (&B)[34],
		const wchar_t (&C)[34], const wchar_t (&D)[34],
		wchar_t (&out)[34]);

// Functions to convert an array to a linked list
List* arr_to_list(const wchar_t (&arr)[34]);
void print_list(const List* head);

// Function to calculate the resulting set using linked lists
List* calc_set_list(const List* A, const List* B, const List* C, const List* D);

int main()
{
	// For cyrillic letters
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale(""));
	// Random generation
	srand((unsigned)time(NULL));
	// Time benchmarking
	/* const auto t_start = std::chrono::high_resolution_clock::now(); */

	// MAIN LOGIC
	wchar_t A[34], B[34], C[34], D[34], E[34];
	// Example
	/* wchar_t A[] = L"бвгде";
	wchar_t B[] = L"авгде";
	wchar_t C[] = L"абгде";
	wchar_t D[] = L"абвде"; */

	// Example from input
	input_set(A, L"A");
    input_set(B, L"B");
	input_set(C, L"C");
    input_set(D, L"D");


	// Random generated examples
	// wchar_t A[34], B[34], C[34], D[34];
	// set_fill_random(A);
	// set_fill_random(B);
	// set_fill_random(C);
	// set_fill_random(D);

	std::wcout << "Sets:\n";
	print_set(A);
	print_set(B);
	print_set(C);
	print_set(D);

	calc_set_arr(A, B, C, D, E);

	std::wcout << L"Resulting set (array) E: ";
	print_set(E);

	// Convert arrays to linked lists
	List* LA = arr_to_list(A);
    List* LB = arr_to_list(B);
    List* LC = arr_to_list(C);
    List* LD = arr_to_list(D);

	// Calculate the resulting set using linked lists
	List* LE = calc_set_list(LA, LB, LC, LD);

	std::wcout << L"Resulting set (list)) E: ";
    print_list(LE);
	// END

	/* const auto t_end = std::chrono::high_resolution_clock::now();
	const double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::wcout << "Time elapsed: " << time_elapsed << '\n';  */
}

// Functions to input or print sets
void input_set(wchar_t (&result)[34], const wchar_t* name)
{
	std::wcout << L"Enter set " << name << L": ";
	std::wcin.getline(result, 34);
}

void print_set(const wchar_t (&set)[34])
{
	for (int i = 0; set[i] != '\0'; i++) {
		std::wcout << set[i];
	}
	std::wcout << '\n';
}

void print_list(const List* head)
{
	for (const List* p = head; p != nullptr; p = p->next) {
        std::wcout << p->el;
    }
    std::wcout << L'\n';
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

// Function arr to list
List* arr_to_list(const wchar_t (&arr)[34])
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

void calc_set_arr(const wchar_t (&A)[34], const wchar_t (&B)[34],
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

List* calc_set_list(const List* A, const List* B, const List* C, const List* D) 
{
    List* head = nullptr;
    List* tail = nullptr;

    for (const List* p = A; p != nullptr; p = p->next) {
        wchar_t ch = p->el;
        if (contains(B, ch) && contains(C, ch) && !contains(D, ch)) {
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
