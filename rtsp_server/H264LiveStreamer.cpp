// H264LiveStreamer.cpp : Defines the entry point for the console application.
//

/*===============================================================================

Project: H264LiveStreamer
Module: main.cxx

Copyright (c) 2014-2015, Rafael Palomar <rafaelpalomaravalos@gmail.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

=============================================================================*/

#include <iostream>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>
#include <opencv2/opencv.hpp>
#include "H264LiveServerMediaSession.h"

int main(int argc, char* argv[])
{
    /*cv::VideoCapture cap(0);
    cv::Mat rawImage;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    //cap.set(CV_CAP_PROP_AUTOFOCUS,0);
    while(1)
    {
    cap >> rawImage;
    cv::imshow("frame",rawImage);
    cv::waitKey(1);
    }*/

    OutPacketBuffer::maxSize = 6000000;
	TaskScheduler* taskSchedular = BasicTaskScheduler::createNew();
	BasicUsageEnvironment* usageEnvironment = BasicUsageEnvironment::createNew(*taskSchedular);
	RTSPServer* rtspServer = RTSPServer::createNew(*usageEnvironment, 8558
            , NULL);

	if (rtspServer == NULL)
	{
		*usageEnvironment << "Failed to create rtsp server ::" << usageEnvironment->getResultMsg() << "\n";
		exit(1);
	}
	std::string streamName = "usb1";
	ServerMediaSession* sms = ServerMediaSession::createNew(*usageEnvironment, streamName.c_str(), streamName.c_str(), "Live H264 Stream");
	H264LiveServerMediaSession *liveSubSession = H264LiveServerMediaSession::createNew(*usageEnvironment, true);
	sms->addSubsession(liveSubSession);

	rtspServer->addServerMediaSession(sms);

	char* url = rtspServer->rtspURL(sms);
	*usageEnvironment << "Play the stream using url " << url << "\n";
	delete[] url;
	taskSchedular->doEventLoop();

    return 0;

}
