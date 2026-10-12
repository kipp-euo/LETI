#include <iostream>
#include <algorithm>
#include <vector>


int main()
{
	std::vector<int> aboba = {1, 221, 3, -4, 5};
	// std::sort(aboba.begin(), aboba.end());
	std::ranges::sort(aboba);

	for (auto elem : aboba) {
		std::cout << elem << '\n';
	}
}


