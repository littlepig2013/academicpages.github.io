//
//  DataReader.cpp
//

#include "DataReader.h"
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include <sstream>



bool DataReader::readADJ(string path, map<int, vector<Edge> > & adja)
{
    ifstream adj_in(path.c_str(), ios::in);
    string sline;
    int src = 0, dst, type;
    
    while(adj_in >> src >> dst >> type)
    {
        Edge tempedge(dst, type, 0.0, 0.0);
        if(adja.find(src) == adja.end()){
            vector<Edge> t;
            //t.clear();
            //adj[src] = t;
            adja.insert(make_pair(src, t));
        }
        
        adja[src].push_back(tempedge);
    }
    adj_in.close();
    return true;
}


bool DataReader::readNodeName(string path, map<int, string> &node_name,map<int,string> &node_type_name){
    ifstream input_in(path.c_str(), ios::in);
    string name;
    int id;
    while (input_in >> name >> id)
    {
            map<int,string>::iterator iter;
            iter=node_type_name.find(id);
            if (iter==node_type_name.end()){
                node_type_name[id]=name;
            }
    }
    input_in.close();
    return true;
    
}

bool DataReader::readNodeIdToType(string path, map<int,vector<int> > &node_id_to_type){
    ifstream input_in(path.c_str(), ios::in);
    
    string myline;
    int id, baseid;
    while (getline(input_in, myline))
    {
        istringstream iss(myline);
        iss >> baseid;
        while (iss >> id)
        {
            node_id_to_type[baseid].push_back(id);
        }
    }
    input_in.close();
    return true;
}

bool DataReader::readEdgeName(string path,map<int,string> &edge_name){
    ifstream input_in(path.c_str(), ios::in);
    string name;
    int id;
    while (input_in >> name >> id){
        edge_name[id]=name;
    }
    input_in.close();
    return true;
}
