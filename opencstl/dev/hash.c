#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
struct Hash {
	int* base;
	unsigned char* bloom;
#define EMPTY 0
#define INSERTED 1
#define DELETED 2
	int length;
	int capacity;
};
typedef struct Hash Hash;
int is_prime(int n) {
	int i = 5;
	if (!(n & 1))
		return n == 2;
	if (n % 3 == 0)
		return n == 3;
	while (i*i <= n)
		if (n%i++ == 0)return 0;
	return n != 1;
}
size_t hash4(size_t key) {
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);
	return key;
}
size_t hash(void* _key, size_t n) {
	char* key = _key;
	char* e = key + n;
	size_t ret = 0;
	while (key+sizeof(size_t) <= e) {
		size_t tmp = hash4(*(size_t*)key);
		ret ^= tmp;
		key += sizeof(size_t);
	}
	return ret;
}

size_t HashFunc1(int key, int size) {
	return hash(&key, sizeof(int)) % size;
	//return key%size;
	//return hash4(key)%size;
}
size_t HashFunc2(int key, int size) {
	size_t h1 = HashFunc1(key, size);
	return h1 % (size - 1) + 1;
}
int _Insert(int* base,unsigned char* bloom,int capacity, int key) {
	int h1 = HashFunc1(key,capacity);
	int h2 = HashFunc2(key, capacity);
	int pos = h1;
	while (bloom[pos] == INSERTED) {
		if (base[pos] == key)return 0;
		pos = (pos+h2)%capacity;
	}
	bloom[pos] = INSERTED;
	base[pos] = key;
	return 1;
}
void Insert(Hash* hash, int key) {
	if (hash->length > hash->capacity*0.7) {
		int capacity = hash->capacity * 2;
		while (is_prime(capacity) == 0)capacity++;
		int* base = (int*)calloc(capacity, sizeof(int));
		unsigned char* bloom=(unsigned char*)calloc(capacity, sizeof(unsigned char));
		for (int i = 0; i < hash->capacity; i++) {
			if (hash->bloom[i] == true) {
				_Insert(base, bloom, capacity, hash->base[i]);
			}
		}
		free(hash->base);
		free(hash->bloom);
		hash->base = base;
		hash->bloom = bloom;
		hash->capacity = capacity;
	}
	if (_Insert(hash->base, hash->bloom, hash->capacity, key)) {
		hash->length++;
	}
}
void Erase(Hash* hash, int key) {
	int h1 = HashFunc1(key, hash->capacity);
	int h2 = HashFunc2(key, hash->capacity);
	int pos = h1;
	while (hash->bloom[pos] != EMPTY && hash->base[pos]!=key) {
		pos = (pos + h2) % hash->capacity;
	}
	if (hash->base[pos] != key) {
		return;
	}
	int sw = pos;
	int mv = pos;
	while (hash->bloom[mv] != EMPTY) {
		if (hash->bloom[mv] != DELETED && HashFunc1(hash->base[mv], hash->capacity) == h1) {
			sw = mv;
		}
		mv = (mv + h2) % hash->capacity;
	}

	hash->base[pos] = hash->base[sw];
	hash->bloom[pos] = hash->bloom[sw];
	
	hash->bloom[sw] = DELETED;
	hash->length--;
}
Hash Create() {
	Hash hash;
	hash.base = (int*)calloc(7, sizeof(int));
	hash.bloom = (unsigned char*)calloc(7, sizeof(unsigned char));
	hash.length = 0;
	hash.capacity = 7;
	return hash;
}
int main() {
	Hash hash = Create();


	for (int i = 0; i < 10000; i ++) {
		Insert(&hash, i);
	}
	for (int i = 0; i < 10000; i++) {
		Erase(&hash, i);
	}

	for (int i = 0; i < hash.capacity; i++) {
		hash.bloom[i]==INSERTED ? printf("[%3d]", hash.base[i]) : printf("[---]");
	}
	puts("");
	printf("length : %d\n", hash.length);
	
	return 0;
}