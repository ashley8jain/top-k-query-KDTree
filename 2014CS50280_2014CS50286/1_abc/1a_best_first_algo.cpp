#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <algorithm>
#include <time.h>
#include <limits.h>

using namespace std;

// construct balanced KD Tree

class Node{
public:
    vector<double> point;
    vector<double> min,max;
    Node *left,*right;
};

Node* insert(Node *root, vector<double> point,int depth,int dim){
    
    if(root==NULL)
    {
        root = new Node();
        root->point = point;
        root->left = root->right = NULL;
        return root;
    }
    
    int split_dim = depth % dim;
    
    if(point[split_dim] < root->point[split_dim])
    {
        root->left = insert(root->left, point, depth+1, dim);
    }
    else
        root->right = insert(root->right, point, depth+1, dim);
    
    return root;
}



int curr_dim = 0;

bool compare(vector<double> p1, vector<double> p2){
    return (p1[curr_dim] < p2[curr_dim]);
}

Node* build(int start, int end, vector<vector<double> > points, int depth, int dim){
    
    cout<<start<<", "<<end<<endl;
    
    if(start==end){
        Node* root = new Node();
        root->point = root->min = root->max = points[start];
        root->left = NULL;
        root->right = NULL;
        //        cout<<"stop"<<endl;
        return root;
    }
    
    if(start>end)
        return NULL;
    
    curr_dim = depth % dim;
    sort(points.begin()+start,points.begin()+end+1,compare);
    
    Node* root = new Node();
    int length = end - start +1;
    //    cout<<"length: "<<length<<endl;
    int mid = start+length/2;
    //    cout<<"mid: "<<mid<<endl;
    root->point = points[mid];
    
    for(int i=0;i<dim;i++){
        root->min.push_back(INT_MAX);
        root->max.push_back(INT_MIN);
        for(int j=start; j<=end; j++){
            if(root->min[i] > points[j][i]){
                root->min[i] = points[j][i];
            }
            if(root->max[i] < points[j][i]){
                root->max[i] = points[j][i];
            }
        }
    }
    
    if(start!=mid)
        root->left = build(start, mid-1, points, depth+1, dim);
    else
        root->left = NULL;
    
    root->right = build(mid+1, end, points, depth+1, dim);
    
    return root;
}


Node* constructKdTree(vector<vector<double> > points,int n,int dim,Node* root){
    
    root = build(0, points.size()-1, points, 0, dim);
//    curr_dim = 0;
//    sort(points.begin(),points.end(),compare);
//    swap(points[0], points[(points.size()-1)/2]);
//
//    for(int i=0;i<n;i++){
//        root = insert(root,points[i],0,dim);
//    }
    
    return root;
}

// best-first-algorithm
class object{
public:
    vector<double> point;
    double dist;
    object(){};
    object(vector<double> pt,double d){
        point = pt;
        dist = d;
    }
};

class object2{
public:
    Node* root;
    double dist;
    object2(Node* tmp,double d){
        root = tmp;
        dist = d;
    }
};

double squared_dist(vector<double> point1,vector<double> point2,int dim){
    double dis = 0.0;
    double mag = 0.0;
    for(int i=0;i<dim;i++){
        mag = point1[i]-point2[i];
        dis+= (mag*mag);
    }
    return dis;
}

class comparator{
public:
    int operator() (const object& p1, const object& p2){
        return p1.dist < p2.dist;
    }
};

class comparator2{
public:
    int operator() (const object2& o1,const object2& o2){
        return o1.dist > o2.dist;
    }
};

//delta
double dist(Node* node, vector<double> point, int dim){
    double distn = 0;
    double magn = 0;
    for(int i=0;i<dim;i++){
        if(point[i]<node->min[i]){
            magn = point[i]-node->min[i];
            distn += magn*magn;
        }
        else if(point[i]>node->max[i]){
            magn = point[i]-node->max[i];
            distn += magn*magn;
        }
        
    }
    return distn;
}

bool comparator_ties(object &o1, object &o2){
    if(o1.dist!=o2.dist)
        return true;
    else{
        for(int i=0;i<o1.point.size();i++){
            if(o1.point[i]>o2.point[i])
                return false;
            else if(o1.point[i]<o2.point[i])
                return true;
        }
    }
    return true;
}


void best_first_algo(Node* root,int n,int dim,vector<double> query_point,int k){
    priority_queue<object,vector<object>,comparator> answer_set;
    priority_queue<object2,vector<object2>,comparator2> candidates;
    
    object2 *root_o = new object2(root,0);
    candidates.push(*root_o);
    
    
    while(!candidates.empty()){
        //        cout<<"start: "<<endl;
        object2 o2 = candidates.top();
        candidates.pop();
        
        object* o = new object(o2.root->point, squared_dist(o2.root->point, query_point, dim));
        
        if (answer_set.size() < k)
            answer_set.push(*o);
        else if(o->dist < answer_set.top().dist){
            answer_set.pop();
            answer_set.push(*o);
        }
        
        
        if(o2.root->left!=NULL){
            candidates.push(*(new object2(o2.root->left,dist(o2.root->left,query_point,dim))));
        }
        
        if(o2.root->right!=NULL){
            candidates.push(*(new object2(o2.root->right,dist(o2.root->right,query_point,dim))));
        }
        
    }
    
}







int main(int argc, char* argv[]){
    char* dataset_file = argv[1];
    
    int n,dim;
    ifstream infile;
    infile.open(dataset_file);
    infile >> dim >> n;
    
    double inp;
    vector<vector<double> > points;
    for(int i=0;i<n;i++){
        
        vector<double> point;
        for(int j=0;j<dim;j++){
            infile >> inp;
            point.push_back(inp);
        }
        points.push_back(point);
    }
    infile.close();
    
    Node *root = NULL;
    root = constructKdTree(points,n,dim,root);
    
    char* query_file = new char[100];
    int k;
    cin >> query_file >> k;
    
    
    infile.open(query_file);
    int d_q;
    int n_q;
    infile>>d_q>>n_q;
    vector<vector<double> > query_points;
    for(int i=0;i<n_q;i++){
        vector<double> query_point;
        for(int j=0;j<d_q;j++){
            infile>>inp;
            query_point.push_back(inp);
        }
        query_points.push_back(query_point);
    }
    
    infile.close();
    
    double avg_time = 0;
    double elapsed_time;
    
    clock_t start,end;
    for(int i=0;i<n_q;i++){
        start = clock();
        best_first_algo(root, n, d_q, query_points[i], k);
        end = clock();
        elapsed_time= ((double) end-start);
        //        cout<<elapsed_time<<endl;
        avg_time+= elapsed_time/CLOCKS_PER_SEC;
    }
    avg_time/=n_q;
    cout<<"avg. time: "<<avg_time<<endl;
}

