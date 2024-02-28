#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>


typedef std::map<std::string, std::vector<int>> EntityMap;

void add(EntityMap &store, const std::string &tag, int value) {
    store[tag].push_back(value);
}

std::vector<int> &get(EntityMap &store, const std::string &tag) {
    return store[tag];
}

void remove(std::vector<int> &values, int value) {

    values.erase(std::remove_if(values.begin(),
                                values.end(),
                                [value](int e) { return e == value; }),
                 values.end());
}

void remove(EntityMap &store, int value) {
    for (auto &kv: store) {
        remove(kv.second, value);
    }
}

void print(const EntityMap &store) {
    std::cout << "Store:\n";
    for (const auto &kv: store) {
        std::cout << "tag: " << kv.first << ": (";
        for (auto el: kv.second) {
            std::cout << " " << el;
        }
        std::cout << ")\n";
    }
    std::cout << "------\n";
}

void print(const std::vector<int> &v) {
    std::cout << "Vector contains:";
    for (auto el: v) {
        std::cout << " " << el;
    }
    std::cout << '\n';
}

int main() {
    EntityMap store;

    add(store, "player", 10);
    add(store, "player", 20);
    print(store);

    add(store, "player", 30);
    add(store, "bullet", 5);
    add(store, "bullet", 10);
    print(store);

    print(get(store, "player"));
    print(get(store, "bullet"));
    print(get(store, "non-existent"));

    remove(store, 10);
    print(store);

    return 0;
}
