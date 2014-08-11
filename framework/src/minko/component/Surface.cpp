/*
Copyright (c) 2013 Aerys

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

#include "minko/component/Surface.hpp"

#include "minko/scene/Node.hpp"
#include "minko/geometry/Geometry.hpp"
#include "minko/material/Material.hpp"
#include "minko/render/Effect.hpp"
#include "minko/render/DrawCall.hpp"
#include "minko/render/Pass.hpp"
#include "minko/render/Program.hpp"
#include "minko/data/Container.hpp"
#include "minko/data/Provider.hpp"
#include "minko/component/Renderer.hpp"

using namespace minko;
using namespace minko::scene;
using namespace minko::data;
using namespace minko::component;
using namespace minko::geometry;
using namespace minko::render;
using namespace minko::material;

Surface::Surface(std::string		name,
				 Geometry::Ptr 		geometry,
				 Material::Ptr 		material,
				 Effect::Ptr		effect,
				 const std::string&	technique) :
	AbstractComponent(),
	_name(name),
	_geometry(geometry),
	_material(material),
	_effect(effect),
	_technique(technique),
	_visible(true),
	_rendererToVisibility(),
	_rendererToComputedVisibility(),
	_techniqueChanged(TechniqueChangedSignal::create()),
	_visibilityChanged(VisibilityChangedSignal::create()),
	_computedVisibilityChanged(VisibilityChangedSignal::create()),
	_targetAddedSlot(nullptr),
	_targetRemovedSlot(nullptr),
	_addedSlot(nullptr),
	_removedSlot(nullptr)
{
	if (_effect == nullptr)
		throw std::invalid_argument("effect");
	if (!_effect->hasTechnique(_technique))
		throw std::logic_error("Effect does not provide a '" + _technique + "' technique.");
}

void
Surface::targetAdded(scene::Node::Ptr target)

{
	auto targetData	= target->data();

	_addedSlot = target->added()->connect(std::bind(
		&Surface::addedHandler,
		std::static_pointer_cast<Surface>(shared_from_this()),
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3
	));

	_removedSlot = target->removed()->connect(std::bind(
		&Surface::removedHandler,
		std::static_pointer_cast<Surface>(shared_from_this()),
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3
	));

    addProviderToCollection(_material->data(), "materials");
    addProviderToCollection(_geometry->data(), "geometries");
    addProviderToCollection(_effect->data(), "effects");
}

void
Surface::targetRemoved(scene::Node::Ptr target)
{
	auto data = target->data();

	_removedSlot = nullptr;
	
    removeProviderFromCollection(_material->data(), "materials");
    removeProviderFromCollection(_geometry->data(), "geometries");
    removeProviderFromCollection(_effect->data(), "effects");
}

void
Surface::addedHandler(Node::Ptr, Node::Ptr target, Node::Ptr)
{
}

void
Surface::removedHandler(Node::Ptr, Node::Ptr target, Node::Ptr)
{
}

void
Surface::geometry(geometry::Geometry::Ptr geometry)
{
    removeProviderFromCollection(_geometry->data(), "geometries");

    _geometry = geometry;
    addProviderToCollection(_geometry->data(), "geometries");
}

void
Surface::material(material::Material::Ptr material)
{
    removeProviderFromCollection(_material->data(), "materials");

    _material = material;
    addProviderToCollection(_material->data(), "materials");
}

void
Surface::effect(render::Effect::Ptr		effect, 
				const std::string&		technique)
{
	setEffectAndTechnique(effect, technique, true);
}

void
Surface::visible(component::Renderer::Ptr	renderer, 
				 bool						value)
{
	if (visible(renderer) != value)
	{
		_rendererToVisibility[renderer] = value;
		_visibilityChanged->execute(
			std::static_pointer_cast<Surface>(shared_from_this()), 
			renderer, 
			value
		);
	}
}

void
Surface::computedVisibility(component::Renderer::Ptr	renderer, 
							bool						value)
{
	if (computedVisibility(renderer) != value)
	{
		_rendererToComputedVisibility[renderer] = value;
		_computedVisibilityChanged->execute(
			std::static_pointer_cast<Surface>(shared_from_this()), 
			renderer, 
			value
		);
	}
}

void
Surface::setEffectAndTechnique(Effect::Ptr			effect,
							   const std::string&	technique,
							   bool					updateDrawcalls)
{
	if (_effect == effect && _technique == technique)
		return;
	
	if (effect == nullptr)
		throw std::invalid_argument("effect");
	if (!effect->hasTechnique(technique))
		throw std::logic_error("Effect does not provide a '" + technique + "' technique.");

	target()->data()->removeProvider(_effect->data());
	target()->data()->addProvider(effect->data());

	_effect		= effect;
	_technique	= technique;

	_techniqueChanged->execute(
		std::static_pointer_cast<Surface>(shared_from_this()), 
		_technique, 
		updateDrawcalls
	);
}