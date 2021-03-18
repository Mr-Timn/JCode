#include <map>
#include <iostream>

int main(int argc, char** argv) {
    std::multimap<char, int> mm;

    mm.insert(std::pair<char, int>('a', 9));
    mm.insert(std::pair<char, int>('a', 2));
    mm.insert(std::pair<char, int>('a', 3));
    mm.insert(std::pair<char, int>('a', 4));
    mm.insert(std::pair<char, int>('a', 5));
    mm.insert(std::pair<char, int>('a', 6));
    mm.insert(std::pair<char, int>('a', 7));

    std::pair<std::multimap<char, int>::iterator, std::multimap<char, int>::iterator> ret;
    ret = mm.equal_range('a');

    std::multimap<char, int>::iterator itf = ret.first;
    std::multimap<char, int>::iterator ite = ret.second;
    ite--;

    std::cout << "Start: " << itf->second << std::endl;
    std::cout << "End: " << ite->second << std::endl;

    return 0;
}