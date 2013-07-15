#pragma once
#ifndef COMMON_H
#define COMMON_H

/* 
 * Precompiled header included by everyone.
 */

/* Detect memory leaks */
#if (_DEBUG && 0)
#include <vld.h>
#endif

/* Streams */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

/* Data structures */
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <utility>
#include <stack>

/* Memory management and smart pointers */
#include <memory>

/* Misc. */
#include <regex>
#include <chrono>
#include <random>

/* Multithreading */
#include <future>

/* Boost stuff */
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/preprocessor.hpp>
#include <boost/assign.hpp>

#ifdef _WIN32
#define REGEX_NAMESPACE std
#else
#include <boost/regex.hpp>
#define REGEX_NAMESPACE boost
#endif

/* Compile-time configuration */
#include "consts.h"

#endif
