newaction {
    trigger     = "generate-docs",
    description = "Generate Documentation",
    execute = function ()
        print("Launching Doxygen...")
        os.execute("doxygen Doxyfile")
    end
}
