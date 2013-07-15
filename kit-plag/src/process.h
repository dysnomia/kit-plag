#pragma once
#ifndef PROCESS_H
#define PROCESS_H
#include "common.h"
#include "ranking.h"
#include "fingerprinting.h"
#include "visualize.h"

/* Main plagiarism detection routine. */
void process(const std::string& ref_name, const std::string& query_name, const std::string& output_dir, 
			 bool report_html = true, bool report_stdout = true);

#endif
