#pragma once

#include <initializer_list>

#include <random>

template<typename T>
class Set {
private:

    struct Node;

public:

    class Iterator {
    public:
        void Print() {
            std::cout << node_ << " ";
        }

        explicit Iterator(Node *node) : node_(node) {}

        void operator++() {
            if (node_ != nullptr) {
                if (node_->right_ != nullptr) {
                    node_ = Min(node_->right_);
                    return;
                }
                while (node_->up_ != nullptr) {
                    if (node_->up_ != nullptr and node_->up_->key_ > node_->key_) {
                        node_ = node_->up_;
                        return;
                    }
                    node_ = node_->up_;
                }
                node_ = nullptr;
            }
        }

        void operator--() {
            if (node_ != nullptr) {
                if (node_->left_ != nullptr) {
                    node_ = Max(node_->left_);
                    return;
                }
                while (node_->up_ != nullptr) {
                    if (node_->up_ != nullptr and node_->up_->key_ < node_->key_) {
                        node_ = node_->up_;
                        return;
                    }
                    node_ = node_->up_;
                }
                node_ = nullptr;
            }
        }

        T operator*() {
            return node_->key_;
        }

        bool operator==(Set<T>::Iterator iter) {
            return node_ == iter.node_;
        }

        bool operator!=(Set<T>::Iterator iter) {
            return node_ != iter.node_;
        }

    private:
        Node *node_{nullptr};
    };

    Set() = default;

    Set(Set<T>::Iterator iter1, Set<T>::Iterator iter2) {
        for (auto iter = iter1; iter != iter2; ++iter) {
            Insert(*iter);
        }
    }

    Set(std::initializer_list<T> list) {
        for (auto &iter: list) {
            Insert(*iter);
        }
    }

    Set(const Set<T> &set) {
        for (auto iter = set.Begin(); iter != set.End(); ++iter) {
            Insert(*iter);
        }
    }

    Set(Set<T> &&set) : node_(std::move(node_)), size_(set.size_) {
        set.node_ = nullptr;
        set.size_ = 0;
    }

    ~Set() {
        GarbageCollect(node_);
    }

    Set<T> &operator=(const Set<T> &set) {
        GarbageCollect(node_);
        node_ = nullptr;
        size_ = 0;
        for (auto iter = set.Begin(); iter != set.End(); ++iter) {
            Insert(*iter);
        }
    }

    Set<T> &operator=(Set<T> &&set) {
        GarbageCollect(node_);
        size_ = set.size_;
        node_ = std::move(set.node_);
        set.node_ = nullptr;
        set.size_ = 0;
    }

    // iterators

    Set<T>::Iterator Begin() const {
        if (node_ == nullptr) {
            return Iterator(nullptr);
        }
        return Iterator(Min(node_));
    }

    Set<T>::Iterator End() const {
        return Iterator(nullptr);
    }

    Set<T>::Iterator Rbegin() const {
        return Iterator(nullptr);
    }

    Set<T>::Iterator Rend() const {
        if (node_ == nullptr) {
            return Iterator(nullptr);
        }
        return Iterator(Max(node_));
    }

    // insert and erase

    void Insert(const T &element) {
        std::pair<Node *, Node *> pair = Split(node_, element);
        if (pair.second != nullptr) {
            if (pair.second->key_ == element) {
                return;
            }
        }
        ++size_;
        pair.first = Merge(pair.first, new Node(element));
        node_ = Merge(pair.first, pair.second);
    }

    void Insert(T &&element) {
        std::pair<Node *, Node *> pair = Split(node_, element);
        if (pair.second != nullptr) {
            if (pair.second->key_ == element) {
                return;
            }
        }
        ++size_;
        pair.first = Merge(pair.first, new Node(std::move(element)));
        node_ = Merge(pair.first, pair.second);
    }

    void Erase(const T &element) {
        if (node_ == nullptr) {
            return;
        }
        Node *node = node_;
        if (node->key_ == element) {
            if (node->left_ != nullptr) {
                node->left_->up_ = nullptr;
            }
            if (node->right_ != nullptr) {
                node->right_->up_ = nullptr;
            }
            node = Merge(node->left_, node->right_);
            --size_;
            delete node_;
            node_ = node;
            return;
        }
        while (true) {
            if (node->left_ != nullptr && node->left_->key_ == element) {
                if (node->left_->right_ != nullptr) {
                    node->left_->right_->up_ = nullptr;
                }
                if (node->left_->left_ != nullptr) {
                    node->left_->left_->up_ = nullptr;
                }
                Node *temp_node = Merge(node->left_->right_, node->left_->left_);
                --size_;
                delete node->left_;
                node->left_ = temp_node;
                if (temp_node != nullptr) {
                    temp_node->up_ = node;
                }
                return;
            }
            if (node->right_ != nullptr && node->right_->key_ == element) {
                if (node->right_->left_ != nullptr) {
                    node->right_->left_->up_ = nullptr;
                }
                if (node->right_->right_ != nullptr) {
                    node->right_->right_->up_ = nullptr;
                }
                Node *temp_node = Merge(node->right_->right_, node->right_->left_);
                --size_;
                delete node->right_;
                node->right_ = temp_node;
                if (temp_node != nullptr) {
                    temp_node->up_ = node;
                }
                return;
            }

            if (element < node->key_) {
                if (node->left_ != nullptr) {
                    node = node->left_;
                } else {
                    break;
                }
            } else {
                if (node->right_ != nullptr) {
                    node = node->right_;
                } else {
                    break;
                }
            }
        }
    }

    Iterator Find(const T& element) {
        return Iterator(find(node_, element));
    }

    // Other operations

    [[nodiscard]] size_t Size() const {
        return size_;
    }

    [[nodiscard]] bool Empty() const {
        return (size_ == 0);
    }

    void Clear() {
        GarbageCollect(node_);
        node_ = nullptr;
        size_ = 0;
    }

    void Print() { // DEBUG
        print(node_);
    } // DEBUG

private:

    struct Node {
        T key_;
        int priprity_{0};
        Node *up_{nullptr};
        Node *left_{nullptr};
        Node *right_{nullptr};
    public:
        explicit Node(const T &key) : key_(key), priprity_(generator()) {}
    };

    static void print(Node *node_) {
        if (node_ != nullptr) {
            std::cout << std::endl;
            std::cout << node_->up_;
            std::cout << std::endl;
            std::cout << node_->key_ << " " << node_->priprity_ << " " << node_;
            std::cout << std::endl;
            std::cout << node_->left_ << " " << node_->right_;
            std::cout << std::endl;
            print(node_->left_);
            print(node_->right_);
        }
    } // DEBUG

    static Node *Min(Node *node) {
        if (node->left_ != nullptr) {
            return Min(node->left_);
        } else {
            return node;
        }
    } // Спуск до минимального элемента в данном поддереве

    static Node *Max(Node *node) {
        if (node->right_ != nullptr) {
            return Max(node->right_);
        } else {
            return node;
        }
    } // Спуск до максимального элемента в данном поддереве

    static Node* find(Node* node, const T& element) {
        if (node != nullptr and element == node->key_) {
            return node;
        } else if (node != nullptr and element != node->key_) {
            if (element < node->key_) {
                return find(node->left_, element);
            } else {
                return find(node->right_, element);
            }
        } else {
            return nullptr;
        }
    }

    static Node *Merge(Node *node1, Node *node2) {
        if (node1 == nullptr || node2 == nullptr) {
            return node1 ? node1 : node2;
        }
        if (node1->priprity_ > node2->priprity_) {
            if (node1->right_ != nullptr) {
                node1->right_->up_ = nullptr;
            }
            node1->right_ = Merge(node1->right_, node2);
            if (node1->right_ != nullptr) {
                node1->right_->up_ = node1;
            }
            return node1;
        } else {
            if (node2->left_ != nullptr) {
                node2->left_->up_ = nullptr;
            }
            node2->left_ = Merge(node1, node2->left_);
            if (node2->left_ != nullptr) {
                node2->left_->up_ = node2;
            }
            return node2;
        }
    }

    static std::pair<Node *, Node *> Split(Node *node, const T &key) {
        if (node == nullptr) {
            return std::pair<Node *, Node *>(nullptr, nullptr);
        }
        if (node->key_ < key) {
            if (node->right_ != nullptr) {
                node->right_->up_ = nullptr;
            }
            std::pair<Node *, Node *> pair = Split(node->right_, key);
            node->right_ = pair.first;
            if (node->right_ != nullptr) {
                node->right_->up_ = node;
            }
            if (pair.second != nullptr) {
                pair.second->up_ = nullptr;
            }
            return std::pair<Node *, Node *>(node, pair.second);
        } else {
            std::pair<Node *, Node *> pair = Split(node->left_, key);
            node->left_ = pair.second;
            if (pair.second != nullptr) {
                pair.second->up_ = node;
            }
            if (pair.first != nullptr) {
                pair.first->up_ = nullptr;
            }
            return std::pair<Node *, Node *>(pair.first, node);
        }
    }

    static void GarbageCollect(Node *node) {
        if (node != nullptr) {
            GarbageCollect(node->left_);
            GarbageCollect(node->right_);
            delete node;
        }
    }

    static std::minstd_rand generator;
    Node *node_{nullptr};
    size_t size_{0};

};

template<typename T>
std::minstd_rand Set<T>::generator;