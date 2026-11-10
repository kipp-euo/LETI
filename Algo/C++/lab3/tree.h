#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

class Node {
private:
	int data;
	Node* left;
	Node* right;

public:
	explicit Node(int val, Node* l = nullptr, Node* r = nullptr)
	    : data(val), left(l), right(r) {}
	~Node() {
		delete left;
		delete right;
	}

	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

	friend class Tree;
};

class Tree {
private:
	Node* root;
	int num;
	int max_num;
	size_t max_row;
	size_t max_data_width;

	// Запрет копирования и перемещения
	Tree(const Tree&) = delete;
	Tree(Tree&&) = delete;
	Tree& operator=(const Tree&) = delete;
	Tree& operator=(Tree&&) = delete;

	// Рекурсивная функция для создания дерева
	Node* build_tree(size_t row) {
		if (row >= max_row || num >= max_num || (row > 0 && rand() % 3 == 0)) {
			return nullptr;
		}

		num++;
		Node* node = new Node(0);
		node->left = build_tree(row + 1);
		node->right = build_tree(row + 1);

		return node;
	}

	// Рекурсивная функция для симметричной маркировки дерева
	void make_symmetric_marking(Node* node, int& counter) {
		if (!node) {
			return;
		}

		make_symmetric_marking(node->left, counter);
		node->data = ++counter;
		max_data_width =
		    std::max(max_data_width, std::to_string(counter).length());
		make_symmetric_marking(node->right, counter);
	}

public:
	Tree(int start_num = 0, int maxnum = 10, size_t maxrow = 4)
	    : root(nullptr), num(start_num), max_num(maxnum), max_row(maxrow),
	      max_data_width(1) {}

	~Tree() { delete root; }

	void make_tree() {
		delete root;
		root = nullptr;
		num = 0;
		max_data_width = 1;
		root = build_tree(0);
		int counter = 0;
		make_symmetric_marking(root, counter);
	}

	bool exist() const { return root != nullptr; }

	// Обход в ширину + вычисление Варианта 7
	void bfs() const {
		if (!root) {
			return;
		}

		std::queue<const Node*> q;
		q.push(root);

		// Счетчик вершин с <= 1 потомком
		int countTargetNodes = 0;

		std::cout << "BFS traversal (Symmetrically marked values): ";

		while (!q.empty()) {
			const Node* curr = q.front();
			q.pop();

			std::cout << curr->data << " ";

			int childrenCount = 0;
			if (curr->left) {
				childrenCount++;
				q.push(curr->left);
			}
			if (curr->right) {
				childrenCount++;
				q.push(curr->right);
			}

			// Условие варианта 7: не более одного потомка (0 или 1)
			if (childrenCount <= 1) {
				countTargetNodes++;
			}
		}

		std::cout << "\nVariant 7 result (nodes with <= 1 child): "
		          << countTargetNodes << "\n";
	}

	// Выдача дерева на экран
	void print_tree() const {
		if (!exist()) {
			std::cout << "Tree is empty.\n";
			return;
		}

		// Разворачиваем дерево по уровням как полное бинарное (с nullptr на
		// месте пустых)
		std::vector<std::vector<const Node*>> levels;
		levels.push_back(std::vector<const Node*>{root});

		while (true) {
			std::vector<const Node*> next;
			bool has_children = false;
			for (const Node* n : levels.back()) {
				next.push_back(n ? n->left : nullptr);
				next.push_back(n ? n->right : nullptr);
				if (n && (n->left || n->right)) {
					has_children = true;
				}
			}
			if (!has_children) {
				break;
			}
			levels.push_back(std::move(next));
		}

		const size_t cell = max_data_width + 1;
		const size_t depth = levels.size();

		for (size_t i = 0; i < depth; i++) {
			const size_t slot = cell << (depth - 1 - i);
			std::string line;

			for (const Node* n : levels[i]) {
				const std::string s = n ? std::to_string(n->data) : "";
				const size_t left_pad = (slot - s.length()) / 2;
				line += std::string(left_pad, ' ');
				line += s;
				line += std::string(slot - s.length() - left_pad, ' ');
			}

			line.erase(line.find_last_not_of(' ') + 1);
			std::cout << line << '\n';
		}
	}
};

#endif
