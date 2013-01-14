#################################################################################
# This file is part of the Wikireader project
# Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#/

cmake_minimum_required(VERSION 2.8.9)
project(Wikireader)

option(BUILD_WIKIREADER_BLACKBERRY "Build Wikireader for Blackberry 10" ON)

SET(CMAKE_CXX_FLAGS "-Wall -fPIC")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS} -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)

####### Core #######
#add_subdirectory(core)

###### Frontends ######
add_subdirectory(frontends)

###### CPACK part ######
#include(CPack)

###### CTEST part ######
#include(CTest)

