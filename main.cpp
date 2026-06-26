#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool isEnd;
    int frequency;

    TrieNode() {
        isEnd = false;
        frequency = 0;

        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    void dfs(TrieNode* node,
             string current,
             vector<pair<string, int>>& results) {

        if (node->isEnd) {
            results.push_back({current, node->frequency});
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                dfs(node->children[i],
                    current + char('a' + i),
                    results);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {

        TrieNode* curr = root;

        for (char ch : word) {

            ch = tolower(ch);

            if (!isalpha(ch))
                continue;

            int idx = ch - 'a';

            if (curr->children[idx] == nullptr) {
                curr->children[idx] = new TrieNode();
            }

            curr = curr->children[idx];
        }

        curr->isEnd = true;
    }

    bool search(string word) {

        TrieNode* curr = root;

        for (char ch : word) {

            ch = tolower(ch);

            if (!isalpha(ch))
                return false;

            int idx = ch - 'a';

            if (curr->children[idx] == nullptr) {
                return false;
            }

            curr = curr->children[idx];
        }

        if (curr->isEnd) {
            curr->frequency++;
            return true;
        }

        return false;
    }

    vector<pair<string, int>> autocomplete(string prefix) {

        TrieNode* curr = root;

        for (char ch : prefix) {

            ch = tolower(ch);

            if (!isalpha(ch))
                return {};

            int idx = ch - 'a';

            if (curr->children[idx] == nullptr) {
                return {};
            }

            curr = curr->children[idx];
        }

        vector<pair<string, int>> results;

        dfs(curr, prefix, results);

        sort(results.begin(),
             results.end(),
             [](const auto& a, const auto& b) {
                 return a.second > b.second;
             });

        return results;
    }
};

int main() {

    Trie trie;

    ifstream fin("dictionary.txt");

    string word;

    while (fin >> word) {
        trie.insert(word);
    }

    fin.close();

    int choice;

    while (true) {

        cout << "\n=============================\n";
        cout << " SMART AUTOCOMPLETE SYSTEM\n";
        cout << "=============================\n";
        cout << "1. Search Word\n";
        cout << "2. Add Word\n";
        cout << "3. Autocomplete\n";
        cout << "4. Exit\n";
        cout << "Enter Choice: ";

        if (!(cin >> choice)) {

            cout << "\nInvalid input! Please enter a number.\n";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            continue;
        }

        switch (choice) {

        case 1: {

            cout << "\nEnter word: ";
            cin >> word;

            if (trie.search(word))
                cout << "Word Found\n";
            else
                cout << "Word Not Found\n";

            break;
        }

        case 2: {

            cout << "\nEnter new word: ";
            cin >> word;

            trie.insert(word);

            ofstream fout("dictionary.txt", ios::app);

            if (fout.is_open()) {
                fout << word << "\n";
                fout.close();
            }

            cout << "Word Added Successfully\n";

            break;
        }

        case 3: {

            string prefix;

            cout << "\nEnter prefix: ";
            cin >> prefix;

            auto suggestions =
                trie.autocomplete(prefix);

            if (suggestions.empty()) {

                cout << "No Suggestions Found\n";
            }
            else {

                cout << "\nSuggestions:\n";

                for (auto& p : suggestions) {

                    cout << "- "
                         << p.first
                         << " (searched "
                         << p.second
                         << " times)\n";
                }
            }

            break;
        }

        case 4:

            cout << "\nThank you for using the system.\n";
            return 0;

        default:

            cout << "\nInvalid Choice. Try Again.\n";
        }
    }

    return 0;
}