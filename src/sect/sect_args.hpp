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

#pragma once

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace kat
{
    #define DEFAULT_OUTPUT_PREFIX "./kat_sect_output"
    const uint16_t  DEFAULT_THREADS         = 1;
    const uint16_t  DEFAULT_GC_BINS         = 1001;
    const uint16_t  DEFAULT_CVG_BINS        = 1001;
    const bool      DEFAULT_CVG_LOG         = true;
    const bool      DEFAULT_BOTH_STRANDS    = false;

    class SectArgs
    {
    public:
        const char * fasta_arg;
        const char * db_arg;
        const char * input_type;
        const char * output_prefix;
        uint16_t gc_bins;
        uint16_t cvg_bins;
        bool cvg_logscale;
        uint16_t threads_arg;
        bool both_strands;
        bool verbose;

        // Default constructor
        SectArgs() :
            output_prefix(DEFAULT_OUTPUT_PREFIX), gc_bins(DEFAULT_GC_BINS), cvg_bins(DEFAULT_CVG_BINS),
            cvg_logscale(DEFAULT_CVG_LOG), threads_arg(DEFAULT_THREADS), both_strands(DEFAULT_BOTH_STRANDS), verbose(false)
        {}

        // Constructor that parses command line options
        SectArgs(int argc, char* argv[]) :
            output_prefix(DEFAULT_OUTPUT_PREFIX), gc_bins(DEFAULT_GC_BINS), cvg_bins(DEFAULT_CVG_BINS),
            cvg_logscale(DEFAULT_CVG_LOG), threads_arg(DEFAULT_THREADS), both_strands(DEFAULT_BOTH_STRANDS), verbose(false)
        {
            parse(argc, argv);
        }




    #define seqcvg_args_USAGE "Usage: kat sect [options] -f <fasta_file_path> db_path\n"
        const char * usage() const
        {
            return seqcvg_args_USAGE;
        }

        void error(const char *msg)
        {
            cerr << endl
                 << "Error: " << msg << endl << endl
                 << usage() << endl
                 << "Use --help for more information" << endl << endl;
            exit(1);
        }


    #define sect_args_HELP "Estimates coverage for sequences in a fasta file using jellyfish K-mer counts. " \
      "K-mers containing any Ns derived from sequences in the fasta files will have 0 coverage. " \
      "Output from this tool will consist of a file in fasta format containing K-mer coverage counts for each K-mer of each fasta entry. " \
      "In addition, a space separated table file containing the mean coverage score and GC % of each sequence is produced.  The row order is identical to the original fasta file.\n\n" \
      "Options (default value in (), *required):\n" \
      " -f, --fasta=string          *Fasta file contains sequences that should have coverage estimated.\n" \
      " -o, --output_prefix=string  Prefix path for all output files generated by this tool (" DEFAULT_OUTPUT_PREFIX ").\n" \
      " -x, --gc_bins=uint16        Number of bins for the gc data when creating the contamination matrix (1001).\n" \
      " -y, --cvg_bins=uint16       Number of bins for the cvg data when creating the contamination matrix (1001).\n" \
      " -l, --cvg_logscale          Compresses cvg scores into logscale for determining the cvg bins within the contamination matrix.\n" \
      "                             Otherwise compresses cvg scores by a factor of 0.1 into the available bins.\n" \
      " -t, --threads=uint16        The number of threads to use (1).\n" \
      " -C, --both_strands          IMPORTANT: Whether the jellyfish hash contains K-mers produced for both strands.\n" \
      "                             If this is not set to the same value as was produced during jellyfish counting then output from sect will be unpredicatable.\n" \
      "     --usage                 Usage\n" \
      "     --help                  This message\n" \

        const char * help() const
        {
            return sect_args_HELP;
        }

    #define sect_args_HIDDEN "Hidden options:"
        const char * hidden() const
        {
            return sect_args_HIDDEN;
        }


        void parse(int argc, char *argv[])
        {
            int c;
            int help_flag = 0;
            int usage_flag = 0;
            int verbose_flag = 0;
            int cvg_logscale_flag = 0;

            static struct option long_options[] =
            {
                {"verbose", no_argument,       &verbose_flag, 'v'},
                {"fasta",   required_argument, 0, 'f'},
                {"output_prefix",  required_argument, 0, 'o'},
                {"gc_bins", required_argument, 0, 'x'},
                {"cvg_bins", required_argument, 0, 'y'},
                {"cvg_logscale", no_argument,  &cvg_logscale_flag, 'l'},
                {"threads", required_argument, 0, 't'},
                {"both_strands", required_argument, 0, 'C'},
                {"help",  no_argument,         &help_flag, 'h'},
                {"usage", no_argument,         &usage_flag, 'u'},
                {0, 0, 0, 0}
            };

            static const char *short_options = "f:o:x:y:lt:Cvuh";

            if (argc <= 1)
            {
                cerr << endl
                     << usage() << endl
                     << help() << endl;
                exit(1);
            }

            while (true)
            {
                /* getopt_long stores the option index here. */
                int index = -1;

                c = getopt_long (argc, argv, short_options, long_options, &index);

                /* Detect the end of the options. */
                if (c == -1)
                    break;

                switch (c)
                {
                case ':':
                    cerr << "Missing required argument for "
                              << (index == -1 ? std::string(1, (char)optopt) : std::string(long_options[index].name))
                              << endl << endl;
                    exit(1);
                case 'h':
                    cout << usage() << endl
                         << help() << endl;
                    exit(0);
                case 'u':
                    cout << usage() << endl
                         << "Use --help for more information." << endl << endl;
                    exit(0);
                case '?':
                    cerr << "Use --usage or --help for some help" << endl << endl;
                    exit(1);
                case 'v':
                    verbose = true;
                    break;
                case 'f':
                    fasta_arg = optarg;
                    break;
                case 'o':
                    output_prefix = optarg;
                    break;
                case 't':
                    threads_arg = atoi(optarg);
                    if (both_strands)
                        cerr << "Jellyfish hash to be treated as containing double_stranded information." << endl;
                    break;
                case 'x':
                    gc_bins = atoi(optarg);
                    break;
                case 'y':
                    cvg_bins = atoi(optarg);
                    break;
                case 'C':
                    both_strands = true;
                    break;
                }
            }

            if (help_flag)
            {
                cout << usage() << endl
                     << help() << endl;
                exit(0);
            }

            if (usage_flag)
            {
                cout << usage() << endl
                     << "Use --help for more information." << endl << endl;
                exit(0);
            }

            // Parse arguments
            if(argc - optind != 1)
                error("Requires exactly 1 argument.");
            db_arg = argv[optind++];
        }

        bool outputGiven()
        {
            return output_prefix == NULL ? false : true;
        }


        void print()
        {
            if (verbose)
                cerr << "Verbose flag set" << endl;

            if (fasta_arg)
                cerr << "Fasta file: " << fasta_arg << endl;

            if (threads_arg)
                cerr << "Threads requested: " << threads_arg << endl;

            if (gc_bins)
                cerr << "Number of GC bins for matrix: " << gc_bins << endl;

            if (cvg_bins)
                cerr << "Number of coverage bins for matrix: " << cvg_bins << endl;

            if (db_arg)
                cerr << "Jellyfish hash: " << db_arg << endl;

            if (output_prefix)
                cerr << "Output prefix: " << output_prefix << endl;

            if (both_strands)
                cerr << "Jellyfish hash to be treated as containing double_stranded information." << endl;


            cerr << endl;
        }

    private:
    };
}
