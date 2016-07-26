//
// Created by fausto on 26/07/16.
//

using namespace minko;
using namespace minko::component;
using namespace minko::geometry;
using namespace minko::render;
using namespace minko::scene;
using namespace minko::video;

VideoPlayerPreview::VideoPlayerPreview() :
        AbstractComponent()
{
}

void
VideoPlayerPreview::initialize()
{
    _frameBeginSlot = _sceneManager->frameBegin()->connect(std::bind(
            &VideoPlayerPreview::frameBeginHandler,
            std::static_pointer_cast<VideoPlayerPreview>(shared_from_this()),
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
    ));

    _frameEndSlot = _sceneManager->frameEnd()->connect(std::bind(
            &VideoPlayerPreview::frameEndHandler,
            std::static_pointer_cast<VideoPlayerPreview>(shared_from_this()),
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
    ));

    _previewSurface = Surface::create(
            QuadGeometry::create(_context, 1, 1, 1, 1),
            material::BasicMaterial::create(),
            _sceneManager->assets()->effect("effect/Background.effect")
    );
}

void
VideoPlayerPreview::targetAdded(scene::Node::Ptr target)
{
    _videoPlayerFrameReceivedSlot = _videoPlayer->frameReceived()->connect(
            std::bind(
                    &VideoPlayerPreview::frameReceivedHandler,
                    std::static_pointer_cast<VideoPlayerPreview>(shared_from_this()),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4,
                    std::placeholders::_5
            ));

    if (_videoPreviewTarget != nullptr)
        _previewSurface->material()->data()->set("diffuseMap", _videoPreviewTarget->sampler());

    target->addComponent(_previewSurface);
}

void
VideoPlayerPreview::targetRemoved(scene::Node::Ptr target)
{
    target->removeComponent(_previewSurface);

    _previewSurface = nullptr;

    _videoPlayerFrameReceivedSlot = nullptr;
}

void
VideoPlayerPreview::frameBeginHandler(SceneManager::Ptr sceneManager, float time, float deltaTime)
{
}

void
VideoPlayerPreview::frameEndHandler(SceneManager::Ptr sceneManager, float time, float deltaTime)
{
}

void
VideoPlayerPreview::frameReceivedHandler(AbstractVideoPlayer::Ptr           videoPlayer,
                                         const std::vector<unsigned char>&  data,
                                         int                                width,
                                         int                                height,
                                         ImageFormatType                    format)
{
    if (_videoPreviewTarget == nullptr ||
        width != _videoPreviewTarget->width() ||
        height != _videoPreviewTarget->height())
    {
        initializeVideoPreviewTarget(data, width, height, format);
    }
    else if (_updatePreviewWhenFrameReceived)
    {
        updateVideoPreviewTarget(data, width, height, format);
    }
}

void
VideoPlayerPreview::forceBackgroundUpdate()
{
    if (_previewSurface != nullptr && _videoPreviewTarget != nullptr)
        _previewSurface->material()->data()->set("diffuseMap", _videoPreviewTarget->sampler());

}

void
VideoPlayerPreview::initializeVideoPreviewTarget(const std::vector<unsigned char>&  data,
                                                 int width,
                                                 int height,
                                                 ImageFormatType format)
{
    if (_videoPreviewTarget != nullptr)
        _videoPreviewTarget->dispose();

    switch (format)
    {
        case ImageFormatType::RGB:
            _videoPreviewTarget = RectangleTexture::create(_context, width, height, render::TextureFormat::RGB);
            break;
        case ImageFormatType::RGBA:
            _videoPreviewTarget = RectangleTexture::create(_context, width, height, render::TextureFormat::RGBA);
            break;
        default:
            break;
    }

    updateVideoPreviewTarget(data, width, height, format);

    if (_previewSurface != nullptr)
    {
        _previewSurface->material()->data()->set("diffuseMap", _videoPreviewTarget->sampler());
        _previewSurface->material()->data()->set("cameraRatio", float(width) / float(height));
    }
}

void
VideoPlayerPreview::updateVideoPreviewTarget(const std::vector<unsigned char>&  data,
                                             int                                width,
                                             int                                height,
                                             ImageFormatType                    format)
{
    // TODO
    // setup specific (hardware) conversion for formats such as YUV

    if (_videoPreviewTarget != nullptr && width > 0 && height > 0)
    {
        std::static_pointer_cast<RectangleTexture>(_videoPreviewTarget)->data(const_cast<unsigned char*>(data.data()), width, height);
        _videoPreviewTarget->upload();
    }
}