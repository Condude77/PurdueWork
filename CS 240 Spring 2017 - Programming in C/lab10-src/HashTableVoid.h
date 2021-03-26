//THIS IS SKELETON CODE COPIED FROM CLASS SLIDES

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
struct HashTableVoidEntry {
	const char * _key;
	void * _data;
	HashTableVoidEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
class HashTableVoid {
public:
	// Number of buckets
	enum { TableSize = 2039};
	// Array of the hash buckets.
	HashTableVoidEntry **_buckets;

	// Obtain the hash code of a key
	int hash(const char * key);

public:
	HashTableVoid();

	//Add a record to the hash table. Returns true if key already exists.
	// Substitute content if key already exists.
	bool insertItem( const char * key, void * data);
	
	//CUSTOM FUNCTION
	//adds a user to a specific room.
	bool insertUser(const char * user, void * password, int roomNumber);

	//CUSTOM FUNCTION
	//sends a message to the room.
	bool insertMessage(const char * message, void * user, int roomNumber);

	// Find a key in the dictionary and place in "data" the corresponding record
	// Returns false if key is does not exist
	bool find( const char * key, void ** data);
	
	// Removes an element in the hash table. Return false if key does not exist.
	bool removeElement(const char * key);

	//CUSTOM FUNCTION
	//removes user from room
	bool removeUser(const char * user, int roomNumber);
};
class HashTableVoidIterator {

	int _currentBucket;
	HashTableVoidEntry *_currentEntry;
	HashTableVoid * _hashTable;

public:
	HashTableVoidIterator(HashTableVoid * hashTable);
	bool next(const char * & key, void * & data);

	//CUSTOM FUNCTION
	//gets messages
	void retrieveMessages(int fd, const char * & message, void * & user, int roomNumber, int lastMessageNumber);

	//CUSTOM FUNCTION
	//gets user in room
	void retrieveUser(int fd, const char * & message, void * & user, int roomNumber);

	//CUSTOM FUNCTION
	//checks if user is in a room
	bool userInRoom(const char * user, int roomNumber);
};
