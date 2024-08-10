project "Server"
  files {"src/**.cpp", "src/**.h"}
  language "C++"
  kind "ConsoleApp"
  cppdialect "C++17"
  staticruntime "off"

  includedirs{
    "src",
    "%{wks.location}/common/src"
  }

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/objs/" .. outputdir .. "/%{prj.name}")

  filter "configurations:Debug"
    defines { "WL_DEBUG" }
    runtime "Debug"
    symbols "On"

  filter "configurations:Release"
    defines { "WL_RELEASE" }
    runtime "Release"
    optimize "On"
    symbols "On"

  filter "configurations:Dist"
    kind "WindowedApp"
    defines { "WL_DIST" }
    runtime "Release"
    optimize "On"
    symbols "Off"
