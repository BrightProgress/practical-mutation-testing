/*
 * Dictionary.hpp
 *
 *  Created on: 26-Dec-2021
 *  Author : Prahlad Sampath
 */

#include "dbc.hpp"

#include <memory>
#include <string>
#include <vector>

class Dictionary {
public:
	Dictionary();
	~Dictionary();

	void add(std::string const & toAdd);

	bool check(std::string const & word, bool toLower = true) const;

	void listing(std::string const & prefix = std::string()) const;

	size_t size() const;

private:
	std::string prefix_;
	std::unique_ptr<Dictionary> suffix_[26];
	bool isWord_;
	bool isEmpty_;
	bool isRoot_;

	// private constructor
	Dictionary(std::string const & word);

	// Workhorse function - assumes validated word.
	void add_(std::string const & toAdd);

	// private access function with invariant
	inline std::unique_ptr<Dictionary>& suffix(size_t  idx);
	inline std::unique_ptr<Dictionary> const & suffix(size_t  idx) const;

	// class invariant
	bool classInvariant_();
};

namespace utils {
	std::vector<std::string> split(std::string const& wordList);
	void splitAndAddToDict(Dictionary& dict, std::string line);
}