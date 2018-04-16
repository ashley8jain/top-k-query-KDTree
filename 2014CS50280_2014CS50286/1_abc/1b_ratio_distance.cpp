#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <algorithm>
#include <time.h>
#include <math.h>

using namespace std;



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

void sequential_scan(vector<vector<double> > points,int n,int dim,vector<double> query_point,int k,ofstream& outfile, double& secnd, double& hundth){
    
    priority_queue<object,vector<object>,comparator> pq;

    double sqDist;
    
    for(int i=0;i<n;i++){
        sqDist = squared_dist(points[i], query_point, dim);
        
        if (pq.size() < k)
            pq.push(*(new object(points[i],sqDist)));
        else{
            if(sqDist < pq.top().dist){
                pq.pop();
                pq.push(*(new object(points[i],sqDist)));
            }
        }
    }

    vector<object> answers_pts(pq.size());
    while(!pq.empty()){
        object o = pq.top();
//        cout<<o.dist<<" ";
        answers_pts[pq.size()-1] = o;
        pq.pop();
    }
    
    secnd+= answers_pts[1].dist;
    hundth+= answers_pts[99].dist;
    
    for(int i=0;i<answers_pts.size();i++){
        for(int j=0;j<dim;j++){
            outfile<<answers_pts[i].point[j]<<" ";
        }
        outfile<<"\n";
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
    
    ofstream outfile;
    outfile.open("results.txt");

    double secnd,hundth;
    secnd = hundth = 0;
    for(int i=0;i<n_q;i++){
        sequential_scan(points, n, d_q, query_points[i], k, outfile, secnd, hundth);
    }
    secnd /= n_q;
    hundth /= n_q;
    secnd = sqrt(secnd);
    hundth = sqrt(hundth);
    cout<<"second: "<<secnd<<",100th: "<<hundth<<endl;
    double ratio = secnd/hundth;
    cout<<ratio<<endl;
    
    outfile.close();
}
