# program_common.tcl - Program the FPGA bitstream, run PS initialization,
# and download the ARM ELF file onto the target.
#
# Usage: xsct program_common.tcl <elf_file> <bitstream_file> <ps7_init_tcl>
#
# This script connects to the target, programs the FPGA using the given bitstream,
# runs the PS initialization script (ps7_init.tcl), downloads the ELF, and starts execution.

if {[llength $argv] != 3} {
    puts "Usage: xsct program_common.tcl <elf_file> <bitstream_file> <ps7_init_tcl>"
    exit 1
}

# Retrieve arguments
set elf_file [lindex $argv 0]
set bit_file [lindex $argv 1]
set ps7_init_tcl [lindex $argv 2]

puts "Connecting to target..."
connect

# Select the PS target.
targets -set -nocase -filter {name =~ "ARM Cortex-A9 MPCore #0"}

# Program the FPGA bitstream.
puts "Programming FPGA bitstream: $bit_file"
fpga -f $bit_file

# Run the PS initialization script.
puts "Running PS initialization: $ps7_init_tcl"
source $ps7_init_tcl
ps7_init
ps7_post_config


# Download the ELF file to program the ARM processor.
puts "Downloading ELF file: $elf_file"
dow $elf_file

# Start execution.
puts "Starting execution..."
con

exit
