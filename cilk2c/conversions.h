/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 * 
 *************************************************************************/
/*
 * Copyright (c) 1994-2002 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _CONVERSIONS_H_
#define _CONVERSIONS_H_
FILE_IDENTITY(conversions_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/conversions.h $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");


GLOBAL Node *UsualUnaryConversions(Node *node, Bool f_to_d);
GLOBAL Node *UsualUnaryConversionType(Node *type);
GLOBAL void UsualBinaryConversions(Node **node1p, Node **node2p);
GLOBAL void UsualPointerConversions(Node **node1p, Node **node2p, Bool allow_void_or_zero);
GLOBAL Node *AssignmentConversions(Node *expr, Node *to_type);
GLOBAL Node *CallConversions(Node *expr, Node *to_type);
GLOBAL Node *ReturnConversions(Node *expr, Node *to_type);
GLOBAL Node *CastConversions(Node *expr, Node *to_type);
GLOBAL Node *ConditionalConversions(Node **truep, Node **falsep);

#endif				/* _CONVERSIONS_H_ */
