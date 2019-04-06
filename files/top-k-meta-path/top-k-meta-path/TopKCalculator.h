#ifndef __TopKCalculator__
#define __TopKCalculator__

#include <vector>
#include <map>
#include <set>
#include <cstring>

#include "HIN_Graph.h"
#define ALPHA 1
#define STRENGTH_ALPHA 1
#define BPCRW_GAMMA 0.5

using namespace std;

class TfIdfNode
{
public:
	int curr_edge_type_;
	map<int, set<int>> curr_nodes_with_parents_;// node id -> parent node ids
	double max_support_;
	vector<int> meta_path_;
	TfIdfNode* parent_;
	TfIdfNode(int curr_edge_type, map<int, set<int>> curr_nodes_with_parents, double max_support, vector<int> meta_path, TfIdfNode* parent);
	~TfIdfNode();
};

class TfIdfNodePointerCmp
{
public:
	bool operator () (TfIdfNode* & node_p1, TfIdfNode* & node_p2);
};

class TopKCalculator
{
private:
	static int factorial(int n);
	static double getRarity(int src, int dst, set<int> & srcSimilarNodes, set<int> & dstSimilarNodes, vector<int> & meta_path, HIN_Graph & hin_graph_);
	static double getHit(set<int> & srcSimilarNodes, set<int> & dstSimilarNodes, vector<int> meta_path, HIN_Graph & hin_graph_);
	static void updateTopKMetaPaths(double tfidf, double tf, double rarity, vector<int> meta_path, int k, vector<pair<vector<double>, vector<int>>> & topKMetaPath_);
	static bool isConnectedMain(int src, int dst, set<int> src_next_entities, set<int> dst_next_entities, vector<int> meta_path, HIN_Graph & hin_graph_);
	static void getNextEntities(int eid, int edge_type, set<int> & next_entities, HIN_Graph & hin_graph_);
	static void getNextEntities(set<int> eids, int edge_type, set<int> & next_entities, HIN_Graph & hin_graph_);
	static double getSupport(int src, int dst, TfIdfNode* curr_tfidf_node, vector<int> meta_path, HIN_Graph & hin_graph_);
	static double getMaxSupport(double candidateSupport);
public:
	static int penalty_type_;// 1 -> beta^l; 2 -> factorial of l; 3 -> 1/l
	static double beta_;
	static int rarity_type_;// 1 -> true rarity; 0 -> 1(constant); 2 -> Strenth-based rarity
	static int support_type_;// 1 -> MNI; 0 -> 1(constant); 3 -> Strength-based MNI; 4 -> Strength
	static double penalty(int length);
	static set<int> getSimilarNodes(int eid, map<int, HIN_Node> & hin_nodes_, bool output_flag=true);
	static bool isConnected(int src, int dst, vector<int> meta_path, HIN_Graph & hin_graph_);
	static vector<pair<vector<double>, vector<int>>> getTopKMetaPath_TFIDF(int src, int dst, int k, HIN_Graph & hin_graph_);	
	static void saveToFile(vector<pair<double, vector<int>>> topKMetaPaths, string file_name);
	static vector<vector<int>> loadMetaPaths(string file_name);
};

#endif
