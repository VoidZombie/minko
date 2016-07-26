//
// Created by fausto on 26/07/16.
//

#include "minko/component/SceneManager.hpp"
#include "minko/log/Logger.hpp"
#include "minko/video/AndroidVideoPlayer.hpp"
#include "minko/video/ImageFormat.hpp"

#include <jni.h>
#include "SDL.h"
#include <android/log.h>

using namespace minko;
using namespace minko::component;
using namespace minko::video;

struct AndroidVideoPlayer::JniImpl
{
    jclass _videoPlayerClass;
    jmethodID _videoPlayerStartMethodID;
    jmethodID _videoPlayerStopMethodID;
    jmethodID _videoPlayerDesiredSizeMethodID;
    jmethodID _videoPlayerRetrieveFrameMethodID;
};

static
JNIEnv*
retrieveJniEnv()
{
    return reinterpret_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
}

AndroidVideoPlayer::AndroidVideoPlayer() :
        _frameReceived(FrameSignal::create())
{
}

void
AndroidVideoPlayer::initialize()
{
    _jniImpl = std::shared_ptr<JniImpl>(new JniImpl());

    auto jniEnv = retrieveJniEnv();

    _jniImpl->_videoPlayerClass = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(jniEnv->FindClass("minko/plugin/video/VideoPlayer")));

    _jniImpl->_videoPlayerStartMethodID = jniEnv->GetStaticMethodID(_jniImpl->_videoCameraClass, "start", "()V");
    _jniImpl->_videoPlayerStopMethodID = jniEnv->GetStaticMethodID(_jniImpl->_videoCameraClass, "stop", "()V");
    _jniImpl->_videoPlayerDesiredSizeMethodID = jniEnv->GetStaticMethodID(_jniImpl->_videoCameraClass, "desiredSize", "(II)V");
    _jniImpl->_videoPlayerRetrieveFrameMethodID = jniEnv->GetStaticMethodID(_jniImpl->_videoCameraClass, "retrieveFrame", "([I)[B");
}

AbstractVideoPlayer::Ptr
AndroidVideoPlayer::desiredSize(unsigned int width, unsigned int height)
{
    retrieveJniEnv()->CallStaticVoidMethod(_jniImpl->_videoPlayerClass, _jniImpl->_videoPlayerDesiredSizeMethodID, width, height);
}

void
AndroidVideoPlayer::start()
{
    retrieveJniEnv()->CallStaticVoidMethod(_jniImpl->_videoPlayerClass, _jniImpl->_videoPlayerStartMethodID);
}

void
AndroidVideoPlayer::stop()
{
    retrieveJniEnv()->CallStaticVoidMethod(_jniImpl->_videoPlayerClass, _jniImpl->_videoPlayerStopMethodID);
}

void
AndroidVideoPlayer::requestFrame()
{

}