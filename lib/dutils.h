/*
 * dutils.h
 * This file is part of DUtils
 *
 * Copyright (C) 2015 - 2017 - Marc Volker Dickmann
 *
 * DUtils is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * DUtils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DUtils. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __DUTILS_H__
#define __DUTILS_H__

/* DUTILS STUFF */

#define DUTILS_VERSION "0.0.3 A"

/* BASH COLORS */

#define BC_TXT_BLACK "\033[0;30m"	/* Black */
#define BC_TXT_RED "\033[0;31m"		/* Red */
#define BC_TXT_GREEN "\033[0;32m"	/* Green */
#define BC_TXT_YELLOW "\033[0;33m"	/* Yellow */
#define BC_TXT_BLUE "\033[0;34m"	/* Blue */
#define BC_TXT_PURPLE "\033[0;35m"	/* Purple */
#define BC_TXT_CYAN "\033[0;36m"	/* Cyan */
#define BC_TXT_WHITE "\033[0;37m"	/* White */

#define BC_TXT_HGH_BLACK "\033[0;90m"	/* Black - High */
#define BC_TXT_HGH_RED "\033[0;91m"	/* Red */
#define BC_TXT_HGH_GREEN "\033[0;92m"	/* Green */
#define BC_TXT_HGH_YELLOW "\033[0;93m"	/* Yellow */
#define BC_TXT_HGH_BLUE "\033[0;94m"	/* Blue */
#define BC_TXT_HGH_PURPLE "\033[0;95m"	/* Purple */
#define BC_TXT_HGH_CYAN "\033[0;96m"	/* Cyan */
#define BC_TXT_HGH_WHITE "\033[0;97m"	/* White */

#define BC_BLD_BLACK "\033[1;30m"	/* Black - Bold */
#define BC_BLD_RED "\033[1;31m"		/* Red */
#define BC_BLD_GREEN "\033[1;32m"	/* Green */
#define BC_BLD_YELLOW "\033[1;33m"	/* Yellow */
#define BC_BLD_BLUE "\033[1;34m"	/* Blue */
#define BC_BLD_PURPLE "\033[1;35m"	/* Purple */
#define BC_BLD_CYAN "\033[1;36m"	/* Cyan */
#define BC_BLD_WHITE "\033[1;37m"	/* White */

#define BC_BLD_HGH_BLACK "\033[1;90m"	/* Black - Bold - High */
#define BC_BLD_HGH_RED "\033[1;91m"	/* Red */
#define BC_BLD_HGH_GREEN "\033[1;92m"	/* Green */
#define BC_BLD_HGH_YELLOW "\033[1;93m"	/* Yellow */
#define BC_BLD_HGH_BLUE "\033[1;94m"	/* Blue */
#define BC_BLD_HGH_PURPLE "\033[1;95m"	/* Purple */
#define BC_BLD_HGH_CYAN "\033[1;96m"	/* Cyan */
#define BC_BLD_HGH_WHITE "\033[1;97m"	/* White */

#define BC_UND_BLACK "\033[4;30m"	/* Black - Underline */
#define BC_UND_RED "\033[4;31m"		/* Red */
#define BC_UND_GREEN "\033[4;32m"	/* Green */
#define BC_UND_YELLOW "\033[4;33m"	/* Yellow */
#define BC_UND_BLUE "\033[4;34m"	/* Blue */
#define BC_UND_PURPLE "\033[4;35m"	/* Purple */
#define BC_UND_CYAN "\033[4;36m"	/* Cyan */
#define BC_UND_WHITE "\033[4;37m"	/* White */

#define BC_BAK_BLACK "\033[40m"		/* Black - Background */
#define BC_BAK_RED "\033[41m"		/* Red */
#define BC_BAK_GREEN "\033[42m"		/* Green */
#define BC_BAK_YELLOW "\033[43m"	/* Yellow */
#define BC_BAK_BLUE "\033[44m"		/* Blue */
#define BC_BAK_PURPLE "\033[45m"	/* Purple */
#define BC_BAK_CYAN "\033[46m"		/* Cyan */
#define BC_BAK_WHITE "\033[47m"		/* White */

#define BC_BAK_HGH_BLACK "\033[0;100m"	/* Black - Background - High */
#define BC_BAK_HGH_RED "\033[0;101m"	/* Red */
#define BC_BAK_HGH_GREEN "\033[0;102m"	/* Green */
#define BC_BAK_HGH_YELLOW "\033[0;103m"	/* Yellow */
#define BC_BAK_HGH_BLUE "\033[0;104m"	/* Blue */
#define BC_BAK_HGH_PURPLE "\033[0;105m"	/* Purple */
#define BC_BAK_HGH_CYAN "\033[0;106m"	/* Cyan */
#define BC_BAK_HGH_WHITE "\033[0;107m"	/* White */

#define BC_TXT_UND "\033[4m"		/* Text Underline */
#define BC_TXT_RST "\033[0m"		/* Text Reset */

/* BITS */

#define EMBIANCONV(a) ((((a) >> 24) &0xff) | (((a) << 8) & 0xff0000) | (((a) >> 8) & 0xff00) | (((a) << 24) & 0xff000000))

#define BITCOPY(a,b,c,d) ((-(((a) >> (b)) & 1) ^ (c)) & (1 << (d)))
#define BITGET(a,b) (((a) >> (b)) & 1)

/* BOOL */
#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

/* CHARTOINT */

#define CHARTOINT(a) ((a) - (48))

#endif /* __DUTILS_H__ */
