/*
 * Dictionary.hpp
 *
 *  Created on: 26-Dec-2021
 *  Author : Prahlad Sampath
 */

#include <memory>
#include <string>

class Dictionary {
public:
	Dictionary();
	virtual ~Dictionary();

	void addWord(std::string const & toAdd);

	void listing(std::string const & prefix = std::string()) const;
	size_t size() const;

private:
	std::string prefix_;
	Dictionary * suffix_[26];
	bool isWord_;
	bool isEmpty_;
};
