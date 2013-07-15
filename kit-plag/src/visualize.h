#pragma once
#ifndef VISUALIZE_H
#define VISUALIZE_H
#include "common.h"
#include "plag_algorithm.h"
#include "preprocess.h"

/*
 * Beginning and end of html file common to all reports.
 */
void visualize_begin(std::ostream& os, const std::string& title, const std::vector<Normalized>& normal);
void visualize_end(std::ostream& os);

/* Prints a per-measure summary */
void visualize_summary(std::ostream& os, const std::vector<Normalized>& refs, const Normalized& query,
					   std::vector<double> res, const std::string& measure_name);

/* Logs (to stdout) the results of the measure calculations for one query. */
void log_begin(const AlgorithmType& type);
void log_normalize(const std::string& name);
void log_results(const std::string& name, const std::vector<double>& results, const std::vector<Normalized>& refs);

/* New section in console log */
void log_header(const std::string& str);

#endif

