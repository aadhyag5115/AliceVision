// This file is part of the AliceVision project.
// Copyright (c) 2017 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/mvsUtils/MultiViewParams.hpp>
#include <aliceVision/mvsData/StaticVector.hpp>
#include <aliceVision/mvsData/Pixel.hpp>
#include <aliceVision/depthMap/DepthSimMap.hpp>

namespace aliceVision {
namespace depthMap {

struct SgmParams;
class PlaneSweepingCuda;

/**
 * @brief Depth Map Estimation Semi Global Matching
 */
class SemiGlobalMatchingRc
{
public:
    SemiGlobalMatchingRc(const SgmParams& sgmParams, const mvsUtils::MultiViewParams& mp, PlaneSweepingCuda& cps, int rc);
    ~SemiGlobalMatchingRc();

    bool sgmRc();

    const StaticVector<int>& getTCams() const { return _tCams; }
    const StaticVector<float>& getDepths() const { return _depths; }
    const DepthSimMap& getDepthSimMap() const { return _depthSimMap; }

private:

    std::string getIdDepthMapFileName(IndexT viewId, int scale, int step) const;
    std::string getDepthMapFileName(IndexT viewId, int scale, int step) const;
    std::string getSimMapFileName(IndexT viewId, int scale, int step) const;
    std::string getTCamsFileName(IndexT viewId) const;
    std::string getDepthsFileName(IndexT viewId) const;

    float getMinTcStepAtDepth(float depth, float minDepth, float maxDepth, StaticVector<StaticVector<float>*>* alldepths);
    bool selectBestDepthsRange(int nDepthsThr, StaticVector<float>* rcSeedsDistsAsc);
    bool selectBestDepthsRange(int nDepthsThr, StaticVector<StaticVector<float>*>* alldepths);

    /**
     * @brief Compute depths of the principal ray of reference camera rc visible by a pixel in a target camera tc
     *        providing meaningful 3d information.
     */
    StaticVector<StaticVector<float>*>* computeAllDepthsAndResetTCams(float midDepth);

    /**
     * @brief Fill depthsTcamsLimits member variable with index range of depths to sweep
     */
    void computeDepthsTcamsLimits(StaticVector<StaticVector<float>*>* alldepths);
    void checkStartingAndStopppingDepth() const;
    void computeDepths(float minDepth, float maxDepth, float scaleFactor, StaticVector<StaticVector<float>*>* alldepths);
    void computeDepthsAndResetTCams();


    const SgmParams& _sgmParams;
    const mvsUtils::MultiViewParams& _mp;
    PlaneSweepingCuda& _cps;
    const int _rc;
    const int _width;
    const int _height;

    StaticVector<int> _tCams;
    StaticVector<float> _depths;
    StaticVector<Pixel> _depthsTcamsLimits;
    DepthSimMap _depthSimMap;

};

} // namespace depthMap
} // namespace aliceVision
