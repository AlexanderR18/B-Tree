#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "ArgumentManager.h"
using namespace std;

struct Node
{
  int *keys;
  Node** childptr; //Two start because it is a node structure that holds the dynamic array
  bool leaf;
  int size;


Node(int degree)
{
  keys = new int[degree];
  childptr = new Node*[degree+1];
  leaf = true;
  size = 0;
  for(int i=0; i<degree; i++)
  childptr[i] = nullptr;
}

void splitChild(Node*parent, int d){
  Node* rightNode = new Node(d);
  int mid = (d-1)/2;
  int rightLoc = 0, childInc = 0;
  int numKeyNodes = parent->size;
  int valueToAdd = parent->keys[mid];  
  float value = ceil(float(d)/2);
  int childAdd = value;
    
  
  for(int i = mid+1; i < numKeyNodes; i++){
  rightNode->keys[rightLoc] = parent->keys[i];
  rightLoc++;
  parent->size = (parent->size)-1;
  rightNode->size = (rightNode->size)+1;
  }
  
  if(!parent->leaf == true){
  for(int i = childAdd; i<=d; i++){
    rightNode->childptr[childInc] = parent->childptr[i];
    childInc++;
  }
      rightNode->leaf = parent->leaf;
  }
  int a = d-1;
  while(childptr[a] != parent){
    childptr[a+1] = childptr[a];
    a--;
    }
    childptr[a+1] = rightNode;
    int i = d-1;
  while(i>=1 && !keys[i-1])
    i--;
  while(i>=1 && keys[i-1] > valueToAdd){
    keys[i] = keys[i-1];
    i--;
    }
    keys[i] = valueToAdd;
    size++;
    parent->size = (parent->size)-1;

  //Copy half of left node to right node
  
  //Copy half the pointer of left node to right node if it is not a lead node

  //Find the correct position to add the new array

  //Add the middle keys to the parent

}

Node* searchNode(int data, Node*n, int d, Node*temp)
{
  if(leaf == true){
  int i = d;
  while(!keys[i-1])
    i--;
  while(i != 0 && keys[i-1] > data){
    keys[i] = keys[i-1];
    i--;
  }
  keys[i] = data;
  size += 1;
  }
  else{
  int i = 0;
  
    
  while(i<size && data>keys[i])
    i++;
  childptr[i]->searchNode(data, this, d, temp);
  }

  
  if(size == d){
    if(this == temp){
    Node* temp = new Node(d);
    temp->leaf = false;
    temp->childptr[0] = this;
    temp->splitChild(this, d);
    return temp;
  }
    else
    n->splitChild(this, d);
  }


  
  return temp;
}

void printLevel(Node* t, int level, ofstream &output){
    if(level>=2 && leaf==true)
      return;
    else if(level==1){
    for(int i = 0; i<size; i++)
      output << keys[i] << " ";
    }
    else{
    for(int i=0; i<=size; i++)
      childptr[i]->printLevel(t, level-1, output);
    }

  
  }

void getHeight(Node*t,int height,int & a){
    if(leaf == true){
      a = height;
      return;    
    }
    childptr[0]->getHeight(t, height+1, a);
  }

};


//Add first split after
class btree
{
  private:
  Node* root;
  int degree;

  public:
  btree(int _degree)
{
  root = nullptr;
  degree = _degree;
}

int getHeight(){
  int a;
  root->getHeight(root, 1, a);
  return a;
}

bool isEmpty(std::ifstream &in){
  return in.peek() == std::ifstream::traits_type::eof();
}


void insert(int data){
  if(root==nullptr){
    root =new Node(degree);
    root->keys[0]=data;
    root->size=1;
    }
  else{
    root = root->searchNode(data,root,degree,root);
  }
}

/*
void addAtLeaf(Node* parent, Node* n, int data)
{
  if(n->leaf)
  {
    int i = n->size;
    while(i != 0 && n->keys[i-1] > data)
      {
        n->keys[i] = n->keys[i-1];
        i--;
      }
    n->keys[i] = data;
    n->size +=1;
  }
  else{
    int i=0;
    while(i < n->size && data > n->keys[i]){
      i++;
    }
    addAtLeaf(n,n->childptr[i], data);
  }

  if(n->size == degree){
    if(n==root){
      Node*temp = new Node(degree);
      temp->leaf = false;
      temp->childptr[0] = n;
      splitChild(temp, data);
      root = temp;
    }
    else{
      splitChild(parent,data);
    }
  }
}
*/


void printLevel(int level, ofstream &outputFile){
    root->printLevel(root, level, outputFile);
  }


void duplicate(ifstream& file, btree& tree){
    int temp;
    vector<int>val;
    vector<int>sorted;
    while(!file.eof()){
      file >> temp;
      val.push_back(temp);
    }
    sorted = val;
    sort(sorted.begin(), sorted.end());
    for(int i = 0; i<sorted.size(); i++){
    if(sorted[i] == sorted[i+1]){
    for(int j = val.size(); j>0; j--){
    if(sorted[i] == val[j]){
      val.erase(val.begin()+j, val.begin()+(j+1));
      break;
        }
      }
    }
  }
    for(int i = 0; i< val.size(); i++)
      tree.insert(val[i]);
  }
};


int main(int argc, char * argv[]) {
  ArgumentManager am(argc,argv);
  string input = am.get("input");
  string command = am.get("command");
  string output = am.get("output");
  
  ifstream in(input);
  ifstream com(command);
  ofstream out(output);


  /////////////////////////////////

  
  if(in.peek()==std::ifstream::traits_type::eof() || com.peek()==std::ifstream::traits_type::eof()){
  } 
  else{
    string d;
    string c;
    com >> c;
    int equalIndex = c.find("=");
    int degree;
    for(int i = equalIndex+1; i<c.size(); i++)
    d += c[i];
    degree = stoi(d);

    btree Tree(degree);
    Tree.duplicate(in, Tree);
    int height = Tree.getHeight();
    
    out << "Height=" << height << endl;
    string line;
    int level;
    while(getline(com, line)){
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    if(line[0] == 'L'){
    int space = line.find("l")+1;
    string l;
    for(int i = space+1; i<line.size(); i++)
    l+=line[i];
    level = stoi(l);
    if(!(level>height))
    Tree.printLevel(level,out);
      else
      out<<"Empty";
      out<<endl;
      }
    }
  }




  ////////////////////////////
  in.close();
  out.close();
  com.close();
  return 0;
}