/*
 * Dictionary_utils.hpp
 *
 *  Created on: 26-Dec-2021
 *  Author : Prahlad Sampath
 */

#include "Dictionary.hpp"

namespace utils {
	std::vector<std::string> split(std::string const& wordList);
	void splitAndAddToDict(Dictionary& dict, std::string line);
}