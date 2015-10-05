CC=gcc
LIB_FLAGS=-fPIC -shared
OPT_FLAGS=-g -O3 -Wall -std=gnu99

all: clean measure-cyg run

timing:
	$(CC) $(OPT_FLAGS) $(LIB_FLAGS) src/libtiming_tsc/timing.c -o lib/libtiming_tsc.so

libshadowstack-serial libshadowstack-parallel:
	$(CC) $(PP_FLAGS) $(OPT_FLAGS) $(LIB_FLAGS) -o lib/lib$(LIBNAME).$(HOSTNAME).so src/driver.c $(LDFLAGS)
	$(CC) -std=gnu99 $(OPT_FLAGS) -finstrument-functions target.c -L./lib -l$(LIBNAME).$(HOSTNAME) -o target.$(LIBNAME).$(HOSTNAME).exe


libshadowstack-serial: PP_FLAGS+=-DSERIAL_OPT
libshadowstack-serial: LIBNAME=shadowstack.serial

libshadowstack-parallel: LIBNAME=shadowstack.parallel

measure-cyg: LDFLAGS+=-L./lib -ltiming_tsc
measure-cyg: timing libshadowstack-parallel libshadowstack-serial
	ln -s target.shadowstack.parallel.$(HOSTNAME).exe target.exe

run:
	readlink target.exe
	./target.exe

clean:
	rm -f lib/*.so lib/*.a
	rm -f *.exe
