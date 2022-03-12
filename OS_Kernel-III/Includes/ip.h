/*
; ip.
; ===

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 161                  $:  Revision of last commit
; $Date:: 2017-06-15 15:01:25#$:  Date of last commit
;
; Project:	uKOS
; Goal:		IP macros.
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;                                              __ ______  _____
;   Franzi Edo.                         __  __/ //_/ __ \/ ___/
;   5-Route de Cheseaux                / / / / ,< / / / /\__ \
;   CH 1400 Cheseaux-Noréaz           / /_/ / /| / /_/ /___/ /
;                                     \__,_/_/ |_\____//____/
;   edo.franzi@ukos.ch
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU Affero General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU Affero General Public License for more details.
;
;   You should have received a copy of the GNU Affero General Public License
;   along with this program. If not, see <http://www.gnu.org/licenses/>.
;
;------------------------------------------------------------------------
*/

#ifndef	__IP__
#define	__IP__

// String macros
// -------------

#define	LOC_CONST_STRG(aId)		static const char_t	aId	__attribute__ ((aligned (4)))

#define	LOC_STATI_STRG(aId)		static 		 char_t	aId	__attribute__ ((aligned (4)))

#define	GLB_CONST_STRG(aId)		const		 char_t	aId	__attribute__ ((aligned (4)))

#define	KCOPYRIGHT_FRANZI		"uKOS uKernel is (c) of Franzi Edo., 1992-2017\n" \
								"                          All rights reserved\n" \
								"                           edo.franzi@ukos.ch\n\n";

#define	KSTRIP					"uKOS is a development environment for embedded\n" \
								"real-time applications.\n\n" \
								"uKOS is an I.P. of Franzi Edo.\n" \
								"                   5-Route de Cheseaux\n" \
								"                   CH-1400 Cheseaux-Noreaz\n\n" \
								"                   email: edo.franzi@ukos.ch\n\n";

#define	KSTRLOGO				"\n" \
								"(c) 1992-2017, Franzi Edo., http://www.ukos.ch\n" \
								"______________________________________________\n\n" \
								"          __ ______  ____\n" \
								"    __ __/ //_/ __ \\/ __/   ______   uKernel Operating System\n" \
								"   / // / ,< / /_/ /\\ \\    ______    edo.franzi@ukos.ch\n" \
								"  / .,_/_/|_|\\____/___/\n" \
								" /_/\n\n" \
								"Design & Development by Franzi Edo.\n" \
								"All rights reserved.\n\n";

#define	KSTRLOGO_HELP			"(c) 1992-2017, Franzi Edo., http://www.ukos.ch\n" \
								"______________________________________________\n\n" \
								"          __ ______  ____\n" \
								"    __ __/ //_/ __ \\/ __/   ______   uKernel Operating System\n" \
								"   / // / ,< / /_/ /\\ \\    ______    edo.franzi@ukos.ch\n" \
								"  / .,_/_/|_|\\____/___/\n" \
								" /_/\n\n" \
								"Design & Development by Franzi Edo.\n" \
								"All rights reserved.\n\n" \
								"Available commands:\n\n";

#define	KSTRLOGO_BOOT			"(c) 1992-2017, Franzi Edo., http://www.ukos.ch\n" \
								"______________________________________________\n\n" \
								"          __ ______  ____\n" \
								"    __ __/ //_/ __ \\/ __/   ______   uKernel Operating System\n" \
								"   / // / ,< / /_/ /\\ \\    ______    edo.franzi@ukos.ch\n" \
								"  / .,_/_/|_|\\____/___/\n" \
								" /_/\n\n" \
								"Design & Development by Franzi Edo.\n" \
								"All rights reserved.\n\n" \
								"Waiting for the new uKOS system code in Motorola S format.\n\n";
#endif
