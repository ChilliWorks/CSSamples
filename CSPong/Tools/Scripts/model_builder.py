#!/usr/bin/python
#
#  model_builder.py
#  CSPong
#  Created by Scott Downie on 30/06/2014.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2014 Tag Games Limited
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
#

import sys
import os
import subprocess
import shutil
import file_system_utils

relative_tool_path = "../../ChilliSource/Tools/ColladaToCSModel.jar"
#------------------------------------------------------------------------------
# Walks the input directory and converts all DAEs into csmodels.
#
# @author S Downie
#
# @param Input path
# @param Output path
#------------------------------------------------------------------------------
def build(input_path, output_path):
	
	print("-----------------------------------------")
	print("           Building Models")
	print("-----------------------------------------")

	if(input_path.endswith("/") == False):
		input_path = input_path + "/"

	if(output_path.endswith("/") == False):
		output_path = output_path + "/"

	file_system_utils.delete_directory(output_path)

	for directory, sub_dirs, filenames in os.walk(input_path):
		input_dir = directory
		output_dir = os.path.join(output_path, input_dir[len(input_path):len(input_dir)]);

		for filename in filenames:
			if file_system_utils.has_extension(filename, ".dae") == True:
				if os.path.exists(output_dir) == False:
					os.makedirs(output_dir);

				output_file_path = os.path.splitext(os.path.join(output_dir, filename))[0] + ".csmodel"
				build_model(os.path.join(directory, filename), output_file_path)

	print (" ")

#------------------------------------------------------------------------------
# Converts a single DAE to a csmodel
#
# @author S Downie
#
# @param Input path
# @param Output path
#------------------------------------------------------------------------------
def build_model(input_filepath, output_filepath):
	print(output_filepath)

	tool_path = file_system_utils.get_path_from_here(relative_tool_path)
	subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_path, "--input", input_filepath, "--output", output_filepath, "--swapyandz"]);

#------------------------------------------------------------------------------
# The entry point into the script.
#
# @author S Downie
#
# @param The list of arguments.
#------------------------------------------------------------------------------
def main(args):
	if not len(args) is 3:
		print("ERROR: Incorrect parameters supplied.")
		return

	input_path = args[1]
	output_path = args[2]
	build_models(input_path, output_path)

if __name__ == "__main__":
	main(sys.argv)

