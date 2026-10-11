#ifndef SET_ARRAY_H
#define SET_ARRAY_H
#include <iostream>
#include <chrono>
#include <ctime>


class SetArray
{
    private: 
        wchar_t data[33];

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
            for (int i = 0; i < 33; i++) {
                data[i] = 0;
            }
        }

        SetArray(const wchar_t (&arr)[33]) {
            for (int i = 0; i < 33; i++) {
                data[i] = arr[i];
            }
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
                if ((B.contains(ch) && C.contains(ch)) && !D.contains(ch)) {
                    this->data[i++] = ch;
                }
            }
            this->data[i++] = '\0';
        }

        void benchmark(const SetArray& A,const SetArray& B, const SetArray& C, const SetArray& D)
        {
            auto t_start = std::chrono::high_resolution_clock::now();

            this->calc_set(A, B, C, D);

            auto t_end = std::chrono::high_resolution_clock::now();
            double time_elapsed = std::chrono::duration<double, std::milli>(
                    t_end - t_start).count();
            std::wcout << L"Time elapsed (SetArray): " << time_elapsed << '\n';

            std::wcout << L"Resulting set E: ";
            this->print_set();
        }
};

#endif