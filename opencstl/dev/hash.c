#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef unsigned int uint32;
typedef unsigned long long uint64;

bool is_prime(const uint32 x) {
	if (x == 2 || x == 3 || x == 5 || x == 7 || x == 11) return true;
	if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0) return false;
	if (x < 121) return (x > 1);

	static const uint32 bases[] = {
		15591, 2018, 166, 7429, 8064, 16045, 10503, 4399, 1949, 1295, 2776, 3620, 560, 3128, 5212, 2657,
		2300, 2021, 4652, 1471, 9336, 4018, 2398, 20462, 10277, 8028, 2213, 6219, 620, 3763, 4852, 5012,
		3185, 1333, 6227, 5298, 1074, 2391, 5113, 7061, 803, 1269, 3875, 422, 751, 580, 4729, 10239,
		746, 2951, 556, 2206, 3778, 481, 1522, 3476, 481, 2487, 3266, 5633, 488, 3373, 6441, 3344,
		17, 15105, 1490, 4154, 2036, 1882, 1813, 467, 3307, 14042, 6371, 658, 1005, 903, 737, 1887,
		7447, 1888, 2848, 1784, 7559, 3400, 951, 13969, 4304, 177, 41, 19875, 3110, 13221, 8726, 571,
		7043, 6943, 1199, 352, 6435, 165, 1169, 3315, 978, 233, 3003, 2562, 2994, 10587, 10030, 2377,
		1902, 5354, 4447, 1555, 263, 27027, 2283, 305, 669, 1912, 601, 6186, 429, 1930, 14873, 1784,
		1661, 524, 3577, 236, 2360, 6146, 2850, 55637, 1753, 4178, 8466, 222, 2579, 2743, 2031, 2226,
		2276, 374, 2132, 813, 23788, 1610, 4422, 5159, 1725, 3597, 3366, 14336, 579, 165, 1375, 10018,
		12616, 9816, 1371, 536, 1867, 10864, 857, 2206, 5788, 434, 8085, 17618, 727, 3639, 1595, 4944,
		2129, 2029, 8195, 8344, 6232, 9183, 8126, 1870, 3296, 7455, 8947, 25017, 541, 19115, 368, 566,
		5674, 411, 522, 1027, 8215, 2050, 6544, 10049, 614, 774, 2333, 3007, 35201, 4706, 1152, 1785,
		1028, 1540, 3743, 493, 4474, 2521, 26845, 8354, 864, 18915, 5465, 2447, 42, 4511, 1660, 166,
		1249, 6259, 2553, 304, 272, 7286, 73, 6554, 899, 2816, 5197, 13330, 7054, 2818, 3199, 811,
		922, 350, 7514, 4452, 3449, 2663, 4708, 418, 1621, 1171, 3471, 88, 11345, 412, 1559, 194,
	};

	uint64 h = x;
	h = ((h >> 16) ^ h) * 73244475;
	h = ((h >> 16) ^ h) * 73244475;
	h = ((h >> 16) ^ h) & 255;

	uint32 a = bases[h];

	uint32 d = x - 1, s = 0;
	while ((d & 1) == 0) {
		++s;
		d >>= 1;
	}

	uint64 cur = 1, pw = d;
	uint64 base = a % x;
	while (pw) {
		if (pw & 1) cur = (cur * base) % x;
		base = (base * base) % x;
		pw >>= 1;
	}

	if (cur == 1 || cur == (uint64)x - 1) return true;

	for (uint32 r = 1; r < s; r++) {
		cur = (cur * cur) % x;
		if (cur == (uint64)x - 1) return true;
	}

	return false;
}

struct Hash {
	int* base;
	unsigned char* tombstone;
	size_t type_size;
#define EMPTY 0
#define INSERTED 1
#define DELETED 2
	size_t length;
	size_t capacity;
};
typedef struct Hash Hash;

// 32비트 정수를 위한 고분포 해시 (MurmurHash3 mixer 기반)
size_t hash32(void* _key) {
	uint32 h = *(uint32*)_key;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return (size_t)h;
}

// 64비트 정수를 위한 고분포 해시 (SplitMix64 mixer 기반)
size_t hash64(void* _key) {
	uint64 x = *(uint64*)_key;
	x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
	x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
	x = x ^ (x >> 31);
	return (size_t)x;
}

// 범용 데이터 블록을 위한 해시 (기존 로직 개선)
size_t hash(void* _key, size_t n) {
	unsigned char* key = (unsigned char*)_key;
	size_t ret = 0xCBF29CE484222325ULL; // FNV offset basis 느낌의 초기값

	// 4바이트 또는 8바이트 단위로 끊어서 처리
	while (n >= sizeof(size_t)) {
		ret ^= (sizeof(size_t) == 8) ? hash64(key) : hash32(key);
		ret *= 0x100000001b3ULL; // 소수 곱셈을 통한 혼합
		key += sizeof(size_t);
		n -= sizeof(size_t);
	}
	return ret;
}

// 입력값이 1, 2, 3처럼 연속적이어도 출력값은 완전히 흩어집니다.
size_t hash_mixer(void* key, size_t n) {
	unsigned char* data = (unsigned char*)key;

	size_t h = 0x9e3779b97f4a7c15ULL; // seed (golden ratio)

	// size_t 단위로 처리 (8 or 4 bytes)
	while (n >= sizeof(size_t)) {
		size_t k;
		memcpy(&k, data, sizeof(size_t)); // unaligned-safe

		// mix (SplitMix64 스타일)
		k ^= k >> 30;
		k *= 0xbf58476d1ce4e5b9ULL;
		k ^= k >> 27;
		k *= 0x94d049bb133111ebULL;
		k ^= k >> 31;

		h ^= k;
		h *= 0x9e3779b97f4a7c15ULL;

		data += sizeof(size_t);
		n -= sizeof(size_t);
	}

	// tail 처리
	size_t tail = 0;
	for (size_t i = 0; i < n; i++) {
		tail |= ((size_t)data[i]) << (i * 8);
	}

	if (n > 0) {
		tail ^= tail >> 30;
		tail *= 0xbf58476d1ce4e5b9ULL;
		tail ^= tail >> 27;
		tail *= 0x94d049bb133111ebULL;
		tail ^= tail >> 31;

		h ^= tail;
		h *= 0x9e3779b97f4a7c15ULL;
	}

	// final avalanche (매우 중요)
	h ^= h >> 30;
	h *= 0xbf58476d1ce4e5b9ULL;
	h ^= h >> 27;
	h *= 0x94d049bb133111ebULL;
	h ^= h >> 31;

	return h;
}

// 첫 번째 위치 결정
size_t HashFunc1(int key, size_t capacity) {
	return hash_mixer(&key, sizeof(key)) % capacity;
}

// 충돌 시 점프 간격 결정 (이중 해싱용)
// h1과 독립적이어야 하며, 반드시 홀수이거나 capacity와 서로소여야 합니다.
size_t HashFunc2(int key, size_t capacity) {
	uint32 h = (uint32)key;
	h = ~h + (h << 15); // 간단한 보조 믹서
	h = h ^ (h >> 12);
	// 점프 값은 1 ~ (capacity-1) 사이의 값이어야 함
	return (size_t)((h % (capacity - 1)) + 1);
}
int* Find(Hash* hash, int key) {
    int h1 = HashFunc1(key, hash->capacity);
	int h2 = HashFunc2(key, hash->capacity);
	int pos = h1;
	while (hash->tombstone[pos] != EMPTY) {
		if (hash->tombstone[pos] == INSERTED && hash->base[pos] == key)return &hash->base[pos];
		pos = (pos + h2) % hash->capacity;
	}
	return NULL;
}
int _Insert(int* base, unsigned char* tombstone, int capacity, int key) {
	int h1 = HashFunc1(key, capacity) ;
    printf("h1 : %d\n", h1);
	int h2 = HashFunc2(key, capacity);
	int pos = h1;
	while (tombstone[pos] == INSERTED) {
		if (base[pos] == key)return 0;
		pos = (pos + h2) % capacity;
	}
	tombstone[pos] = INSERTED;
	base[pos] = key;
	return 1;
}
void Insert(Hash* hash, int key) {
	if (hash->length > hash->capacity * 0.7) {
		int capacity = hash->capacity * 2;
		while (is_prime(capacity) == 0)capacity++;
		int* base = (int*)calloc(capacity, sizeof(int));
		unsigned char* tombstone = (unsigned char*)calloc(capacity, sizeof(unsigned char));
		for (int i = 0; i < hash->capacity; i++) {
			if (hash->tombstone[i] == true) {
				_Insert(base, tombstone, capacity, hash->base[i]);
			}
		}
		free(hash->base);
		free(hash->tombstone);
		hash->base = base;
		hash->tombstone = tombstone;
		hash->capacity = capacity;
	}
	if (_Insert(hash->base, hash->tombstone, hash->capacity, key)) {
		hash->length++;
	}
}
void Erase(Hash* hash, int key) {
	int h1 = HashFunc1(key, hash->capacity) ;
	int h2 = HashFunc2(key, hash->capacity);
	int pos = h1;
	while (hash->tombstone[pos] != EMPTY && hash->base[pos] != key) {
		pos = (pos + h2) % hash->capacity;
	}
	if (hash->base[pos] != key) {
		return;
	}
	int sw = pos;
	int mv = pos;
	while (hash->tombstone[mv] != EMPTY) {
		if (hash->tombstone[mv] != DELETED && HashFunc1(hash->base[mv],hash->capacity) == h1) {
			sw = mv;
		}
		mv = (mv + h2) % hash->capacity;
	}

	hash->base[pos] = hash->base[sw];
	hash->tombstone[pos] = hash->tombstone[sw];

	hash->tombstone[sw] = DELETED;
	hash->length--;
}
void* Begin(Hash* hash) {
	for (int i = 0; i < hash->capacity; i++) {
		if (hash->tombstone[i] == INSERTED)return &hash->base[i];
	}
	return NULL;
}
void *End(Hash* hash) {
	return NULL;
}
void* Next(Hash* hash, void* it) {
	int* ptr = (int*)it;
	int idx = ptr - hash->base + 1;
	while (idx < hash->capacity) {
		if (hash->tombstone[idx] == INSERTED)return &hash->base[idx];
		idx++;
	}
	return NULL;
}
void Clear(Hash* hash) {
	memset(hash->base, 0, hash->capacity * hash->type_size);
    memset(hash->tombstone, 0, hash->capacity * sizeof(unsigned char));
	hash->length = 0;
}
void Free(Hash* hash) {
	free(hash->base);
	free(hash->tombstone);
	hash->base = NULL;
	hash->tombstone = NULL;
	hash->length = 0;
	hash->capacity = 0;
}
Hash CreateIntHash() {
	Hash hash;
	hash.base = (int*)calloc(7, sizeof(int));
	hash.tombstone = (unsigned char*)calloc(7, sizeof(unsigned char));
	hash.length = 0;
	hash.capacity = 7;
    hash.type_size = sizeof(int);
	return hash;
}
int main() {
	Hash hash = CreateIntHash();


	for (int i = 0; i < 10000; i++) {
		Insert(&hash, i);
	}
	//for (int i = 0; i < 10000; i++) {
	//	Erase(&hash, i);
	//}

	for (int i = 0; i < hash.capacity; i++) {
		hash.tombstone[i] == INSERTED ? printf("[%3d]", hash.base[i]) : printf("[---]");
	}

	for(void* it = Begin(&hash); it != End(&hash); it = Next(&hash, it)) {
		int* val = (int*)it;
		printf("%d ", *val);
	}
	puts("");
	printf("length : %d\n", hash.length);
    printf("capacity : %d\n", hash.capacity);
    Free(&hash);
	return 0;
}