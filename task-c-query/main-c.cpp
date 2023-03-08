#include <iostream>
#include <cassert>
#include <random>
#include <algorithm>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<uint64_t> range(0, 100000);

template<typename V>
V mmin(V v1, V v2, V v3) {
	if (v1 <= v2 && v1 <= v3) return v1;
	else if (v2 <= v1 && v2 <= v3) return v2;
	else return v3;
}

template<typename V>
class Treap {
private:
	uint64_t y;
	V value;
	size_t count = 1;
	V min;
	Treap* left = nullptr;
	Treap* right = nullptr;
public:
	Treap(uint64_t y, V value) : y(y), value(value), min(value) {};

	static V safeMin(Treap* treap) {
		return treap ? treap->min : std::numeric_limits<V>::max();
	}

	static size_t safeCount(Treap* treap) {
		return treap ? treap->count : 0;
	}

	static Treap* insert(Treap* treap, size_t element, V value, uint64_t priority) {
		auto [l, r] = split(treap, element);
		return merge(merge(l, new Treap(priority, value)), r);
	}

	static std::pair<Treap*, Treap*> split(Treap* treap, size_t k) {
		if (treap == nullptr) return { nullptr, nullptr };
		size_t lc = safeCount(treap->left);
		if (k <= lc) {
			auto [l, r] = split(treap->left, k);
			treap->left = r;
			treap->min = mmin(safeMin(treap->left), safeMin(treap->right), treap->value);
			treap->count = safeCount(treap->left) + safeCount(treap->right) + 1;
			return { l, treap };
		}
		else {
			auto [l, r] = split(treap->right, k - lc - 1);
			treap->right = l;
			treap->min = mmin(safeMin(treap->left), safeMin(treap->right), treap->value);
			treap->count = safeCount(treap->left) + safeCount(treap->right) + 1;
			return { treap, r };
		}
	}

	static Treap* merge(Treap* t1, Treap* t2) {
		if (t1 == nullptr) return t2;
		if (t2 == nullptr) return t1;
		if (t1->y > t2->y) {
			t1->right = merge(t1->right, t2);
			t1->min = mmin(safeMin(t1->left), safeMin(t1->right), t1->value);
			t1->count = safeCount(t1->left) + safeCount(t1->right) + 1;
			return t1;
		}
		else {
			t2->left = merge(t1, t2->left);
			t2->min = mmin(safeMin(t2->left), safeMin(t2->right), t2->value);
			t2->count = safeCount(t2->left) + safeCount(t2->right) + 1;
			return t2;
		}
	}

	static V getMin(Treap* treap, size_t left, size_t right) {
		if (!treap) return 0;
		auto [l, cr] = split(treap, left);
		auto [c, r] = split(cr, right - left + 1);
		V res = safeMin(c);
		treap = merge(merge(l, c), r);
		return res;
	}
};

using value_t = int64_t;
using MyTreap = Treap<value_t>;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	size_t n;
	std::cin >> n;
	MyTreap* treap = nullptr;
	for (size_t i = 0; i < n; i++) {
		char id;
		std::cin >> id;
		if (id == '+') {
			size_t ind;
			value_t val;
			std::cin >> ind >> val;
			treap = MyTreap::insert(treap, ind, val, range(rng));
		}
		else {
			size_t i1, i2;
			std::cin >> i1 >> i2;
			std::cout << MyTreap::getMin(treap, i1 - 1, i2 - 1) << '\n';
		}
	}
	return 0;
}