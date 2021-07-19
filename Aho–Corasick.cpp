#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

struct Ac_Node {
    Ac_Node() {
        memset(child, NULL, sizeof child);
    }
    Ac_Node *child[26];  //26个子节点
    Ac_Node *fail;    //匹配失败时
    vector<int> exist; //存储当前节点所表示单词的长度
};

class Trie {
private:
    Ac_Node *root; //根节点，不存放任何数据

    void insert(const string &);//构建字典树用，向字典树插入一个新单词

public:
    Trie() {
        root = new Ac_Node;
        root->fail = root;
    }
    explicit Trie(const vector<string> &str){
        root = new Ac_Node;
        root->fail = root;
        insert(str);
    }
    void insert(const vector<string> &);//插入多个字符串

    void find(const string &);//查找函数
};

void Trie::insert(const vector<string> &str) {
    int sz = str.size();
    for (int i = 0; i < sz; i++) {
        insert(str[i]);
    }
    queue<Ac_Node *> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            if (q.front() == root) {
                for (auto &j : q.front()->child) {
                    if (j) {
                        j->fail = root;
                        q.push(j);
                    }
                }
            } else {
                Ac_Node *temp = q.front()->fail;
                for (int j = 0; j < 26; j++) {
                    if (q.front()->child[j]) {
                        if (temp->child[j]) {
                            q.front()->child[j]->fail = temp->child[j];
                            q.push(q.front()->child[j]);
                            q.front()->child[j]->exist.insert(
                                    q.front()->child[j]->exist.end(),
                                    temp->child[j]->exist.begin(),
                                    temp->child[j]->exist.end());
                        } else {
                            q.front()->child[j]->fail = root;
                            q.push(q.front()->child[j]);
                        }
                    }
                }
            }
            q.pop();
        }
    }
}

void Trie::insert(const string &str) {
    int len = str.length();
    Ac_Node *temp = root;
    for (int i = 0; i < len; i++) {
        int n = str[i] - 'a';
        if (!temp->child[n]) {
            temp->child[n] = new Ac_Node;
        }
        temp = temp->child[n];
    }
    temp->exist.push_back(len);
}

void Trie::find(const string &str) {
    int len = str.size();
    Ac_Node *travel = root;
    for (int i = 0; i < len; i++) {
        int n = str[i] - 'a';
        if (travel->child[n]) {
            travel = travel->child[n];
            if (!travel->exist.empty()) {
                for (int j : travel->exist) {
                    cout << "find ";
                    for (int p = i - j + 1; p <= i; p++) {
                        cout << str[p];
                    }
                    cout << " at: " << i - j + 1 << endl;
                }
            }
        } else {
            while (travel != root) {
                travel = travel->fail;
                if (travel->child[n]) {
                    i--;
                    break;
                }
            }
        }
    }
}

void ac_find(const string &source, const vector<string> &str) {
    Trie tree(str);
    tree.find(source);
}

int main() {
    vector<string> str = {"he", "his", "hers", "she"};
    string s = "ahishers";

    ac_find(s, str);

    return 0;
}