#!/usr/bin/python
#  The MIT License (MIT)
#
#  Copyright (c) 2017 Tag Games Limited
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#-----------------------------------------------------------------------------------
import sys
sys.path.append("../../ChilliSource/Tools/Scripts/")
import os
import rpi_build

# If cross compiling replace these variables with the appropriate toolchain
COMPILER = "g++"
ARCHIVER = "ar"
LINKER = "g++"
NUM_JOBS = 2 # Set to None to unrestrict

PROJECT_ROOT = os.path.normpath("../..")
APP_SRC_ROOT = os.path.normpath("{}/AppSource".format(PROJECT_ROOT))
BUILD_DIR = os.path.normpath("{}/Projects/RPi/Build".format(PROJECT_ROOT))
OUTPUT_DIR = os.path.normpath("{}/Projects/RPi/Output".format(PROJECT_ROOT))
ADDITIONAL_COMPILER_FLAGS_TARGET_MAP = { "debug":"-DCS_LOGLEVEL_VERBOSE",  "release":"-DCS_LOGLEVEL_WARNING"}
ADDITIONAL_INCLUDE_PATHS = "-I{0}/AppSource".format(PROJECT_ROOT)
ADDITIONAL_LIBRARY_PATHS = ""
ADDITIONAL_LIBRARIES = ""
ADDITIONAL_SRC_DIRS = [os.path.normpath('{}/'.format(APP_SRC_ROOT))]
APP_NAME = "CSRunner"

# The entry point into the script.
#
# @param args 
#	The list of arguments - Should have an argument "debug" or "release" optionally followed by "clean"
#
def main(args):

	rpi_build.run(args=args,
	num_jobs=NUM_JOBS,
	app_name=APP_NAME,
	compiler_path=COMPILER, linker_path=LINKER, archiver_path=ARCHIVER, 
	additional_libs=ADDITIONAL_LIBRARIES, additional_lib_paths=ADDITIONAL_LIBRARY_PATHS, additional_include_paths=ADDITIONAL_INCLUDE_PATHS,
	additional_compiler_flags_map=ADDITIONAL_COMPILER_FLAGS_TARGET_MAP,
	app_source_dirs=ADDITIONAL_SRC_DIRS,
	project_root=PROJECT_ROOT, build_root=BUILD_DIR, output_root=OUTPUT_DIR)

if __name__ == "__main__":
	main(sys.argv[1:])
