#include "HIN_Graph.h"
#include "Utils.h"
#include "TopKCalculator.h"

#include <cstring>
#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;


vector<string> split(string str, string separator){
	vector<string> result;
	int cutAt;
	while( (cutAt = str.find_first_of(separator)) != str.npos ){
		if(cutAt > 0){
			result.push_back(str.substr(0, cutAt));
		}else{
			result.push_back("");
		}
		str = str.substr(cutAt + 1);
	}

	if(str.length() > 0){
		result.push_back(str);
	}else{
		result.push_back("");
	}

	return result;
}

HIN_Graph loadACMGraph(map<int,string> & node_name, map<int, vector<Edge>> & adj, map<int,string> & node_type_name, map<int,int> & node_type_num, map<int,vector<int>> & node_id_to_type, map<int,string> & edge_name) {
		clock_t t1, t2, t3;

		t1 = clock();
		DataReader::readADJ("ACM/ACMAdj.txt", adj);
		DataReader::readNodeName("ACM/ACMEntityName.txt", node_name, node_type_name);
		DataReader::readNodeIdToType("ACM/ACMEntityType.txt", node_id_to_type);
		DataReader::readEdgeName("ACM/ACMEdgeType.txt",edge_name);
		t2 = clock();

		cerr << "Take " << (0.0 + t2 - t1)/CLOCKS_PER_SEC << "s to read ACM dataset" << endl;

		HIN_Graph ACM_Graph;
		ACM_Graph.buildGraph(node_name, adj, node_type_name, node_type_num, node_id_to_type, edge_name);
		t3 = clock();
		cerr << "Take " << (0.0 + t3 - t2)/CLOCKS_PER_SEC << "s to construct ACM graph" << endl;

		return ACM_Graph;
}


HIN_Graph loadHinGraph(const char* dataset, map<int,string> & node_name, map<int, vector<Edge>> & adj, map<int,string> & node_type_name, map<int,int> & node_type_num, map<int,vector<int>> & node_id_to_type, map<int,string> & edge_name){
	if(strcmp(dataset, "ACM") == 0){
		return loadACMGraph(node_name, adj, node_type_name, node_type_num, node_id_to_type, edge_name);
	}else{
		cerr << "Unsupported dataset" << endl;
		return HIN_Graph ();
	}
}


void loadMetaInfo(string dataset, HIN_Graph & hin_graph_, string inDir){
        string line;

        map<int, int> nodeTypeNum;
        ifstream nodeTypeNumIn((inDir + dataset + DEFAULT_CACHE_NODE_TYPE_NUM_FILE_SUFFIX).c_str(), ios::in);
        while(getline(nodeTypeNumIn, line)){
                vector<string> strs = split(line, "\t");
                int node_type = atoi(strs[0].c_str());
                int node_type_num = atoi(strs[1].c_str());
                nodeTypeNum[node_type] = node_type_num;
        }
        nodeTypeNumIn.close();

        map<int, int> edgeTypeNum;
        ifstream edgeTypeNumIn((inDir + dataset + DEFAULT_CACHE_EDGE_TYPE_NUM_FILE_SUFFIX).c_str(), ios::in);
        while(getline(edgeTypeNumIn, line)){
                vector<string> strs = split(line, "\t");
                int edge_type = atoi(strs[0].c_str());
                int edge_type_num = atoi(strs[1].c_str());
                edgeTypeNum[edge_type] = edge_type_num;
        }
        edgeTypeNumIn.close();

        map<int, pair<double, double>> edgeTypeAvgDegree;
        ifstream edgeTypeAvgDegreeIn((inDir + dataset + DEFAULT_CACHE_EDGE_TYPE_AVG_DEG_FILE_SUFFIX).c_str(), ios::in);
        while(getline(edgeTypeAvgDegreeIn, line)){
                vector<string> strs = split(line, "\t");
                int edge_type = atoi(strs[0].c_str());
                double avgInDegree = stod(strs[1].c_str());
                double avgOutDegree = stod(strs[2].c_str());
                edgeTypeAvgDegree[edge_type] = make_pair(avgInDegree, avgOutDegree);
        }
        edgeTypeAvgDegreeIn.close();

        hin_graph_.node_type_num_ = nodeTypeNum;
        hin_graph_.edge_type_num_ = edgeTypeNum;
        hin_graph_.edge_type_avg_degree_ = edgeTypeAvgDegree;
}

void getMetaInfo(string dataset, string outDir){

        HIN_Graph hin_graph_;

        map<int, vector<Edge>> adj;
        map<int, string> node_name;
        map<int, string> node_type_name;
        map<int, int> node_type_num;
        map<int, vector<int>> node_id_to_type;
        map<int, string> edge_name;

        hin_graph_ = loadHinGraph(dataset.c_str(), node_name, adj, node_type_name, node_type_num, node_id_to_type, edge_name);

        map<int, vector<HIN_Edge> > hin_edges_src_ = hin_graph_.edges_src_;
        map<int, HIN_Node> hin_nodes_ = hin_graph_.nodes_;

        map<int, int> edgeTypeNum; 
	map<int, int> nodeTypeNum;

        map<int, pair<set<int>, set<int>>> edgeTypeNodes; // edge type -> (input nodes, output nodes)
        map<int, pair<double, double> > edgeTypeAvgDegree; // node type -> (average inner degree, average outer degree)

        for( map<int, HIN_Node>::iterator iter = hin_nodes_.begin(); iter != hin_nodes_.end(); iter++){
                vector<int> curr_node_types_id_ = iter->second.types_id_;
                int node_types_num = curr_node_types_id_.size();
                for(int i = 0; i < node_types_num; i++){
                        int curr_node_type = curr_node_types_id_[i];
                        if(nodeTypeNum.find(curr_node_type) != nodeTypeNum.end()){
                                nodeTypeNum[curr_node_type] += 1;
                        }else{
                                nodeTypeNum[curr_node_type] = 1;
                        }
                }

        }


        for(map<int, vector<HIN_Edge> >::iterator iter = hin_edges_src_.begin(); iter != hin_edges_src_.end(); iter++){
                int src_node_id = iter->first;
                vector<HIN_Edge> curr_edges_src_ = iter->second;
                for(vector<HIN_Edge>::iterator edge_iter = curr_edges_src_.begin(); edge_iter != curr_edges_src_.end(); edge_iter++){
                        if(hin_nodes_.find(src_node_id) != hin_nodes_.end()){
                                int edge_type = edge_iter->edge_type_;
                                if(edgeTypeNum.find(edge_type) != edgeTypeNum.end()){
                                        edgeTypeNum[edge_type] += 1;
                                }else{
                                        edgeTypeNum[edge_type] = 1;
                                }

                                int dst_node_id = edge_iter->dst_;


                                if(edgeTypeNodes.find(edge_type) == edgeTypeNodes.end()){
					set<int> input_nodes;
                                        set<int> output_nodes;
                                        input_nodes.insert(src_node_id);
                                        output_nodes.insert(dst_node_id);
                                        edgeTypeNodes[edge_type] = make_pair(input_nodes, output_nodes);

                                }else{
                                        edgeTypeNodes[edge_type].first.insert(src_node_id);
                                        edgeTypeNodes[edge_type].second.insert(dst_node_id);
                                }

                        }

                }
        }

        for(map<int, pair<set<int>, set<int> > >::iterator iter = edgeTypeNodes.begin(); iter != edgeTypeNodes.end(); iter++){
                int edge_type = iter->first;
                if(edgeTypeNum.find(edge_type) != edgeTypeNum.end()){
                        int curr_edge_type_num = edgeTypeNum[edge_type];
                        edgeTypeAvgDegree[edge_type] = make_pair(curr_edge_type_num*1.0/iter->second.first.size(), curr_edge_type_num*1.0/iter->second.second.size());
                }

        }


        // output
        ofstream nodeTypeNumOut(outDir + dataset + DEFAULT_CACHE_NODE_TYPE_NUM_FILE_SUFFIX, ios::out);
        for(map<int, int>::iterator iter = nodeTypeNum.begin(); iter != nodeTypeNum.end(); iter++){
                nodeTypeNumOut << iter->first << "\t" << iter->second << endl;
        }
        nodeTypeNumOut.close();

        ofstream edgeTypeNumOut(outDir + dataset + DEFAULT_CACHE_EDGE_TYPE_NUM_FILE_SUFFIX, ios::out);
        for(map<int, int>::iterator iter = edgeTypeNum.begin(); iter != edgeTypeNum.end(); iter++){
                edgeTypeNumOut << iter->first << "\t" << iter->second << endl;
        }
        edgeTypeNumOut.close();

        ofstream edgeTypeAvgDegreeOut(outDir + dataset + DEFAULT_CACHE_EDGE_TYPE_AVG_DEG_FILE_SUFFIX, ios::out);
        for(map<int, pair<double, double>>::iterator iter = edgeTypeAvgDegree.begin(); iter != edgeTypeAvgDegree.end(); iter++){
                edgeTypeAvgDegreeOut << iter -> first << "\t" << iter->second.first << "\t" << iter->second.second << endl;
        }
        edgeTypeAvgDegreeOut.close();
}



string getFileName(int src, int dst, string tfidf_type, int k, string dataset, double beta){
		return string(DEFAULT_OUTPUT_DIR) + "/" + dataset + "_" + tfidf_type + "_" + to_string(beta) + "_" + to_string(src) + "_" + to_string(dst) + "_" + to_string(k) + ".txt";
}

void tfidfSetup(const char* tfidf_type, int penalty_type, double beta){

		TopKCalculator::penalty_type_ = penalty_type;
		TopKCalculator::beta_ = beta;

		if(strcmp(tfidf_type, "M-S") == 0){
				TopKCalculator::support_type_ = 1;
				TopKCalculator::rarity_type_ = 1;
		}else if(strcmp(tfidf_type, "S-M-S") == 0){
				TopKCalculator::support_type_ = 3;
				TopKCalculator::rarity_type_ = 1;
		}else if(strcmp(tfidf_type, "B-S") == 0){
				TopKCalculator::support_type_ = 0;
				TopKCalculator::rarity_type_ = 1;
		}else if(strcmp(tfidf_type, "SLV1") == 0){
				TopKCalculator::support_type_ = 4;
				TopKCalculator::rarity_type_ = 0;
				TopKCalculator::penalty_type_ = 3;
		}else if(strcmp(tfidf_type, "SLV2") == 0){
				TopKCalculator::support_type_ = 5;
				TopKCalculator::rarity_type_ = 0;
				TopKCalculator::penalty_type_ = 4;
		}else if(strcmp(tfidf_type, "SP") == 0){
				TopKCalculator::support_type_ = 0;
				TopKCalculator::rarity_type_ = 0;
		}
}
