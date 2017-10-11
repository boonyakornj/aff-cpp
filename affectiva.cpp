// Need to run on g++ version not greater than 4.8

#include <fstream>
#include <iostream>
#include "VideoDetector.h"
#include <boost/filesystem.hpp>

#include "AFaceListener.hpp"
#include "PlottingImageListener.hpp"
#include "StatusListener.hpp"

using namespace affdex;
using namespace std;

int main(int argc, char ** argsv)
{
  // Constant variables
  const double processFrameRate = 30;
  const unsigned int maxNumFaces = 1;
  const FaceDetectorMode faceConfig = FaceDetectorMode::LARGE_FACES;
  string videoPath = "59c13748145ae01ec3675d3b.mp4";
  bool drawDisplay = false;

  // Create a video detector to analyse the video
  shared_ptr<VideoDetector> detector;
  detector = std::make_shared<VideoDetector>(processFrameRate, maxNumFaces, faceConfig);

  // Configure the detector
  string classifierPath = "/home/boonyakorn/affdex-sdk/data";
  detector->setClassifierPath(classifierPath);

  // Initialize output file
  boost::filesystem::path csvPath(videoPath);
  boost::filesystem::path fileExt = csvPath.extension();
  csvPath.replace_extension(".csv");
  std::ofstream csvFileStream(csvPath.c_str());

  // Choose classifiers
  shared_ptr<PlottingImageListener> listenPtr(new PlottingImageListener(csvFileStream, drawDisplay));
  detector->setDetectAllExpressions(true);
  detector->setDetectAllEmotions(true);
  detector->setDetectAllEmojis(true);
  detector->setDetectAllAppearances(true);
  detector->setImageListener(listenPtr.get());

  // Initialize detector
  detector->start();

  // Process video
  bool loop = false;
  do 
  {
    shared_ptr<StatusListener> videoListenPtr = std::make_shared<StatusListener>();
    detector->setProcessStatusListener(videoListenPtr.get());

    do {
      if (listenPtr->getDataSize() > 0)
      {
        std::pair<Frame, std::map<FaceId, Face>> dataPoint = listenPtr->getData();
        Frame frame = dataPoint.first;
        std::map<FaceId, Face> faces = dataPoint.second;

        cerr << "timestamp: " << frame.getTimestamp()
        << " cfps: " << listenPtr->getCaptureFrameRate()
        << " pfps: " << listenPtr->getProcessingFrameRate()
        << " faces: "<< faces.size() << endl;

        listenPtr->outputToFile(faces, frame.getTimestamp());
      }
    } while(videoListenPtr->isRunning() || listenPtr->getDataSize() > 0);
  } while(loop);

  // Stop detector and finish process
  detector->stop();
  csvFileStream.close();

  cout << "Output written to file: " << csvPath << endl;

  return 0;
}