#ifndef SET_H
#define SET_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <queue>
#include <cmath>
#include <set>
#include <forward_list>
#include <memory>

namespace hset {

    template<class ValueType> class Set {

    private:
        struct node {
            ValueType key;
            int height;
            node*  leftNode;
            node*  rightNode;
            node*  parentNode;

            node(ValueType val = ValueType()) : key(val), height(1), leftNode(nullptr), rightNode(nullptr), parentNode(nullptr) {}
        };

        node*  root = nullptr;
        node*  endElem = new node();

        size_t sz = 0;


        int Height(node*  curNode) const {
            if (curNode == nullptr) {
                return 0;
            }
            return curNode->height;
        }

        int BalFact(node*  curNode) const {
            return Height(curNode->rightNode) - Height(curNode->leftNode);
        }

        void HeightRecalc(node*  curNode) {
            curNode->height = std::max(Height(curNode->leftNode), Height(curNode->rightNode)) + 1;
        }

        node*  RotateRight(node*  curNode)
        {
            node*  leftNode = curNode->leftNode;
            if (curNode->parentNode == nullptr)
                leftNode->parentNode = nullptr;
            curNode->leftNode = leftNode->rightNode;
            if (curNode->leftNode != nullptr) {
                curNode->leftNode->parentNode = curNode;
            }
            leftNode->rightNode = curNode;
            if (leftNode->rightNode != nullptr) {
                leftNode->rightNode->parentNode = leftNode;
            }
            HeightRecalc(curNode);
            HeightRecalc(leftNode);
            return leftNode;
        }

        node*  RotateLeft(node*  curNode)
        {
            node*  rightNode = curNode->rightNode;
            if (curNode->parentNode == nullptr)
                rightNode->parentNode = nullptr;
            curNode->rightNode = rightNode->leftNode;
            if (curNode->rightNode != nullptr) {
                curNode->rightNode->parentNode = curNode;
            }
            rightNode->leftNode = curNode;
            if (rightNode->leftNode != nullptr) {
                rightNode->leftNode->parentNode = rightNode;
            }
            HeightRecalc(curNode);
            HeightRecalc(rightNode);
            return rightNode;
        }

        node*  BalanceNode(node*  curNode) {
            HeightRecalc(curNode);
            if (BalFact(curNode) >= 2) {
                if (Height(curNode->rightNode->leftNode) > Height(curNode->rightNode->rightNode)) {
                    curNode->rightNode = RotateRight(curNode->rightNode);
                    if (curNode->rightNode != nullptr) {
                        curNode->rightNode->parentNode = curNode;
                    }
                }
                return RotateLeft(curNode);
            }
            else if (BalFact(curNode) <= -2) {
                if (Height(curNode->leftNode->leftNode) < Height(curNode->leftNode->rightNode)) {
                    curNode->leftNode = RotateLeft(curNode->leftNode);
                    if (curNode->leftNode != nullptr) {
                        curNode->leftNode->parentNode = curNode;
                    }
                }
                return RotateRight(curNode);
            }
            return curNode;
        }

        node*  InsertImpl(node*  curNode, ValueType newKey) {
            if (curNode == nullptr) {
                ++sz;
                return new node(newKey);
            }
            if (newKey < curNode->key) {
                curNode->leftNode = InsertImpl(curNode->leftNode, newKey);
                if (curNode->leftNode != nullptr) {
                    curNode->leftNode->parentNode = curNode;
                }
            }
            else if (curNode->key < newKey) {
                curNode->rightNode = InsertImpl(curNode->rightNode, newKey);
                if (curNode->rightNode != nullptr) {
                    curNode->rightNode->parentNode = curNode;
                }
            }
            return BalanceNode(curNode);
        }

        node*  FindMin(node*  curNode) const {
            if (curNode->leftNode == nullptr) {
                return curNode;
            }
            return FindMin(curNode->leftNode);
        }

        node* FindMax(node* curNode) const {
            if (curNode->rightNode == nullptr) {
                return curNode;
            }
            return FindMax(curNode->rightNode);
        }

        node* FindVal(node* curNode, const ValueType key) const {
            if (curNode == nullptr)
                return nullptr;
            if (curNode->key < key) {
                return FindVal(curNode->rightNode, key);
            }
            else if (key < curNode->key) {
                node* resNode = FindVal(curNode->leftNode, key);
                if (resNode == nullptr) {
                    return curNode;
                }
                return resNode;
            }
            return curNode;
        }

        node*  RemoveMin(node* curNode) noexcept
        {
            if (curNode->leftNode == nullptr) {
                return curNode->rightNode;
            }
            curNode->leftNode = RemoveMin(curNode->leftNode);
            if (curNode->leftNode != nullptr) {
                curNode->leftNode->parentNode = curNode;
            }
            return BalanceNode(curNode);
        }

        node*  RemoveImpl(node*  curNode, ValueType delKey) {
            if (curNode == nullptr) {
                return nullptr;
            }
            if (delKey < curNode->key) {
                curNode->leftNode = RemoveImpl(curNode->leftNode, delKey);
                if (curNode->leftNode != nullptr) {
                    curNode->leftNode->parentNode = curNode;
                }
                return BalanceNode(curNode);
            }
            else if (curNode->key < delKey) {
                curNode->rightNode = RemoveImpl(curNode->rightNode, delKey);
                if (curNode->rightNode != nullptr) {
                    curNode->rightNode->parentNode = curNode;
                }
                return BalanceNode(curNode);
            }
            else {
                --sz;
                node* leftNode = curNode->leftNode, *rightNode = curNode->rightNode;
                node* oldParentNode = curNode->parentNode;
                delete curNode;
                if (rightNode == nullptr) {
                    if (leftNode != nullptr)
                        leftNode->parentNode = oldParentNode;
                    return leftNode;
                }
                node* newCurRoot = FindMin(rightNode);
                newCurRoot->rightNode = RemoveMin(rightNode);
                if (newCurRoot->rightNode != nullptr) {
                    newCurRoot->rightNode->parentNode = newCurRoot;
                }
                newCurRoot->leftNode = leftNode;
                if (newCurRoot->leftNode != nullptr) {
                    newCurRoot->leftNode->parentNode = newCurRoot;
                }
                newCurRoot->parentNode = oldParentNode;
                return BalanceNode(newCurRoot);
            }
        }

        node* NextNode(node* curNode) const {
            if (curNode->rightNode != nullptr) {
                curNode = curNode->rightNode;
                while (curNode->leftNode != nullptr) {
                    curNode = curNode->leftNode;
                }
                return curNode;
            }
            if (curNode == root)
                return endElem;
            node* lastNode = curNode;
            curNode = curNode->parentNode;
            while (curNode != nullptr) {
                if (curNode->leftNode == lastNode) {
                    return curNode;
                }
                lastNode = curNode;
                if (curNode == root)
                    curNode = nullptr;
                else
                    curNode = curNode->parentNode;
            }
            return endElem;
        }

        node* PrevNode(node* curNode) const {
            if (curNode == endElem)
                return FindMax(root);
            if (curNode->leftNode != nullptr) {
                curNode = curNode->leftNode;
                while (curNode->rightNode != nullptr) {
                    curNode = curNode->rightNode;
                }
                return curNode;
            }
            if (curNode == root)
                return nullptr;
            node* lastNode = curNode;
            curNode = curNode->parentNode;
            while (curNode != nullptr) {
                if (curNode->rightNode == lastNode) {
                    return curNode;
                }
                lastNode = curNode;
                if (curNode == root)
                    curNode = nullptr;
                else
                    curNode = curNode->parentNode;
            }
            return nullptr;
        }

        void ClearSet() noexcept {
            for (int i = 0; i < sz; ++i) {
                node* delNode = FindMin(root);
                root = RemoveMin(root);
                if (root != nullptr)
                    root->parentNode = nullptr;
                delete delNode;
            }
            root = nullptr;
            sz = 0;
        }

    public:
        class iterator {

        private:
            node* curPtr = nullptr;
            const Set* curSet = nullptr;

        public:
            iterator(node* newCurPtr = nullptr, const Set* newcurSet = nullptr) :
                curPtr(newCurPtr), curSet(newcurSet) {}

            iterator& operator++ () {
                curPtr = curSet->NextNode(curPtr);
                return *this;
            }

            iterator& operator++ (int) {
                curPtr = curSet->NextNode(curPtr);
                return *this;
            }

            iterator& operator-- () {
                curPtr = curSet->PrevNode(curPtr);
                return *this;
            }

            iterator& operator-- (int) {
                curPtr = curSet->PrevNode(curPtr);
                return *this;
            }

            const ValueType operator* () const {
                return curPtr->key;
            }

            const ValueType* operator-> () const {
                return &(curPtr->key);
            }

            bool operator== (const iterator& other) const {
                return other.curPtr == curPtr;
            }

            bool operator!= (const iterator& other) const {
                return !(*this == other);
            }
        };

        iterator begin() const {
            if (root == nullptr)
                return iterator(endElem, this);
            return iterator(FindMin(root), this);
        }

        iterator end() const {
            return iterator(endElem, this);
        }

        size_t size() const {
            return sz;
        }

        bool empty() const {
            return sz == 0;
        }

        void insert(ValueType newKey) {
            root = InsertImpl(root, newKey);
            if (root != nullptr)
                root->parentNode = nullptr;
        }

        void erase(ValueType delKey) {
            root = RemoveImpl(root, delKey);
            if (root != nullptr)
                root->parentNode = nullptr;
        }

        Set() {}

        template<typename ForwardIt>
        Set(ForwardIt first, ForwardIt last) {
            while (first != last) {
                insert(*first);
                ++first;
            }
        }

        Set(const std::initializer_list<const ValueType>& ValList) : Set(ValList.begin(), ValList.end()) {}

        Set(const Set& otherSet) : Set(otherSet.begin(), otherSet.end()) {}

        ~Set() {
            ClearSet();
            delete endElem;
        }

        Set& operator= (const Set& other) {
            if (other.endElem == endElem)
                return *this;
            ClearSet();
            for (auto curIter = other.begin(); curIter != other.end(); ++curIter) {
                insert(*curIter);
            }
            return *this;
        }

        iterator find(ValueType key) const {
            node* curNode = FindVal(root, key);
            if (curNode == nullptr || curNode->key < key || key < curNode->key)
                return iterator(endElem, this);
            return iterator(curNode, this);
        }

        iterator lower_bound(ValueType key) const {
            node* curNode = FindVal(root, key);
            if (curNode == nullptr)
                return iterator(endElem, this);
            return iterator(curNode, this);
        }
    };

}
#endif // !SET_H 