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

#include "manta/SVModelScoreInfo.hh"

#include <cassert>


struct VcfWriterScoredSV
{
protected:

    void
    setScoreInfo(
        const SVScoreInfo& baseInfo)
    {
        _baseInfoPtr=&baseInfo;
    }

    void
    clearScoreInfo()
    {
        _baseInfoPtr=nullptr;
    }

    const SVScoreInfo&
    getBaseInfo() const
    {
        assert(nullptr != _baseInfoPtr);
        return *_baseInfoPtr;
    }

private:
    const SVScoreInfo* _baseInfoPtr = nullptr;
};
