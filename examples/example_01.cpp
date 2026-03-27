#include<map>
#include<vector>
#include<string>
#include<iostream>
using namespace std;
int main()
{
    vector<string> containers = {
        "vector", "list", "set", "map", "stack", "queue", "priority_queue", "unordered_map", "unordered_set"
    };
    cout << "Number of elements: " << containers.size() << endl;
    map<int, string> d;

    for (int i = 0; i < containers.size(); i++)
    {
        d.insert({ i, containers[i] });
    }
    for (auto it = d.rbegin(); it != d.rend(); ++it)
    {
        std::cout  << it->first  << ": "<< it->second  << std::endl;
    }
    return 0;
}