# Time-stamp: <05/09/09 21:10:32 ptr>
# $Id: sys.mak,v 1.1 2007-01-31 09:10:40 hogepiyo Exp $

INSTALL := /usr/ucb/install

INSTALL_SO := ${INSTALL} -c -m 0755
INSTALL_A := ${INSTALL} -c -m 0644
INSTALL_EXE := ${INSTALL} -c -m 0755

EXT_TEST := /usr/bin/test