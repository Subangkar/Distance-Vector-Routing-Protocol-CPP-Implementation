//
// Created by subangkar on 12/7/18.
//

#ifndef DVR_UTILS_H
#define DVR_UTILS_H

//#include <bits/stdc++.h>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

#define IS_IN_LIST(item, list) (find(list.begin(), list.end(), item) != list.end())


bool startsWith(string str, string start) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	transform(start.begin(), start.end(), start.begin(), ::tolower);
	return str.compare(0, start.length(), start) == 0;
}

template<typename T>
void print_container(std::ostream &os, const T &container, const std::string &delimiter) {
	std::copy(std::begin(container),
	          std::end(container),
	          std::ostream_iterator<typename T::value_type>(os, delimiter.c_str()));
}


#endif //DVR_UTILS_H
