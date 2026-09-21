#include <iostream>
#include <vector>


int main()
{
	// std::vector<int> data = {1, 2, 3, 4, 5};
	std::vector<char> chars(500);
	for (auto elem : chars) {
		std::cout << elem << ' ';
	}
	std::cout << '\n';
}


