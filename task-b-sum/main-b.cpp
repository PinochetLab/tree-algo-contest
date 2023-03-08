#include <iostream>
#include <cassert>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<uint64_t> range(0, 100000);

class Treap {
private:
	uint64_t x;
	uint64_t y;
	size_t sum;
	Treap* left = nullptr;
	Treap* right = nullptr;
public:
	Treap(uint64_t x, uint64_t y) : x(x), y(y), sum(x) {};

	static size_t safeSum(Treap* treap) {
		return treap ? treap->sum : 0;
	}

	static bool contains(const Treap* treap, size_t element) {
		if (treap == nullptr) return false;
		if (treap->x == element) return true;
		else if (element < treap->x) return contains(treap->left, element);
		else return contains(treap->right, element);
	}

	static Treap* insert(Treap* treap, uint64_t element, uint64_t priority) {
		auto [l, r] = split(treap, element);
		return merge(merge(l, new Treap(element, priority)), r);
	}

	static std::pair<Treap*, Treap*> split(Treap* treap, uint64_t k) {
		if (!treap) return { nullptr, nullptr };
		if (k <= treap->x) {
			auto [l, r] = split(treap->left, k);
			treap->left = r;
			treap->sum -= safeSum(l);
			return { l, treap };
		}
		else {
			auto [l, r] = split(treap->right, k);
			treap->right = l;
			treap->sum -= safeSum(r);
			return { treap, r };
		}
	}

	static Treap* merge(Treap* t1, Treap* t2) {
		if (t1 == nullptr) return t2;
		if (t2 == nullptr) return t1;
		if (t1->y > t2->y) {
			t1->sum += t2->sum;
			t1->right = merge(t1->right, t2);
			return t1;
		}
		else {
			t2->sum += t1->sum;
			t2->left = merge(t1, t2->left);
			return t2;
		}
	}

	static size_t getSum(Treap* treap, size_t left, size_t right) {
		auto [l, r1] = split(treap, left);
		auto [c, r] = split(r1, right + 1);
		size_t sum = safeSum(c);
		treap = merge(merge(l, c), r);
		return sum;
	}
};

int main() {
	size_t n;
	std::cin >> n;
	Treap* treap = nullptr;
	int64_t lastQ = -1;
	for (size_t i = 0; i < n; i++) {
		char c;
		std::cin >> c;
		if (c == '+') {
			size_t a;
			std::cin >> a;
			if (lastQ > 0) a = (a + size_t(lastQ)) % size_t(1e9);
			if (!Treap::contains(treap, a)) {
				treap = Treap::insert(treap, a, range(rng));
			}
			lastQ = -1;
		}
		else {
			size_t l, r;
			std::cin >> l >> r;
			lastQ = Treap::getSum(treap, l, r);
			std::cout << lastQ << '\n';
		}
	}
	return 0;
}