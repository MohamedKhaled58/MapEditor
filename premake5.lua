workspace "MapEditor"
architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include directories relative to root folder (solution directory)
	IncludeDir = {}
	IncludeDir["ImGui"] = "MapEditor/vendor/imgui"


	-- MapEditor project
	
	project "MapEditor"
		location "MapEditor"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files {
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/libigl-stb/stb_image.h", -- only if used
			"%{prj.name}/vendor/imgui/*.cpp",
		}
		
		includedirs {
			"%{prj.name}/src",
			"%{prj.name}/vendor/imgui",
			"%{prj.name}/vendor/imgui/backends"
		}

		links {
		    "d3d11",
		    "dxgi"
		}


		filter "system:windows"
			cppdialect "c++17"
			systemversion "latest"

		defines {
			"CH_PLATFORM_WINDOWS",
			"CH_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
  filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"