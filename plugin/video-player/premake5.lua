PROJECT_NAME = path.getname(os.getcwd())

minko.project.library("minko-plugin-" .. PROJECT_NAME)

	removeplatforms { "linux64", "linux32" }

	minko.plugin.enable("sdl")

	files {
		"lib/**.hpp",
		"lib/**.h",
		"lib/**.cpp",
		"lib/**.c",
		"src/**.cpp",
		"include/**.hpp",
		"include/**.h",
		"src/**.mm"
	}

	includedirs {
		"include",
		"src"
	}

	configuration { "not android" }
		excludes {
			minko.plugin.path("video-player") .. "/include/minko/video/AndroidVideoPlayer.hpp",
			minko.plugin.path("video-player") .. "/src/minko/video/AndroidVideoPlayer.cpp"
		}

	--[[configuration { "not html5" }
		excludes {
			minko.plugin.path("video-player") .. "/include/minko/video/HTML5VideoCamera.hpp",
			minko.plugin.path("video-player") .. "/src/minko/video/HTML5VideoCamera.cpp"
		}

	configuration { "not ios" }
		excludes {
			minko.plugin.path("video-player") .. "/include/minko/video/ios/**",
			minko.plugin.path("video-player") .. "/src/minko/video/ios/**"
		}

	configuration { "ios" }
		buildoptions { "-x objective-c++" }]]
