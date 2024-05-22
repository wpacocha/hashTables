#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <string>
#include <numeric>

using namespace std;
using namespace std::chrono;

// Funkcje haszujące
//funkcja hashujaca na bazie ASCII
unsigned int hashASCII(const std::string &key) {
    unsigned int hash = 0;
    for (char c : key) {
        hash += static_cast<unsigned int>(c);
    }
    return hash;
}

//funkcja hashujaca na bazie djb2
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
};

int main() {
    int tab[] = {1000, 10000, 100000};
    int repetitions = 100;
    string keyBase = "struktury danych";

    for (int data : tab) {
        vector<long long> insertTimesASCII(repetitions);
        vector<long long> removeTimesASCII(repetitions);
        vector<long long> insertTimesDJB2(repetitions);
        vector<long long> removeTimesDJB2(repetitions);

        cout << "-----------------------" << data << "-----------------------" << endl;

        // Testowanie ASCII
        for (int r = 0; r < repetitions; ++r) {
            HashTable<string, int> hashTableASCII(hashASCII, data);

            // Dodanie elementów
            auto start = high_resolution_clock::now();
            for (int i = 0; i < data; i++) {
                hashTableASCII.insert(keyBase + to_string(i), i);
            }
            auto stop = high_resolution_clock::now();
            insertTimesASCII[r] = duration_cast<nanoseconds>(stop - start).count();

            // Usunięcie elementu
            start = high_resolution_clock::now();
            hashTableASCII.remove(keyBase + "0");
            stop = high_resolution_clock::now();
            removeTimesASCII[r] = duration_cast<nanoseconds>(stop - start).count();
        }

        // Testowanie DJB2
        for (int r = 0; r < repetitions; ++r) {
            HashTable<string, int> hashTableDJB2(hashDJB2, data);

            // Dodanie elementów
            auto start = high_resolution_clock::now();
            for (int i = 0; i < data; i++) {
                hashTableDJB2.insert(keyBase + to_string(i), i);
            }
            auto stop = high_resolution_clock::now();
            insertTimesDJB2[r] = duration_cast<nanoseconds>(stop - start).count();

            // Usunięcie elementu
            start = high_resolution_clock::now();
            hashTableDJB2.remove(keyBase + "0");
            stop = high_resolution_clock::now();
            removeTimesDJB2[r] = duration_cast<nanoseconds>(stop - start).count();
        }

        // Obliczenie średnich czasów
        long long avgInsertTimeASCII = accumulate(insertTimesASCII.begin(), insertTimesASCII.end(), 0LL) / repetitions;
        long long avgRemoveTimeASCII = accumulate(removeTimesASCII.begin(), removeTimesASCII.end(), 0LL) / repetitions;
        long long avgInsertTimeDJB2 = accumulate(insertTimesDJB2.begin(), insertTimesDJB2.end(), 0LL) / repetitions;
        long long avgRemoveTimeDJB2 = accumulate(removeTimesDJB2.begin(), removeTimesDJB2.end(), 0LL) / repetitions;

        // Wyświetlenie wyników
        cout << "ASCII: Średni czas dodania " << data << " elementów: " << avgInsertTimeASCII << " nanosekund." << endl;
        cout << "ASCII: Średni czas usunięcia elementu: " << avgRemoveTimeASCII << " nanosekund." << endl<<endl;
        cout << "DJB2: Średni czas dodania " << data << " elementów: " << avgInsertTimeDJB2 << " nanosekund." << endl;
        cout << "DJB2: Średni czas usunięcia elementu: " << avgRemoveTimeDJB2 << " nanosekund." << endl;

        cout << endl;
    }
    return 0;
}
