/*
Copyright (c) 2014 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "minko/Any.hpp"
#include "minko/Common.hpp"
#include "minko/video/AbstractVideoPlayer.hpp"

namespace minko
{
    namespace video
    {
        class AndroidVideoPlayer :
            public AbstractVideoPlayer
        {
        public:
            typedef std::shared_ptr<AndroidVideoPlayer> Ptr;

        private:
            typedef std::shared_ptr<component::SceneManager> SceneManagerPtr;

            struct JniImpl;

        private:
            FrameSignal::Ptr _frameReceived;

            std::shared_ptr<JniImpl> _jniImpl;

        public:
            ~AndroidVideoPlayer() = default;

            inline
            static
            Ptr
            create()
            {
                auto instance = Ptr(new AndroidVideoPlayer());

                instance->initialize();

                return instance;
            }

            AbstractVideoPlayer::Ptr
            desiredSize(unsigned int width, unsigned int height);

            inline
            FrameSignal::Ptr
            frameReceived()
            {
                return _frameReceived;
            }

            void
            start();

            void
            stop();

        private:
            AndroidVideoPlayer();

            void
            initialize();

            void
            requestFrame();
        };
    }
}
