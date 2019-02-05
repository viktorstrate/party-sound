# Phaethon - A FLOSS resource explorer for BioWare's Aurora engine games
#
# Phaethon is the legal property of its developers, whose names
# can be found in the AUTHORS file distributed with this source
# distribution.
#
# Phaethon is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# Phaethon is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Phaethon. If not, see <http://www.gnu.org/licenses/>.

# Try to find MAD library and include path.
# Once done this will define
#
# MAD_INCLUDE_DIRS - where to find mad.h, etc.
# MAD_LIBRARIES - List of libraries when using mad.
# MAD_FOUND - True if mad found.

if(WIN32)
    find_path(MAD_INCLUDE_DIR mad.h $ENV{PROGRAMFILES}/MAD/include DOC "The directory where mad.h resides")
    find_library(MAD_LIBRARY NAMES mad PATHS $ENV{PROGRAMFILES}/MAD/lib DOC "The libmad library")

else(WIN32)
    find_path(MAD_INCLUDE_DIR mad.h DOC "The directory where mad.h resides")
    find_library(MAD_LIBRARY NAMES libmad.a DOC "The libmad library")

endif(WIN32)

if(MAD_INCLUDE_DIR AND MAD_LIBRARY)
    set(MAD_FOUND 1)
    set(MAD_LIBRARIES ${MAD_LIBRARY})
    set(MAD_INCLUDE_DIRS ${MAD_INCLUDE_DIR})
else(MAD_INCLUDE_DIR AND MAD_LIBRARY)
    set(MAD_FOUND 0)
    set(MAD_LIBRARIES)
    set(MAD_INCLUDE_DIRS)
endif(MAD_INCLUDE_DIR AND MAD_LIBRARY)

mark_as_advanced(MAD_INCLUDE_DIR)
mark_as_advanced(MAD_LIBRARY)
mark_as_advanced(MAD_FOUND)

if(NOT MAD_FOUND)
    set(MAD_DIR_MESSAGE "libmad was not found. Make sure MAD_LIBRARY and MAD_INCLUDE_DIR are set.")
    if(NOT MAD_FIND_QUIETLY)
        message(STATUS "${MAD_DIR_MESSAGE}")
    else(NOT MAD_FIND_QUIETLY)
        if(MAD_FIND_REQUIRED)
            message(FATAL_ERROR "${MAD_DIR_MESSAGE}")
        endif(MAD_FIND_REQUIRED)
    endif(NOT MAD_FIND_QUIETLY)
else(NOT MAD_FOUND)
    message(STATUS "Found libmad: ${MAD_LIBRARY}")
endif(NOT MAD_FOUND)