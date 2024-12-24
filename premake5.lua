workspace "MemoryManagerWorkSpace"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"ReleaseWithDebInfo"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "MemoryManager"
	location "MemoryManager"
	kind "SharedLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files{
		 "%{prj.name}/include/**.h",
		 "%{prj.name}/src/**.cpp",
	}

	includedirs{
		"%{prj.name}/deps/imgui/include",
		"%{prj.name}/include"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		buildoptions { "/IMPLIB:bin/" .. outputdir .. "/MemoryManager/MemoryManager.lib" }

		defines{
			"MM_PLATFORM_WINDOWS",
			"MM_BUILD_DLL"
		}

	postbuildcommands {
		"if not exist \"$(SolutionDir)bin\\" .. outputdir .. "\\Sandbox\" mkdir \"$(SolutionDir)bin\\" .. outputdir .. "\\Sandbox\"",
		"xcopy /Q /Y /I \"$(SolutionDir)bin\\" .. outputdir .. "\\MemoryManager\\MemoryManager.dll\" \"$(SolutionDir)bin\\" .. outputdir .. "\\Sandbox\\\" > nul"
	}




	filter "configurations:Debug"
		defines "MM_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		optimize "On"

	filter "configurations:ReleaseWithDebInfo"
		symbols "On"
		optimize "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files{
		 "%{prj.name}/**.h",
		 "%{prj.name}/**.cpp",
	}

	includedirs{
		"%{prj.name}/deps/imgui/include",
		"MemoryManager/src"
	}

	links{
		"MemoryManager"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines{
			"MM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MM_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		optimize "On"

	filter "configurations:ReleaseWithDebInfo"
		symbols "On"
		optimize "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
