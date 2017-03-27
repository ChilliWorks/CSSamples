#!/usr/bin/python
#
#  text_builder.py
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

import argparse
import os
import shutil
import subprocess
import sys

import file_system_utils

LANGUAGES = ["en"]
RELATIVE_TOOL_FILE_PATH = "../../ChilliSource/Tools/CSTextBuilder.jar"

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build(input_directory_path, output_directory_path):
    """
    Walks the input directory and converts all xls sheets into cstext
    
    :Authors: S Downie
    
    :param input_directory_path: The input directory path.
    :param output_directory_path: The output directory path.
    """
    print("-----------------------------------------")
    print("             Building Text")
    print("-----------------------------------------")

    file_system_utils.delete_directory(output_directory_path)

    for current_input_directory_path, input_sub_directory_paths, file_names in os.walk(input_directory_path):
        current_output_directory_path = os.path.join(output_directory_path, current_input_directory_path[len(input_directory_path):])

        for file_name in file_names:
            if file_system_utils.has_extension(file_name, ".xls"):
                if os.path.exists(current_output_directory_path) == False:
                    os.makedirs(current_output_directory_path);
                
                for language in LANGUAGES:
                    input_file_path = os.path.join(current_input_directory_path, file_name);
                    output_file_path = os.path.splitext(os.path.join(current_output_directory_path, file_name))[0] + ".cstext"
                    
                    if language is not "en":
                        output_file_path = os.path.splitext(output_file_path)[0] + "." + language + ".cstext"

                    build_text(input_file_path, output_file_path, language)

    print (" ")
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build_text(input_file_path, output_file_path, language):
    """
    Converts a single language column in the xls sheet to a cstext file
    
    :Authors: S Downie
    
    :param input_file_path: The input file path.
    :param output_file_path: The output file path.
    """
    print(output_file_path)

    tool_file_path = file_system_utils.get_path_from_here(RELATIVE_TOOL_FILE_PATH)
    tool_args = ["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", tool_file_path, "--input", input_file_path, "--output", output_file_path, "--language", language, "--logginglevel", "error"]
    subprocess.call(tool_args)
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def parse_arguments():
    """
    Parses the given argument list.
    
    :Authors: Ian Copland
    
    :returns: A container for the parsed arguments.
    """
    script_desc = 'Converts all xls files in the input directory to cstext format and outputs them to the given output directory. Sub directories will be recursed.'
    
    parser = argparse.ArgumentParser(description=script_desc)
    parser.add_argument('-i', '--input', dest='input_directory_path', type=str, required=True, help="The input directory containing xls files.")
    parser.add_argument('-o', '--output', dest='output_directory_path', type=str, required=True, help="The output directory where the output cstext files should be saved.")
    
    return parser.parse_args()
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
if __name__ == "__main__":
    args = parse_arguments();
    build(args.input_directory_path, args.output_directory_path)

