#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>

class Node {
    int data;       // Метка / значение вершины
    Node* left;     // Указатель на левого потомка
    Node* right;    // Указатель на правого потомка

public:
    Node(int val, Node* l = nullptr, Node* r = nullptr) 
        : data(val), left(l), right(r) {}
    ~Node() { 
        delete left; 
        delete right; 
    }
    friend class Tree;  
};

class Tree {
    Node* root;             // Указатель на корень дерева
    char num, maxnum;       // Счётчик тегов и максимальный тег
    const int trwide = 200;  // Ширина изображения дерева
    int maxrow, offset;     // Максимальная глубина, смещение корня

    // Запрет копирования и перемещения
    Tree(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree& operator=(Tree&&) = delete;

    // Рекурсивная функция для создания дерева
    Node* MakeNode(int row) {
        if (row >= maxrow || num >= maxnum || (row > 0 && rand() % 3 == 0)) {
            return nullptr;
        }

        num++;
        Node* newNode = new Node(0);
        newNode->left = MakeNode(row + 1);
        newNode->right = MakeNode(row + 1);

        return newNode;
    };

    // Рекурсивная функция для симметричной маркировки дерева
    void MakeSymmetricMarking(Node* node, int& counter) {
        if (!node) return;

        MakeSymmetricMarking(node->left, counter);
        node->data = ++counter;
        MakeSymmetricMarking(node->right, counter);
    };

    // Рекурсивная функция для вывода дерева на экран (исправленная)
    void OutNodes(Node* v, int r, int c, char** SCREEN, int current_offset) {
        if (!v) return;

        std::string valStr = std::to_string(v->data);
        for (size_t i = 0; i < valStr.length() && (c + i) < (size_t)trwide; ++i) {
            SCREEN[r][c + i] = valStr[i];
        }

        int next_offset = std::max(4, (int)(current_offset * 0.45)); // Уменьшаем смещение для потомков

        if (v->left) {
            OutNodes(v->left, r + 2, c - next_offset, SCREEN, next_offset);
        }
        if (v->right) {
            OutNodes(v->right, r + 2, c + next_offset, SCREEN, next_offset);
        }
    }

public:
    Tree(char num = 0, char maxnum = 10, int maxrow = 4)
        : root(nullptr), num(num), maxnum(maxnum), maxrow(maxrow), offset(0) {}

    ~Tree() {delete root;}

    void MakeTree() {
        delete root;
        root = nullptr;
        num = 0;
        root = MakeNode(0); 
        int counter = 0;
        MakeSymmetricMarking(root, counter);
    }

    bool exist() const {return root != nullptr;}

    // Обход в ширину + вычисление Варианта 7
    void BFS() {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);

        int countTargetNodes = 0; // Счетчик вершин с <= 1 потомком

        std::cout << "BFS traversal (Symmetrically marked values): ";

        while (!q.empty()) {
            Node* curr = q.front();
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

        std::cout << "\nVariant 7 result (nodes with <= 1 child): " << countTargetNodes << "\n";
    };

    // Выдача дерева на экран
    void OutTree() {
        if (!root) {
            std::cout << "Tree is empty.\n";
            return;
        }

        int screenRows = maxrow * 2 + 1;
        char** SCREEN = new char*[screenRows];
        for (int i = 0; i < screenRows; ++i) {
            SCREEN[i] = new char[trwide];
            for (int j = 0; j < trwide; ++j) SCREEN[i][j] = ' ';
        }

        int center_x = trwide / 2.5;
        int start_offset = trwide / 5; // Начальное смещение для корня
        OutNodes(root, 0, center_x, SCREEN, start_offset);

        std::cout << "\n--- Tree Structure ---\n";
        for (int i = 0; i < screenRows; ++i) {
            bool hasContent = false;
            for (int j = 0; j < trwide; ++j) {
                if (SCREEN[i][j] != ' ') hasContent = true;
            }
            if (hasContent) {
                int last_char = trwide - 1;
                while (last_char >= 0 && SCREEN[i][last_char] == ' ') {
                    last_char--;
                }
                for (int j = 0; j <= last_char; ++j) {
                    std::cout << SCREEN[i][j];
                }
                std::cout << "\n";
            }
        }
        std::cout << "---------------------------------------------------------------------------------------\n\n";

        for (int i = 0; i < screenRows; ++i) delete[] SCREEN[i];
        delete[] SCREEN;
    };   
};

#endif