/*
; stub.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Kern - Stub management.
;
;			This module implements the stub primitives.
;
; 			stub system calls
; 			-----------------
;
;			void	stub_kern_init(void);
;			void	stub_kern_runKernel(void);
;			void	stub_kern_loadOverlay(uint32_t idModule);
;			void	stub_kern_setLowPower(uint8_t mode);
;			void	stub_kern_runPrecise(uint16_t time, void (*code)(void));
;			void	stub_kern_initStackFrame(volatile proc_t *handle, const void *argument);
;			void	stub_kern_setUnixTime(uint32_t unixTime);
;			void	stub_kern_getUnixTime(uint32_t *unixTime);
;			void	stub_kern_timeStmp(uint64_t *timeStmp);
;			void	stub_kern_timeNext(void);
;			void	stub_kern_getPC(const uint32_t *stackProcess, uint32_t *pc);
;			void	stub_kern_getFunctionName(const uint32_t pc, const char_t **function);
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

#ifndef	__STUB__
#define	__STUB__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	stub_kern_init(void);
extern	void	stub_kern_runKernel(void);
extern	void	stub_kern_loadOverlay(uint32_t idModule);
extern	void	stub_kern_setLowPower(uint8_t mode);
extern	void	stub_kern_runPrecise(uint16_t time, void (*code)(void));
extern	void	stub_kern_initStackFrame(volatile proc_t *handle, const void *argument);
extern	void	stub_kern_setUnixTime(uint32_t unixTime);
extern	void	stub_kern_getUnixTime(uint32_t *unixTime);
extern	void	stub_kern_timeStmp(uint64_t *timeStmp);
extern	void	stub_kern_timeNext(void);
extern	void	stub_kern_getPC(const uint32_t *stackProcess, uint32_t *pc);
extern	void	stub_kern_getFunctionName(const uint32_t pc, const char_t **function);

#ifdef	__cplusplus
}
#endif
#endif
