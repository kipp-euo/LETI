#include <clocale>
#include <iostream>
#include <ctime>
#include "tree.h"

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(nullptr)));

    Tree myTree(0, 100, 20); // maxnum = 100 узлов, maxrow = 10 уровней
        myTree.MakeTree();

    if (myTree.exist()) {
        myTree.OutTree(); // Выводим графическое изображение дерева
        myTree.BFS();     // Выполняем обход и считаем Вариант 7
    }

    return 0;
}