#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <string>
#include <numeric>

using namespace std;
using namespace std::chrono;

// Funkcje haszujące
// Funkcja hashująca na bazie ASCII
unsigned int hashASCII(const std::string &key) {
    unsigned int hash = 0;
    for (char c : key) {
        hash += static_cast<unsigned int>(c);
    }
    return hash;
}

// Funkcja hashująca na bazie djb2
unsigned int hashDJB2(const std::string &key) {
    unsigned int hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + static_cast<unsigned int>(c); // hash * 33 + c
    }
    return hash;
}

// Klasa tablicy haszującej
template<typename K, typename V>
class HashTable {
private:
    vector<list<pair<K, V>>> table;
    unsigned int (*hashFunction)(const K& key);
    int size;

public:
    HashTable(unsigned int (*hashFunc)(const K& key), int tableSize) : hashFunction(hashFunc), size(tableSize) {
        table.resize(size);
    }

    void insert(const K& key, const V& value) {
        unsigned int hashValue = hashFunction(key) % size;
        table[hashValue].emplace_back(key, value);
    }

    void remove(const K& key) {
        unsigned int hashValue = hashFunction(key) % size;
        auto& cell = table[hashValue];
        auto it = cell.begin();
        while (it != cell.end()) {
            if (it->first == key) {
                it = cell.erase(it);
                return;
            } else {
                ++it;
            }
        }
    }

    void printHashForKey(const K& key) {
        cout << "Hash dla \"" << key << "\": " << hashFunction(key) << endl;
    }
};

int main() {
    int tab[] = {1000, 10000, 100000};
    int repetitions = 100;
    string keyBase = "struktury danych";

    for (int data : tab) {
        cout << "-----------------------" << data << "-----------------------" << endl;

        // Testowanie ASCII
        {
            HashTable<string, int> hashTableASCII(hashASCII, data);

            // Dodanie elementów
            auto start = high_resolution_clock::now();
            for (int i = 0; i < data; i++) {
                hashTableASCII.insert(keyBase + to_string(i), i);
            }
            auto stop = high_resolution_clock::now();
            long long insertTimeASCII = duration_cast<nanoseconds>(stop - start).count();
            cout << "ASCII: Czas dodania " << data << " elementów: " << insertTimeASCII << " nanosekund." << endl;

            // Usunięcie elementu
            start = high_resolution_clock::now();
            hashTableASCII.remove(keyBase + "0");
            stop = high_resolution_clock::now();
            long long removeTimeASCII = duration_cast<nanoseconds>(stop - start).count();
            cout << "ASCII: Czas usunięcia elementu: " << removeTimeASCII << " nanosekund." << endl;

        }

        // Testowanie DJB2
        {
            HashTable<string, int> hashTableDJB2(hashDJB2, data);

            // Dodanie elementów
            auto start = high_resolution_clock::now();
            for (int i = 0; i < data; i++) {
                hashTableDJB2.insert(keyBase + to_string(i), i);
            }
            auto stop = high_resolution_clock::now();
            long long insertTimeDJB2 = duration_cast<nanoseconds>(stop - start).count();
            cout << "DJB2: Czas dodania " << data << " elementów: " << insertTimeDJB2 << " nanosekund." << endl;

            // Usunięcie elementu
            start = high_resolution_clock::now();
            hashTableDJB2.remove(keyBase + "0");
            stop = high_resolution_clock::now();
            long long removeTimeDJB2 = duration_cast<nanoseconds>(stop - start).count();
            cout << "DJB2: Czas usunięcia elementu: " << removeTimeDJB2 << " nanosekund." << endl;

        }

        cout << endl;
    }
    return 0;
}
