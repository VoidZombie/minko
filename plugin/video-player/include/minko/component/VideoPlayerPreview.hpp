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
#include "minko/VideoPlayerCommon.hpp"
#include "minko/component/AbstractComponent.hpp"

namespace minko
{
	namespace component
	{
		class VideoPlayerPreview :
			public AbstractComponent
		{
		public:
			typedef std::shared_ptr<VideoPlayerPreview> Ptr;

		private:
            typedef std::shared_ptr<SceneManager> SceneManagerPtr;
			typedef std::shared_ptr<video::AbstractVideoPlayer> AbstractVideoPlayerPtr;

			typedef std::shared_ptr<AbstractComponent> AbstractComponentPtr;
            typedef std::shared_ptr<Surface> SurfacePtr;
            typedef std::shared_ptr<scene::Node> NodePtr;

            typedef std::shared_ptr<render::AbstractContext> AbstractContextPtr;
            typedef std::shared_ptr<render::AbstractTexture> AbstractTexturePtr;

		private:
            SceneManagerPtr _sceneManager;
            AbstractContextPtr _context;
			AbstractVideoPlayerPtr _videoPlayer;

            std::shared_ptr<render::AbstractTexture> _videoPreviewTarget;
            SurfacePtr _previewSurface;

			Any _targetAddedSlot;
            Any _targetRemovedSlot;

            Any _frameBeginSlot;
            Any _frameEndSlot;

            Any _videoPlayerFrameReceivedSlot;
            
            bool _updatePreviewWhenFrameReceived;

		public:
			inline
			static
			Ptr
			create(SceneManagerPtr sceneManager, AbstractContextPtr context, AbstractVideoPlayerPtr videoPlayer, bool updatePreviewWhenFrameReceived = true)
			{
				auto instance = Ptr(new VideoPlayerPreview());

                instance->_sceneManager = sceneManager;
                instance->_context = context;
				instance->_videoPlayer = videoPlayer;
                instance->_updatePreviewWhenFrameReceived = updatePreviewWhenFrameReceived;
                
                instance->initialize();

				return instance;
			}

            inline
            SurfacePtr 
            previewSurface() 
            {
                return _previewSurface;
            }

            inline
            AbstractTexturePtr
            videoPreviewTarget()
            {
                return _videoPreviewTarget;
            }
            void
            forceBackgroundUpdate();

		private:
			VideoPlayerPreview();

			void
			initialize();

			void
			targetAdded(NodePtr target);

			void
			targetRemoved(NodePtr target);

            void
            frameBeginHandler(SceneManagerPtr sceneManager, float time, float deltaTime);

            void
            frameEndHandler(SceneManagerPtr sceneManager, float time, float deltaTime);

            void
            frameReceivedHandler(
                AbstractVideoPlayerPtr videoPlayer,
                const std::vector<unsigned char>& data,
                int width,
                int height,
                video::ImageFormatType format
            );

            void
            initializeVideoPreviewTarget(const std::vector<unsigned char>& data, int width, int height, video::ImageFormatType format);

            void
            updateVideoPreviewTarget(const std::vector<unsigned char>& data, int width, int height, video::ImageFormatType format);
		};
	}
}
