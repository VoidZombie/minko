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

#pragma once

#include "minko/Common.hpp"

#include "minko/Signal.hpp"

namespace minko
{
	namespace render
	{
		class DrawCall :
            public std::enable_shared_from_this<DrawCall>
		{
		public:
			typedef std::shared_ptr<DrawCall> Ptr;

		private:
			typedef std::shared_ptr<AbstractContext>	AbsCtxPtr;
            typedef std::shared_ptr<data::Container>    ContainerPtr;

		private:
			std::shared_ptr<data::Container>					        _data;
			const std::unordered_map<std::string, std::string>&	        _inputNameToBindingName;

			std::vector<std::function<void(AbsCtxPtr)>>			        _func;

            std::list<Signal<ContainerPtr, const std::string&>::Slot>   _propertyChangedSlots;

		public:
			static inline
			Ptr
			create(ContainerPtr						                    data,
				   const std::unordered_map<std::string, std::string>&	inputNameToBindingName)
			{
                auto dc = std::shared_ptr<DrawCall>(new DrawCall(data, inputNameToBindingName));

                dc->bind();

				return dc;
			}

			void
			render(std::shared_ptr<AbstractContext> context);

			void
			initialize(ContainerPtr				                    data,
					   const std::map<std::string, std::string>&	inputNameToBindingName);

		private:
			DrawCall(ContainerPtr                   						data,
					 const std::unordered_map<std::string, std::string>&	inputNameToBindingName);

			void
			bind();

            template <typename T>
            T
            getDataProperty(const std::string& propertyName)
            {
                watchProperty(propertyName);

                return _data->get<T>(propertyName);
            }

            bool
            dataHasProperty(const std::string& propertyName);

            void
            watchProperty(const std::string& propertyName);

            void
            boundPropertyChangedHandler(ContainerPtr        data,
                                        const std::string&  propertyName);
		};		
	}
}
