//
// Created by explorer on 2020/12/24.
//

#ifndef RHTTPD_KEYMATCH_H
#define RHTTPD_KEYMATCH_H

#include <map>
#include <initializer_list>
#include <functional>

class NotFindException : public std::exception {
public:
    const char *what() const noexcept {
        return "key not find";
    }
};

template<typename K, typename V, typename Container>
class KeyNode {
public:
    KeyNode<K, V, Container> *add_sub(Container container, unsigned size) {
        if (size == 0) {
            return this;
        }
        K key = *container;
        auto it = this->sub_node.find(key);
        KeyNode<K, V, Container> *node;
        if (it == this->sub_node.end()) {
            node = new KeyNode<K, V, Container>;
            this->sub_node[key] = node;
        } else {
            node = (*it).second;
        }
        return node->add_sub(container + 1, size - 1);
    }

    bool del_sub(Container container, unsigned size) {
        if (size == 0) {
            this->content_value = false;
        } else {
            K key = *container;
            auto it = this->sub_node.find(key);
            if (it == this->sub_node.end()) {
                return false;
            }
            KeyNode<K, V, Container> *node = (*it).second;

            if (node->del_sub(container + 1, size - 1)) {
                delete node;
                this->sub_node.erase(it);
            }
        }

        if (this->sub_node.empty()) {
            return true;
        } else {
            return false;
        };
    }

    void set_value(V &value) {
//        std::swap(this->value, value);
        this->value = std::move(value);
        this->content_value = true;
    }

    V &get_value() {
        return this->value;
    }

    void iter(std::function<void(V &)> func) {
        for (auto p : this->sub_node) {
            p.second->iter(func);
        }
        if (this->content_value) {
            func(this->value);
        }
    }

    KeyNode<K, V, Container> *walk(Container container, unsigned size) {
        if (size == 0) {
            return this;
        }
        K key = *container;
        auto it = this->sub_node.find(key);
        if (it == this->sub_node.end()) {
            return nullptr;
        }
        auto node = (*it).second;
        return node->walk(container + 1, size - 1);
    }

    bool has_value() {
        return content_value;
    }

    ~KeyNode() {
        for (auto i : this->sub_node) {
            delete i.second;
        }
    }

private:
    std::map<K, KeyNode<K, V, Container> *> sub_node;
    V value{};
    bool content_value = false;
};

template<typename K, typename V, typename Container>
class KeyMatch : public KeyNode<K, V, Container> {
public:

    KeyMatch() = default;;

    KeyMatch(std::initializer_list<std::tuple<Container, int, V>> list) {
        Container c;
        V v;
        int size;
        for (auto l : list) {
            std::tie(c, size, v) = l;
            this->insert(c, size, v);
        }
    }

    bool insert(Container container, unsigned size, V value) {
        auto node = this->add_sub(container, size);
        if (node->has_value()) {
            return false;   // key already exist
        } else {
            node->set_value(value);
        }
    }

    V &search(Container container, unsigned size) {
        auto node = this->walk(container, size);
        if (node == nullptr || !node->has_value()) {
            throw NotFindException();
        } else {
            return node->get_value();
        }
    }

    void del(Container container, unsigned size) {
        this->del_sub(container, size);
    }
};

#endif //RHTTPD_KEYMATCH_H
