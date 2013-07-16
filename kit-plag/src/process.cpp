#include "common.h"
#include "consts.h"
#include "process.h"
#include "fileutils.h"
#include "preprocess.h"

using namespace std;
using namespace std::chrono;
using boost::format;
namespace fs = boost::filesystem;

/* Measures the time that a function f needs in s. Repeats rep times for accuracy. */
template<typename T>
static double measure(int rep, T f) {
	auto before = high_resolution_clock::now();
	for (int i = 0; i < rep; ++i) {
		(void) f();
	}
	auto after = high_resolution_clock::now();
	return duration_cast<duration<double>>(after - before).count() / rep;
}

/* Reads a single file and normalizes it. */
typedef pair<string, Normalized> ReadFile;
ReadFile get_single_file(const std::string& filename, const std::string& id, bool report_stdout) {
	/* Log that a text has been normalized. */
	if (report_stdout) {
		log_normalize(filename);
	}

	ifstream file_in;
	open_input(filename, file_in);
	string orig = read_whole_stream(file_in);
	Normalized normal = normalize(id, orig);
	return make_pair(orig, normal);
}

/* Reads a list of file names from names, normalizes them and adds them to algorithm data structures. 
	 orig: original texts, normal: normal texts. */
static void get_texts(const std::string& names, vector<string>& orig /* out */, vector<Normalized>& normal /* out */,
					  bool report_html, bool report_stdout, bool is_reference, const vector<Algorithm*>& algorithms) {
	ifstream names_in;
	open_input(names, names_in);
	auto dir = fs::path(names);
	dir.remove_filename();

	/* Create promises to read and normalise the files. */
	string line;
	vector<future<ReadFile>> file_readers;
	for (string file; names_in >> file; ) {
		auto name = dir / file;
		file_readers.push_back(async([=] {
			return get_single_file(name.string(), name.stem().string(), report_stdout);
		}));
	}
	
	/* Now actually read the files. */
	for (auto& file_reader: file_readers) {
		auto read_file = file_reader.get();
		orig.push_back(read_file.first);
		normal.push_back(read_file.second);
		
		/* Add as reference if necessary */
		if (is_reference) {
			for (auto& algo: algorithms) {
				algo->add_reference(normal.back());
			}
		}

		/* Remove unnecessary information from memory if not needed for later visualization. 
		   Somewhat hackish. */
		Normalized& cur = normal.back();
		if (!report_html) {
			orig.pop_back();
			if (is_reference) {
				cur.words.clear();
			}

			cur.indexes.clear();
			cur.end_indexes.clear();
		}
	}
}

/* Compute and visualize the plagiarism testing result for one algorithm. (non-pure) */
static void do_algorithm(Algorithm& algo, const AlgorithmType& measure,
						 const vector<Normalized>& refs, const vector<string>& refs_orig,        
						 const vector<Normalized>& queries, const vector<string>& queries_orig,
						 vector<ofstream>& output_files,
						 bool report_html, bool report_stdout, bool divide) {
	if (report_stdout) {
		log_begin(measure);
	}

	for (int i = 0; i < int(queries.size()); ++i) {
		const auto& query = queries[i];
		auto& os = output_files[i];

		/* Main part: compute similarity scores */
		vector<double> res = algo.compute(query);

		// HACK: HACK: HACK: divide by maximum value to normalise, assumption: query is in references 
		if (divide) {
			double max_similarity = *max_element(res.begin(), res.end());
			for (double& val: res) {
				val /= max_similarity;
			}
		}

		// report on stdout
		if (report_stdout) {
			log_results(query.name, res, refs);
		}

		// actual html report
		if (report_html) {
			os << format(HTML_MEASURE_BEGIN) % measure.short_name;
			visualize_summary(os, refs, query, res, measure.name);
			for (int j = 0; j < int(refs.size()); ++j) {
				algo.visualize(os, queries[i], queries_orig[i], refs[j], refs_orig[j]);
			}
			os << HTML_MEASURE_END;
		}
	}

	if (report_stdout) {
		cout << endl << endl;
	}
} 

/* 
 * Main function for processing a set queries vs. a set of references.
 *    ref_file:  file containing the file names of the references. 
 *	 query_file: file containing the file names of the queries.
 *	 output_dir: directory to put the reports in. 
 */
void process(const std::string& refs_file, const std::string& queries_file, const std::string& output_dir, 
			 bool report_html /*= true*/, bool report_stdout /*= true*/) {
	/* Initialise the used algorithm variants. */
	Ranking ranking;
	vector<Fingerprint> fps;
	vector<Algorithm*> algos;
	algos.push_back(&ranking);
	for (const auto& variant: fingerprint_variants) {
		fps.push_back(Fingerprint(variant));
	}
	for (int i = 0; i < int(fps.size()); ++i) {
		algos.push_back(&fps[i]);
	} 

	/* Read and normalize both texts and references. Also add the references to the algos. */
	vector<Normalized> refs_normal, queries_normal;
	vector<string> refs_orig, queries_orig;
	/* Read references */
	if (report_stdout) {
		log_header("Reading references");
	}
	get_texts(refs_file, refs_orig, refs_normal, report_html, report_stdout, true, algos);
	if (report_stdout) {
		cout << endl;
	}
	/* Read queries */
	if (report_stdout) {
		log_header("Reading queries");
	}
	get_texts(queries_file, queries_orig, queries_normal, report_html, report_stdout, false, algos);
	if (report_stdout) {
		cout << endl << endl;
	}

	/* Open the report-file per query */
	vector<ofstream> output_files(queries_normal.size());
	if (report_stdout) {
		cout << fixed << setprecision(6);
	}
	if (report_html) {
		for (int i = 0; i < int(output_files.size()); ++i) {
			open_output(output_dir + "/output_" + queries_normal[i].name + ".html", output_files[i]);
			output_files[i] << fixed << setprecision(6);
			visualize_begin(output_files[i], "Plagiarism report for " + queries_normal[i].name, refs_normal);
		}
	}

	/* Fingerprinting part */
	for (int i = 0; i < int(fps.size()); ++i) {
		do_algorithm(fps[i], fingerprint_variants[i], refs_normal, refs_orig, 
					 queries_normal, queries_orig, output_files, report_html, report_stdout, false);
	}

	/* Ranking part */
	for (const auto& type: ranking_variants) {
		ranking.set_type(type);
		do_algorithm(ranking, type, refs_normal, refs_orig, 
					 queries_normal, queries_orig, output_files, report_html, report_stdout, true);
	}
	
	/* Finish report */
	if (report_html) {
		for (int i = 0; i < int(output_files.size()); i++) {
			visualize_end(output_files[i]);
		}
	}
}
