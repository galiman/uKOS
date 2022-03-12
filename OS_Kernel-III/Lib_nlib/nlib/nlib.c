/*
; nlib.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
; Version:	1.2
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		newLib interface for gcc C compiler (reentrant version).
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

#include	<uKOS.h>
#include	<_ansi.h>
#include 	<reent.h>
#include 	<errno.h>
#include 	<string.h>
#include 	<malloc.h>

typedef	struct	_reent		reent_t;
typedef	struct	devOptTab	devOptTab_t;
typedef			_CLOCK_T_ 	clock_t;
typedef			_off_t		off_t;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"nlib         nlib manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "nLib manager\n"
									"============\n\n"

									"This manager ...\n\n";

struct devOptTab {
			const	char_t		*name;
					int			(*open_r )(reent_t *reent, const char_t *file, int flags, int mode);
					int			(*close_r)(reent_t *reent, int fd);
					_ssize_t	(*write_r)(reent_t *reent, int fd, const void *buf, size_t cnt);
					_ssize_t	(*read_r )(reent_t *reent, int fd,       void *buf, size_t cnt);
		};

// Prototypes
// ==========

static	int			com1_open_r (reent_t *reent, const char_t *file, int flags, int mode);
static	int			com1_close_r(reent_t *reent, int fd);
static	_ssize_t	com1_write_r(reent_t *reent, int fd, const void *buf, size_t cnt);
static	_ssize_t	com1_read_r (reent_t *reent, int fd, void *buf, size_t cnt);

const	devOptTab_t	devoptab_com1 = {
						"com1",
						com1_open_r,
						com1_close_r,
						com1_write_r,
						com1_read_r
					};

const	devOptTab_t	*devoptab_list[] = { 
						&devoptab_com1, 	// stdin  fd = 0
						&devoptab_com1,		// stdout fd = 1
						&devoptab_com1,		// stderr fd = 2
						0
				 	};

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Nlib, KIDNLIB, KNEWLIBNUM, NULL, "2.0", (1<<BSHOW));

// For I/O implementation
// ======================

/*
 * \brief _isatty_r
 *
 */
int	_isatty_r(reent_t *reent, int fd) {

	return (1);
}

/*
 * \brief _times_r
 *
 */
clock_t _times_r(reent_t *reent, struct tms *buf) {

	return ((clock_t)-1);
}

/*
 * \brief _wait_r
 *
 */
int _wait_r(reent_t *reent, int *status) {

	reent->_errno = ECHILD;
	return (-1);
}

/*
 * \brief __errno
 *
 */
int *__errno(void) {

	return ((int *)&_impure_ptr->_errno);
}

/*
 * \brief _fork_r
 *
 */
int _fork_r(reent_t *reent) {

	reent->_errno = ENOTSUP;
	return (-1);
}

/*
 * \brief _stat_r
 *
 */
int _stat_r(reent_t *reent, const char_t *file, struct stat *pstat) {

	#if 0
	pstat->st_mode = S_IFCHR;
	#endif

	return (0);
}

/*
 * \brief _fstat_r
 *
 */
int _fstat_r(reent_t *reent, int fd, struct stat *pstat) {

	#if 0
	pstat->st_mode = S_IFCHR;
	#endif

	return (0);
}

/*
 * \brief _link_r
 *
 */
int _link_r(reent_t *reent, const char_t *oldName, const char_t *newName) {

	reent->_errno = EMLINK;
	return (-1);
}

/*
 * \brief _unlink_r
 *
 */
int _unlink_r(reent_t *reent, const char_t *name) {

	reent->_errno = EMLINK;
	return (-1);
}

/*
 * \brief _lseek_r
 *
 */
off_t	_lseek_r(reent_t *reent, int fd, off_t pos, int whence) {

	return (0);
}

/*
 * \brief _getpid_r
 *
 */
int _getpid_r(reent_t *reent) {

	return (1);
}

/*
 * \brief _kill_r
 *
 */
int _kill_r(reent_t *reent, int pid, int sig) {

	reent->_errno = EINVAL;
	return (-1);
}

/*
 * \brief _sbrk_r
 *
 */
void	*_sbrk_r(reent_t *reent, ptrdiff_t incr) {
	uint8_t		*base;

	base  = (uint8_t *)syos_malloc(KEXTERNAL, incr);
	return ((void *)base);
}

/*
 * \brief __malloc_lock
 *
 */
void	__malloc_lock(reent_t *reent) {

}

/*
 * \brief __malloc_unlock
 *
 */
void	__malloc_unlock(reent_t *reent) {

}

// com1 I/O routines
// =================

/*
 * \brief _open_r
 *
 */
int	_open_r(reent_t *reent, const char_t *file, int flags, int mode) {
			int32_t			wFd = 0, fd = -1;
			bool_t			terminate = FALSE;
	const	devOptTab_t		*channel;

	do {
		channel = devoptab_list[wFd];
		if (strcmp(channel->name, file) == 0) {
			fd = wFd;
			terminate = TRUE;
		}
	} while ((devoptab_list[wFd++] != 0) && (!terminate));

	if (fd != -1) {
		channel = devoptab_list[fd];
		devoptab_list[fd]->open_r(reent, file, flags, mode);
	}
	else {
		reent->_errno = ENODEV;
	}
	return ((int)fd);
}

/*
 * \brief _close_r
 *
 */
int _close_r(reent_t *reent, int fd) {

	return (devoptab_list[fd]->close_r(reent, fd));
}

/*
 * \brief _write_r
 *
 */
_ssize_t	_write_r(reent_t *reent, int fd, const void *buf, size_t cnt) {

	return (devoptab_list[fd]->write_r(reent, fd, buf, cnt));
}

/*
 * \brief _read_r
 *
 */
_ssize_t	_read_r(reent_t *reent, int fd, void *buf, size_t cnt) {

	return (devoptab_list[fd]->read_r(reent, fd, buf, cnt));
}

/*
 * \brief com1_open_r
 *
 */
int com1_open_r(reent_t *reent, const char_t *file, int flags, int mode) {

	return (0);
}

/*
 * \brief com1_close_r
 *
 */
int	com1_close_r(reent_t *reent, int fd) {

	return (0);
}

/*
 * \brief com1_write_r
 *
 */
#define	KLNSNDBUFF	80

void	outLine(uint32_t device, uint8_t *ascii, uint32_t size) {
	int32_t		status;

	while (TRUE) {
		status = comm_write(device, ascii, size);
		if (status != KCOMMNOERR) {
			kern_switchFast();
		}
		else {

// Give some time to allow the manager to send the data
// before sending another bloc

			kern_suspendProcess(1);
			return;
		}
	}
}

_ssize_t	com1_write_r(reent_t *reent, int fd, const void *buf, size_t cnt) {
				uint32_t	device, i, j;
	static		uint8_t 	ascii[KLNSNDBUFF+2];
	const		uint8_t 	*wkAscii;
	volatile	proc_t 		*process;

	kern_getProcessRun(&process);
	device = process->oSpecification.oCommManager;

	RESERVE_COMM(device, KDEVWRITE);
	wkAscii = (const uint8_t *)buf;
	comm_flush(device);

	j = 0;
	for (i = 0; i < cnt; i++) {
		ascii[j++] = *wkAscii++;
		if (j >= KLNSNDBUFF) {
			outLine(device, ascii, j);
			j = 0;
		}
	}

	if (j > 0) {
		outLine(device, ascii, j);
	}
	RELEASE_COMM(device, KDEVWRITE);

	return ((_ssize_t)cnt);
}

/*
 * \brief com1_read_r
 *
 */
char_t	inbyte(uint32_t device) {
	uint8_t		byte;
	int32_t		status;
	uint32_t	size;

	do {
		kern_switchFast();
		size = 1;
		status = comm_read(device, &byte, &size);
	} while (status != KCOMMNOERR);
	return ((char_t)byte);
}

_ssize_t	com1_read_r(reent_t *reent, int fd, void *buf, size_t cnt) {
				char_t		*ascii;
				int32_t 	i;
				uint32_t	device;
				bool_t		terminate = FALSE;
	volatile	proc_t 		*process;

	i = (int32_t)cnt;
	if (cnt > 0) {
		kern_getProcessRun(&process);
		device = process->oSpecification.oCommManager;

		RESERVE_COMM(device, KDEVREAD);
		ascii = (char_t *)buf;

		i = 0;
		do {
			*(ascii + i) = inbyte(device);
			if ((*(ascii + i) == '\n') || (*(ascii + i) == '\r')) {
				i++;
				terminate = TRUE;
			}
		} while ((++i < (int32_t)cnt) && (!terminate));
		RELEASE_COMM(device, KDEVREAD);

	}
	return ((_ssize_t)i);
}

