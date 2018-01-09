#!/usr/bin/python
#
#  file_system_utils.py
#  CSRunner
#  Created by Ian Copland on 22/01/2015.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2015 Tag Games Limited
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

import errno
import shutil
import sys
import os

#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def get_path_from_here(path):
    """
    Calculates the absolute path from a path relative to this script file.
    
    :Authors: Ian Copland
    
    :param path: The path relative to this source file.
    
    :returns: The absolute path.
    """
    return os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), path));
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def copy_directory(src_path, dst_path):
    """
    Copy the given dst directory or create it if it doesn't exist with the 
    contents of the src directory. This will overwrite exisiting files of the 
    same name in the dst directory.
    
    :Authors: S Downie
    
    :param src_path: The source directory path.
    :param dst_path: The destination directory path.
    """
    if os.path.exists(dst_path) == False:
        os.makedirs(dst_path)

    for item in os.listdir(src_path):
        src = os.path.join(src_path, item)
        dst = os.path.join(dst_path, item)
        if os.path.isdir(src):
            copy_directory(src, dst)
        else:
            shutil.copy2(src, dst)
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def delete_directory(directory_path):
    """
    Deletes a directory if it exists. Does not return an error if trying to 
    delete a directory that doesn't exist.
    
    :Authors: Ian Copland
    
    :param directory_path: The path to the directory to delete.
    """
    if os.path.exists(directory_path) == True:
        shutil.rmtree(directory_path)
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------  
def delete_file(file_path):
    """
    Deletes a file if it exists. Does not return an error if trying to delete a
    file that doesn't exist.
    
    :Authors: Ian Copland
    
    :param file_path: The path to the file to delete.
    """
    if os.path.isfile(file_path) == True:
        os.remove(file_path)
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def has_extension(file_path, extension):
    """
    :Authors: Ian Copland
    
    :param file_path: The file path.
    :param extension: The extension. This is not case sensitive.
    
    :returns: Whether or not the path has the given extension.
    """
    lowercase_filepath = file_path.lower()
    lowercase_extension = extension.lower()
    return lowercase_filepath.endswith(lowercase_extension)


