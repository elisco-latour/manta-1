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
///
///

#pragma once

#include "blt_util/SimpleAlignment.hh"
#include "htsapi/bam_record.hh"


void
getAlignment(
    const bam_record& bamRead,
    SimpleAlignment& al);

SimpleAlignment
getAlignment(
    const bam_record& bamRead);

/// generate a mate alignment, assuming same read length and perfect alignment
SimpleAlignment
getFakeMateAlignment(
    const bam_record& bamRead);


