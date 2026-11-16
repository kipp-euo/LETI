#ifndef SET_ARRAY_H
#define SET_ARRAY_H

#include <iostream>
#include <chrono>
#include <ctime>

class SetArray
{
private: 
    wchar_t data[33];
    int object_id;

    static int get_next_id() {
        static int counter = 0;
        return ++counter;
    }

    bool contains(wchar_t elem) const
    {
        for (int i = 0; i < 33; i++) {
            if (data[i] == elem) {
                return true;
            }
        }
        return false;
    }

public:
    SetArray() {
        object_id = get_next_id();
        std::wcout << L"[TRACE] Конструктор по умолчанию вызван для объекта Set #" << object_id << L"\n";
        for (int i = 0; i < 33; i++) {
            data[i] = 0;
        }
    }

    SetArray(const wchar_t (&arr)[33]) {
        object_id = get_next_id();
        std::wcout << L"[TRACE] Конструктор инициализации вызван для объекта Set #" << object_id << L"\n";
        for (int i = 0; i < 33; i++) {
            data[i] = arr[i];
        }
    }

    ~SetArray() {
        std::wcout << L"[TRACE] Деструктор вызван для уничтожения объекта Set #" << object_id << L"\n";
    }

    void print_set()
    {
        for (int i = 0; i < 33; i++) {
            if (data[i] != 0) {
                std::wcout << data[i];
            }
        }
        std::wcout << '\n';
    }
    
    void calc_set(const SetArray& A, const SetArray& B, const SetArray& C, const SetArray& D)
    {
        int i = 0;
        for (int j = 0; j < 33; j++) {
            wchar_t ch = A.data[j];
            if (ch != 0) {
                if (B.contains(ch) && C.contains(ch) && !D.contains(ch)) {
                    bool exists = false;
                    for (int k = 0; k < i; k++) {
                        if (this->data[k] == ch) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        this->data[i++] = ch;
                    }
                }
            }
        }
        this->data[i] = '\0';
    }

    void benchmark(const SetArray& A, const SetArray& B, const SetArray& C, const SetArray& D)
    {
        const int iterations = 1000000;
        auto t_start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; i++) {
            this->calc_set(A, B, C, D);
        }

        auto t_end = std::chrono::high_resolution_clock::now();
        double total_time = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        double avg_time = total_time / iterations;

        std::wcout << L"Average time per iteration (SetArray): " << avg_time << L" ms\n";
        std::wcout << L"Resulting set E: ";
        this->print_set();
    }
};

#endif