# build_common.tcl - Updated for Vitis 2022.2 with an Empty Application template,
# custom source import, and correct linker settings.
# Usage: xsct build_common.tcl <project_name> <xsa_path> <src_dir>

proc build_project {project_name xsa_path src_dir} {
    # Set workspace to current directory.
    set ws [pwd]
    setws $ws

    # Create the application project using the "Empty Application" template.
    app create -name $project_name -hw $xsa_path -os standalone -proc ps7_cortexa9_0 -template "Empty Application"

    # Import source files.
    importsources -name $project_name -path "$src_dir/src"
    importsources -name $project_name -path "$src_dir/examples/$project_name"

    # Set the linker script.
    app config -name $project_name -set linker-script "$src_dir/examples/$project_name/lscript.ld"

    # Add the math library.
    app config -name $project_name -add libraries m

    # Build the application project.
    app build -name $project_name
}

if {[llength $argv] == 3} {
    build_project {*}$argv
} else {
    puts "Error: Invalid arguments. Usage: xsct build_common.tcl <project> <xsa> <src>"
    exit 1
}
