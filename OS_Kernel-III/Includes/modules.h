/*
; modules.
; ========

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
; Goal:		Module IDs for uKOS systems.
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

#ifndef	__MODULES__
#define	__MODULES__

// Specification module macro
// --------------------------

#ifdef	__cplusplus
#define	MODULE(name, family, idModule, execution, revision, flag) \
		extern	"C"	const	module_t	a##name##_Specifications = { \
											((family<<24) | (idModule<<8) | '_'), \
											aStrApplication, \
											aStrHelp, \
											(int32_t (*)(uint32_t argc, const char_t *argv[]))execution, \
											revision, \
											flag \
										}

#else
#define	MODULE(name, family, idModule, execution, revision, flag) \
					const	module_t	a##name##_Specifications = { \
											((family<<24) | (idModule<<8) | '_'), \
											aStrApplication, \
											aStrHelp, \
											(int32_t (*)(uint32_t argc, const char_t *argv[]))execution, \
											revision, \
											flag \
										}
#endif

// Header, directory and module structures
// ---------------------------------------

typedef	struct	header		header_t;
typedef	struct	directory	directory_t;
typedef	struct	module		module_t;

struct header {
			uint32_t	oIdModule;											// Module identifier
			int32_t		(*go)(void);										// Ptr on the restart module (_start)
			uint32_t	oLnApplication;										// Length of the application
	const	module_t	*oModule;											// Ptr on the module
};

struct directory {
			uint8_t		oWhere;												// Module location
			#define		KNOMODULE		0									// KNOMODULE = no more modules
			#define		KBUILDIN		1									// KBUILDIN = module built in
			#define		KSERIALEEPROM	2									// KSERIALEEPROM = module built in the serial EEPROM

	const	module_t	*oModule;											// Ptr on the module
};

struct module {
			uint32_t	oIdModule;											// Module identifier
	const	char_t		*oStrApplication;									// Ptr on the application string
	const	char_t		*oStrHelp;											// Ptr on the help string
			int32_t		(*oExecution)(uint32_t argc, const char_t *argv[]);	// Ptr on the execution code
	const	char_t		*oStrRevision;										// Program Revision
			uint8_t		oFlag;												// Module flag
			#define		BSHOW			0									// Visible in help
			#define		BEXECONSOLE		1									// Executable from the Console
			#define		BCONFIDENTIAL	2									// No comment
};

// ID of the library families
// --------------------------

#define	KIDKERN			'K'													// "K x x _" ---> Kern
#define	KIDTBOX			'B'													// "B x x _" ---> TBox
#define	KIDGENE			'L'													// "L x x _" ---> Gene
#define	KIDPERI			'I'													// "I x x _" ---> Peri
#define	KIDCOMM			'C'													// "C x x _" ---> Comm
#define	KIDENGI			'E'													// "E x x _" ---> Engi
#define	KIDCOMPANY		'F'													// "F x x _" ---> Company
#define	KIDPROTOCOL		'P'													// "P x x _" ---> protocol code
#define	KIDTOOL			'X'													// "X x x _" ---> tool code
#define	KIDSTARTUP		'S'													// "S x x _" ---> Start-up code
#define	KIDPROCESS		'T'													// "T x x _" ---> Process
#define	KIDOVERLAY		'O'													// "O x x _" ---> Overlay process
#define	KIDNLIB			'N'													// "N x x _" ---> Nlib
#define	KIDSHAR			'H'													// "H x x _" ---> Shar
#define	KIDWRAPPER		'W'													// "W x x _" ---> Wrapper
#define	KIDAPPLICATION	'A'													// "A x x _" ---> stand-alone executable code

// LibKern

#define	KKERNNUM		(('0'<<8)+'0')										// kern manager

// LibTBox

#define	KGLOBNUM		(('0'<<8)+'0')										// glob manager
#define	KMISCNUM		(('0'<<8)+'1')										// misc manager
#define	KSTRGNUM		(('0'<<8)+'2')										// strg manager

// LibGene

#define	KSYOSNUM		(('0'<<8)+'0')										// syos manager
#define	KIOTXNUM		(('0'<<8)+'1')										// iotx manager
#define	KTEXTNUM		(('0'<<8)+'2')										// text manager
#define	KUSDCNUM		(('0'<<8)+'3')										// usdc manager
#define	KXFERNUM		(('0'<<8)+'4')										// xfer manager
#define	KJPEGNUM		(('0'<<8)+'5')										// jpeg manager

// LibPeri

#define	KVIS0NUM		(('0'<<8)+'0')										// vis0 manager
#define	KPRT0NUM		(('0'<<8)+'1')										// prt0 manager
#define	KADC0NUM		(('0'<<8)+'2')										// adc0 manager
#define	KIMG0NUM		(('0'<<8)+'3')										// img0 manager
#define	KTMP0NUM		(('0'<<8)+'4')										// tmp0 manager
#define	KI2C0NUM		(('0'<<8)+'5')										// i2c0 manager
#define	KI2C1NUM		(('0'<<8)+'6')										// i2c1 manager
#define	KIMU0NUM		(('0'<<8)+'7')										// imu0 manager
#define	KIMS2NUM		(('0'<<8)+'8')										// ims2 manager
#define	KOLE0NUM		(('0'<<8)+'9')										// ole0 manager

// LibComm

#define	KCOMMNUM		(('0'<<8)+'0')										// comm manager
#define	KUSB0NUM		(('0'<<8)+'1')										// usb0 manager
#define	KUSB1NUM		(('0'<<8)+'2')										// usb1 manager
#define	KURT0NUM		(('0'<<8)+'3')										// urt0 manager
#define	KURT1NUM		(('0'<<8)+'4')										// urt1 manager
#define	KURT2NUM		(('0'<<8)+'5')										// urt2 manager
#define	KURT3NUM		(('0'<<8)+'6')										// urt3 manager
#define	KURT4NUM		(('0'<<8)+'7')										// urt4 manager
#define	KBLT0NUM		(('0'<<8)+'8')										// blt0 manager

// LibEngi

#define	KSQEENUM		(('0'<<8)+'0')										// sqee manager
#define	KMPYTNUM		(('0'<<8)+'1')										// mpyt manager

// Protocols

#define	KANUM			(('A'<<8)+'0')										// A module
#define	KBNUM			(('B'<<8)+'0')										// B module
#define	KLNUM			(('L'<<8)+'0')										// L module
#define	KVNUM			(('V'<<8)+'0')										// V module
#define	KRNUM			(('R'<<8)+'0')										// R module
#define	KWNUM			(('W'<<8)+'0')										// W module
#define	KXNUM			(('X'<<8)+'0')										// X module (demo)

// Tools

#define	KBANGLANUM		(('B'<<8)+'A')										// bangla module
#define	KBENCHNUM		(('B'<<8)+'E')										// bench module
#define	KBINFILLNUM		(('B'<<8)+'F')										// binfill module
#define	KCONSOLENUM		(('C'<<8)+'O')										// console module
#define	KCYCLENUM		(('C'<<8)+'Y')										// cycle module
#define	KDUMPNUM		(('D'<<8)+'P')										// dump module
#define	KECHONUM		(('E'<<8)+'C')										// echo module
#define	KFILLNUM		(('F'<<8)+'I')										// fill module
#define	KHEXLOADERNUM	(('H'<<8)+'L')										// hexloader module
#define	KJTAGNUM		(('J'<<8)+'T')										// jtag module
#define	KKILLNUM		(('K'<<8)+'I')										// kill module
#define	KLISTNUM		(('L'<<8)+'S')										// list module
#define	KMANNUM			(('M'<<8)+'N')										// man module
#define	KMEMORYNUM		(('M'<<8)+'E')										// memory module
#define	KMEMXNUM		(('M'<<8)+'X')										// memx module
#define	KPDP11LOADERNUM	(('P'<<8)+'L')										// pdp11loader module
#define	KPROCESSNUM		(('P'<<8)+'R')										// process module
#define	KROMABLE0		(('R'<<8)+'0')										// romable module 0
#define	KROMABLE1		(('R'<<8)+'1')										// romable module 1
#define	KROMABLE2		(('R'<<8)+'2')										// romable module 2
#define	KROMABLE3		(('R'<<8)+'3')										// romable module 3
#define	KRESTARTNUM		(('R'<<8)+'Z')										// restart module
#define	KRUNNUM			(('R'<<8)+'N')										// run module
#define	KSLOADERNUM		(('S'<<8)+'L')										// sloader module
#define	KSQENUM			(('S'<<8)+'Q')										// sqe module
#define	KMPYNUM			(('M'<<8)+'P')										// mpy module
#define	KTESTNUM		(('T'<<8)+'S')										// test module
#define	KUART0NUM		(('U'<<8)+'0')										// uart0 module
#define	KUART1NUM		(('U'<<8)+'1')										// uart1 module
#define	KUART2NUM		(('U'<<8)+'2')										// uart2 module
#define	KUART3NUM		(('U'<<8)+'3')										// uart3 module
#define	KUKOSNUM		(('U'<<8)+'K')										// uKOS module
#define	KVIEWERNUM		(('V'<<8)+'i')										// viewer module
#define	KFLASHNUM		(('F'<<8)+'P')										// flash management

// startup

#define	KCRT0NUM		(('C'<<8)+'0')										// crt0 module
#define	KINITNUM		(('I'<<8)+'N')										// init module
#define	KEXCENUM		(('E'<<8)+'X')										// exce module
#define	KBOOTNUM		(('B'<<8)+'O')										// boot module
#define	KCMNSNUM		(('C'<<8)+'M')										// cmns module

// Processes

#define	KALIVENUM		(('A'<<8)+'L')										// alive module
#define	KSTARTUPNUM		(('S'<<8)+'T')										// startup module
#define	KIMAGERNUM		(('I'<<8)+'M')										// imager module
#define	KTEMPERATURENUM	(('X'<<8)+'T')										// temperature module (demo) 
#define	KEEPROMNUM		(('E'<<8)+'E')										// eeprom module (test)

// Overlays

#define	KPACK0NUM		(('P'<<8)+'0')										// pack0 module
#define	KPACK1NUM		(('P'<<8)+'1')										// pack1 module

// Newlib

#define	KNEWLIBNUM		(('0'<<8)+'0')										// newlib module

// Shared

#define	KSPI2NUM		(('0'<<8)+'0')										// spi2 shared module
#define	KSPI4NUM		(('0'<<8)+'1')										// spi4 shared module
#define	KSPI5NUM		(('0'<<8)+'2')										// spi5 shared module

// Wrapper

#define	KCMSISNUM		(('C'<<8)+'M')										// CMSIS wrapper module

// Applications

#define	KAPPLICATIONNUM	(('U'<<8)+'A')										// user application module
#endif
