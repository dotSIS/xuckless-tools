# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = chadrw.c chadwm.c util.c
OBJ = ${SRC:.c=.o}

all: options chadwm

options:
	@echo chadwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

chadwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f chadwm ${OBJ} chadwm-${VERSION}.tar.gz

dist: clean
	mkdir -p chadwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		chadwm.1 chadrw.h util.h ${SRC} chadwm.png transient.c chadwm-${VERSION}
	tar -cf chadwm-${VERSION}.tar chadwm-${VERSION}
	gzip chadwm-${VERSION}.tar
	rm -rf chadwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f chadwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/chadwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < chadwm.1 > ${DESTDIR}${MANPREFIX}/man1/chadwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/chadwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/chadwm\
		${DESTDIR}${MANPREFIX}/man1/chadwm.1

.PHONY: all options clean dist install uninstall
