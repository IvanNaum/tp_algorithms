#include <iostream>
#include <vector>
#include <optional>
#include <cassert>

/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с
помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
проверки принадлежности данной строки множеству.

1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

 */

const size_t DEFAULT_SIZE = 8;

template<class Hasher>
struct HashTable {
    explicit HashTable(size_t init_size = DEFAULT_SIZE)
            : _size(0), _table(init_size, std::nullopt) {};

    HashTable(HashTable &) = delete;

    HashTable &operator=(const HashTable &) = delete;

    ~HashTable() = default;

    bool insert(const std::string &key) {
        size_t index = 0;
        size_t hashed_index = get_probe(key, index);

        while (!cell_can_accept(hashed_index)) {
            ++index;
            if (_table[hashed_index] == key || index == _table.size()) {
                return false;
            }
            hashed_index = get_probe(key, index);
        }

        _table[hashed_index] = key;
        ++_size;
        if (size() > _table.size() / 4 * 3) {
            _rehash_table();
        }
        return true;
    }

    bool exclude(const std::string &key) {
        size_t index = 0;
        size_t hashed_index = get_probe(key, index);
//        do {
//            assert(_table.size() != index);
//            hashed_index = get_probe(key, index);
//            ++index;
//        } while (key != _table[hashed_index]);
        while (_table[hashed_index] != key) {
            ++index;
            if (index == _table.size() || cell_is_empty(hashed_index)) {
                return false;
            }
            hashed_index = get_probe(key, index);
        }

        _table[hashed_index]->clear();
        --_size;
        return true;
    }

    bool find(const std::string &key) {
        size_t index = 0;
        size_t hashed_index = get_probe(key, index);

        while (!cell_is_empty(hashed_index) && index != _table.size()) {
            if (_table[hashed_index] == key) {
                return true;
            }
            ++index;
            hashed_index = get_probe(key, index);
        }
        return false;
    }

    size_t size() { return _size; };

private:
    void _rehash_table() {
        HashTable<Hasher> new_hash_table = HashTable(_table.size() * 2);
        for (auto k: _table) {
            if (k && !k->empty()) {
                new_hash_table.insert(k.value());
            }
        }
        std::swap(this->_table, new_hash_table._table);
        std::swap(this->_size, new_hash_table._size);
    }

    bool cell_can_accept(size_t index) {
        return cell_is_empty(index) || cell_is_deleted(index);
    }

    bool cell_is_empty(size_t index) {
        return !_table[index];
    }

    bool cell_is_deleted(size_t index) {
        return _table[index] && _table[index]->empty();
    }

    size_t get_probe(const std::string &key, size_t index) {
        return (_hasher(key) + index + 1) % _table.size();
    }

    Hasher _hasher;
    size_t _size;
    std::vector<std::optional<std::string>> _table;
};

struct StringHasher {
    explicit StringHasher(size_t prime = 97) {};

    ~StringHasher() = default;

    StringHasher(StringHasher &) = delete;

    StringHasher &operator=(const StringHasher &) = delete;


    size_t operator()(const std::string &str) const {
        assert(!str.empty());

        auto hash = (size_t) str.front();
        for (size_t i = 1; i < str.size(); ++i) {
            hash = prime * hash + str[i];
        }
        return hash;
    }

private:
    size_t prime;
};

void test_hashtable() {
    {
        HashTable<StringHasher> hash_table;
        assert(hash_table.insert("string1"));
        assert(hash_table.insert("string2"));
        assert(hash_table.insert("string3"));
        assert(hash_table.find("string1"));
        assert(hash_table.find("string2"));
        assert(hash_table.find("string3"));

        assert(hash_table.size() == 3);

        assert(hash_table.exclude("string1"));
        assert(!hash_table.find("string1"));

        assert(hash_table.exclude("string2"));
        assert(!hash_table.find("string2"));

        assert(hash_table.exclude("string3"));
        assert(!hash_table.find("string3"));

        assert(hash_table.size() == 0);
    }
    {
        HashTable<StringHasher> hash_table;
        std::string str = "string";

        assert(hash_table.insert(str));
        assert(!hash_table.insert(str));

        assert(hash_table.find(str));
        assert(hash_table.exclude(str));
        assert(!hash_table.find(str));
    }
    {
        HashTable<StringHasher> hash_table;
        int test_size = 10;
        std::string str = "string";
        for (int i = 0; i < test_size; ++i) {
            assert(hash_table.insert(str + std::to_string(i)));
        }
        for (int i = 0; i < test_size; ++i) {
            assert(hash_table.find(str + std::to_string(i)));
        }
        for (int i = 0; i < test_size; ++i) {
            assert(hash_table.exclude(str + std::to_string(i)));
        }
        for (int i = 0; i < test_size; ++i) {
            assert(!hash_table.find(str + std::to_string(i)));
        }
    }
}

int main() {
//    test_hashtable();
//    std::cout << "Success test!" << std::endl;

    HashTable<StringHasher> hash_table;

    char command;
    std::string str;
    while (std::cin >> command >> str) {
        switch (command) {
            case '+':
                std::cout << (hash_table.insert(str) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (hash_table.exclude(str) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (hash_table.find(str) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}
