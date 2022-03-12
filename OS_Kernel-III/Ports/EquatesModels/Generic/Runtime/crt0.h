/*
; crt0.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2007-05-27
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		crt0 for the uKOS system.
;			See "ld.pdf" file, "Using LD, the GNU linker" page 48.
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

#ifndef	__CRT0__
#define	__CRT0__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	int32_t		_start(void);
extern	int32_t		boot(void);
extern	int			main(void);
extern	void		exit(int number);

extern	void		init_init(void);
extern	void		exce_init(void);
extern	void		cmns_init(void);
extern	void		cmns_send(uint32_t commManager, const char_t *ascii);
extern	void		cmns_receive(uint32_t commManager, uint8_t *data);

#ifdef	__cplusplus
}
#endif

#endif
