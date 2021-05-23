#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

class Treap {
    static inline minstd_rand generator;

    struct Node {
        int priority, value;
        Node *lhs = nullptr, *rhs = nullptr;
        int size;
        Node(int value) : value(value), priority(generator()), size(1) {}
    } *root = nullptr;

    static int getSize(Node* now) {
        return now ? now->size : 0;
    }

    static void update(Node *&now) {
        if (now) {
            now->size = getSize(now->lhs) + 1 + getSize(now->rhs);
        }
    }

    static Node *merge(Node *a, Node *b) {
        if (!a || !b) {
            return a ? a : b;
        }
        if (a->priority > b->priority) {
            a->rhs = merge(a->rhs, b);
            update(a);
            return a;
        } else {
            b->lhs = merge(a, b->lhs);
            update(b);
            return b;
        }
    }

    static void split(Node *now, int k, Node *&a, Node *&b) {
        if (!now) {
            a = b = nullptr;
            return;
        }
        if (getSize(now->lhs) < k) {
            split(now->rhs, k - getSize(now->lhs) - 1, now->rhs, b);
            a = now;
        } else {
            split(now->lhs, k, a, now->lhs);
            b = now;
        }
        update(a);
        update(b);
    }
    void death(Node *now) {
        if (!now) {
            return;
        }
        death(now->lhs);
        death(now->rhs);
        delete now;
    }

public:
    int get(int index) {
        Node *less, *great, *equal;
        split(root, index, less, great);
        split(great, 1, equal, great);
        int result = equal->value;
        root = merge(merge(less, equal), great);
        return result;
    }

    /*void insert(int key) {
        Node *less, *great;
        split(root, key, less, great);
        less = merge(less, new Node(key));
        root = merge(less, great);
    }

    void erase(int key) {
        Node *less, *great, *equal;
        split(root, key, less, great);
        split(great, key + 1, equal, great);
        root = merge(less, great);
    }*/

    void pushBack(int value) {
        root = merge(root, new Node(value));
    }

    void toFront(int l, int r) {
        Node *less, *equal, *great;
        split(root, l, less, great);
        split(root, r - l + 1, equal, great);
        root = merge(merge(equal, less), great);
    }
    ~Treap() {
        death(root);
    }
};

int main() {
    ios::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("input.txt", "r", stdin);

    int n, m;
    cin >> n >> m;

    Treap t;

    for (int i = 1; i <= n; ++i)
        t.pushBack(i);

    for (int i = 0; i < n; ++i) {
        cout << t.get(i) << " ";
    }
    return 0;
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        t.toFront(l - 1, r - 1);
    }

    for (int i = 0; i < n; ++i) {
        cout << t.get(i) << " ";
    }

    return 0;
}