#ifndef SET_LIST_H
#define SET_LIST_H

#include <iostream>
#include <chrono>
#include <cstdlib>


class SetList
{
private:
	struct Node {
		wchar_t el;
		Node* next;

		Node(wchar_t e, Node* n = nullptr) : el(e), next(n) {}
		~Node() { delete next; }
		
		void* operator new(size_t size) {
			return ::operator new(size);
		}
		void operator delete(void* p) noexcept {
			::operator delete(p);
		}
	};
	Node* head;

	bool contains(wchar_t elem) const {
		for (Node* p = head; p; p = p->next) {
			if (p->el == elem) return true;
		}
		return false;
	}
	void clear() {
		delete head;
		head = nullptr;
	}

public:
	SetList() : head(nullptr) {}

	SetList(const wchar_t (&arr)[33]) : head(nullptr) {
		Node** current = &head;
		for (int i = 0; i < 33 && arr[i] != 0; i++) {
			*current = new Node(arr[i]);
			current = &((*current)->next);
		}
	}

	~SetList() { clear(); }

	void print_set() {
		for (Node* p = head; p; p = p->next) {
			std::wcout << p->el;
		}
		std::wcout << '\n';
	}

	void calc_set(const SetList& A, const SetList& B, const SetList& C, const SetList& D) {
		clear();
		Node** current = &head;
		for (Node* p = A.head; p; p = p->next) {
			wchar_t ch = p->el;
			if ((B.contains(ch) && C.contains(ch)) && !D.contains(ch)) {
				*current = new Node(ch);
				current = &((*current)->next);
			}
		}
	}

	void benchmark(const SetList& A, const SetList& B, const SetList& C, const SetList& D) {
		auto t_start = std::chrono::high_resolution_clock::now();

		this->calc_set(A, B, C, D);

		auto t_end = std::chrono::high_resolution_clock::now();
		double time_elapsed = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		std::wcout << L"Time elapsed (SetList): " << time_elapsed << L" ms\n";
		std::wcout << L"Resulting set E: ";
		this->print_set();
	}
};


#endif
