#!/usr/bin/python
#
#  content_builder.py
#  CSRunner
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

import argparse
import os
import sys

import model_builder
import music_builder
import sfx_builder
import text_builder
import texture_atlas_builder
import texture_builder

MODEL_DIRECTORY_PATH = "Models/"
MUSIC_DIRECTORY_PATH = "Music/"
SFX_DIRECTORY_PATH = "SFX/"
TEXT_DIRECTORY_PATH = "Text/"
TEXTURE_ATLAS_DIRECTORY_PATH = "TextureAtlases/"
TEXTURE_DIRECTORY_PATH = "Textures/"

#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def build(input_directory_path, output_directory_path):
    """
    Runs all of the individual asset pipeline build scripts. 
    
    :Authors: Ian Copland
    
    :param input_directory_path: The input directory path.
    :param output_directory_path: The output directory path.
    """
    model_builder.build(os.path.join(input_directory_path, MODEL_DIRECTORY_PATH), os.path.join(output_directory_path, MODEL_DIRECTORY_PATH))
    texture_builder.build(os.path.join(input_directory_path, TEXTURE_DIRECTORY_PATH), os.path.join(output_directory_path, TEXTURE_DIRECTORY_PATH))
    texture_atlas_builder.build(os.path.join(input_directory_path, TEXTURE_ATLAS_DIRECTORY_PATH), os.path.join(output_directory_path, TEXTURE_ATLAS_DIRECTORY_PATH))
    music_builder.build(os.path.join(input_directory_path, MUSIC_DIRECTORY_PATH), os.path.join(output_directory_path, MUSIC_DIRECTORY_PATH))
    sfx_builder.build(os.path.join(input_directory_path, SFX_DIRECTORY_PATH), os.path.join(output_directory_path, SFX_DIRECTORY_PATH))
    text_builder.build(os.path.join(input_directory_path, TEXT_DIRECTORY_PATH), os.path.join(output_directory_path, TEXT_DIRECTORY_PATH))
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def parse_arguments():
    """
    Parses the given argument list.
    
    :Authors: Ian Copland
    
    :returns: A container for the parsed arguments.
    """
    script_desc = 'The default content pipeline for a ChilliSource application. This only provides a starting point and should be extended or replaced as per the requirements of the project.'
    
    parser = argparse.ArgumentParser(description=script_desc)
    parser.add_argument('-i', '--input', dest='input_directory_path', type=str, required=True, help="The input directory containing the assets which should be built.")
    parser.add_argument('-o', '--output', dest='output_directory_path', type=str, required=True, help="The output directory where the built assets should be saved.")
    
    return parser.parse_args()
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
if __name__ == "__main__":
    args = parse_arguments();
    build(args.input_directory_path, args.output_directory_path)

