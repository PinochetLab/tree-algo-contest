#include <iostream>
#include <cassert>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<uint64_t> range(0, 100000);

template<typename V>
class Treap {
private:
	uint64_t y;
	V value;
	size_t count = 1;
	Treap* left = nullptr;
	Treap* right = nullptr;
public:
	Treap(uint64_t y, V value) : y(y), value(value) {};

	static size_t safeCount(Treap* treap) {
		return treap ? treap->count : 0;
	}

	static Treap* insert(Treap* treap, size_t element, V value, uint64_t priority) {
		auto [l, r] = split(treap, element);
		return merge(merge(l, new Treap(priority, value)), r);
	}

	static Treap* remove(Treap* treap, size_t element) {
		if (treap == nullptr) return nullptr;
		size_t lc = safeCount(treap->left);
		if (lc == element) {
			return merge(treap->left, treap->right);
		}
		else {
			if (element < lc) {
				treap->left = remove(treap->left, element);
			}
			else {
				treap->right = remove(treap->right, element - lc - 1);
			}
			treap->count--;
			return treap;
		}
	}

	static std::pair<Treap*, Treap*> split(Treap* treap, size_t k) {
		if (treap == nullptr) return { nullptr, nullptr };
		size_t lc = safeCount(treap->left);
		if (k <= lc) {
			auto [l, r] = split(treap->left, k);
			treap->count -= safeCount(l);
			treap->left = r;
			return { l, treap };
		}
		else {
			auto [l, r] = split(treap->right, k - lc - 1);
			treap->count -= safeCount(r);
			treap->right = l;
			return { treap, r };
		}
	}

	static Treap* merge(Treap* t1, Treap* t2) {
		if (t1 == nullptr) return t2;
		if (t2 == nullptr) return t1;
		if (t1->y > t2->y) {
			t1->count += t2->count;
			t1->right = merge(t1->right, t2);
			return t1;
		}
		else {
			t2->count += t1->count;
			t2->left = merge(t1, t2->left);
			return t2;
		}
	}

	static void print(Treap* treap) {
		if (treap == nullptr) return;
		print(treap->left);
		std::cout << treap->value << ' ';
		print(treap->right);
	}
};

using value_t = size_t;
using MyTreap = Treap<value_t>;

int main() {
	size_t n, m;
	std::cin >> n >> m;
	MyTreap* treap = nullptr;
	for (size_t i = 0; i < n; i++) {
		value_t a;
		std::cin >> a;
		treap = MyTreap::insert(treap, i, a, range(rng));
	}
	for (size_t i = 0; i < m; i++) {
		std::string id;
		std::cin >> id;
		if (id == "del") {
			size_t ind;
			std::cin >> ind;
			treap = MyTreap::remove(treap, ind - 1);
		}
		else {
			size_t ind;
			value_t val;
			std::cin >> ind >> val;
			treap = MyTreap::insert(treap, ind, val, range(rng));
		}
	}

	std::cout << MyTreap::safeCount(treap) << '\n';

	MyTreap::print(treap);

	return 0;
}