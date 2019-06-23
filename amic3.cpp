#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <functional>
#include <set>
#include <memory>

template<class ValueType> class Set {

private:
    struct node {
        ValueType key;
        int height;
        std::shared_ptr<node> leftNode;
        std::shared_ptr<node> rightNode;
        std::shared_ptr<node> parentNode;

        node(ValueType val = ValueType()) : key(val), height(1), leftNode(nullptr), rightNode(nullptr), parentNode(nullptr) {}
    };
public:
    int raksla(std::shared_ptr<node> fuckme = std::shared_ptr<node>(new node)) {
        node a;
        
        fuckme->leftNode.reset(new node);
        return 0;
    }
};

int main() {
    Set<int> a;
    a.raksla();
    return 0;
}
    