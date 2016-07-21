/*
 * fitting.h
 *
 *      Author:
 *         Nicolas Acero
 */
#ifndef FITTING_H_
#define FITTING_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <lane_detector/DetectorConfig.h>
#include <cv.h>
#include <lane_detector/LaneDetector.hh>
#include <lane_detector/LaneDetectorOpt.h>
#include <lane_detector/utils.h>
#include <lane_detector/InversePerspectiveMapping.hh>
#include <lane_detector/fittingApproach.h>
#include <lane_detector/lane_tracker/Ctracker.h>

class Fitting {
public:
        inline Fitting(){
        };
        inline void setConfig(lane_detector::DetectorConfig& config) {
                this->config = config;
                lane_detector::utils::translateConfiguration(config, this->lanesConf);
                tracker.setDt(config.tracking_dt);
                tracker.setAccelNoiseMag(config.tracking_acc_noise_magnitude);
                tracker.setDistThres(config.tracking_dist_threshold);
                tracker.setMaximumAllowedSkippedFrames(config.tracking_num_absent_frames);
                tracker.setMinimumSeenFrames(config.tracking_num_seen_frames);
                tracker.setMaxTraceLength(50);
                driving_orientation = config.driving_orientation == 0? lane_detector::on_the_right :
                                                                       lane_detector::on_the_left;
        };
        inline void setDrivingOrientation(lane_detector::Driving driving_orientation) {
          this->driving_orientation = driving_orientation;
        }
        void fitting(cv::Mat& original, cv::Mat& preprocessed, LaneDetector::IPMInfo& ipmInfo, std::vector<LaneDetector::Box>& boxes);
private:
        void findCurrentLane(const std::vector<cv::Point2f>& centroids, std::vector<cv::Point2f>& current_lane);
        float calcCost(std::vector<cv::Point2f>& combination);
        lane_detector::DetectorConfig config;
        LaneDetector::LaneDetectorConf lanesConf;
        LaneDetector::IPMInfo ipmInfo;
        CTracker tracker;
        lane_detector::Driving driving_orientation;
};

#endif /* FITTING_H_ */
