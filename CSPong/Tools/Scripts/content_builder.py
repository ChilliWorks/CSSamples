#!/usr/bin/python
#
#  content_builder.py
#  CSPong
#  Created by Ian Copland on 22/01/2015.
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

import os
import sys
import model_builder
import text_builder
import texture_atlas_builder
import texture_builder
import sfx_builder
import music_builder

model_directory_path = "Models/"
text_directory_path = "Text/"
texture_atlas_directory_path = "TextureAtlases/"
texture_directory_path = "Textures/"
sfx_directory_path = "SFX/"
music_directory_path = "Music/"

#------------------------------------------------------------------------------
# The entry point into the script.
#
# @author Ian Copland
#
# @param The list of arguments.
#------------------------------------------------------------------------------
def main(args):
	if not len(args) is 3:
		print("ERROR: Incorrect parameters supplied.")
		return

	input_path = args[1]
	output_path = args[2]

	model_builder.build(os.path.join(input_path, model_directory_path), os.path.join(output_path, model_directory_path))
	text_builder.build(os.path.join(input_path, text_directory_path), os.path.join(output_path, text_directory_path))
	texture_atlas_builder.build(os.path.join(input_path, texture_atlas_directory_path), os.path.join(output_path, texture_atlas_directory_path))
	texture_builder.build(os.path.join(input_path, texture_directory_path), os.path.join(output_path, texture_directory_path))
	sfx_builder.build(os.path.join(input_path, sfx_directory_path), os.path.join(output_path, sfx_directory_path))
	music_builder.build(os.path.join(input_path, music_directory_path), os.path.join(output_path, music_directory_path))

if __name__ == "__main__":
	main(sys.argv)

