project_name = "scheme"
solution (project_name)
platforms { "x32", "x64" }
language "C"
configurations { "debug", "release" }                

---------------------------------------------------------
-- Project 
project (project_name)
    kind "ConsoleApp" 
    basedir "./"
    targetdir "./"      
    location(_ACTION)
    objdir(_ACTION .. "/obj")
    debugdir("./")

    includedirs{ "include/" } 
    files { "src/*.h", "src/*.c"}
    libdirs { "lib/" }

-- Debug configurations
    configuration "debug"
         defines {"DEBUG"}
         flags {"Symbols"}
         targetname(project_name .. "_d")
         links{ "dl", "m" } 
-- Release configurations
    configuration "release"
        defines {"NDEBUG"}   
		flags {"Optimize"}		
        targetname(project_name)
        links{ } 
        

-- Linux platform
    configuration{ "linux" }
        defines { "STANDALONE" }

-- Windows platform
-- configuration{  "vs**" }
--      defines { "WIN32", "_CRT_SECURE_NO_WARNINGS", "STANDALONE" }

---------------------------------------------------------
--Lib
project ("lib" .. project_name)
    kind "StaticLib"
    basedir "./"
    targetdir "./"      
    location(_ACTION)
    objdir(_ACTION .. "/obj")
    includedirs{ "include" } 
    files { "src/*.h", "src/*.c"}
    targetdir "lib/"      

-- Debug configurations
    configuration "debug"
         defines {"DEBUG"}
         flags {"Symbols"}
         targetname(project_name .. "_d")

-- Release configurations
    configuration "release"
        defines {"NDEBUG"}        
        targetname(project_name)
        flags {"Optimize"}

-- Linux platform
    configuration { "linux"  }
        defines {  }
        links{ "dl", "m" }
-- Windows platform
-- configuration { "vs20**" }
--     defines { "WIN32", "_CRT_SECURE_NO_WARNINGS"}

project_name = "example_embedded"
---------------------------------------------------------
-- Project 
project (project_name)
    kind "ConsoleApp" 
    basedir ("./")
    targetdir("./")
    location(_ACTION)
    objdir(_ACTION .. "/obj")
    debugdir("./")
    libdirs { "lib/" }
    includedirs { "include/" }
    files { "examples/embedded.c"}

-- Debug configurations
    configuration "debug"
         defines {"DEBUG"}
         flags {"Symbols"}
         targetname(project_name .. "_d")

-- Release configurations
    configuration "release"
        defines {"NDEBUG"}   
		flags {"Optimize"}		
        targetname(project_name)
        links{ } 

-- Linux platform
    configuration{ "linux" }
         links{  } 
         defines { }

    configuration{ "linux", "debug" }
         links{ "scheme_d", "dl", "m" } 

    configuration{ "linux", "release" }
         links{ "scheme" } 

-- Windows platform
-- configuration{  "vs**" }
--      defines { "WIN32", "_CRT_SECURE_NO_WARNINGS", "STANDALONE" }

