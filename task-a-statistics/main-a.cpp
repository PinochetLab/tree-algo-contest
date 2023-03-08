#include <iostream>
#include <cassert>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<uint64_t> range(0, 100000);

class Treap {
private:
	int64_t x;
	uint64_t y;
	size_t count = 1;
	Treap* left = nullptr;
	Treap* right = nullptr;
public:
	Treap(int64_t x, uint64_t y) : x(x), y(y) {};

	static size_t saveCount(Treap* treap) {
		return treap ? treap->count : 0;
	}

	static Treap* insert(Treap* treap, int64_t element, uint64_t priority) {
		if (!treap) return new Treap(element, priority);
		if (treap->y < priority) {
			auto [l, r] = split(treap, element);
			Treap* t = new Treap(element, priority);
			t->left = l;
			t->right = r;
			t->count = saveCount(t->left) + saveCount(t->right) + 1;
			return t;
		}
		else {
			assert(element != treap->x);
			if (element < treap->x) {
				treap->left = insert(treap->left, element, priority);
			}
			else {
				treap->right = insert(treap->right, element, priority);
			}
			treap->count++;
			return treap;
		}
	}

	static Treap* remove(Treap* treap, int64_t element) {
		if ( !treap ) return nullptr;
		if ( treap->x == element ) {
			Treap* t = merge(treap->left, treap->right);
			delete treap;
			return t;
		}
		else {
			if ( element < treap->x ) {
				treap->left = remove(treap->left, element);
			}
			else {
				treap->right = remove(treap->right, element);
			}
			treap->count--;
			return treap;
		}
	}

	static std::pair<Treap*, Treap*> split(Treap* treap, int64_t k) {
		if (!treap) return { nullptr, nullptr };
		if ( k <= treap->x ) {
			auto [l, r] = split(treap->left, k);
			treap->left = r;
			treap->count -= saveCount(l);
			return { l, treap };
		}
		else {
			auto [l, r] = split(treap->right, k);
			treap->right = l;
			treap->count -= saveCount(r);
			return { treap, r };
		}
	}

	static Treap* merge(Treap* t1, Treap* t2) {
		if ( t1 == nullptr ) return t2;
		if ( t2 == nullptr ) return t1;
		if ( t1->y > t2->y ) {
			t1->count += t2->count;
			t1->right = merge(t1->right, t2);
			// t1->count = saveCount(t1->left) + saveCount(t1->right);
			return t1;
		}
		else {
			t2->count += t1->count;
			t2->left = merge(t1, t2->left);
			// t2->count = saveCount(t2->left) + saveCount(t2->right);
			return t2;
		}
	}

	static int64_t statistics(Treap* treap, size_t k) {
		size_t rs = saveCount(treap->right);
		if (rs == k) return treap->x;
		else if (rs > k) return statistics(treap->right, k);
		else return statistics(treap->left, k - rs - 1);
	}
};

int main() {
	size_t n;
	std::cin >> n;
	Treap* treap = nullptr;
	for (size_t i = 0; i < n; i++) {
		int id;
		std::cin >> id;
		if (id == 0) {
			size_t k;
			std::cin >> k;
			std::cout << Treap::statistics(treap, k - 1) << '\n';
		}
		else {
			int64_t num;
			std::cin >> num;
			if (id == 1) treap = Treap::insert(treap, num, range(rng));
			else treap = Treap::remove(treap, num);
		}
	}
	return 0;
}