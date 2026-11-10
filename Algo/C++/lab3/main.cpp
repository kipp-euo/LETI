#include "tree.h"
#include <clocale>
#include <ctime>

int main() {
	setlocale(LC_ALL, "Russian");
	srand(static_cast<unsigned int>(time(nullptr)));

	Tree myTree(0, 100, 6); // maxnum = 100 узлов, maxrow = 10 уровней
	myTree.make_tree();

	if (myTree.exist()) {
		myTree.print_tree(); // Выводим графическое изображение дерева
		std::cout << '\n';
		myTree.bfs(); // Выполняем обход и считаем Вариант 7
	}

	return 0;
}
