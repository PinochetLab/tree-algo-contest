#include <iostream>
#include <cassert>
#include <random>
#include <vector>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution<double> range(0, 1000);

using V = int32_t;

class Treap {
public:
	double y;
	V value;
	size_t count = 1;
	Treap* left = nullptr;
	Treap* right = nullptr;
	Treap(V value) : y(range(rng)), value(value) {};

	Treap(V value, double y) : y(y), value(value) {};

	static Treap* insert(Treap* treap, V value) {
		return merge(treap, new Treap(value));
	}

	static std::pair<V, Treap*> split(Treap* treap) {
		if (treap == nullptr) return { 0, nullptr };
		if (treap->left) {
			auto [v, r] = split(treap->left);
			Treap* t = new Treap(treap->value, treap->y);
			t->count = treap->count - 1;
			t->left = r;
			t->right = treap->right;
			return { v, t };
		}
		else {
			return { treap->value, treap->right };
		}
	}

	static Treap* merge(Treap* t1, Treap* t2) {
		if (t1 == nullptr) return t2;
		if (t2 == nullptr) return t1;
		Treap* res;
		if (t1->y > t2->y) {
			res = new Treap(t1->value, t1->y);
			res->left = t1->left;
			res->right = merge(t1->right, t2);
		}
		else {
			res = new Treap(t2->value, t2->y);
			res->right = t2->right;
			res->left = merge(t1, t2->left);
		}
		res->count = t1->count + t2->count;
		return res;
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);


	size_t n;
	std::cin >> n;

	std::vector<Treap*> treaps;
	treaps.reserve(n + 1);
	treaps.push_back(nullptr);

	for (size_t i = 0; i < n; i++) {
		int id;
		std::cin >> id;
		if (id > 0) {
			size_t ind;
			V val;
			std::cin >> ind >> val;
			treaps.push_back(Treap::insert(treaps[ind], val));
		}
		else {
			size_t ind;
			std::cin >> ind;
			auto [v, r] = Treap::split(treaps[ind]);
			std::cout << v << '\n';
			treaps.push_back(r);
		}
	}
}