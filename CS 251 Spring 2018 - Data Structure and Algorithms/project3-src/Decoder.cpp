#include <iostream>
#include <bitset>
#include "Decoder.h"


Decoder::Decoder(string huff_file_path){
	fp = huff_file_path;
	mh = new MinHeap();
	ht = new HuffTree();
	unique = 0;
	header = 0;
}

void Decoder::buildFrequencyTableFromFile(){
	for(int i = 0; i < 256; ++i){
		frequency_table[i] = 0;
	}
	ifstream infile;
	infile.open(fp.c_str());
	infile.read((char *)&unique, 2);
	unsigned char val;
	unsigned int frequency;
	while(header != unique){
		infile.read((char *)&val, 1);
		infile.read((char *)&frequency, 4);
		frequency_table[val] = frequency;
		header++;
	}
	infile.close();
}

void Decoder::decode(){
	for(int i = 0; i < 256; i++){
		if(frequency_table[i] > 0){
			unique++;
			unsigned char v = i;
			unsigned int f = frequency_table[i];
			TreeNode * tmp = new TreeNode(v, f);
			mh->insert(tmp);
		}
	}
	ht->buildTree(mh);
	ht->generateCodes();
}

void Decoder::writeUncompressedFile(string file_path){
	ofstream outfile;
	outfile.open(file_path.c_str());
	ifstream infile;
	infile.open(fp);
	
	unsigned int total = 0;
	for(int i = 0; i < 256; i++){
		total += frequency_table[i];
	}

	infile.read((char *)&unique, 2);
	unsigned char val;
	unsigned int frequency;
	header = 0;
	while(header != unique){
		infile.read((char *)&val, 1);
		infile.read((char *)&frequency, 4);
		frequency_table[val] = frequency;
		header++;
	}
	
	unsigned int body = 0;
	TreeNode * root = ht->getRoot();
	string str = "";
	unsigned char ch;
	while(body != total){
		infile.read((char *)&ch, 1);
		bitset<8> bit(ch);
		for(int i = 7; i >= 0; i--){
			if(bit[i]){
				root = root->getLeft();
			}else{
				root = root->getRight();
			}
			if(root->isLeafNode()){
				unsigned char b = root->getVal();
				outfile << b;
				body++;
				root = ht->getRoot();
				if(body == total){
					break;
				}
			}
		}
	}
	infile.close();
	outfile.close();
}

Decoder::~Decoder(){
	delete mh;
	delete ht;
}
