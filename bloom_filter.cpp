#include <limits>
#include <cstddef>
#include <stdio.h>
#include <iomanip>

using namespace std;

template <typename T>
class Bloom_filter
{
private:
    unsigned long int *flags;
    unsigned long int size;

    unsigned long int hash_1(const T);
    unsigned long int hash_2(const T);

public:
    Bloom_filter(unsigned long int num_elem);
    ~Bloom_filter();
    Bloom_filter(const Bloom_filter &that);
    Bloom_filter(Bloom_filter &&that);
    void put(const T);
    void fetch(const T);
    bool is_here(const T);
};

int main (void) {
    int i;

    Bloom_filter<int> bf(30);
    for (i = 0; i < 20; i++) {
        bf.put(i);
    }
    for (i = 0; i < 5; i++) {
        bf.fetch(i);
    }
    bf.put(2);
    for (i = 0; i < 30; i++) {
        if (bf.is_here(i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }
    printf("\n");
    return 0;
}

template <typename T>
Bloom_filter<T>:: Bloom_filter(unsigned long int num_elem = 10000)
{   
    unsigned long int i; 

    size = num_elem * 3;
    //k = 2, m = n * (k / ln(2))
    flags = new unsigned long int[size];
    for  (i = 0; i < size; i++) {
        flags[i] = 0;
    }
}

template <typename T>
Bloom_filter<T>:: ~Bloom_filter()
{
    delete[] flags;
}

template <typename T>
Bloom_filter<T>:: Bloom_filter(const Bloom_filter<T> &that)
{
    unsigned long int i;

    size = that.size;
    flags = new unsigned long int[size];
    for (i = 0; i < size; i++) {
        flags[i] = that.flags[i];
    }
}

template <typename T>
Bloom_filter<T>:: Bloom_filter(Bloom_filter<T> &&that)
{
    flags = that.flags;
    size = that.size;

    that.size = 0;
    that.flags = nullptr;
}

template <typename T>
bool Bloom_filter<T>::is_here(const T elem)
{
    if (flags[this->hash_1(elem)] > 0&&
    flags[this->hash_2(elem)] > 0) {
        return true;
    }
    return false;
}

template <typename T>
void Bloom_filter<T>:: put(const T elem)
{
    flags[this->hash_1(elem)]++;
    flags[this->hash_2(elem)]++;
}

template <typename T>
void Bloom_filter<T>:: fetch(const T elem)
{
    unsigned long int min;

    if (is_here(elem)) {
        min = flags[this->hash_1(elem)] < flags[this->hash_2(elem)] ?
                flags[this->hash_1(elem)] : flags[this->hash_2(elem)];
        flags[this->hash_1(elem)] -= min;
        flags[this->hash_2(elem)] -= min;
    }
}

template <typename T>
unsigned long int Bloom_filter<T>:: hash_1(const T elem)
{
    unsigned long int aux;

    aux = hash<T>{}(elem);
    aux = hash<unsigned long int>{}(aux * 2);
    return aux % size;
}

template <typename T>
unsigned long int Bloom_filter<T>:: hash_2(const T elem)
{
    unsigned long int aux;
    
    aux = hash<T>{}(elem);
    aux = hash<unsigned long int>{}(aux * 2 + 1);
    return aux % size;
}
