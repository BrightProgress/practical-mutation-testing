#pragma once
/*
 * Dictionary_utils.hpp
 *
 *  Created on: 03-Jul-2022
 *  Author : Prahlad Sampath
 */

#include "Dictionary.hpp"

#include <memory>
#include <string>
#include <vector>

namespace utils {
	std::vector<std::string> split(std::string const& wordList);
	void splitAndAddToDict(Dictionary& dict, std::string line);
}