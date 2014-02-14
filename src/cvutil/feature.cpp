#include "cvutil.h"

CVFeatureMatcher::CVFeatureMatcher(
        int _maxPoints) :
    keypoints(vector<cv::KeyPoint>(_maxPoints)) {
    descriptors = cv::Mat();

    // Parameters for ORB features
    int nfeatures=_maxPoints;
    float scaleFactor=1.2f;
    int nlevels=8;
    int edgeThreshold=31;
    int firstLevel=0;
    int WTA_K=2;
    int scoreType=cv::ORB::HARRIS_SCORE;
    int patchSize=31;

    normType = cv::NORM_HAMMING;

    if (WTA_K == 3 || WTA_K == 4) {
        normType = cv::NORM_HAMMING2;
    }

    orb = unique_ptr<cv::ORB>(new cv::ORB(nfeatures, scaleFactor, nlevels,
                edgeThreshold, firstLevel, WTA_K, scoreType, patchSize));
}

int CVFeatureMatcher::detectFeatures(
        const CImg<uint8_t>& grayImg) {
    keypoints.clear();

    cv::Mat img = cv::Mat(grayImg.height(), grayImg.width(), CV_8UC1,
            (void*) grayImg.data());

    (*orb)(img, cv::Mat(), keypoints, descriptors);

    return keypoints.size();
}

int CVFeatureMatcher::match(
        const CVFeatureMatcher& other,
        vector<tuple<int, int>>& matchList,
        int maxMatches) {
    cvMatchList.clear();

    cv::BFMatcher matcher(normType, true);

    matcher.match(descriptors, other.descriptors, cvMatchList);

    std::sort(cvMatchList.begin(), cvMatchList.end());

    for (int i = 0; i < min((int) cvMatchList.size(), maxMatches); i++) {
        const cv::DMatch& match = cvMatchList[i];

        matchList.push_back(make_tuple(
                    match.queryIdx, match.trainIdx));
    }

    return cvMatchList.size();
}

int CVFeatureMatcher::match(
        const CVFeatureMatcher& other,
        vector<tuple<float, float, float, float>>& matchedPoints) {
    cvMatchList.clear();

    cv::BFMatcher matcher(normType, true);

    matcher.match(descriptors, other.descriptors, cvMatchList);

    std::sort(cvMatchList.begin(), cvMatchList.end());

    for (const cv::DMatch& match : cvMatchList) {
        int aIdx = match.queryIdx;
        int bIdx = match.trainIdx;

        matchedPoints.push_back(make_tuple(
                    keypoints[aIdx].pt.x,
                    keypoints[aIdx].pt.y,
                    other.keypoints[bIdx].pt.x,
                    other.keypoints[bIdx].pt.y));
    }

    return cvMatchList.size();
}
