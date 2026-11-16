#include "tree.h"
#include <clocale>
#include <ctime>

int main() {
	setlocale(LC_ALL, "Russian");
	srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "==================================================\n";
	std::cout << "              ТЕСТ 1: ТЕСТ-КОНСТАНТА              \n";
	std::cout << "==================================================\n\n";

	Tree testTree;
	testTree.make_test_tree();
	if (testTree.exist()) {
		std::cout << "--- Структура тестового дерева ---\n";
		testTree.print_tree();
		std::cout << '\n';
		testTree.bfs();
	}
    
	Tree myTree(0, 100, 6); // maxnum = 100 узлов, maxrow = 10 уровней
	myTree.make_tree();

	if (myTree.exist()) {
		myTree.print_tree(); // Выводим графическое изображение дерева
		std::cout << '\n';
		myTree.bfs(); // Выполняем обход и считаем Вариант 7
	}

	return 0;
}
