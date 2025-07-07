#include <bits/stdc++.h>
using namespace std;

string text;

struct Node {
  map<char, Node *> children;
  Node *suffixLink;
  int start;
  int *end;

  Node(int start, int *end)
      : suffixLink(nullptr),
        start(start),        
        end(end)             
  {}
};

Node *root = nullptr;       
Node *lastNewNode = nullptr; 
Node *activeNode = nullptr;
int activeEdge = -1;         
int activeLength = 0;
int remainingSuffixCount = 0;
int leafEnd = -1;          
int *rootEnd = nullptr;     

Node *newNode(int start, int *end) {
  Node *node = new Node(start, end);
  return node;
}

int edgeLength(Node *n) { return *(n->end) - n->start + 1; }

bool walkDown(Node *currNode) {
  if (activeLength >= edgeLength(currNode)) {
    activeEdge += edgeLength(currNode);
    activeLength -= edgeLength(currNode);
    activeNode = currNode;
    return true;
  }
  return false;
}

void extendSuffixTree(int pos) {
  leafEnd = pos;
  remainingSuffixCount++;
  lastNewNode = nullptr;

  while (remainingSuffixCount > 0) {
    if (activeLength == 0) {
      activeEdge =
          pos;
    }
    char currentChar = text[activeEdge];

    if (activeNode->children.find(currentChar) == activeNode->children.end()) {
      activeNode->children[currentChar] = newNode(pos, &leafEnd);
      if (lastNewNode != nullptr) {
        lastNewNode->suffixLink = activeNode;
        lastNewNode = nullptr;
      }
    } else {
      Node *nextNode = activeNode->children[currentChar];
      if (walkDown(nextNode)) {
        continue;
      }
      if (text[nextNode->start + activeLength] == text[pos]) {
        if (lastNewNode != nullptr) {
          lastNewNode->suffixLink = activeNode;
          lastNewNode = nullptr;
        }
        activeLength++;
        break;
      }
      int *splitEnd = new int(nextNode->start + activeLength - 1);
      Node *split = newNode(nextNode->start, splitEnd);
      activeNode->children[currentChar] = split;
      split->children[text[pos]] = newNode(pos, &leafEnd);
      nextNode->start += activeLength;
      split->children[text[nextNode->start]] = nextNode;
      if (lastNewNode != nullptr) {
        lastNewNode->suffixLink = split;
      }
      lastNewNode = split;
    }
    remainingSuffixCount--;
    if (activeNode == root && activeLength > 0) {
      
      activeLength--;
      activeEdge = pos - remainingSuffixCount + 1;
    } else if (activeNode != root) {
      activeNode = activeNode->suffixLink;
    }
  }
}

void buildSuffixTree() {
  rootEnd = new int(-1);
  root = newNode(-1, rootEnd);
  root->suffixLink = root;
  activeNode = root;
  for (int i = 0; i < (int)text.size(); i++) {
    extendSuffixTree(i);
  }
}


void printSuffixes(Node *n, string current) {
  if (n->start != -1) {
    current += text.substr(n->start, *(n->end) - n->start + 1);
  }
  if (n->children.empty()) {
    cout << current << endl;
    return;
  }
  for (auto &p : n->children) {
    printSuffixes(p.second, current);
  }
}

int main() {
  text = "banana$";
  buildSuffixTree();
  cout << "Sufijos representados en el Suffix Tree:" << endl;
  printSuffixes(root, "");
  return 0;
}
