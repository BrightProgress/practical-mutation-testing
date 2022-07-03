#pragma once
/*
 * Dictionary.hpp
 *
 *  Created on: 26-Dec-2021
 *  Author : Prahlad Sampath
 */

#include <memory>
#include <string>
#include <vector>

class Dictionary {
public:
	Dictionary();
	~Dictionary();

	void add(std::string const & toAdd);

	bool check(std::string const & word, bool toLower = true) const;

	size_t size() const;

private:
	std::string prefix_;
	std::unique_ptr<Dictionary> suffix_[26];
	bool isWord_;
	bool isEmpty_;
};