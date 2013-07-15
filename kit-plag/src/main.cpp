/* 
 * Simple plagiarism detector.
 */

#include "common.h"
#include "process.h"

using namespace std;
using namespace boost::program_options;

int main(int argc, char** argv) {
	// File names for references and queries
	string references_file, queries_file, output_file;
	// Enable visualization (html) and logging (stdout)
	bool report_html, report_stdout;

	// Named program options
	options_description desc(
		"./" + string(argv[0]) + " [options]\n"
		"Options"
		);
	desc.add_options()
		("help,h", "produces this help message")
		("references,r", value<string>(&references_file)->default_value("test/refs.txt"),
			"file containing names of references")
		("queries,q", value<string>(&queries_file)->default_value("test/queries.txt"),
			"file containing names of queries")
		("output,o", value<string>(&output_file)->default_value("output/"),
			"output directory")
		("visualize,v", value<bool>(&report_html)->default_value(true),
			"visual results? (html)")
		("log,l", value<bool>(&report_stdout)->default_value(true),
			"print scores? (stdout)")
	;

	try {
		// Parse program options
		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		// Print help if necessary
		if (vm.count("help")) {
			cerr << desc << endl;
			return 1;
		}

		process(references_file, queries_file, output_file, report_html, report_stdout);
	} catch (std::runtime_error& e) {
		cerr << e.what() << endl;
		return 1;
	} catch (std::exception& e) {
		cerr << e.what() << endl;
		cerr << desc << endl;
		return 1;
	}
	
	return 0;
}