// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Manta
// Copyright (c) 2013-2015 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/sequencing/licenses/>
//

///
/// \author Chris Saunders
///

#pragma once

#include "EdgeOptions.hh"
#include "manta/Program.hh"
#include "options/AlignmentFileOptions.hh"
#include "options/CallOptionsDiploid.hh"
#include "options/CallOptionsShared.hh"
#include "options/CallOptionsSomatic.hh"
#include "options/ReadScannerOptions.hh"
#include "options/SVRefinerOptions.hh"

#include <string>
#include <vector>


struct GSCOptions
{
    AlignmentFileOptions alignFileOpt;
    EdgeOptions edgeOpt;
    ReadScannerOptions scanOpt;
    SVRefinerOptions refineOpt;
    CallOptionsShared callOpt;
    CallOptionsDiploid diploidOpt;
    CallOptionsSomatic somaticOpt;

    std::string graphFilename;
    std::string referenceFilename;
    std::string statsFilename;
    std::string chromDepthFilename;
    std::string truthVcfFilename;
    std::string edgeRuntimeFilename;
    std::string edgeStatsFilename;

    std::string candidateOutputFilename;
    std::string diploidOutputFilename;
    std::string somaticOutputFilename;

    bool isVerbose = false; ///< provide some high-level log info to assist in debugging

    bool isSkipAssembly = false; ///< if true, skip assembly and run a low-resolution, breakdancer-like subset of the workflow

    bool isSkipScoring = false; ///< if true, skip quality scoring and output candidates only

    bool isSkipRemoteReads = false; ///< if true, don't search for non-local mapq0 mate pairs for assembly

    bool isRNA = false; ///< if true, RNA specific filtering on candidates and diploid scoring is used

    unsigned minCandidateSpanningCount = 3; ///< how many spanning evidence observations are required to become a candidate?

    unsigned minScoredVariantSize = 51; ///< min size for scoring and scored output following candidate generation
};


void
parseGSCOptions(
    const manta::Program& prog,
    int argc, char* argv[],
    GSCOptions& opt);
