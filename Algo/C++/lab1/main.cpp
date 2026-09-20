#include <chrono>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <ctime>


struct List {
	char el;
	List * next;

	List(char e, List *n = nullptr) : el(e), next(n) {}
	~List() { delete next; }
};

// Function to generate a random set of characters
void GenerateRandomSet(char* S)
{
	const char universe[] = "абвгдежзийклмнопрстуфхцчшщъыьэюя";
	int i = 0;
	for (int k = 0; universe[k] != '\0'; k++) {
		if (rand() % 2 == 1) {
			S[i++] = universe[k];
		}
	}
	S[i] = '\0';
}

bool contains(const List* l, char elem)
{
	for (; l != nullptr; l = l->next) {
		if (l->el == elem) {
			return true;
		}
	}
	return false;
}
bool contains(const char* arr, char elem)
{
	for (int i = 0; arr[i] != '\0'; i++) {
		if (arr[i] == elem) {
			return true;
		}
	}
	return false;
}

// Function to calculate the set of characters that are present in A, B, and C but not in D
void CalcSetArr(const char* A, const char* B, const char* C, const char* D, char* E)
{
	int i = 0;
	for (int j = 0; A[j] != '\0'; j++) 
	{
		char ch = A[j];
		if (contains(B, ch) && contains(C, ch) && !contains(D, ch)) 
		{
			if (!contains(E, ch)) 
			{
				E[i] = ch;
				i++;
				E[i] = '\0';
			}	
		}
	}
}

int main()
{
	/* const char* arr = "ABOBA";
	const List* l = new List('E'); */

	// Test data
	/* const char* A = "абвгде";
    const char* B = "бвгж";
    const char* C = "вгдз";
    const char* D = "г"; */

	char E[100], A[100], B[100], C[100], D[100];

	/* std::cout << contains(arr, 'E') << ' ' << contains(l, 'E') << '\n'; */

	const auto t_start = std::chrono::high_resolution_clock::now();

	GenerateRandomSet(A);
	GenerateRandomSet(B);
	GenerateRandomSet(C);
	GenerateRandomSet(D);

	std::cout << "Generated sets:\n";
    std::cout << "A = " << A << '\n';
    std::cout << "B = " << B << '\n';
    std::cout << "C = " << C << '\n';
    std::cout << "D = " << D << '\n';

	CalcSetArr(A, B, C, D, E);

	std::cout << "Resulting set E: " << E << '\n';

	const auto t_end = std::chrono::high_resolution_clock::now();
	const double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::cout << "Time elapsed: " << time_elapsed << '\n'; 
}
