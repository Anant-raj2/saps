-- Clean Function --
premake.modules.lua = {}
local m = premake.modules.lua

local p = premake
newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function ()
      print("clean the build...")
      os.rmdir("./bin")
      os.rmdir("./objs")

      print("clean the tests...")
      os.rmdir("./tests/bin")
      os.rmdir("./tests/objs")

      print("clean the makefiles...")
      os.remove("./server/Makefile")
      os.remove("./client/Makefile")
      os.remove("./common/Makefile")
      os.remove("./tests/Makefile")
      os.remove("./Makefile")
      print("done.")
   end
}
