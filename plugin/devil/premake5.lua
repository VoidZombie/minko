PROJECT_NAME = path.getname(os.getcwd())

minko.project.library("minko-plugin-" .. PROJECT_NAME)
	kind "StaticLib"
	language "C++"
	files { "**.hpp", "**.h", "**.cpp", "**.c" }
	includedirs {
		"src",
		"lib/devil/src/include",
		"lib/devil/src/src-IL/include",
		"lib/devil/src/src-ILU/include",
		"lib/devil/src/src-ILUT/include",
		"lib/devil/lib/include",
		"lib/devil/lib/libjpeg",
		"lib/devil/lib/libtiff",
		"lib/devil/lib/libpng",
		"lib/devil/lib/zlib",
		"lib/devil/lib/libjasper/include",
		"lib/devil/lib/lcms/include"
	}
	defines
	{
	}

	configuration { "debug"}
		defines { "DEBUG" }
		flags { "Symbols" }
		targetdir "bin/debug"

	configuration { "release" }
		defines { "NDEBUG" }
		targetdir "bin/release"

	configuration { "linux" }
		defines {}