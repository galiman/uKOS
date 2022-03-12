#!/bin/bash
# usage: sh build.sh

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2011-01-05
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 148                  $:  Revision of last commit
# $Date:: 2017-06-04 17:55:00#$:  Date of last commit
#
# Project:	uKOS
# Goal:		Building all the engines.
#
#			usage:
#			sh build.sh
#
#   (c) 1992-2017, Franzi Edo.
#   --------------------------
#                                              __ ______  _____
#   Franzi Edo.                         __  __/ //_/ __ \/ ___/
#   5-Route de Cheseaux                / / / / ,< / / / /\__ \
#   CH 1400 Cheseaux-Noréaz           / /_/ / /| / /_/ /___/ /
#                                     \__,_/_/ |_\____//____/
#   edo.franzi@ukos.ch
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------------------------------------------------

PATH_ENGINES=${PATH_UKOS_KERNEL}/Ports/EquatesModels/Cores/CORTEX_M7/engines

if [ -z "$PATH_UKOS_KERNEL" ]; then
	echo "Variable PATH_UKOS_KERNEL is not set!"
	exit 1
fi

echo "Building all ..."

cd ${PATH_ENGINES}/lme/Confidential
make
cd ${PATH_ENGINES}/mpy/Construction
make
cd ${PATH_ENGINES}
make
echo "Terminated!"
