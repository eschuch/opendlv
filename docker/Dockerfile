# Dockerfile - Dockerfile to create a Docker image from OpenDLV.
# Copyright (C) 2016 Christian Berger
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# Date: 2016-05-20

FROM seresearch/odsimulation:latest

# Set locale (fix the locale warnings)
RUN localedef -v -c -i en_US -f UTF-8 en_US.UTF-8 || :

RUN apt-get install -y libpopt-dev opendavinci-odcandatastructuregenerator opendavinci-oddatastructuregenerator opendavinci-odcantools

ADD ./ /opt/data

WORKDIR /opt/data

RUN mkdir -p build-docker && cd build-docker && PATH=$PATH:/usr/bin cmake -D CMAKE_INSTALL_PREFIX=/opt/odlv -D OPENDAVINCI_DIR=/usr/ .. && make
