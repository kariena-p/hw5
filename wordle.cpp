#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// std::set<std::string> wordle(
//     const std::string& in,
//     const std::string& floating,
//     const std::set<std::string>& dict)

void findValid(const std::string& in,
               const std::string& floating,
               std::string& current,
               const std::set<std::string>& dict,
               std::set<std::string>& result,
               int loc); 


bool hasPrefix(const std::set<std::string>& dict, const std::string& pre);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> result;
    std::string current = in;

    findValid(in, floating, current, dict, result, 0);

    return result;
}


////////////////////////////////////////////////////////////////////////////////////////
bool hasPrefix(const std::set<std::string>& dict, const std::string& pre) {
    auto it = dict.lower_bound(pre);
    if(it == dict.end()) return false;
    return it->compare(0, pre.size(), pre) == 0;
}

void findValid(const std::string& in,
               const std::string& floating,
               std::string& current,
               const std::set<std::string>& dict,
               std::set<std::string>& result,
               int loc)
{
    int n = in.size();

    // BASE CASE
    if(loc == n) {
        if(floating.empty() && dict.find(current) != dict.end()) {
            result.insert(current);
        }
        return;
    }

    // PRUNE: prefix must be possible
    if(!hasPrefix(dict, current.substr(0, loc)))
        return;

    int blanks = n - loc;
    if(blanks < (int)floating.size())
        return;

    if(in[loc] != '-') {
        current[loc] = in[loc];
        findValid(in, floating, current, dict, result, loc+1);
        return;
    }

    // CASE 1: must use floating letters
    if((int)floating.size() == blanks) {
        for(size_t i = 0; i < floating.size(); i++) {
            char f = floating[i];
            std::string newFloating = floating;
            newFloating.erase(i,1);

            current[loc] = f;
            findValid(in, newFloating, current, dict, result, loc+1);
        }
        return;
    }

    // CASE 2: try floating letters
    for(size_t i = 0; i < floating.size(); i++) {
        char f = floating[i];
        std::string newFloating = floating;
        newFloating.erase(i,1);

        current[loc] = f;
        findValid(in, newFloating, current, dict, result, loc+1);
    }

    // CASE 3: try non-floating letters
    for(char c = 'a'; c <= 'z'; c++) {
        if(floating.find(c) == std::string::npos) {
            current[loc] = c;
            findValid(in, floating, current, dict, result, loc+1);
        }
    }
}
