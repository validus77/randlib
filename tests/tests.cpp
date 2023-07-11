#include <iostream>
#include "SkipList.h"

using namespace validus_randlab;
int main(int argc, char** argv) {
    std::cout << "Test " << std::endl;
    SkipList<int> list;

    list.insert(5);
    list.insert(2);
    list.insert(6);
    list.insert(20);
    list.insert(32);
    list.insert(13);
    list.insert(56);
    list.insert(93);


    for (int v: list) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    if (list.find(13) != list.end()) {
        std::cout << "found 13" << std::endl;
    } else {
        std::cout << "not found 13" << std::endl;
    }

    list.erase(list.find(13));
    for (int v: list) {
        std::cout << v << " ";
    }

    std::cout << std::endl;

    SkipList<int>::iterator it = list.find(6);
    list.erase(it, it + 4);
    for (int v: list) {
        std::cout << v << " ";
    };

    std::cout << std::endl;
    return 0;
}
