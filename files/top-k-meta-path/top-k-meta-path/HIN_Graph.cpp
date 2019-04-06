//
//  HIN_Graph.cpp
//

#include "HIN_Graph.h"
#include <algorithm>
#include <fstream>

using namespace std;

HIN_Edge::HIN_Edge(int s, int d, int e):src_(s), dst_(d), edge_type_(e)
{
    
}
HIN_Edge::HIN_Edge(){}

bool HIN_Edge::operator==(const HIN_Edge &e)
{
    if(this->dst_ != e.dst_)
        return false;
    if(this->src_ != e.src_)
        return false;
    if(this->edge_type_ != e.edge_type_)
        return false;
    return true;
}


void HIN_Graph::buildGraph(map<int, string> node_name, map<int, vector<Edge> > adj, map<int, string> node_type_name, map<int, int> node_type_num, map<int, vector<int> > node_id_to_type, map<int, string> edge_name)
{
/*
 
    
 */   map<int, string>::iterator iter;
    node_types_ = node_type_name;
    for(iter = node_type_name.begin(); iter != node_type_name.end(); ++iter){
        node_types_id_[iter->second] = iter->first;
        //node_types_.push_back(iter->second);
    }
    edge_types_.push_back("NONERELATIONFORYAGO");
    for(iter = edge_name.begin(); iter != edge_name.end(); ++iter){
        edge_types_id_[iter->second] = iter->first;
        edge_types_.push_back(iter->second);
    }
    
    for(map<int, string>::iterator i = node_name.begin(); i != node_name.end(); ++i){
        int TheOldID = i->first;
        string key = i->second;
        HIN_Node tempNode;
        tempNode.id_ = TheOldID;
        tempNode.title_ = TheOldID;
        tempNode.key_ = key;
        
        nodes_[TheOldID] = tempNode;
        key2id_[key] = TheOldID;
    }
    
    for(map<int, vector<int> >::iterator iter2 = node_id_to_type.begin(); iter2 != node_id_to_type.end(); ++iter2){
        
        int TheOldID = iter2->first;
	vector<int> curr_types_id_ = iter2->second;

        for(int i = 0; i < curr_types_id_.size(); ++i){
            nodes_[TheOldID].types_id_.push_back(curr_types_id_[i]);
            //cout << node_id_to_type[TheOldID][i] << endl;
        }
    }
    
    map<int, vector< Edge> >::iterator iterator;
    for(iterator = adj.begin(); iterator != adj.end(); ++iterator){
        int src = iterator->first;
        for(int j = 0; j < iterator->second.size(); ++j){
            int dst = iterator->second[j].dst_;
            int oldType = iterator->second[j].type_;
            if(oldType < 0){
                /*
                int type = -oldType;
                HIN_Edge tempEdge(dst, src, type);
                tempEdge.pro_ = iterator->second[j].rpro_;
                tempEdge.rpro_ = iterator->second[j].pro_;
                if(find(edges_src_[tempEdge.src_].begin(), edges_src_[tempEdge.src_].end(), tempEdge) == edges_src_[tempEdge.src_].end()){
                    edges_src_[tempEdge.src_].push_back(tempEdge);
                }
                if(find(edges_dst_[tempEdge.dst_].begin(), edges_dst_[tempEdge.dst_].end(), tempEdge) == edges_dst_[tempEdge.dst_].end()){
                    edges_dst_[tempEdge.dst_].push_back(tempEdge);
                }
                 */
                
            }else{
                int type = oldType;
                HIN_Edge tempEdge(src, dst, type);
               
                edges_src_[tempEdge.src_].push_back(tempEdge);
                edges_dst_[tempEdge.dst_].push_back(tempEdge);
            }
            
        }
    }
    for(int i = 0; i < node_types_.size(); ++i){
        idOftheSameType_[i] = (vector<int>());
    }
    for(int i = 0; i < nodes_.size(); ++i){
        for(vector<int>::iterator j = nodes_[i].types_id_.begin(); j != nodes_[i].types_id_.end(); ++j){
            int type = *j;
            idOftheSameType_[type].push_back(i);
        }
    }
}

