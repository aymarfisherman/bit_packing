common_deps_dir = "D:/workspace/common_deps/"
reps_dir = "D:/workspace/reps/"

workspace "Workspace"
	configurations {"Debug", "Release"}
	location "./build"
    
    includedirs {    
        common_deps_dir .. "boost_1_64_0",
        reps_dir .. "**/include",
    }
    
    libdirs {
        common_deps_dir .. "boost_1_64_0/stage/lib",
        reps_dir .. "**/lib",
	}
    
    filter "configurations:Debug"
		targetsuffix "_d"
		defines {"DEBUG", "_DEBUG", "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS", "MSVC_IMPLICIT_LINK"}
		flags {"Symbols", "Unicode"}
		
	filter "configurations:Release"        
		defines {"NDEBUG", "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS", "MSVC_IMPLICIT_LINK"}
		flags {"Unicode"}
		optimize "On"

project "BitPacking"
	kind "StaticLib"
	targetdir "./lib"
	
	includedirs {
		"./include",
	}
	
	files {
		"./include/**.h",
		"./src/**.cpp",
	}
	
project "BitPackingTests"
	kind "ConsoleApp"
	targetdir "./bin"
	
    includedirs {    
        common_deps_dir .. "googletest/googletest/include",
		common_deps_dir .. "googletest/googlemock/include",
        reps_dir .. "gtest_wrapper/include",
        "./include",
	}
	
	libdirs {
		common_deps_dir .. "googletest/lib",
	}
	
	files {
		"./tests/main.cpp",
	}
	
	links {
		"BitPacking",
	}