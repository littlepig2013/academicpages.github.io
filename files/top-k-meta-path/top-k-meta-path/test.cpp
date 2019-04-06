#include "TopKCalculator.h"
#include "Utils.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <ctime>

#define F1_BETA 1

using namespace std;

void output(vector<double> result, string metric){
	cout << endl;
	if(result.size() > 0){
		cout << "Metrics" << "\t" << metric << "\t" << metric << "@k" << endl;
		cout.precision(6);
		cout << fixed;
		for(int i = 0; i < result.size(); i++){
			cout << i+1 << "\t" << result[i] << "\t";
			double sum = 0.0;
			for(int j = 0; j <= i; j++){
				sum += result[j]; 
			}
			cout << sum/(i + 1) << endl;	
		}

	}else{
		cerr << "No Meta Pah Found in the HIN Graph." << endl;
	}

}

void printUsage(const char* argv[]){
	cout << "Usage: " << endl;
	cout << argv[0] << " --connectivity dataset (positive pairs file name) (negative pairs file name) k TF-IDF-type length-penalty (beta)" << endl;
	cout << endl;

	cout << "\t TF-IDF-type:" << endl;
	cout << "\t\t M-S -> MNI-based Support" << endl;
	cout << "\t\t S-M-S -> Strength-based M-S" << endl;
	cout << "\t\t B-S -> Binary-based Support" << endl;
	cout << "\t\t SLV1 -> Strength & Length based Version 1" << endl;
        cout << "\t\t SLV2 -> Strength & Length based Version 2" << endl;
	cout << "\t\t SP -> Shortest Path" << endl;
	
	cout << "connectivity mode:" << endl;
	cout << "\t length-penalty(l is the meta-path's length): " << endl;
	cout << "\t\t 1 -> 1/beta^l" << endl;
	cout << "\t\t 2 -> 1/factorial(l)" << endl;
	cout << "\t\t 3 -> 1/l" << endl;
	cout << "\t\t 4 -> 1/e^l" << endl;

	cout << endl;

	
	cout << "Please note that:" << endl;
	cout << "\t (1) If you want to run an experiment of any strength-based method, you should run training mode first using topKQuery to accelerate it." << endl;
	cout << "\t (2) Evaluating the accuracy of a meta path is time-consiming" << endl;
	cout << endl;


}

bool readConnectivitySampleFile(string pos_file, string neg_file, vector<pair<int, int>> & pos_pairs, vector<pair<int, int>> & neg_pairs){
	ifstream posSamplesIn(pos_file.c_str(), ios::in);
	ifstream negSamplesIn(neg_file.c_str(), ios::in);
	if(!posSamplesIn.good() || !negSamplesIn.good()){
		cerr << "Error when reading sample files" << endl;
		return false;
	}

	pos_pairs.clear();	
	neg_pairs.clear();
	string pos_line, neg_line;
	while(getline(posSamplesIn, pos_line) && getline(negSamplesIn, neg_line)){
		vector<string> pos_strs = split(pos_line, "\t");
		vector<string> neg_strs = split(neg_line, "\t");	
		if(pos_strs.size() != 2){
			cerr << "Unsupported format for the pair: " << pos_line << endl;
		}else{
			int pos_src = atoi(pos_strs[0].c_str());
			int pos_dst = atoi(pos_strs[1].c_str());
			pos_pairs.push_back(make_pair(pos_src, pos_dst));

			int neg_src = atoi(neg_strs[0].c_str());	
			int neg_dst = atoi(neg_strs[1].c_str());
			neg_pairs.push_back(make_pair(neg_src, neg_dst));
		}

	}
	posSamplesIn.close();
	negSamplesIn.close();
	return true;
		
}



double getConnectivityAccuracy(vector<int> meta_path, vector<pair<int, int>> pos_pairs, vector<pair<int, int>> neg_pairs, HIN_Graph & hin_graph_){

	int pos_hit_count = 0; 
	int neg_hit_count = 0;
	for(vector<pair<int, int>>::iterator iter = pos_pairs.begin(); iter != pos_pairs.end(); iter++){
		if(TopKCalculator::isConnected(iter->first, iter->second, meta_path, hin_graph_)){
			pos_hit_count++;	
		}		
	}
	for(vector<pair<int, int>>::iterator iter = neg_pairs.begin(); iter != neg_pairs.end(); iter++){
		if(TopKCalculator::isConnected(iter->first, iter->second, meta_path, hin_graph_)){
			neg_hit_count++;	
		}		
	}
	return pos_hit_count*1.0/(pos_hit_count + neg_hit_count);
}



int main(int argc, const char * argv[]) {

	if(argc > 5){
		
		int penalty_type = DEFAULT_PENALTY_TYPE;
		double beta = DEFAULT_BETA;
		string tfidf_type = DEFAULT_TFIDF_TYPE;
		string test_type;

				if(strcmp(argv[1], "--connectivity") == 0 || strcmp(argv[1], "-c") == 0){
					test_type = "connectivity";
					if(argc > 7){
						tfidf_type = argv[6];
						penalty_type = atoi(argv[7]);
						if(penalty_type != 1 && penalty_type != 2){
							cerr << "The penalty_type parameter must be 1 or 2" << endl;
							return -1;
                                		}
					}
						
					if(argc > 8 && penalty_type == 1){
						beta = atof(argv[8]);
						if(beta <= 0 || beta >= 1){ 
							cerr << "The beta parameter must be greater than 0 and less than 1" << endl; 
							return -1;
						}							
					}
					tfidfSetup(tfidf_type.c_str(), penalty_type, beta);
			
				}else{
						printUsage(argv);
						return -1;
				}
		
		string dataset, pos_pairs_file_name;
			int k;
			dataset = argv[2];
			pos_pairs_file_name = argv[3];
			k = atoi(argv[5]);

		HIN_Graph hin_graph_;

			map<int, vector<Edge>> adj;
			map<int,string> node_name;
			map<int,string> node_type_name;
			map<int,int> node_type_num;
			map<int,vector<int>> node_id_to_type;
			map<int,string> edge_name;

			hin_graph_ = loadHinGraph(dataset.c_str(), node_name, adj, node_type_name, node_type_num, node_id_to_type, edge_name);

			 if(strcmp(tfidf_type.c_str(), "S-M-S") == 0 || strcmp(tfidf_type.c_str(), "SLV1") == 0 || strcmp(tfidf_type.c_str(), "SLV2") == 0 ){	
				loadMetaInfo(dataset, hin_graph_);
			}
		vector<pair<int, int>> pos_pairs;

		vector<pair<int, int>> neg_pairs;
			if(strcmp(test_type.c_str(), "connectivity") == 0 && !readConnectivitySampleFile(pos_pairs_file_name, argv[4], pos_pairs, neg_pairs)){
					return -1;
			}

		
		int sample_size = pos_pairs.size();
		vector<double> accuracy_result (k, 0.0);

		double time_cost = 0.0;


		set<int> candidate_entities;

		for(int i = 0; i < pos_pairs.size(); i++){
					int src = pos_pairs[i].first;
					int dst = pos_pairs[i].second;

					vector<pair<vector<double>, vector<int>>> topKMetaPaths;
			clock_t t2, t1;
			t1 = clock();
			topKMetaPaths = TopKCalculator::getTopKMetaPath_TFIDF(src, dst, k, hin_graph_);
			t2 = clock();
			double curr_time_cost = (double) ((0.0 + t2 - t1)/CLOCKS_PER_SEC);
			cout << "Time cost for this pair is " << curr_time_cost << " seconds" << endl;
			time_cost += curr_time_cost;
			if(strcmp(test_type.c_str(), "efficiency") == 0){
				topKMetaPaths.clear();
				continue;
			}	
	
			vector<double> tmp_accuracy_result;	
			vector<double> tmp_recall_result;
			for(vector<pair<vector<double>, vector<int>>>::iterator iter_path = topKMetaPaths.begin(); iter_path != topKMetaPaths.end(); iter_path++){
				
				vector<int> curr_meta_path = iter_path->second;
	
				double accuracy;
				if(strcmp(test_type.c_str(), "connectivity") == 0){
					accuracy = getConnectivityAccuracy(curr_meta_path, pos_pairs, neg_pairs, hin_graph_);	
				}
	
				tmp_accuracy_result.push_back(accuracy);	
				for(int j = 0; j < curr_meta_path.size(); j++){
					int curr_edge_type = curr_meta_path[j];
					if(curr_edge_type < 0){
						curr_edge_type = -curr_edge_type;
						if(edge_name.find(curr_edge_type) != edge_name.end()){
							cout << "[" << edge_name[curr_edge_type] << "]^(-1)" << "\t";
						}else{
							cout << "NA" << "\t";
						}
					}else{
						if(edge_name.find(curr_edge_type) != edge_name.end()){
							cout << "[" << edge_name[curr_edge_type] << "]" << "\t";	
						}else{
							cout << "NA" << "\t";
						}
					}
				}	
				cout << endl;	
				cout.precision(4);
						cout << fixed;
				cout << "accuracy: " << accuracy << endl;
								
				
			}			
			cout << endl;
			topKMetaPaths.clear();
			for(int j = 0; j < k; j++){
				accuracy_result[j] += tmp_accuracy_result[j];
			}
		}

						
			
		cout << "Average time cost is " << time_cost/sample_size << " seconds" << endl;

		// calculate the average value
		for(int j = 0; j < k; j++){
			accuracy_result[j] /= sample_size;
		}

		output(accuracy_result, "Accuracy");

		
	}else{
		printUsage(argv);
		return -1;
	}		

	
	return 0;	

}
