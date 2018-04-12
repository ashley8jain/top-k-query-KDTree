#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>

using namespace std;

class Node{
public:
    double point[20];
    Node *left,*right;
};

Node* insert(Node *root, double point[20],int depth,int dim){
    
    if(root==NULL)
    {
        root = new Node();
        for(int i=0; i<dim; i++)
        {
            root->point[i] = point[i];
            root->left = root->right = NULL;
        }
        return root;
    }
    
    int split_dim = depth % dim;
    
    if(point[split_dim] < root->point[split_dim] )
    {
        root->left = insert(root->left, point, depth+1, dim);
    }
    else
        root->right = insert(root->right, point, depth+1, dim);
    
    return root;
}

Node* constructKdTree(double points[][20],int n,int dim,Node* root){
    
    for(int i=0;i<n;i++){
        root = insert(root,points[i],0,dim);
    }
    
    return root;
}

void best_first_algo(double points[][20],int n,int dim,double query_point[20],int k){
    
}















double squared_dist(double point1[20],double point2[20],int dim){
    double dis = 0.0;
    double mag = 0.0;
    for(int i=0;i<dim;i++){
        mag = point1[i]-point2[i];
        dis+= (mag*mag);
    }
    return dis;
}






class object{
public:
    double *point;
    double dist;
    object(double *pt,double d){
        point = pt;
        dist = d;
    }
};

class comparator{
public:
    int operator() (const object& p1, const object& p2){
        return p1.dist < p2.dist;
    }
};

void sequential_scan(double points[][20],int n,int dim,double query_point[20],int k){
    
    priority_queue<object,vector<object>,comparator> pq;
    cout<<endl;
    for(int j=0;j<dim;j++){
        cout<<query_point[j]<<" ";
    }
    cout<<endl;
    
    for(int i=0;i<n;i++){
        object *o = new object(points[i],squared_dist(points[i], query_point, dim));
        cout<<o->point[0]<<endl;
        cout<<o->dist<<endl<<endl;
        if (pq.size() < k)
            pq.push(*o);
        else if(o->dist < pq.top().dist){
            pq.pop();
            pq.push(*o);
        }
//        pq.push(o);
    }
    
//    cout<<endl<<"answer: "<<endl;
    
    ofstream outfile;
    outfile.open("results.txt");
    
    vector<vector<double> > answers_pts(pq.size());
    while(!pq.empty()){
        object o = pq.top();
        for(int j=0;j<dim;j++){
//            cout<<o.point[j]<<" ";
            answers_pts[pq.size()-1].push_back(o.point[j]);
        }
        pq.pop();
//        cout<<endl;
    }
    
    for(int i=0;i<answers_pts.size();i++){
        for(int j=0;j<dim;j++){
            outfile<<answers_pts[i][j]<<" ";
        }
        outfile<<"\n";
    }
    
}














int main(int argc, char* argv[]){
    
    char* dataset_file = argv[1];
    

    // [TODO] Construct kdTree using dataset_file here
    int n,dim;
    ifstream infile;
    infile.open(dataset_file);
    infile >> dim >> n;
    
    double points[n][20];
    for(int i=0;i<n;i++){
        for(int j=0;j<dim;j++){
            infile >> points[i][j];
//            cerr<<points[i][j]<<" ";
        }
//        cerr<<endl;
    }
    infile.close();

    Node *root = NULL;
    root = constructKdTree(points,n,dim,root);
    
    
    
    
    
    
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
    infile>>d_q;
    double query_point[20];
    for(int i=0;i<d_q;i++){
        infile>>query_point[i];
    }
    infile.close();
    
    
    //kNN
    sequential_scan(points, n, dim, query_point, k);
//    best_first_algo(points, n, dim, query_point, k);
    
    
    
    // Convey to parent that results.txt is ready by sending "1" on stdout | Timer will stop now and this process will be killed
    cout << 1 << endl;
}