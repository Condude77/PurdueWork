//THIS IS SKELETON CODE COPIED FROM CLASS SLIDES

#include "HashTableVoid.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <algorithm>

using namespace std;

int HashTableVoid::hash(const char * key)
{
	int h = 0;
	const char * p = key;
	while (*p) {
		h += *p;
		p++;
	}
	return h % TableSize;
}

HashTableVoid::HashTableVoid()
{
	_buckets = (HashTableVoidEntry **) malloc(TableSize*sizeof(HashTableVoidEntry*));
	for ( int i = 0; i < TableSize; i++) {
		_buckets[i] = NULL;
	}
}

bool HashTableVoid::insertItem( const char * key, void * data)
{
	// Get hash bucket
	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];
	while (e!=NULL) {
		if (!strcmp(e->_key, key)) {
			// Entry found
			e->_data = data;
			return true;
		}
		e = e->_next;
	}
	// Entry not found. Add it.
	e = new HashTableVoidEntry;
	e->_key = strdup(key);
	e->_data = data;
	e->_next = _buckets[h];
	_buckets[h] = e;
	return false;
}

//CUSTOM FUNCTION
bool HashTableVoid::insertUser(const char * user, void * password, int roomNumber){
	HashTableVoidEntry * e = _buckets[roomNumber];
	while(e != NULL){
		if(!strcmp(e->_key, user)){
			e->_data = password;
			return true;
		}
		e = e->_next;
	}
	e = new HashTableVoidEntry;
	e->_key = strdup(user);
	e->_data = strdup((char *)password);
	e->_next = _buckets[roomNumber];
	_buckets[roomNumber] = e;
	return false;
}

//CUSTOM FUNCTION
bool HashTableVoid::insertMessage(const char * message, void * user, int roomNumber){
	HashTableVoidEntry * e = _buckets[roomNumber];
	if(e == NULL){		//first message
		e = new HashTableVoidEntry;
		e->_key = strdup(message);
		e->_data = strdup((char *)user);
		e->_next = _buckets[roomNumber];
		_buckets[roomNumber] = e;
	}else{
		while(e->_next != NULL){
			if(!strcmp(e->_next->_key, message)){
				e->_next->_data = user;
				return true;
			}
			e = e->_next;
		}
		HashTableVoidEntry * e2 = new HashTableVoidEntry;
		e2->_key = strdup(message);
		e2->_data = strdup((char *)user);
		e->_next = e2;
		e2->_next = NULL;
	}
	return false;
}

bool HashTableVoid::find( const char * key, void ** data)
{

	//Get hash bucket
	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];
	while (e!=NULL) {
		if (!strcmp(e->_key, key)) {
			// Entry found
			*data = e->_data;
			return true;
		}
		e = e->_next;
	}
	return false;
}

bool HashTableVoid::removeElement(const char * key)
{
	// Get hash bucket
	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];
	HashTableVoidEntry * prev = NULL;
	while (e!=NULL) {
		if (!strcmp(e->_key, key)) {
			// Entry found
			if (prev != NULL) {
				prev->_next = e->_next;
			} else {
				_buckets[h] = e->_next;
			}
			//free(e->_key);
			delete e;
			return true;
		}
		prev = e;
		e = e->_next;
	}
	return false;
}

//CUSTOM FUNCTION
bool HashTableVoid::removeUser(const char * user, int roomNumber){
	HashTableVoidEntry * e = _buckets[roomNumber];
	HashTableVoidEntry * prev = NULL;
	while(e != NULL){
		if(!strcmp(e->_key, user)){
			if(prev != NULL){
				prev->_next = e->_next;
			}else{
				_buckets[roomNumber] = e->_next;
			}
			delete e;
			return true;
		}
		prev = e;
		e = e->_next;
	}
	return false;
}

HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable){
	_hashTable = hashTable;
	_currentBucket = 0;
	_currentEntry = _hashTable->_buckets[_currentBucket];
}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data)
{
	if(_currentEntry != NULL) { 
	        data = _currentEntry->_data;
		key = _currentEntry->_key;
		_currentBucket++;
		_currentEntry = _hashTable->_buckets[_currentBucket];
		return true;
	}else{
		for(; _currentBucket < _hashTable->TableSize;_currentBucket++) { 
		        _currentEntry = _hashTable->_buckets[_currentBucket];
			if(_currentEntry != NULL){
				data = _currentEntry->_data;
				key = _currentEntry->_key;
				_currentBucket++;
				_currentEntry = _hashTable->_buckets[_currentBucket];
				return true;
			}
		}
	}
	return false;
}

//CUSTOM FUNCTION
bool HashTableVoidIterator::userInRoom(const char * user, int roomNumber){
	_currentEntry = _hashTable->_buckets[roomNumber];
	while(_currentEntry != NULL){
		if(!strcmp((char *)_currentEntry->_key, user)){
			return true;
		}
		_currentEntry = _currentEntry->_next;
	}
	return false;
}

//CUSTOM FUNCTION
void HashTableVoidIterator::retrieveMessages(int fd, const char * & message, void * & user, int roomNumber, int lastMessageNumber){
	_currentEntry = _hashTable->_buckets[roomNumber];
	int messageNumber = 0;
	bool noNewMessage = true;
	while(_currentEntry != NULL){
		user = _currentEntry->_data;
		message = _currentEntry->_key;
		_currentEntry = _currentEntry->_next;
		if(messageNumber >= lastMessageNumber){
			char temp[1000];
			sprintf(temp, "%d %s %s", messageNumber, (char *)user, message);
			const char * msg = (const char *)temp;
			write(fd, msg, strlen(msg));
			const char * msg2 = "\r\n";
			write(fd, msg2, strlen(msg2));
			noNewMessage = false;
		}
		messageNumber++;
	}
	if(noNewMessage){
		const char * msg = "NO-NEW-MESSAGES\r\n";
		write(fd, msg, strlen(msg));
	}else{
		const char * msg = "\r\n";
		write(fd, msg, strlen(msg));
	}
	return;
}

//CUSTOM FUNCTION
void HashTableVoidIterator::retrieveUser(int fd, const char * & user, void * & password, int roomNumber){
	_currentEntry = _hashTable->_buckets[roomNumber];
	vector<string> list;
	while(_currentEntry != NULL){
		password = _currentEntry->_data;
		user = _currentEntry->_key;
		_currentEntry = _currentEntry->_next;
		list.push_back(user);
	}
	sort(list.begin(), list.end());
	for(int i = 0; i < list.size(); i++){
		const char * msg = list[i].c_str();
		write(fd, msg, strlen(msg));
		const char * msg2 = "\r\n";
		write(fd, msg2, strlen(msg2));
	}
	const char * msg = "\r\n";
	write(fd, msg, strlen(msg));
	return;
}
