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

#----------------------------------------
# @param the file path.
# @param the extension.
#
# @author Ian Copland
#
# @return whether or not the path has
# the extension.
#----------------------------------------
def HasExtension(filepath, extension):
    lower_filepath = filepath.lower()
    lower_extension = extension.lower()
    return lower_filepath.endswith(lower_extension)

#----------------------------------------
# Walks the input directory and packs
# all pngs in each folder onto an atlas
# for that folder
#
# @author S Downie
#
# @param Input path
# @param Output path
#----------------------------------------
def BuildTextureAtlases(input_path, output_path):
	print("Building atlases...")

	if(input_path.endswith("/") == False):
		input_path = input_path + "/"

	if(output_path.endswith("/") == False):
		output_path = output_path + "/"

	for directory, sub_dirs, filenames in os.walk(input_path):
		for sub_dir in sub_dirs:
			input_dir = os.path.join(directory, sub_dir)
			output_dir = os.path.join(output_path, input_dir[len(input_path):len(input_dir)]);

		if len(sub_dirs) == 0:
			contains_png = False
			for filename in filenames:
				if HasExtension(filename, ".png") == True:
					contains_png = True
					break

			if contains_png == True:
				split_dirs = output_dir.split("/")
				split_dirs.pop()
				final_output_dir = "/".join(split_dirs)

				if os.path.exists(final_output_dir) == False:
					os.makedirs(final_output_dir);

				BuildTextureAtlas(directory, final_output_dir)

	print ("Atlas building finished")
	print("-----------------------------------------")
	print("-----------------------------------------")

#----------------------------------------
# Depending on the file path and the tags
# build a suitable filename: i.e.
# Texture/GUI/Med -> GUI.med.csatlas
# Texture/GUI/Med.Wide -> GUI.med.wide.csatlas
#
# @author S Downie
#
# @param Input path
#
# @return Output name
#----------------------------------------
def GenerateAtlasName(input_filepath):
	split_path = input_filepath.split("/")

	if(len(split_path) < 2):
		print("ERROR: Path has no Tag folders i.e. (Med, High, Low)")

	name = split_path[len(split_path) - 2]

	split_tags = split_path[len(split_path) - 1].split(".")

	for tag in split_tags:
		if tag.lower() != "common":
			name = name+"."+tag.lower()

	return name+".csatlas"

#----------------------------------------
# Builds a single atlas from the pngs
# in the given directory.
#
# @author S Downie
#
# @param Input path
# @param Output path
#----------------------------------------
def BuildTextureAtlas(input_filepath, output_filepath):
	name = GenerateAtlasName(input_filepath)

	taggedoutput_filepath = os.path.join(output_filepath, name)
	print("Building atlas: " + taggedoutput_filepath)

	tool_path = os.path.join("..", "..", "ChilliSource", "Tools", "CSAtlasBuilder.jar")

	subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_path, "--input", input_filepath, "--output", taggedoutput_filepath, "--maxwidth", "4096", "--maxheight", "4096", "--padding", "2"]);

#----------------------------------------------------------------------
# The entry point into the script.
#
# @author S Downie
#
# @param The list of arguments.
#----------------------------------------------------------------------
def main(args):
	if not len(args) is 3:
		print("ERROR: Missing input and output paths")
		return

	input_path = args[1]
	output_path = args[2]

	if os.path.exists(output_path) == True:
		shutil.rmtree(output_path)

	BuildTextureAtlases(input_path, output_path)

if __name__ == "__main__":
	main(sys.argv)

