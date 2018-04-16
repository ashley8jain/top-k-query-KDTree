#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <algorithm>
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
    
//    root = build(0, points.size()-1, points, 0, dim);
    curr_dim = 0;
    sort(points.begin(),points.end(),compare);
    swap(points[0], points[(points.size()-1)/2]);
    
    for(int i=0;i<n;i++){
        root = insert(root,points[i],0,dim);
    }
    
    return root;
}


















// sequential scan

double squared_dist(vector<double> point1,vector<double> point2,int dim){
    double dis = 0.0;
    double mag = 0.0;
    for(int i=0;i<dim;i++){
        mag = point1[i]-point2[i];
        dis+= (mag*mag);
    }
    return dis;
}

bool break_tie(vector<double> p1,vector<double> p2,int dim){
    for(int i=0;i<dim;i++){
        if(p1[i]>p2[i])
            return false;
        else if(p1[i]<p2[i])
            return true;
    }
    return false;
}


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

class comparator{
public:
    int operator() (const object& p1, const object& p2){
        return p1.dist < p2.dist;
    }
};

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

void sequential_scan(vector<vector<double> > points,int n,int dim,vector<double> query_point,int k,ofstream& outfile){
    
    priority_queue<object,vector<object>,comparator> pq;

    double sqDist;
//    vector<vector<double> > lastPoints;
    
    for(int i=0;i<n;i++){
        sqDist = squared_dist(points[i], query_point, dim);
        
        if (pq.size() < k)
            pq.push(*(new object(points[i],sqDist)));
        else{
            object top = pq.top();
            if(sqDist < top.dist){
                pq.pop();
                pq.push(*(new object(points[i],sqDist)));
//                if(!lastPoints.empty())
//                    lastPoints.clear();
            }
//            else if(sqDist == top.dist){
//                lastPoints.push_back(points[i]);
//            }
        }
    }

    vector<object> answers_pts(pq.size());
    while(!pq.empty()){
        object o = pq.top();
//        cout<<o.dist<<" ";
        answers_pts[pq.size()-1] = o;
        pq.pop();
    }
    
//    sort(answers_pts.begin(),answers_pts.end(),comparator_ties);
    
    for(int i=0;i<answers_pts.size();i++){
        for(int j=0;j<dim;j++){
            outfile<<answers_pts[i].point[j]<<" ";
        }
        outfile<<"\n";
    }
    
}















// best-first-algorithm

class object2{
public:
    Node* root;
    double dist;
    object2(Node* tmp,double d){
        root = tmp;
        dist = d;
    }
};

class comparator2{
public:
    int operator() (const object2& o1,const object2& o2){
        return o1.dist > o2.dist;
    }
};

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


void best_first_algo(Node* root,int n,int dim,vector<double> query_point,int k, ofstream& outfile){
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
        else{
            break;
        }
        
        
        if(o2.root->left!=NULL){
            candidates.push(*(new object2(o2.root->left,dist(o2.root->left,query_point,dim))));
        }
        
        if(o2.root->right!=NULL){
            candidates.push(*(new object2(o2.root->right,dist(o2.root->right,query_point,dim))));
        }
        
    }
    
    vector<vector<double> > answers_pts(answer_set.size());
    while(!answer_set.empty()){
        object o = answer_set.top();
        cout<<o.dist<<" ";
        answers_pts[answer_set.size()-1] = o.point;
        answer_set.pop();
//                cout<<endl;
    }
    
    for(int i=0;i<answers_pts.size();i++){
        for(int j=0;j<dim;j++){
            outfile<<answers_pts[i][j]<<" ";
        }
        outfile<<"\n";
    }
    
}










// main

int main(int argc, char* argv[]){
    char* dataset_file = argv[1];
    
    // [TODO] Construct kdTree using dataset_file here
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
//            cerr<<points[i][j]<<" ";
        }
        points.push_back(point);
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
    
    ofstream outfile;
    outfile.open("results.txt");
    
    for(int i=0;i<n_q;i++){
        sequential_scan(points, n, dim, query_points[i], k, outfile);
//        best_first_algo(root, n, dim, query_points[i], k, outfile);
    }
    
    outfile.close();
    
    // Convey to parent that results.txt is ready by sending "1" on stdout | Timer will stop now and this process will be killed
    cout << 1 << endl;
}