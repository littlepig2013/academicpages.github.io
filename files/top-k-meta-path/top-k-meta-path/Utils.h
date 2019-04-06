#ifndef __Utils__
#define __Utils__

#include "HIN_Graph.h"
#include "TopKCalculator.h"

#include <vector>
#include <cstring>
#include <map>

#define DEFAULT_PENALTY_TYPE 2
#define DEFAULT_BETA 0.1
#define DEFAULT_TFIDF_TYPE "S-M-S" // MNIS in paper
#define DEFAULT_REFINE_FLAG false
#define DEFAULT_OUTPUT_TYPE 1
#define DEFAULT_SCORE_FUNCTION 1
#define DEFAULT_OUTPUT_DIR "newTopKResult"
#define DEFAULT_CACHE_DIRECTORY "cache/"
#define DEFAULT_CACHE_NODE_TYPE_NUM_FILE_SUFFIX "nodeTypeNum.txt"
#define DEFAULT_CACHE_EDGE_TYPE_NUM_FILE_SUFFIX "edgeTypeNum.txt"
#define DEFAULT_CACHE_EDGE_TYPE_AVG_DEG_FILE_SUFFIX "edgeTypeAvgDegree.txt"


using namespace std;

vector<string> split(string str, string separator = "\t");

HIN_Graph loadHinGraph(const char* dataset, map<int,string> & node_name, map<int, vector<Edge>> & adj, map<int,string> & node_type_name, map<int,int> & node_type_num, map<int,vector<int>> & node_id_to_type, map<int,string> & edge_name);

string getFileName(int src, int dst, string tfidf_type, int k, string dataset, double beta=DEFAULT_BETA);

void tfidfSetup(const char* tfidf_type, int penalty_type, double beta);

void loadMetaInfo(string dataset, HIN_Graph & hin_graph_, string inDir=DEFAULT_CACHE_DIRECTORY);

void getMetaInfo(string dataset, string outDir=DEFAULT_CACHE_DIRECTORY);
#endif
