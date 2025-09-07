# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA



SET(CSV_SOURCES  ha_tina.cc ha_tina.h transparent_file.cc transparent_file.h ccdb.h ccdb_locking.cc)

FIND_LIBRARY(ZOOKEEPER zookeeper_mt)
message("storage/csv CMakeLists.txt found library: " ${ZOOKEEPER})

ADD_LIBRARY(ccdb_locking ccdb_locking.cc) 
TARGET_LINK_LIBRARIES(ccdb_locking ccdb_locking.cc ${ZOOKEEPER}) 



SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})

MYSQL_ADD_PLUGIN(csv ${CSV_SOURCES} STORAGE_ENGINE MANDATORY)

