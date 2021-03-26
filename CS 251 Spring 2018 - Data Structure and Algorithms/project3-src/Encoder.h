#ifndef ENCODER_H
#define ENCODER_H


#include <string>
#include <iostream>
#include <fstream>
#include "TreeNode.h"
#include "MinHeap.h"
#include "HuffTree.h"

using namespace std;

class Encoder
{
	private:
		unsigned frequency_table[256];
		string fp;
		MinHeap * mh;
		HuffTree * ht;
		unsigned short unique;
		// You need to add more class memeber and methods
	public:
		//test_file_path is the input (decoded) file
		Encoder(string file_path);

		//Fills up the frequency_table array where frequency_table[i] 
		//will contain the frequency of char with ASCII code i		
		void buildFrequencyTable();
		
		//Builds the min head and run the encoding algorithm
		void encode();

		//Generates the encoded file and save it as output_file_path
		void writeEncodedFile(string output_file_path);

		~Encoder();
};

#endif
