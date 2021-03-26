#include <iostream>
#include <string>
#include "Encoder.h"
#include "Decoder.h"

using namespace std;

void printHelp(){
	cout << "Invalid arguments." << endl;
	cout << "Type \"huffman [mode] [arguments]\" where mode and its arguments are one of the following:" << endl;
	cout << "-e [src] [dest]: encode the source file and place the result into the specified destination" << endl;
	cout << "-d [src] [dest]: decode the source file and place the result into the specified destination" << endl;
	exit(1);
}


int main (int argc, char** argv){
	//cout << "argc: " << argc << endl;
	//cout << "0: " << argv[0] << endl;
	//cout << "1: " << argv[1] << endl;
	//cout << "2: " << argv[2] << endl;
	//cout << "3: " << argv[3] << endl;
	if(argc != 4){
		printHelp();
	}
	string proc = argv[1];
	string in = argv[2];
	string out = argv[3];
	if(proc == "-e"){
		Encoder * e = new Encoder(in);
		e->buildFrequencyTable();
		e->encode();
		e->writeEncodedFile(out);
		return 0;
	}else if(proc == "-d"){
		Decoder * d = new Decoder(in);
		d->buildFrequencyTableFromFile();
		d->decode();
		d->writeUncompressedFile(out);
		return 0;
	}
	return 1;
}
