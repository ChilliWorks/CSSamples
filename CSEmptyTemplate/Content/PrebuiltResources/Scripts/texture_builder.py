#!/usr/bin/python
#
#  texture_builder.py
#  CSEmptyTemplate
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
# Walks the input directory and converts
# all pngs into csimages
#
# @author S Downie
#
# @param Input path
# @param Output path
#----------------------------------------
def BuildTextures(input_path, output_path):
	print("Building textures...")

	if(input_path.endswith("/") == False):
		input_path = input_path + "/"

	if(output_path.endswith("/") == False):
		output_path = output_path + "/"

	for directory, sub_dirs, filenames in os.walk(input_path):
		for sub_dir in sub_dirs:
			input_dir = os.path.join(directory, sub_dir)
			output_dir = os.path.join(output_path, input_dir[len(input_path):len(input_dir)]);

		if len(sub_dirs) == 0:
			for filename in filenames:
				if HasExtension(filename, ".png") == True:
					split_dirs = output_dir.split("/")
					split_dirs.pop()
					final_output_dir = "/".join(split_dirs)

					if os.path.exists(final_output_dir) == False:
						os.makedirs(final_output_dir);

					BuildTexture(os.path.join(directory, filename), final_output_dir)

	print ("Texture building finished")
	print("-----------------------------------------")
	print("-----------------------------------------")

#----------------------------------------
# Depending on the file path and the tags
# build a suitable filename: i.e.
# Texture/GUI/Med -> GUI.med.csimage
# Texture/GUI/Med.Wide -> GUI.med.wide.csimage
#
# @author S Downie
#
# @param Input path
#
# @return Output name
#----------------------------------------
def GenerateTextureName(input_filepath):
	split_path = input_filepath.split("/")

	if(len(split_path) < 2):
		print("ERROR: Path has no Tag folders i.e. (Med, High, Low)")

	name = os.path.splitext(split_path[len(split_path) - 1])[0]

	split_tags = split_path[len(split_path) - 2].split(".")

	for tag in split_tags:
		if tag.lower() != "common":
			name = name+"."+tag.lower()

	return name+".csimage"

#----------------------------------------
# Converts a single PNG to a csimage
#
# @author S Downie
#
# @param Input path
# @param Output path
#----------------------------------------
def BuildTexture(input_filepath, output_filepath):
	name = GenerateTextureName(input_filepath)

	taggedoutput_filepath = os.path.join(output_filepath, name)
	print("Building texture: " + taggedoutput_filepath)

	tool_path = os.path.join("..", "..", "ChilliSource", "Tools", "PNGToCSImage.jar")

	subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_path, "--input", input_filepath, "--output", taggedoutput_filepath]);

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

	BuildTextures(input_path, output_path)

if __name__ == "__main__":
	main(sys.argv)

