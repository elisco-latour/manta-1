//
// Manta - Structural Variant and Indel Caller
// Copyright (c) 2013-2019 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

/// \file
/// \author Chris Saunders
/// \author Naoki Nariai
///

#pragma once

#include "EdgeRuntimeTracker.hh"
#include "FatSVCandidate.hh"
#include "GSCOptions.hh"
#include "GSCEdgeStatsManager.hh"
#include "appstats/SVFinderStats.hh"
#include "htsapi/bam_streamer.hh"
#include "manta/ChromDepthFilterUtil.hh"
#include "manta/SVCandidateSetData.hh"
#include "manta/SVLocusScanner.hh"
#include "svgraph/EdgeInfo.hh"
#include "svgraph/SVLocusSet.hh"

#include <vector>


struct SVFinder
{
    SVFinder(
        const GSCOptions& opt,
        const SVLocusScanner& readScanner,
        const bam_header_info& bamHeader,
        const AllSampleReadCounts& readCounts,
        EdgeRuntimeTracker& edgeTracker,
        GSCEdgeStatsManager& edgeStatMan);

    ~SVFinder();

    void
    findCandidateSV(
        const SVLocusSet& cset,
        const EdgeInfo& edge,
        SVCandidateSetData& svData,
        std::vector<SVCandidate>& svs);

    void
    checkResult(
        const SVCandidateSetData& svData,
        const std::vector<SVCandidate>& svs) const;

private:

    /// Given a directed edge from the SV locus graph, process and cache data required for SV discovery
    /// from the source (local) node of that edge.
    ///
    /// \param[in] locus Graph locus containing the input edge
    /// \param[in] localNodeIndex Source (local) node of the input directed graph edge
    /// \param[in] remoteNodeIndex Sink (remote) node of the input directed graph edge
    /// \param[in] isLocalNodeGraphEdgeNode1 True if localNodeIndex is nodeIndex1 in the graph
    void
    addSVNodeData(
        const bam_header_info& bamHeader,
        const SVLocus& locus,
        const NodeIndexType localNodeIndex,
        const NodeIndexType remoteNodeIndex,
        const GenomeInterval& searchInterval,
        const reference_contig_segment& localNodeRefSeq,
        const bool isLocalNodeGraphEdgeNode1,
        SVCandidateSetData& svData);

    /// readCandidates are the set of hypotheses generated by individual read pair --
    /// this is the read pair which we seek to assign to one of the identified SVs (in svs)
    /// or we push the candidate into svs to start a new candidate associated with this edge
    ///
    /// this is meant as only a temporary form of hypothesis generation, in the current system
    /// we do at least delineate alternative candidates by strand and region overlap, but over
    /// the longer term we should be able to delineate cluster by a clustering of possible
    /// breakend locations.
    ///
    /// \param[in] isExpandSVCandidateSet if false, don't add new SVs or expand existing SVs
    ///
    void
    assignFragmentObservationsToSVCandidates(
        const SVLocusNode& node1,
        const SVLocusNode& node2,
        const std::vector<SVObservation>& readCandidates,
        const bool isExpandSVCandidateSet,
        SVCandidateSetSequenceFragment& fragment,
        std::vector<FatSVCandidate>& svs,
        const unsigned bamIndex);

    /// \brief Either process the fragment to discover new SVs and expand existing SVs,or
    /// go through and add pairs to existing SVs without expansion
    ///
    void
    processSequenceFragment(
        const SVLocusNode& node1,
        const SVLocusNode& node2,
        const bam_header_info& bamHeader,
        const reference_contig_segment& node1RefSeq,
        const reference_contig_segment& node2RefSeq,
        const unsigned bamIndex,
        const bool isExpandSVCandidateSet,
        std::vector<FatSVCandidate>& svs,
        SVCandidateSetSequenceFragment& fragment,
        SVFinderStats& stats);

    void
    getCandidatesFromData(
        const SVLocusNode& node1,
        const SVLocusNode& node2,
        const bam_header_info& bamHeader,
        const reference_contig_segment& node1RefSeq,
        const reference_contig_segment& node2RefSeq,
        SVCandidateSetData& svData,
        std::vector<SVCandidate>& svs,
        SVFinderStats& stats);

    void
    findCandidateSVImpl(
        const SVLocusSet& cset,
        const EdgeInfo& edge,
        SVCandidateSetData& svData,
        std::vector<SVCandidate>& svs,
        SVFinderStats& stats);

    const ChromDepthFilterUtil&
    dFilter() const
    {
        return *(_dFilterPtr);
    }

    const ReadScannerOptions _scanOpt;
    const std::vector<bool> _isAlignmentTumor;
    std::unique_ptr<ChromDepthFilterUtil> _dFilterPtr;
    const SVLocusScanner& _readScanner;

    const std::string _referenceFilename;

    const bool _isRNA;
    const bool _isVerbose;
    bool _isSomatic;

    typedef std::shared_ptr<bam_streamer> streamPtr;
    std::vector<streamPtr> _bamStreams;

    /// this is only here as syscall cache:
    std::vector<SVObservation> _readCandidates;

    /// throwaway stats tracker...
    SampleEvidenceCounts _eCounts;

    /// rate of spanning read noise estimated from the current dataset for each sample
    /// - estimate is roughly (anom + split)/all
    std::vector<double> _spanningNoiseRate;

    /// rate of assembly read noise estimated from the current dataset for each sample
    /// - assembly read noise means reads with edges which have high mismatch density or soft-clipping
    std::vector<double> _assemblyNoiseRate;

    EdgeRuntimeTracker& _edgeTracker;
    GSCEdgeStatsManager& _edgeStatMan;
};
