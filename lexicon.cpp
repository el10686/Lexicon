#include <iostream>

using namespace std;

class lexicon {
	public:
		lexicon(): root(nullptr) {}
		~lexicon() {
			purge(root);
		}

		void insert(const string &s) {
			if (root == nullptr) {
				root = new node(s, nullptr, nullptr, 1, 1);
			} else {
				insert(s, root, 1);
			}
		}
		int lookup(const string &s) const {
			return lookup(s, root);
		}
		int depth(const string &s) const {
			return depth(s, root);
		}
		void replace(const string &s1, const string &s2) {
			if (s1 == s2) {
				return;
			}
			int times = lookup(s1);
			if (times == 0) {
				return;
			}
			root = Delete(root, s1);
			insert(s2, root, times);
			fixdepth(root, 1);
		}

		friend ostream & operator << (ostream &out, const lexicon &l) {
			l.print(out, l.root);
			return out;
		}
	private:
		struct node {
			string data;
			int times;
			int depth;

			node *left, *right;
			node(const string &str, node *l, node *r, int t, int d): data(str), left(l), right(r), times(t), depth(d) {}
		};

		node *root;

		void purge(node *l) {
			if (l != nullptr) {
				purge(l->left);
				purge(l->right);
				delete l;
			}
		}

		void insert(const string &s, node *l, int times) {
			if (s == l->data) {
				l->times += times;
			}
			else if (s < l->data){
				if (l->left == nullptr) {
					l->left = new node(s, nullptr, nullptr, times, l->depth + 1);
				} else {
					insert(s, l->left, times);
				}
			} else {
				if (l->right == nullptr) {
					l->right = new node(s, nullptr, nullptr, times, l->depth + 1);
				} else {
					insert(s, l->right, times);
				}
			}
		}

		static int lookup(const string &s, node *l) {
			if (l != nullptr) {
				if (s == l->data) {
					return l->times;
				}
				else if (s < l->data) {
					return lookup(s, l->left);
				} else {
					return lookup(s, l->right);
				}
			}
			return 0;
		}

		static int depth(const string &s, node *l) {
			if (l != nullptr) {
				if (s == l->data) {
					return l->depth;
				}
				else if (s < l->data) {
					return depth(s, l->left);
				} else {
					return depth(s, l->right);
				}
			}
			return 0;
		}

		node *Delete(node *l, const string &data) {
			if (l == nullptr) {
				return l;
			}
			else if (data < l->data) {
				l->left = Delete(l->left, data);
			}
			else if (data > l->data) {
				l->right = Delete(l->right, data);
			} else {
				if (l->right == nullptr && l->left == nullptr) {
					delete l;
					l = nullptr;
				}
				else if (l->right == nullptr) {
					node *tmp = l;
					l = l->left;
					delete tmp;
				}
				else if (l->left == nullptr) {
					node *tmp = l;
					l = l->right;
					delete tmp;
				} else {
					node *tmp = findmin(l->left);
					l->data = tmp->data;
					l->times = tmp->times;
					l->left = Delete(l->left, tmp->data);
				}
			}
			return l;
		}

		static node *findmin(node *l) {
			while (l->right != nullptr) {
				l = l->right;
			}
			return l;
		}

		void fixdepth(node *l, int depth) {
			if (l != nullptr) {
				l->depth = depth;
				fixdepth(l->left, depth + 1);
				fixdepth(l->right, depth + 1);
			}
		}

		static void print(ostream &out, node *l) {
			if (l != nullptr) {
				print(out, l->left);
				out << l->data << " " << l->times << endl;
				print(out, l->right);
			}
		}
};

//Driver Program
int main () {
	lexicon l;
	l.insert("the");
	l.insert("boy");
	l.insert("and");
	l.insert("the");
	l.insert("wolf");
	cout << "The word 'the' is found " << l.lookup("the") << " time(s)" << endl;
	cout << "The word 'and' is found at depth " << l.depth("and") << endl;
	cout << l;
	l.replace("boy", "wolf");
	cout << "After replacement \n";
	cout << l;
	cout << "Now the word 'and' is found at depth " << l.depth("and") << endl;
}
