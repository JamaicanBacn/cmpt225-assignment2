#include "StringList.h"
#include <stdexcept>
#include <iostream>
using std::out_of_range;
using std::cout;
using std::endl;

UndoStack::UndoStack()
{
	magnitude = 4;
	top = 0;
	Undo_array = new StringList[magnitude];
}

UndoStack::~UndoStack()
{
	delete Undo_array;
}

void UndoStack::push( StringList ls )
{
	
	if( top + 1 == magnitude )
	{
		magnitude *= 2;
		StringList* temp = new StringList[magnitude];

		cout << "here" << endl;

		for( int i = 0; i < magnitude/2 ; i++)
		{
			temp[i] = Undo_array[i];
		}


		Undo_array = temp;
	}


	Undo_array[top] = ls;
	top++;
}

StringList UndoStack::pop()
{

	StringList* temp = &Undo_array[top];
	Undo_array[top].~StringList();
	top--;

	return *temp;
}

int UndoStack::length()
{
	return magnitude;
}

// Default constructor - makes an empty list of capacity 10
StringList::StringList()
{
	capacity = 10;
	n = 0;
	arr = new string[capacity];
}

// Copy constructor
StringList::StringList(const StringList& other)
{	
	copyList(other);
}

// ***UNDOABLE
// Overloaded assignment operator
StringList& StringList::operator=(const StringList& other)
{
	previous_steps->push(*this);

	if(&other != this)
	{
		delete[] arr;
		copyList(other);
	}
	return *this;
}

// Destructor
StringList::~StringList()
{
	delete[] arr;
}

// ACCESSORS
// Returns the number of elements in the list
int StringList::size() const
{
	return n;
}

// Returns true if the list is empty, false otherwise
bool StringList::empty() const
{
	return n == 0;
}

// Returns the string at the given index
string StringList::get(int i) const
{
	checkBounds(i, "get");
	return arr[i];
}

// Returns the index of the first occurrence of the given string
int StringList::index(string s) const
{
	int i = 0;
	while (i < n && s != arr[i]) {
		i++;
	}
	if (i == n) {
		return -1;
	}
	else {
		return i;
	}
}

// Returns true iff the given string is in the list
bool StringList::contains(string str) const
{
	return !(index(str) == -1);
}

// Returns true if the two lists are equal, false otherwise.
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
bool StringList::operator==(const StringList& sl) const
{
	if (size() != sl.size())
	{
		return false;
	}
	for (int i = 0; i < size(); i++)
	{
		if (get(i) != sl.get(i))
		{
			return false;
		}
	}
	return true;
}

// Returns true if the two lists are not equal, false otherwise
bool StringList::operator!=(const StringList& sl) const
{
	return !(*this == sl);
}

// Returns a string representation of the list.
string StringList::toString() const
{
	string result = "{";
	for (int i = 0; i < size(); i++)
	{
		if (i > 0) {
			result += ", ";
		}
		result += "\"" + get(i) + "\"";
	}
	return result + "}";
}


// MUTATORS

// ***UNDOABLE
// Sets the value at the given index
void StringList::set(int i, string str)
{
	previous_steps->push(*this);
	checkBounds(i, "set");
	arr[i] = str;
}

// ***UNDOABLE
// Inserts the given string *before* the given index
void StringList::insertBefore(int pos, string str)
{
	previous_steps->push(*this);
	// Doesn't use checkBounds because it's okay to insert at the end
	if (pos < 0 || pos > size()) {
		throw out_of_range("StringList::insertBefore index out of bounds");
	}
	checkCapacity();
	for (int i = n; i > pos; i--) {
		arr[i] = arr[i-1];
	}
	arr[pos] = str;
	n++;
}

// ***UNDOABLE
// Inserts the given string at the front of the list
void StringList::insertFront(string str)
{
	previous_steps->push(*this);
	checkCapacity();
	insertBefore(0, str);
}

// ***UNDOABLE
// Inserts the given string at the back of the list
void StringList::insertBack(string str)
{
	previous_steps->push(*this);

	checkCapacity();
	insertBefore(n, str);
}

// ***UNDOABLE
// Removes the element at the given index and moves elements after it down
void StringList::remove(int pos)
{
	previous_steps->push(*this);

	checkBounds(pos, "remove");
	for (int i = pos; i < n; i++) {
		arr[i] = arr[i + 1];
	}
	n--;
}

// ***UNDOABLE
// Empties the list
void StringList::removeAll()
{
	previous_steps->push(*this);
	for (int i = 0; i < n; i++) {
		arr[i] = "";
	}
	n = 0;
}

// Undoes the last operation that modified the list
void StringList::undo()
{
	// All pre_modification StringLists are pushed into stack
	// push and pop the undo stack to have the prev steps undone

	if( previous_steps->length() == 0)
	{
		return;
	}

	*this = previous_steps->pop();
}

// Prints the list
void StringList::print() const
{ 
	for (int i = 0; i < n; i++) {
		cout << arr[i] << endl;
	}
}

// Helper function for checking index bounds.
void StringList::checkBounds(int i, string s) const
{
	if (i < 0 || i >= size()) {
		throw out_of_range("StringList::" + s + " index out of bounds");
	}
}

// POST: Doubles the capacity of the list if n == capacity
void StringList::checkCapacity()
{
	if (n == capacity) {
		capacity *= 2;
		string* temp = new string[capacity];
		
		for (int i = 0; i < n; i++) {
			temp[i] = arr[i];
		}
		delete[] arr;
		arr = temp;
	}
}

// Helper function to copy the contents of another list
void StringList::copyList(const StringList& lst)
{
	capacity = lst.capacity;
	n = lst.n;
	arr = new string[capacity];

	for (int i = 0; i < n; i++) {
		arr[i] = lst.arr[i];
	}
}