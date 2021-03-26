
// String Implementation
// IMPORTANT: Do not use any of the functions in the string C runtime library
// Example. Do not use strcpy, strcmp, etc. Implement your own

// IMPORTANT: See the MyString.h file for a description of
// what each method needs to do.

#include <stdio.h>
#include "MyString.h"

// My own implementation of strlen
int
MyString::slength(const char *s) const
{
  // Add implementation here
	int len = 0;
	while(*s++ != '\0'){
		len++;
	}
 	return len;
}

// Initialize _s. Allocate memory for _s and copy s into _s
void
MyString::initialize(const char * s)
{
  // Add implementation here
  // Allocate memory for _s.
	_s = new char[slength(s)];
  // Copy s into _s
  	int i;
	int length = slength(s);
	for(i = 0; i < length + 1; i++){
		_s[i] = s[i];
	}
	_s[i] = '\0';
}

// Create a MyString from a C string
MyString::MyString(const char * s)
{
  initialize(s);
}

// Create a MyString from a copy of another string
MyString::MyString(const MyString &s)
{
  initialize(s._s);
}

// Create a MyString with an empty string
MyString::MyString()
{
  _s = new char[1];
  *_s = 0;
}

// Assignment operator. Without this operator the assignment is
// just a shallow copy that will copy the pointer _s. If the original _s
// goes away then the assigned _s will be a dangling reference.
MyString &
MyString::operator = (const MyString & other) {
  if (this != &other) // protect against invalid self-assignment
  {
    // deallocate old memory
    delete [] _s;

    // Initialize _s with the "other" object.
    initialize(other._s);

    // by convention, always return *this
    return *this;
  }
}

// Obtain a substring of at most n chars starting at location i
// if i is larger than the length of the string return an empty string.
MyString
MyString::substring(int i, int n)
{
  // Add implementation here
	
  // Make sure that i is not beyond the end of string

  // Allocate memory for substring

  // Copy characters of substring
  if(i > length()){
  	return MyString();
  }
  char * sub;
  sub = new char[n];
  for(int j = 0; j < n; j++){
  	sub[j] = _s[j+i];
  }
  // Return substring
  return MyString(sub);
}

// Remove at most n chars starting at location i
void
MyString::remove(int i, int n)
{
  // Add implementation here

  // If i is beyond the end of string return

  // If i+n is beyond the end trunc string

  // Remove characters
	if(i > length()){
		return;
	}
	if(i+n > length()){
		return;
	}
	for(int j = 0; j < n; j++){
		for(int k = i; k < length()-1; k++){
			_s[k] = _s[k+1];
		}
	}
	_s[length() - n] = '\0';
	return;
}

// Return true if strings "this" and s are equal
bool
MyString::operator == (const MyString & s)
{
  // Add implementation here
  int i = 0;
  while((_s[i] == s._s[i]) && (_s[i] != '\0' && s._s[i] != '\0')){
  	i++;
  }
  return((_s[i] - s._s[i]) == 0);
}


// Return true if strings "this" and s are not equal
bool
MyString::operator != (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while((_s[i] == s._s[i]) && (_s[i] != '\0' && s._s[i] != '\0')){
  	i++;
  }
  return((_s[i] - s._s[i]) != 0);
}

// Return true if string "this" and s is less or equal
bool
MyString::operator <= (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while((_s[i] == s._s[i]) && (_s[i] != '\0' && s._s[i] != '\0')){
  	i++;
  }
  return((_s[i] - s._s[i]) <= 0);
}

// Return true if string "this" is greater than s
bool
MyString::operator > (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while((_s[i] == s._s[i]) && (_s[i] != '\0' && s._s[i] != '\0')){
  	i++;
  }
  return((_s[i] - s._s[i]) > 0);
}

// Return character at position i.  Return '\0' if out of bounds.
char
MyString::operator [] (int i)
{
  // Add implementation here
  if(i > length()){
  	return '\0';
  }
  char * r = new char();
  r = _s+i;
  return *r;
}

// Return C representation of string
const char *
MyString::cStr()
{
  // Add implementation here
  return _s;
}

// Get string length of this string.
int
MyString::length() const
{
  // Add implementation here
  return slength(_s);
}

// Destructor. Deallocate the space used by _s
MyString::~MyString()
{
  // Add implementation here
  delete [] _s;
}

// Concatanate two strings (non member method)
MyString operator + (const MyString &s1, const MyString &s2)
{
  // Add implementation here

  // Allocate memory for the concatenated string

  // Add s1

  // Add s2

  MyString s;
  int size = s1.length() + s2.length() + 1;
  s._s = new char[size];
  int i = 0;
  while(s1._s[i] != '\0'){
  	s._s[i] = s1._s[i];
	i++;
  }
  int j = 0;
  while(s2._s[j] != '\0'){
  	i++;
 	s._s[i] = s2._s[j];
	j++;
  }
  s._s[size] = '\0';
  return s;
}

