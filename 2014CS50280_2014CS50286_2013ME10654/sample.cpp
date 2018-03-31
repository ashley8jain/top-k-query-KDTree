#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

class Node{
    double point[20];
    Node *left,*right;
};

Node* constructKdTree(double points[][20],int n,int d,Node* root){
    
    
    return root;
}

int main(int argc, char* argv[]) {
    
    char* dataset_file = argv[1];
    

    // [TODO] Construct kdTree using dataset_file here
    int n,d;
    ifstream infile;
    infile.open(dataset_file);
    infile >> d >> n;
    
    double points[n][20];
    for(int i=0;i<n;i++){
        for(int j=0;j<d;j++){
            infile >> points[i][j];
//            cerr<<points[i][j]<<" ";
        }
//        cerr<<endl;
    }
    infile.close();

    Node *root = NULL;
    root = constructKdTree(points,n,d,root);
    
    
    
    
    
    
    // Request name/path of query_file from parent by just sending "0" on stdout
    cout << 0 << endl;
    
    // Wait till the parent responds with name/path of query_file and k | Timer will start now
    char* query_file = new char[100];
    int k;
    cin >> query_file >> k;
    // cerr << dataset_file << " " << query_file << " " << k << endl;
    
    
    // [TODO] Read the query point from query_file, do kNN using the kdTree and output the answer to results.txt
    infile.open(query_file);
    int d_q;
    cin>>d_q;
    double query_point[20];
    for(int i=0;i<d_q;i++){
        infile>>query_point[i];
    }
    infile.close();
    
    
    
    
    // Convey to parent that results.txt is ready by sending "1" on stdout | Timer will stop now and this process will be killed
    cout << 1 << endl;
}
