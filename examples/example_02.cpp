#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::fstream fin("words_random.txt", std::ios::in);
    std::string line;
    std::vector<std::string> words;
    while (std::getline(fin, line)) {
        words.push_back(line);
    }
    fin.close();

    std::sort(words.begin(), words.end());

    for (auto& word : words) {
        std::cout << word << std::endl;
    }

    const std::string target = "spring";
    bool found = std::binary_search(words.begin(), words.end(), target);

    auto it = std::lower_bound(words.begin(), words.end(), target);
    long long index = (found) ? std::distance(words.begin(), it) : -1;

    std::cout << "found: " << std::boolalpha << found << "\n";
    std::cout << "index: " << index << "\n";

    return 0;
}