#include <chrono>
#include <cstddef>
#include <iostream>

struct List {
	char el;
	List * next;

	List(char e, List *n = nullptr) : el(e), next(n) {}
	~List() { delete next; }
};

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

int main()
{
	const auto t_start = std::chrono::high_resolution_clock::now();

	const char* arr = "ABOBA";
	const List* l = new List('E');

	std::cout << contains(arr, 'E') << ' ' << contains(l, 'E') << '\n';

	const auto t_end = std::chrono::high_resolution_clock::now();
	const double time_elapsed = std::chrono::duration<double, std::milli>(
			t_end - t_start).count();
	std::cout << "Time elapsed: " << time_elapsed << '\n';
}
