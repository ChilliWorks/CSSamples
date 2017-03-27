#!/usr/bin/python
#
#  texture_atlas_builder.py
#  CSRunner
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

RELATIVE_TOOL_FILE_PATH = "../../ChilliSource/Tools/CSAtlasBuilder.jar"
TEMP_DIRECTORY_NAME_PREFIX = "_temp-textureatlasbuilder-"

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build(input_directory_path, output_directory_path):
    """
    Walks the input directory and packs all pngs in each folder into an atlas
    for that folder.
    
    :Authors: S Downie
    
    :param input_directory_path: The input directory path.
    :param output_directory_path: The output directory path.
    """
    print("-----------------------------------------")
    print("           Building atlases")
    print("-----------------------------------------")

    file_system_utils.delete_directory(output_directory_path)

    for current_input_directory_path, sub_input_directory_paths, file_names in os.walk(input_directory_path):
        current_output_directory_path = os.path.join(output_directory_path, current_input_directory_path[len(input_directory_path):]);

        if len(sub_input_directory_paths) == 0:
            for file_name in file_names:
                if file_system_utils.has_extension(file_name, ".png") == True:
                    contains_png = True
                    break
            
            if os.path.exists(current_output_directory_path) == False:
                os.makedirs(current_output_directory_path);

            build_altases_in_directory(current_input_directory_path, current_output_directory_path, file_names)

    print(" ")
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def get_tags_from_file_name(file_name):
    """
    :Authors: Ian Copland
    
    :param file_name: A file name.
    
    :returns: The tags in the file name, or an empty string if there isn't one.
    """
    if file_name.count('.') > 1:
        first = file_name.find(".")
        last = file_name.rfind(".")
        tags = file_name[first + 1 : last]
        return tags.lower()
    else:
        return ""
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def file_name_has_tags(file_name, tags):
    """
    :Authors: Ian Copland
    
    :param file_name: A file name.
    :param tags: The tags string
    
    :returns: Whether or not the file name has the given tags.
    """
    if get_tags_from_file_name(file_name) == tags.lower():
        return True
    return False
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def remove_tags_from_file_name(file_name):
    """
    :Authors: Ian Copland
    
    :param file_name: A file name.
    
    :returns: The file name with any tags removed.
    """
    if file_name.count('.') > 1:
        first = file_name.find(".")
        last = file_name.rfind(".")
        tagless_file_name = file_name[0 : first] + file_name[last : len(file_name)]
        return tagless_file_name
    else:
        return file_name
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def add_tags_to_file_path(file_path, tags):
    """
    :Authors: Ian Copland
    
    :param file_name: A file name.
    :param tags: The tags string.
    
    :returns: The file path with the given tags added to it.
    """
    if len(tags) > 0:
        period_index = file_path.rfind(".")
        tagged_file_path = file_path[0 : period_index] + "." + tags + file_path[period_index : len(file_path)]
        return tagged_file_path
    else:
        return file_path
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def generate_atlas_file_path(output_directory_path, tags):
    """
    :Authors: Ian Copland
    
    :param output_directory_path: The path to an atlas.
    :param tags: The tags.
    
    :returns: The output atlas file path with tags.
    """
    atlas_file_path = os.path.join(output_directory_path, os.path.basename(output_directory_path) + ".csatlas");
    return add_tags_to_file_path(atlas_file_path, tags)
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build_altases_in_directory(input_directory_path, output_directory_path, file_names):
    """
    Builds each of the different atlases in the given directory. Different atlases
    are build for assets with different "resource tags", i.e Image.low.png and
    Image.high.png would end up on different texture atlases.
    
    :Authors: Ian Copland
    
    :param input_directory_path: The input directory path.
    :param output_directory_path: The output directory path.
    :param file_names: The list of file names.
    """
    remains = [file_name for file_name in file_names if file_system_utils.has_extension(file_name, ".png")]

    while len(remains) > 0:
        tags = get_tags_from_file_name(remains[0])
        tagged_files = [file_name for file_name in remains if file_name_has_tags(file_name, tags)]
        build_atlas_with_tags(input_directory_path, output_directory_path, tagged_files, tags);
        [remains.remove(file_name) for file_name in tagged_files]
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build_atlas_with_tags(input_directory_path, output_directory_path, file_names, tags):
    """
    Builds the given files with the given tags to a texture atlas.
    
    :Authors: Ian Copland
    
    :param input_directory_path: The input directory path.
    :param output_directory_path: The output directory path.
    :param file_names: The file names to build.
    :param tags: The tags.
    """
    temp_dir = TEMP_DIRECTORY_NAME_PREFIX + str(random.randint(0, 2147483647))
    os.mkdir(temp_dir);

    for file_name in file_names:
        source_file_name = os.path.join(input_directory_path, file_name)
        dest_file_name = os.path.join(temp_dir, remove_tags_from_file_name(file_name))
        shutil.copy2(source_file_name, dest_file_name)

    output_file_path = generate_atlas_file_path(output_directory_path, tags)
    build_texture_atlas(temp_dir, output_file_path, tags)

    shutil.rmtree(temp_dir)
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build_texture_atlas(input_directory_path, output_file_path, tags):
    """
    Builds a single atlas from the pngs in the given directory.
    
    :Authors: S Downie
    
    :param input_directory_path: The input directory path.
    :param output_file_path: The output file path.
    :param tags: The tags.
    """
    print(output_file_path)

    #Allow the 'high' texture atlas to be up to 4096x4096; all others must be a maximum of 2048x2048
    max_size = 2048;
    if tags.lower().count("high"):
        max_size = 4096

    tool_file_path = file_system_utils.get_path_from_here(RELATIVE_TOOL_FILE_PATH)
    tool_args = ["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_file_path, "--input", input_directory_path, "--output", output_file_path, "--maxwidth", str(max_size), "--maxheight", str(max_size), "--padding", "2"]
    subprocess.call(tool_args);
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def parse_arguments():
    """
    Parses the given argument list.
    
    :Authors: Ian Copland
    
    :returns: A container for the parsed arguments.
    """
    script_desc = 'Converts all sub-directories containing PNGs in the input directory to csatlas format and outputs them to the given output directory. Sub directories will be recursed.'
    
    parser = argparse.ArgumentParser(description=script_desc)
    parser.add_argument('-i', '--input', dest='input_directory_path', type=str, required=True, help="The input directory containing the image sub directories.")
    parser.add_argument('-o', '--output', dest='output_directory_path', type=str, required=True, help="The output directory where the output csatlas files should be saved.")
    
    return parser.parse_args()
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
if __name__ == "__main__":
    args = parse_arguments();
    build(args.input_directory_path, args.output_directory_path)

