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

    // stable_sort
    std::stable_sort(words.begin(), words.end());

    for (auto& word : words) {
        std::cout << word << std::endl;
    }

    const std::string target = "spring";
    // lower_bound
    auto it = std::lower_bound(words.begin(), words.end(), target);
    long long index = std::distance(words.begin(), it);
    std::cout << "lower_bound: " << target << "(" << index << ")\n";
    return 0;
}