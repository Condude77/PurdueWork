#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "Encoder.h"

Encoder::Encoder(string file_path){
	fp = file_path;
	mh = new MinHeap();
	ht = new HuffTree();
	unique = 0;
}

void Encoder::buildFrequencyTable(){ 
	for(int i = 0; i < 256; ++i){
        	frequency_table[i] = 0;
        }
        ifstream infile;
	infile.open(fp.c_str());
        unsigned char ch;
        
	while(infile.read((char *)&ch, sizeof(ch))){
		frequency_table[ch]++;
        }
	infile.close();
}

void Encoder::encode(){
	for(int i = 0; i < 256; i++){
		if(frequency_table[i] > 0){
			unique++;
			unsigned char v =  i;
			unsigned int f = frequency_table[i];
			TreeNode * tmp = new TreeNode(v,f);
			mh->insert(tmp);
		}
	}
	ht->buildTree(mh);
	ht->generateCodes();
}

void Encoder::writeEncodedFile(string output_file_path){
        ofstream outfile;
	outfile.open(output_file_path.c_str(), ios::binary | ios::out);

	//header
	outfile.write((char *)&unique, sizeof(unique));
	for(int i = 0; i < 256; i++){
		if(frequency_table[i] > 0){
			unsigned char v = i;
			unsigned int f = frequency_table[i];
			outfile.write((char *)&v, sizeof(v));
			outfile.write((char *)&f, sizeof(f));
		}
	}
	
	//body
	ifstream file;
	file.open(fp);
	unsigned char c;
	string str = "";
	while(file >> noskipws >> c){
		if(c >= 0 && c <= 255){
			str += ht->getCharCode(c);
		}
	}
	int i = 0;
	for(i = 0; i < str.length()-8; i+=8){
		string bitstr = str.substr(i, 8);
		long int b = strtol(bitstr.c_str(), NULL, 2);
		outfile.write((char *)&b, 1);
	}
	
	string bitstr = str.substr(i, str.length() % 8);
	long int b = strtol(bitstr.c_str(), NULL, 2);
	b = b << (8 - str.length() % 8);
	outfile.write((char *)&b, 1);
	file.close();
	outfile.close();
}

Encoder::~Encoder(){
	delete mh;
	delete ht;
}
