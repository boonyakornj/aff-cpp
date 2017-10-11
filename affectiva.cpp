// Need to run on g++ version not greater than 4.8

#include "VideoDetector.h"

using namespace affdex;
using namespace std;

int main(int argc, char ** argsv)
{
  // Constant variables
  const double processFrameRate = 30;
  const unsigned int maxNumFaces = 1;
  const FaceDetectorMode faceConfig = FaceDetectorMode::LARGE_FACES;
  string videoPath = "59c13748145ae01ec3675d3b.mp4";

  // Create a video detector to analyse the video
  VideoDetector detector(processFrameRate, maxNumFaces, faceConfig);  // using default value because of `undefined reference error`

  // Configure the detector
  std::string classifierPath = "/home/boonyakorn/affdex-sdk/data";
  detector.setClassifierPath(classifierPath);

  // Choose classifiers
  detector.setDetectSmile(true);
  detector.setDetectJoy(true);
  detector.setDetectAllExpressions(true);
  detector.setDetectAllEmotions(true);
  detector.setDetectAllEmojis(true);
  detector.setDetectAllAppearances(true);

  // Initialize detector
  detector.start();

  // Process video
  detector.process(videoPath);

  // Stop and reset detector
  detector.stop();

  return 0;
}