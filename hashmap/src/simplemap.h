// CSE 303 Assignment #1
// Spring 2018
//
// Description: This file specifies a simple map implemented using two vectors.
// << use templates for creating a map of generic types >>
// One vector is used for storing keys. One vector is used for storing values.
// The expectation is that items in equal positions in the two vectors correlate.
// That is, for all values of i, keys[i] is the key for the item in values[i].
// Keys in the map are not necessarily ordered.
//
// The specification for each function appears as comments.
// Students are responsible for implementing the simple map as specified.

#include <vector>
#include <cassert>
#include <iostream>

template <class K, class V>
class simplemap_t {

    // Define the two vectors of types K and V
	// << use std::vector<K> >>
    std::vector<K> *keys;
    std::vector<V> *values;

  public:
    // The constructor should just initialize the vectors to be empty
    simplemap_t()
	{
		keys = new std::vector<K>;
		values = new std::vector<V>;
    }

    // Insert (key, val) if and only if the key is not currently present in
    // the map.  Returns true on success, false if the key was
    // already present.
    bool insert(K key, V val) {
    	// The following is just an example of using C++11 features,
    	// like the 'auto' type and lambda expression
        for (auto i = keys->begin(); i != keys->end(); ++i)
            if (*i == key)
				return false;
        // insert the key and value.  Note that indices should, by default, match
        // between these two vectors.
        keys->push_back(key);
        values->push_back(val);
		return true;

    }

    // If key is present in the data structure, replace its value with val
    // and return true; if key is not present in the data structure, return
    // false.
    bool update(K key, V val)
	{
		for(auto i = keys->begin(); i < keys->end(); i++)
		{
			if(*i == key)
			{
				int pos = i - keys->begin();
				//values->clear();
				//range based for loop
				//for(auto &x: values) values->insert(x, val);
				for(auto cItr = values->begin(); cItr < values->end(); ++cItr)
				{
					if(cItr - values->begin() == pos)
						values->insert(cItr, val);
				}

				return true;
			}
		}
		return false;
    }

    // Remove the (key, val) pair if it is present in the data structure.
    // Returns true on success, false if the key was not already present.
    bool remove(K key)
	{
		for(auto i = keys->begin(); i != keys->end(); ++i)
		{
			if(*i == key)
			{
				keys->erase(key);
				// a member variable
				if (lookup(key).second)
				values->erase(lookup(key).first);
				return true;
			}
		}
		return false;
    }

	void removeall()
	{
		keys->clear();
		values->clear();
	}

    // If key is present in the map, return a pair consisting of
    // the corresponding value and true. Otherwise, return a pair with the
    // boolean entry set to false.
    // Something to think about: do you want to share the memory of the map with the
	// application thread?
    // ...you might get unexpected behaviors.
    std::pair<V, bool> lookup(K key)
	{
		for(auto i = keys->begin(); i < keys->end(); i++)
		{
			if(*i == key)
			{
				return std::make_pair(values->at(*i), true);
			}
		}
		return std::make_pair(values->at(key), false);
        // TO DO: the following is a default return value, do not use it!
        // return std::make_pair(0, false);
    }

    // Apply a function to each key in the map
    void apply(void (*f)(K, V))
	{
    	for (auto i : *keys)
		{
    		f(i, values->at(i));

    	}
    }

};
