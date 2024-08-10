project "Tests"
  architecture "x86_64"
  kind "ConsoleApp"
  language "C++"
  files {"**.cpp"}
  targetdir ("%{wks.location}/tests/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/tests/objs/" .. outputdir .. "/%{prj.name}")

  includedirs {
    "%{wks.location}/server/src"
  }

  links {
   "Server",
    "googletest"
  }

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
