#!/bin/bash
# usage: sh clean.sh

#------------------------------------------------------------------------
# Author:	Franzi Edo.	The 2011-01-05
# Modifs:
#
# SVN:
# $Author:: efr               $:  Author of last commit
# $Rev:: 185                  $:  Revision of last commit
# $Date:: 2017-08-17 15:20:55#$:  Date of last commit
#
# Project:	uKOS
# Goal:		Cleaning all the projects.
#
#			usage:
#			sh clean.sh
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

PATH_PRG=${PATH_UKOS_KERNEL}/Ports/Targets

if [ -z "$PATH_UKOS_KERNEL" ]; then
	echo "Variable PATH_UKOS_KERNEL is not set!"
	exit 1
fi

TAB=(

#	Families of targets & Variant_x

	"Baphomet_407"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"Baphomet_429"			"Test"		"USB"		"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"Baphomet_746"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"

	"CSEM_icycam"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"

	"CSEM_VIP_429"			"Test"		"USB"		"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"CSEM_VIP_746"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"CSEM_VIP_777"			"Test"		"Test_32M"	"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"

	"Discovery_407"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"Discovery_429"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"Discovery_476"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	"Discovery_746"			"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"

	"LAP_icycam_103"		"Test"		"-"			"-"		"-"		"-"		"-"		"-"		"-"		"-"		"-"
	)

SIZE_TAB=${#TAB[@]}
let "structure = 11"
let "nbElements = SIZE_TAB/structure"

echo "Cleaning all ..."
for (( h = 0 ; h < nbElements ; h++ ))
	do
		let "project = h * structure"
		for (( ACTION = 1 ; ACTION < structure ; ACTION++ ))
			do
				let "index = project + ACTION"
				if [ ${TAB[$index]} != "-" ]; then
					cd ${PATH_PRG}/${TAB[$project]}/Variant_${TAB[$index]}/System
					make clean_all
				fi
			done
	done

echo "Terminated!"
