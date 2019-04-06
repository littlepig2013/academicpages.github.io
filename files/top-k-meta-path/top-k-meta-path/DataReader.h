//
//  DataReader.h
//

#ifndef __DataReader__
#define __DataReader__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>



#define MAX 100000

using namespace std;

class Edge
{
public:
    int dst_;
    int type_;
    double pro_;
    double rpro_;
    Edge(int x, int y, double d1, double d2): dst_(x), type_(y), pro_(d1), rpro_(d2){}
};

class DataReader
{
public:
    static vector<string> split(string s, char c = ' ');
    
    static bool readADJ(string path, map<int, vector<Edge> > & adj);
    
    static bool readNodeName(string path,map<int,string> &node_name, map<int,string> &node_type_name);
    
    static bool readNodeIdToType(string path, map<int, vector<int> > &node_id_to_type);
    
    static bool readEdgeName(string path,map<int,string> &edge_name);
    
};

#endif
