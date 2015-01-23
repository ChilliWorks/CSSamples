#!/usr/bin/python
#
#  texture_atlas_builder.py
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
import random
import file_system_utils

relative_tool_path = "../../ChilliSource/Tools/CSAtlasBuilder.jar"
temp_dir_prefix = "_temp-textureatlasbuilder-"

#------------------------------------------------------------------------------
# Walks the input directory and packs all pngs in each folder onto an atlas
# for that folder
#
# @author S Downie
#
# @param Input path
# @param Output path
#------------------------------------------------------------------------------
def build(input_path, output_path):

	print("-----------------------------------------")
	print("           Building atlases")
	print("-----------------------------------------")

	if(input_path.endswith("/") == False):
		input_path = input_path + "/"

	if(output_path.endswith("/") == False):
		output_path = output_path + "/"

	if os.path.exists(output_path) == True:
		shutil.rmtree(output_path)

	for directory, sub_dirs, file_names in os.walk(input_path):
		output_dir = os.path.join(output_path, directory[len(input_path):len(directory)]);
		if len(sub_dirs) == 0:
			contains_png = False
			for file_name in file_names:
				if file_system_utils.has_extension(file_name, ".png") == True:
					contains_png = True
					break

			if contains_png == True:
				if os.path.exists(output_dir) == False:
					os.makedirs(output_dir);
				build_altases_in_directory(directory, output_dir, file_names)

	print(" ")

#------------------------------------------------------------------------------
# @author Ian Copland
#
# @param A file name.
#
# @return The tags in the file name, or an empty string if there isn't one.
#------------------------------------------------------------------------------
def get_tags_from_file_name(file_name):
	if file_name.count('.') > 1:
		first = file_name.find(".")
		last = file_name.rfind(".")
		tags = file_name[first + 1 : last]
		return tags.lower()
	else:
		return ""

#------------------------------------------------------------------------------
# @author Ian Copland
#
# @param A file name.
# @param The tags string
#
# @return Whether or not the file name has the given tags.
#------------------------------------------------------------------------------
def file_name_has_tags(file_name, tags):
	if get_tags_from_file_name(file_name) == tags.lower():
		return True
	return False

#------------------------------------------------------------------------------
# @author Ian Copland
#
# @param A file name.
#
# @return The file name with any tags removed.
#------------------------------------------------------------------------------
def remove_tags_from_file_name(file_name):
	if file_name.count('.') > 1:
		first = file_name.find(".")
		last = file_name.rfind(".")
		tagless_file_name = file_name[0 : first] + file_name[last : len(file_name)]
		return tagless_file_name
	else:
		return file_name

#------------------------------------------------------------------------------
# @author Ian Copland
#
# @param A file path. 
# @param The tags string.
#
# @return The file path with tags removed.
#------------------------------------------------------------------------------
def add_tags_to_file_path(file_path, tags):
	if len(tags) > 0:
		period_index = file_path.rfind(".")
		tagged_file_path = file_path[0 : period_index] + "." + tags + file_path[period_index : len(file_path)]
		return tagged_file_path
	else:
		return file_path

#------------------------------------------------------------------------------
# @author Ian Copland
#
# @param The path to an atlas.
# @param The tags.
#
# @return The output atlas file path with tags.
#------------------------------------------------------------------------------
def generate_atlas_file_path(output_dir, tags):
	atlas_file_path = os.path.join(output_dir, os.path.basename(output_dir) + ".csatlas");
	return add_tags_to_file_path(atlas_file_path, tags)

#------------------------------------------------------------------------------
# Builds each of the different atlases in the given directory. Different atlases
# are build for assets with different "resource tags", i.e Image.low.png and
# Image.high.png would end up on different texture atlases.
#
# @author Ian Copland
#
# @param Input path
#
# @return Output name
#------------------------------------------------------------------------------
def build_altases_in_directory(input_dir, output_dir, file_names):
	
	remains = [file_name for file_name in file_names if file_system_utils.has_extension(file_name, ".png")]

	while len(remains) > 0:
		tags = get_tags_from_file_name(remains[0])
		tagged_files = [file_name for file_name in remains if file_name_has_tags(file_name, tags)]
		build_atlas_with_tags(input_dir, output_dir, tagged_files, tags);
		[remains.remove(file_name) for file_name in tagged_files]

#------------------------------------------------------------------------------
# Builds the given files with the given tags to a texture atlas.
#
# @author Ian Copland
#
# @param The input directory.
# @param The output directory.
# @param The file names to build.
# @param The tags.
#
# @return Output name
#------------------------------------------------------------------------------
def build_atlas_with_tags(input_dir, output_dir, file_names, tags):
	
	temp_dir = temp_dir_prefix + str(random.randint(0, 2147483647))
	os.mkdir(temp_dir);

	for file_name in file_names:
		source_file_name = os.path.join(input_dir, file_name)
		dest_file_name = os.path.join(temp_dir, remove_tags_from_file_name(file_name))
		shutil.copy2(source_file_name, dest_file_name)

	output_file_path = generate_atlas_file_path(output_dir, tags)
	build_texture_atlas(temp_dir, output_file_path, tags)

	shutil.rmtree(temp_dir)

#------------------------------------------------------------------------------
# Builds a single atlas from the pngs in the given directory.
#
# @author S Downie
#
# @param Input directory path
# @param Output file path
# @param The tags.
#------------------------------------------------------------------------------
def build_texture_atlas(input_file_path, output_file_path, tags):

	print(output_file_path)

	max_size = 2048;
	if tags.lower().count("high"):
		max_size = 4096

	tool_path = file_system_utils.get_path_from_here(relative_tool_path)
	subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_path, "--input", input_file_path, "--output", output_file_path, "--maxwidth", str(max_size), "--maxheight", str(max_size), "--padding", "2"]);

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
	build(input_path, output_path)

if __name__ == "__main__":
	main(sys.argv)

