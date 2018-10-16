-- add modes: debug and release 
add_rules("mode.debug", "mode.release")

-- add target
target("sscc_linux")

    set_languages("cxx14")
    -- set kind
    set_kind("binary")

    -- add files
    add_files("src/*.cpp") 
    add_files("src/memory/*.cpp") 

    add_includedirs("src")
    add_includedirs("src/memory")
