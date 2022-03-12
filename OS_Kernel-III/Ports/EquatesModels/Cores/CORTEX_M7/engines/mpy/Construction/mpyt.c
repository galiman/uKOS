/*
; mpyt.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		mpyt manager.
;			uKOS interface for Micro-Python (www.micropython.com).
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

#include	<string.h>
#include	<types.h>
#include 	<modules.h>
#include 	<ip.h>
#include 	<strg/strg.h>
#include 	<iotx/iotx.h>
#include 	<kern/kern.h>
#include 	<comm/comm.h>
#include 	<mpyt/mpyt.h>
#include	<py/nlr.h>
#include	<py/compile.h>
#include	<py/runtime.h>
#include	<py/repl.h>
#include	<py/gc.h>
#include	<py/mperrno.h>
#include	<lib/utils/pyexec.h>

#define	KSIZEINPUT	256							// Size of the MPYT input buffer

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"mpyt         Mycro-Python manager.                     (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "mpyt manager\n"
									"============\n\n"

									"This manager ...\n\n";

LOC_CONST_STRG(aStrTerminated[])  = "quit";

// Prototypes
// ==========

static	void	_init(void);
static	void	commandLine(const char_t *src, mp_parse_input_kind_t input_kind);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Mpyt, KIDENGI, KMPYTNUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief Configure the mpyt manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * cnfMpyt_t    configure;
 * 
 *    configure.oSize = 90000;
 *    configure.oMemory = (uint8_t *)syos_malloc(KINTERNAL, configure.oSize);
 *    if (configure.oMemory == 0) {
 *        exit(EXIT_FAILURE);
 *    }
 *
 *    status = mpyt_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KMPYTNOERR	OK
 *
 */
int32_t	mpyt_configure(cnfMpyt_t *configure) {

	_init();

	iotx_printf(KSYST, "\nuKOS interface for Micro-Python (www.micropython.com)\n");
	iotx_printf(KSYST, "%s\n\n", "Package 1.9 for uKOS-III");

// Initialize the MPTY session

    gc_init(configure->oMemory, configure->oMemory + configure->oSize);
    mp_init();

	return (KMPYTNOERR);
}

/*
 * \brief Exchange data with the mpyt manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = mpyt_exchangeData();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KMPYTNOERR	OK
 *
 */
int32_t	mpyt_exchangeData(void) {
				char_t		ascii[KSIZEINPUT];
				uint32_t	device, i, size;
				bool_t		terminated = FALSE;
	volatile	proc_t 		*process;

	_init();

	kern_getProcessRun(&process);
	device = process->oSpecification.oCommManager;

	while (!terminated) {
		iotx_waitString(device, ascii, (KSIZEINPUT-4));

		strg_getBufferSize(ascii, &size);
		ascii[size-1] = '\r'; ascii[size-0] = '\n'; ascii[size+1] = '\0';

// Check the "quit"

		terminated = TRUE;
		for (i = 0; i < (size-1); i++) {
			if (ascii[i] != aStrTerminated[i]) {
				terminated = FALSE;
				commandLine(ascii, MP_PARSE_SINGLE_INPUT);
				break;
			}
		}
	}
	iotx_printf(KSYST, "Terminated\n");
    mp_deinit();
	return (KMPYTNOERR);
}

// Local routines
// ==============

/*
 * \brief _init
 *
 * - This function initializes the manager and
 *   has to be called at least once.
 *
 */
static	void	_init(void) {
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;
	}
}

/*
 * \brief _commandLine
 *
 * \param[in]	*ascii		Ptr on the input line
 * \param[in]	input_kind	Input kind (MP_PARSE_SINGLE_INPUT, MP_PARSE_FILE_INPUT)
 * \param[out]	-
 *
 */
static	void	commandLine(const char_t *ascii, mp_parse_input_kind_t input_kind) {
	nlr_buf_t		nlr;
	mp_lexer_t		*lex;
	qstr			sourceName;
	mp_parse_tree_t	parseTree;
	mp_obj_t		moduleFun;

	if (nlr_push(&nlr) == 0) {
		lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, ascii, strlen(ascii), 0);
		sourceName = lex->source_name;
		parseTree = mp_parse(lex, input_kind);
		moduleFun = mp_compile(&parseTree, sourceName, MP_EMIT_OPT_NONE, TRUE);
		mp_call_function_0(moduleFun);
		nlr_pop();
	} else {
		mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
	}
}

/*
 * \brief mp_hal_stdout_tx_strn
 *
 * \param[in]	*ascii		Ptr on the input line
 * \param[in]	size		Size f the ascii
 * \param[out]	-
 *
 */
void	mp_hal_stdout_tx_strn(const uint8_t *ascii, mp_uint_t size) {
				int16_t		i;
				uint32_t	device;
	volatile	proc_t 		*process;

	kern_getProcessRun(&process);
	device = process->oSpecification.oCommManager;

	for (i = 0; i < size; i++) {
		iotx_printf(device, "%c", ascii[i]);
	}
}

/*
 * \brief mp_hal_stdin_rx_chr
 *
 * \param[in]	-
 * \param[out]	char		The read char
 *
 */
char_t	mp_hal_stdin_rx_chr(void) {
				uint8_t		byte;
				int32_t		status;
				uint32_t	size, device;
	volatile	proc_t 		*process;

	kern_getProcessRun(&process);
	device = process->oSpecification.oCommManager;

	do {
		kern_switchFast();
		size = 1;
		status = comm_read(device, &byte, &size);
	} while ((status != KCOMMNOERR) && (size != 1));
	return ((char_t)byte);
}

// Needed by the package
// ---------------------

void	gc_collect(void) {
//	void	*dummy;

	gc_collect_start();
//	gc_collect_root(&dummy, ((mp_uint_t)vStackTop - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
	gc_collect_end();
	gc_dump_info();
}

mp_lexer_t	*mp_lexer_new_from_file(const char_t *filename) {

	mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t	mp_import_stat(const char_t *path) {

	return (MP_IMPORT_STAT_NO_EXIST);
}

mp_obj_t	mp_builtin_open(uint n_args, const mp_obj_t *args, mp_map_t *kwargs) {

	return (mp_const_none);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void	nlr_jump_fail(void *val) {

	while (TRUE);
}

void NORETURN __fatal_error(const char_t *msg) {

	while (TRUE);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {

	iotx_printf(KSYST, "Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed");
}
#endif
