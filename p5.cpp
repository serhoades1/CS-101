#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <queue>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    
    ~TreeNode() { 
        delete left;
        delete right;
    }
};

void buildTree(TreeNode* &root, const vector<int> &inorder, const vector<int> &levelorder);
void HuffmanGetCodes(TreeNode* node, string prefix, vector<string> &output);
string decodeText(const string &encoded, TreeNode* root);
void readTraversalData(const string &filename, vector<int> &traversal);

int findIndex(const vector<int> &arr, int start, int end, int value);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <inorder_file> <levelorder_file> <encoded_text_file>" << endl;
        return 1;
    }

    string inorderFile = argv[1];
    string levelorderFile = argv[2];
    string encodedTextFile = argv[3];
    string input;

    ifstream inorderStream(inorderFile);
    ifstream levelorderStream(levelorderFile);
    ifstream encodedTextStream(encodedTextFile);

    if (!inorderStream || !levelorderStream || !encodedTextStream) {
        cerr << "Error: Unable to open one or more input files." << endl;
        return 1;
    }

    vector<int> inorder, levelorder;
    int num;
    while (inorderStream >> num) inorder.push_back(num);
    while (levelorderStream >> num) levelorder.push_back(num);

    TreeNode *root = nullptr;
    buildTree(root, inorder, levelorder);

    string encodedText;
    // while (!encodedTextStream.eof()) {
        
    // }
    // getline(encodedTextStream, encodedText); // Read the entire line, including spaces
    string line;
    while (getline(encodedTextStream, line)) {
        
        input += line;
    }


    cout << decodeText(input, root);

    delete root;

    return 0;
}

void buildTree(TreeNode* &root, const vector<int> &inorder, const vector<int> &levelorder) {
    unordered_map<int, int> inorderMap;
    for (size_t i = 0; i < inorder.size(); ++i) {
        inorderMap[inorder[i]] = i;
    }
    root = nullptr;

    queue<pair<TreeNode*, pair<int, int>>> q;
    q.push({nullptr, {0, inorder.size() - 1}});
    for (int i : levelorder) {
        auto parent = q.front();
        q.pop();
        int inStart = parent.second.first;
        int inEnd = parent.second.second;
        int inIndex = findIndex(inorder, inStart, inEnd, i);
        TreeNode* newNode = new TreeNode(i);

        if (inIndex > inStart) {
            q.push({newNode, {inStart, inIndex - 1}});
        }

        if (inIndex < inEnd) {
            q.push({newNode, {inIndex + 1, inEnd}});
        }

        if (!root) {
            root = newNode;
        }

        else if (!parent.first->left) {
            parent.first->left = newNode;
        }
        else {
            parent.first->right = newNode;
        }
    }
}

void HuffmanGetCodes(TreeNode *node, string prefix, vector<string> &output) {
    if (node == nullptr) {
        return;
    }
    if (node->left == nullptr && node->right == nullptr) {
        output[node->val] = prefix;
        return;
    }
    else {
        HuffmanGetCodes(node->left, prefix + "0", output);
        HuffmanGetCodes(node->right, prefix + "1", output);
    }
}

string decodeText(const string &encoded, TreeNode *root) {
    string decoded;
    TreeNode *curr = root;

    for (const char &ch : encoded) {
        if (curr == nullptr) {
            cerr << "Error: Invalid Huffman encoding." << endl;
            return decoded;
        }

        if (ch == '0') {
            curr = curr->left;
        } 
        else if (ch == '1') {
            curr = curr->right;
        } 
        else {
            cerr << "Error: Invalid character in encoded text." << endl;
            return decoded;
        }

        if (curr && curr->left == nullptr && curr->right == nullptr) {
            decoded += static_cast<char>(curr->val); 
            curr = root; 
        }
    }


    if (curr != root) {
        cerr << "Error: Incomplete Huffman encoding." << endl;
        return decoded;
    }

    // cout << "You decoded the text correctly. Good job!" << endl;
    return decoded;
}

void readTraversalData(const string &filename, vector<int> &traversal) {
    ifstream file(filename);
    if (file.is_open()) {
        int num;
        while (file >> num) {
            traversal.push_back(num);
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

int findIndex(const vector<int> &arr, int start, int end, int value) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}