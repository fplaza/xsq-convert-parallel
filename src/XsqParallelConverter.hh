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
	auto convert(const std::string&, const std::string&, unsigned, const boost::optional<std::vector<std::string>>&) -> void;

private:
	typedef const char* Arg;
	typedef std::vector<Arg> CommandLine;
	typedef std::vector<CommandLine> CommandLines;

	auto build_command_lines(const std::vector<std::string>&, const std::string&, const std::string&, unsigned) -> CommandLines;
	auto launch_sub_processes(const CommandLines&) -> void;
	inline auto wait_sub_processes() -> void;
};

#endif // XSQ_CONVERTER