workspace "physim2D"
    configurations { "Debug", "Release" }
    architecture "x64"
    cppdialect "C++17"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "physim2D"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    files {
        "game/**.h",
        "game/**.cpp",
        "engine/**.h",
        "engine/**.hpp",
        "engine/**.c",
        "engine/**.cpp",
        "third_party/glad/src/glad.c",
        "third_party/glm/glm/**.hpp",
        "third_party/glm/glm/**.inl"
    }

    removefiles {
        "third_party/glm/test/**",
        "third_party/glm/glm/out/**",
        "third_party/glm/doc/**"
    }

    includedirs {
        "third_party",
        "third_party/SDL",
        "third_party/SDL_image",
        "third_party/SDL_ttf",
        "third_party/glad/include",
        "engine",
        "third_party/glm"
    }

    libdirs {
        "third_party/SDL",
        "third_party/SDL_image",
        "third_party/SDL_ttf"
    }

    links {
        "SDL3",
        "SDL3_image",
        "SDL3_ttf"
    }

    postbuildcommands {
        "{COPY} %{wks.location}/third_party/SDL/SDL3.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/third_party/SDL_image/SDL3_image.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/third_party/SDL_ttf/SDL3_ttf.dll %{cfg.targetdir}"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }
        

        filter "toolset:msc*"
            flags { "MultiProcessorCompile" }
            vectorextensions "AVX"

    filter "system:not windows"
        buildoptions { "-mavx" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"
        runtime "Debug"

        filter "toolset:not msc*"
            buildoptions { "-g", "-O0" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "On"
        optimize "On"
        runtime "Release"