workspace "Saps"
  configurations { "Debug", "Release", "Dist"}
  architecture "x86_64"
  --- startproject
  flags {
    "MultiProcessorCompile"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Core"
  include "server"
  include "client"
group "Misc"
  include "common"
