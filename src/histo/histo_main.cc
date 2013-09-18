//  ********************************************************************
//  This file is part of KAT - the K-mer Analysis Toolkit.
//
//  KAT is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  KAT is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with KAT.  If not, see <http://www.gnu.org/licenses/>.
//  *******************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <jellyfish/compacted_hash.hpp>

#include "histogram.hpp"
#include "histo_args.hpp"
#include "histo_main.hpp"

using std::vector;
using std::string;
using std::cout;
using std::cerr;

using kat::HistoArgs;
using kat::Histogram;

// Start point
int kat::histoStart(int argc, char *argv[])
{
    // Parse args
    HistoArgs args(argc, argv);

    // Print command line args to stderr if requested
    if (args.verbose)
        args.print();

    // Setup output channel
    ofstream_default out(args.output, std::cout);
    if(!out.good())
        die << "Error opening output file '" << args.output << "'" << err::no;

    // Create the sequence coverage object
    Histogram<hash_query_t> histo(&args);

    // Output histo parameters to stderr if requested
    //if (args.verbose)
    //    histo.printVars(cerr);

    // Do the work
    histo.do_it();

    // Output the results
    histo.print(out, args.full);

    // Close the output channel
    out.close();

    return 0;

}
