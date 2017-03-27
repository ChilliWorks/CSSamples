#
#  Android.mk
#  CSRunner
#  Created by Ian Copland on 12/03/2012
#
#  The MIT License (MIT)
#
#  Copyright (c) 2012 Tag Games Limited
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

#prepare the ChilliSource libraries
CS_PROJECT_ROOT=../../../../../../..
include $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/AndroidBuildInitialise.mk

#gather all application source files
APPLICATION_FILES := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/AppSource/' '--extensions' 'cpp,c,cc')

#build the application
include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_APPLICATION)
LOCAL_CXXFLAGS := $(CS_CXXFLAGS)
LOCAL_SRC_FILES := $(APPLICATION_FILES)
LOCAL_WHOLE_STATIC_LIBRARIES := $(CS_STATIC_LIBRARIES)
LOCAL_LDLIBS := $(CS_LDLIBS)
LOCAL_C_INCLUDES:= $(CS_C_INCLUDES) $(CS_PROJECT_ROOT)/AppSource/
include $(BUILD_SHARED_LIBRARY)

#build CPU features for NEON support
$(call import-module,android/cpufeatures)