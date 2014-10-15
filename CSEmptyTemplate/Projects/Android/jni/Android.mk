#
#  Android.mk
#  CSEmptyTemplate
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

#Setup build configurations and build the Chilli Source libraries
ifeq ($(BUILD_CONFIG),Release)
CS_CXXFLAGS_EXTRA := -DCS_ANDROIDEXTENSION_GOOGLEPLAY -DCS_ANDROIDEXTENSION_AMAZON
include ../../ChilliSource/Tools/Scripts/android_build_release.mk
else
CS_CXXFLAGS_EXTRA := -DCS_ANDROIDEXTENSION_GOOGLEPLAY -DCS_ANDROIDEXTENSION_AMAZON
include ../../ChilliSource/Tools/Scripts/android_build_debug.mk
endif

#build the application library
APPLICATION_FILES := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '../../AppSource/' '--extensions' 'cpp,c,cc')

include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_OUTPUT)
LOCAL_CXXFLAGS := $(CS_CXXFLAGS)
LOCAL_SRC_FILES := $(APPLICATION_FILES)
LOCAL_WHOLE_STATIC_LIBRARIES := $(CS_WHOLE_STATIC_LIBRARIES)
LOCAL_LDLIBS := $(CS_LDLIBS)
LOCAL_C_INCLUDES:= $(CS_C_INCLUDES) ../../AppSource/
include $(BUILD_SHARED_LIBRARY)

