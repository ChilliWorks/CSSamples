#!/usr/bin/python
#
#  particle_randomizer.py
#  CSPong
#  Created by Angela Gross on 3/02/2016.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2016 Tag Games Limited
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
import shutil
import random
import file_system_utils

particle_prefixes = ["YellowMagma", "BlueMagma", "PinkIceCream", "BlueIceCream", "Smoke", "Spark", "Beam"]
min_particles = 0
max_particles = 3

#------------------------------------------------------------------------------
# Randomly selecting a particle image suffixed with numbers from min to max 
# num particles and copying it from the input to the output.
#
# @author Angela Gross
#
# @param Input path
# @param Output path
#------------------------------------------------------------------------------
def build(input_path, output_path):

	print("-----------------------------------------")
	print("           Randomizing particle images")
	print("-----------------------------------------")

	if(input_path.endswith("/") == False):
		input_path = input_path + "/"

	if(output_path.endswith("/") == False):
		output_path = output_path + "/"
		
	if (os.path.exists(input_path) == False):
		print("Error: input path of " + input_path + " does not exist.")
		return
		
	if (os.path.exists(output_path) == False):
		os.makedirs(output_path);
		
	for particle_prefix in particle_prefixes:
	
		random_particle = random.randint(min_particles, max_particles)
		targeted_particle_file_path = input_path + particle_prefix + "_" + str(random_particle) + ".png"
		output_particle_file_path = output_path + particle_prefix + ".png"
		
		if os.path.exists(targeted_particle_file_path):
			shutil.copyfile(targeted_particle_file_path, output_particle_file_path)
		else:
			print("Error: File with path " + targeted_particle_file_path + " not found.")

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
		
	