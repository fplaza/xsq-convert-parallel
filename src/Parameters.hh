#ifndef PARAMETERS_HH
#define PARAMETERS_HH

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include <vector>
#include <string>
#include <boost/optional.hpp>

struct Parameters
{
	std::string input_file;
	std::string output_dir;
	unsigned nb_processes;
	boost::optional<std::vector<std::string>> prefixes_wanted;
};

Parameters get_parameters(int argc, char *argv[]);

#endif // PARAMETERS_HH
