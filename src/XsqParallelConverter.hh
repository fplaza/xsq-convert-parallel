#ifndef XSQ_PARALLEL_CONVERTER
#define XSQ_PARALLEL_CONVERTER

// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqFile.hh"
#include <iostream>
#include <string>
#include <boost/optional.hpp>
#include <unistd.h>
#include <sys/wait.h>

class XsqParallelConverter
{
public:
	void convert(const std::string&, const std::string&, unsigned, const boost::optional<std::vector<std::string> >&);

private:
	typedef const char* Arg;
	typedef std::vector<Arg> CommandLine;
	typedef std::vector<CommandLine> CommandLines;

	CommandLines build_command_lines(const std::vector<std::string>&, const std::string&, const std::string&, unsigned);
	void launch_sub_processes(const CommandLines&);
	inline void wait_sub_processes();
};

#endif // XSQ_CONVERTER
