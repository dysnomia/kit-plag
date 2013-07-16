#pragma once
#ifndef CONSTS_H
#define CONSTS_H
#include "common.h"

/* 
 * Compile-time configurations.
 */

/* Preprocessing step. */
const std::string FILE_STOP_WORDS = "data/stop-words-union.txt";
const REGEX_NAMESPACE::regex REGEX_WORD("[\\w]([\\w']|(\\s*-\\s*))+", REGEX_NAMESPACE::regex::optimize);

/* Ranking. */
const double RANKING_MAX_VISUALIZE = 0.8;

/* Fingerprinting. */
const int GRANULARITY = 2;

/* Main html structure. */
const std::string HTML_BEGIN =
	"<!DOCTYPE html>\n"
	"<html lang=\"en\">\n"
	"  <head>\n"
	"    <meta charset=\"utf-8\" />\n"
	"    <script src=\"jquery-1.10.1.min.js\"></script>\n"
	"    <!--<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js\"></script>-->\n"
	"    <script src=\"kit-plag.js\"></script>\n"
	"    <link rel=\"stylesheet\" href=\"kit-plag.css\" />\n"
	"    <title>%1%</title>\n"
	"  </head>\n"
	"  <body>\n"
	"    <header>\n"
	"      <h1>%1%</h1>\n"
	"      <p id=\"global-summary\" class=\"info-text\">Global summary:</p>\n";
const std::string HTML_MAIN_BEGIN =
	"    </header>\n"
	"    <div class=\"main\">\n";
const std::string HTML_MAIN_END =
	"    </div>\n";
const std::string HTML_END =
	"  </body>\n"
	"</html>\n";

/* Divs for compared texts. */
const std::string HTML_MEASURE_BEGIN =
	"      <div class=\"measure\" id=\"measure-%1%\">\n";
const std::string HTML_COMPARE_BEGIN =
	"        <div class=\"compare-container query-%2%\">\n"
	"          <div class=\"query\"><p class=\"info-text\">Query (%1%):</p><pre>\n";
const std::string HTML_COMPARE_MID = 
	"          </pre></div>\n"
	"          <div class=\"ref\"><p class=\"info-text\">Reference (%1%):</p><pre>\n"; 
const std::string HTML_COMPARE_END =
	"          </pre></div>\n"
	"        </div>\n";
const std::string HTML_MEASURE_END = 
	"      </div>\n";

/* Local summary tables. */
const std::string HTML_SUMMARY_BEGIN =
	"        <table class=\"local-summary\"><thead><tr>\n";
const std::string HTML_SUMMARY_MID =
	"        </tr></thead><tbody><tr>\n";
const std::string HTML_SUMMARY_END =
	"          </tr></tbody>\n"
	"        </table>\n";

/* Selection of the measure and the compared texts. */
const std::string HTML_SELECT_BEGIN = 
	"      <table class=\"selector\"><tr><td>Similarity measure:</td><td>\n"
	"        <select id=\"select-%1%\">\n";
const std::string HTML_OPTION = 
	"          <option value=\"%1%\">%2%</option>\n";
const std::string HTML_SELECT_MID =
	"        </select>\n"
	"        </td></tr>\n"
	"        <tr><td>Reference text:</td><td>\n"
	"        <select id=\"select-%1%\">\n";
const std::string HTML_SELECT_END =
	"        </select>\n"
	"        </td></tr>\n"
	"      </table>\n";

/* Mark for whether a reference should be recognized as plagiarized. */
const std::string CLASS_PLAG = "plagiarism";
const std::string CLASS_NOPLAG = "no-plagiarism";

/* Marked sequence of words */
const std::string MARK_BEGIN = "<span class=\"marked %1%\">";
const std::string MARK_END   = "</span>";

/* Informational text preceding some information. */
const std::string INFO_TEXT =
	"      <p class=\"info-text\">%1%</p>\n";

#endif
