#
# Copyright (c) 2001, Sasha Vasko <sasha at aftercode.net>
# Copyright (c) 1998, Guylhem AZNAR <guylhem@oeil.qc.ca>
#

subdirs = libjpeg libpng libungif zlib

LIBJPEG_OBJS = 	libjpeg/jcapimin.o libjpeg/jcapistd.o libjpeg/jccoefct.o \
		libjpeg/jccolor.o libjpeg/jcdctmgr.o libjpeg/jchuff.o \
		libjpeg/jcinit.o libjpeg/jcmainct.o libjpeg/jcmarker.o \
		libjpeg/jcmaster.o libjpeg/jcomapi.o libjpeg/jcparam.o \
		libjpeg/jcphuff.o libjpeg/jcprepct.o libjpeg/jcsample.o \
		libjpeg/jctrans.o libjpeg/jdapimin.o libjpeg/jdapistd.o \
		libjpeg/jdatadst.o libjpeg/jdatasrc.o libjpeg/jdcoefct.o \
		libjpeg/jdcolor.o libjpeg/jddctmgr.o libjpeg/jdhuff.o \
		libjpeg/jdinput.o libjpeg/jdmainct.o libjpeg/jdmarker.o \
		libjpeg/jdmaster.o libjpeg/jdmerge.o libjpeg/jdphuff.o \
		libjpeg/jdpostct.o libjpeg/jdsample.o libjpeg/jdtrans.o \
		libjpeg/jerror.o libjpeg/jfdctflt.o libjpeg/jfdctfst.o \
		libjpeg/jfdctint.o libjpeg/jidctflt.o libjpeg/jidctfst.o \
		libjpeg/jidctint.o libjpeg/jidctred.o libjpeg/jquant1.o \
		libjpeg/jquant2.o libjpeg/jutils.o libjpeg/jmemmgr.o libjpeg/jmemnobs.o

LIBPNG_OBJS = 	libpng/png.o libpng/pngset.o libpng/pngget.o libpng/pngrutil.o \
		libpng/pngtrans.o libpng/pngwutil.o libpng/pngread.o libpng/pngrio.o \
		libpng/pngwio.o libpng/pngwrite.o libpng/pngrtran.o \
		libpng/pngwtran.o libpng/pngmem.o libpng/pngerror.o \
		libpng/pngpread.o libpng/pnggccrd.o

ZLIB_OBJS = 	zlib/adler32.o 	zlib/compress.o zlib/crc32.o \
		zlib/gzio.o 	zlib/uncompr.o 	zlib/deflate.o \
		zlib/trees.o 	zlib/zutil.o 	zlib/inflate.o \
		zlib/infback.o 	zlib/inftrees.o zlib/inffast.o

LIBUNGIF_OBJS = libungif/dgif_lib.o libungif/egif_lib.o libungif/gifalloc.o libungif/gif_err.o

AFTERIMAGE_OBJS= afterbase.o asimage.o ascmap.o asfont.o asimagexml.o asstorage.o \
		asvisual.o blender.o bmp.o char2uni.o draw.o export.o imencdec.o import.o \
		pixmap.o transform.o ungif.o xcf.o ximage.o xpm.o

################################################################
# library specifics :

LIB_INCS= afterimage.h config.h afterbase.h ascmap.h asfont.h asim_afterbase.h \
		asimage.h asimagexml.h asstorage.h asvisual.h blender.h bmp.h char2uni.h \
		draw.h export.h imencdec.h import.h pixmap.h transform.h ungif.h \
		xcf.h ximage.h xpm.h

LIB_OBJS=          $(LIBUNGIF_OBJS) $(AFTERIMAGE_OBJS)

# end specifics
################################################################
# generic makefile stuff :

LIB_DIR_NAME    = libAfterImage
LIB_NAME        = LIBAFTERIMAGE
LIB_STATIC      = libAfterImage.a
LIB_SHARED      = libAfterImage.so
LIB_SHARED_ROOT = libRAfterImage.so
LIB_SHARED_CYG  = cygAfterImage.dll
LIB_SHARED_CYG_AR  = libAfterImage.dll.a
LIBVER          = 0.99
LIBVERMAJOR     = 0

CC		= gcc -m64
CCFLAGS         = -O3 -DNO_DEBUG_OUTPUT -fPIC
# -march=pentiumpro -mcpu=pentiumpro
EXTRA_DEFINES	= 

RANLIB		= ranlib
AR		= ar clq
CP		= /usr/bin/cp
MV		= /usr/bin/mv
RM		= /usr/bin/rm
RMF		= /usr/bin/rm -f
MKDIR		= /usr/bin/mkdir
FIND		= /usr/bin/find
XARGS		= /usr/bin/xargs
LDCONFIG	= /sbin/ldconfig
ROBODOC     = robodoc
LN_S		= ln -s

YACC		= 
LEX		= 
YACCFLAGS	= -d
LEXFLAGS	=

INSTALL		= /usr/bin/install -c
INSTALL_PROGRAM	= /usr/bin/install -c -s -m 755
INSTALL_DATA	= /usr/bin/install -c -m 644
INSTALL_HEADER	= /usr/bin/install -c -m 644
INSTALL_LIB	= /usr/bin/install -c -m 755
INSTALL_SCRIPT	= /usr/bin/install -c -m 755

INCS_EXTRA	=      -I../../../freetype/src/freetype-2.1.9/include
INCS_X		= 
INCS_PRIVATE    = 
INCLUDES	= $(INCS_EXTRA) $(INCS_PRIVATE) $(INCS_X)

USER_LD_FLAGS	= 
LIBS_X		= -lX11    -lSM -lICE -lXext
LIBS_XEXTS	=
LIBS_TEST	= -L./ -lAfterImage -lfreetype -ltiff -lX11    -lSM -lICE -lXext -ljpeg -lpng -lz -lm
LIBS_AFTERIMAGE = -L./ -lfreetype -ltiff -lX11    -lSM -lICE -lXext -ljpeg -lpng -lz -lm
LIBRARIES	= $(LIBS_AFTERIMAGE) $(LIBS_X) $(LIBS_XEXTS)
LIBRARIES_TEST	= $(LIBS_TEST) $(LIBS_X) $(LIBS_XEXTS)

prefix          = /usr/local
exec_prefix     = /usr/local
LIBDIR          = $(DESTDIR)/usr/local/lib
INCLUDEDIR      = $(DESTDIR)/usr/local/include
AFTER_INC_DIR	= $(DESTDIR)/usr/local/include/$(LIB_DIR_NAME)
AFTER_BIN_DIR	= $(DESTDIR)${exec_prefix}/bin
AFTER_MAN_DIR	= $(DESTDIR)/usr/local/man/man3
AFTER_APP_MAN_DIR	= $(DESTDIR)/usr/local/man/man1
AFTER_SHAREDIR	= $(DESTDIR)${prefix}/share/$(LIB_DIR_NAME)
AFTER_DOC_DIR	= $(AFTER_SHAREDIR)/doc

# We want to build both static and dynamic libs, as some service apps may need 
# static library as they gets run at compile time (ASDocGen for example)
# but we only install whats selected
all:            $(LIB_STATIC) $(LIB_STATIC)

install.bin:

install.lib:	$(LIB_STATIC) install.static install.inc

install.static:
		$(INSTALL_SCRIPT) afterimage-libs afterimage-config $(AFTER_BIN_DIR)
		@(if [ -d $(LIBDIR) ] && [ -w $(LIBDIR) ]; then \
		    echo "$(INSTALL_LIB) $(LIB_STATIC) $(LIBDIR)"; \
		    $(INSTALL_LIB) $(LIB_STATIC) $(LIBDIR); \
		    if [ `uname` = "Linux" ]; then \
			if test $(LIBDIR) = "/lib" || test $(LIBDIR) = "/usr/lib"; then \
		    	    echo "" > /dev/null; \
			elif grep -q $(LIBDIR) /etc/ld.so.conf > /dev/null 2>&1; then \
		    	    echo "" > /dev/null; \
			else \
		    	    echo "Unable to find $(LIBDIR) in ld.so.conf. In order to use "; \
		    	    echo "$(LIB_STATIC), you may need to add it or set LD_LIBRARY_PATH."; \
			fi; \
			if test -w /etc; then \
			    echo "$(LDCONFIG)"; \
			    $(LDCONFIG); \
			fi; \
		    fi ;\
		fi \
		)

install.script:

install.dyn:
		@( echo "$(INSTALL_LIB) $(LIB_SHARED).$(LIBVER) $(LIBDIR)"; \
		$(INSTALL_LIB) $(LIB_SHARED).$(LIBVER) $(LIBDIR); \
		$(INSTALL_SCRIPT) afterimage-libs afterimage-config $(AFTER_BIN_DIR); \
		$(RM) -f $(LIBDIR)/$(LIB_SHARED).$(LIBVERMAJOR) $(LIBDIR)/$(LIB_SHARED); \
		$(LN_S) -f $(LIB_SHARED).$(LIBVER) $(LIBDIR)/$(LIB_SHARED).$(LIBVERMAJOR); \
		$(LN_S) -f $(LIB_SHARED).$(LIBVERMAJOR) $(LIBDIR)/$(LIB_SHARED); \
		if test `uname` = "Linux"; then \
		   if test $(LIBDIR) = "/lib" || test $(LIBDIR) = "/usr/lib"; then \
		     echo "" > /dev/null; \
		   elif grep -q $(LIBDIR) /etc/ld.so.conf > /dev/null 2>&1; then \
		     echo "" > /dev/null; \
		   else \
		     echo "Unable to find $(LIBDIR) in ld.so.conf. In order to use "; \
		     echo "$(LIB_SHARED), you may need to add it or set LD_LIBRARY_PATH."; \
		   fi; \
		   echo "$(LDCONFIG)"; \
		   $(LDCONFIG); \
		 fi \
		)

install.cyg:
		@( echo "$(INSTALL_LIB) $(LIB_SHARED_CYG) $(AFTER_BIN_DIR)"; \
		$(INSTALL_LIB) $(LIB_SHARED_CYG) $(AFTER_BIN_DIR); \
		echo "$(INSTALL_LIB) $(LIB_SHARED_CYG) $(LIBDIR)"; \
		$(INSTALL_LIB) $(LIB_SHARED_CYG) $(LIBDIR); \
		echo "$(INSTALL_LIB) $(LIB_SHARED_CYG_AR) $(LIBDIR)"; \
		$(INSTALL_LIB) $(LIB_SHARED_CYG_AR) $(LIBDIR); \
		echo "$(INSTALL_SCRIPT) afterimage-libs afterimage-config $(AFTER_BIN_DIR)"; \
		$(INSTALL_SCRIPT) afterimage-libs afterimage-config $(AFTER_BIN_DIR); \
		)

install.inc:
		@(if [ -d $(AFTER_INC_DIR) ]; then \
		    echo "$(RM) $(AFTER_INC_DIR)/*"; \
		    $(RM) $(AFTER_INC_DIR)/*; \
		 else \
		    if [ -d $(INCLUDEDIR) ]; then \
		       echo "$(MKDIR) $(AFTER_INC_DIR)"; \
		       if $(MKDIR) $(AFTER_INC_DIR); then \
		          echo " ">/dev/null; \
		       else \
		    	  echo "failed to create include directory: $(AFTER_INC_DIR)"; \
		       fi; \
		    else \
		       echo "$(MKDIR) $(INCLUDEDIR)"; \
		       if $(MKDIR) $(INCLUDEDIR); then \
		          echo "$(MKDIR) $(AFTER_INC_DIR)"; \
		          if $(MKDIR) $(AFTER_INC_DIR) >/dev/null; then \
		             echo " ">/dev/null; \
		          else \
		             echo "failed to create include directory: $(AFTER_INC_DIR)"; \
			  fi; \
		       else \
		          echo "failed to create include directory: $(INCLUDEDIR)"; \
		       fi; \
		    fi; \
		 fi; \
		 if [ -d $(AFTER_INC_DIR) ]; then \
		    for file in $(LIB_INCS) ; do  \
			echo "$(INSTALL_HEADER) $$file $(AFTER_INC_DIR)"; \
			$(INSTALL_HEADER) $$file $(AFTER_INC_DIR); \
		    done; \
		 fi \
		)

uninstall.bin:

uninstall.lib:

uninstall.man:

uninstall.script:

clean:
		$(RMF) $(LIB_SHARED) $(LIB_SHARED_CYG) $(LIB_SHARED_CYG_AR) $(LIB_STATIC) *.so.* *.so *.o *~ *% *.bak \#* core ; \
		for I in ${subdirs}; do $(RMF) $$I/*.o $$I/*.obj $$I/*.bak; done

distclean: clean
		$(RMF) $(LIB_SHARED) $(LIB_SHARED_CYG) $(LIB_SHARED_CYG_AR) $(LIB_STATIC)  *.o *.so.* *~ *% *.bak \#* *.orig core Makefile

indent:
		@SRCS=`echo "$(AFTERIMAGE_OBJS) " | sed "s/.o /.c /g"`; \
		if test "x$$SRCS" == "x"; then exit; fi; \
		for i in $$SRCS; do \
		  if (indent -di14 -ts4 -i4 -l120 -lc80 -bad -nbbb -bli0 -c48 -cd48 -ce -cli1 -ncs -nbs -nbbo -hnl < $$i > /tmp/$$i); then \
		    echo "indent $$i"; \
		    mv /tmp/$$i $$i; \
		  fi; \
		done

deps:
		echo -n > .depend ; \
		buf="" ; \
		SRCS=`echo "$(AFTERIMAGE_OBJS) " | sed "s/.o /.c /g"`; \
		echo -n $(LIB_NAME)_OBJS = > .shared ; \
		for file in $$SRCS; do \
		  echo " \\" >> .shared ; \
		  echo -n '		$$('$(LIB_NAME)_PATH')'/$$file >> .shared ; \
		  if test "x$$buf" != "x"; then \
		      echo $$buf >>.depend ; \
		      echo >>.depend ; \
		  fi; \
		  buf="./"`echo "$$file " | sed "s/.c /.o /g"`: ; \
		  for d in `grep "#include \"" < $$file | awk -F \" '{print $$2}'`; do \
		      if test "x$$buf" != "x"; then \
		         echo $$buf \\ >>.depend ; \
		      fi; \
		      echo -n "		" >>.depend ; \
		      buf="$$d "; \
		  done; \
		done; \
		if test "x$$buf" != "x"; then \
		  echo $$buf >>.depend ; \
		fi; \
		echo "">> .shared ; \
		echo "">> .shared ; \
		echo -n $(LIB_NAME)_INCS = >> .shared ; \
		for f in $(LIB_INCS); do \
		  echo " \\" >> .shared ; \
		  echo -n '		$$('$(LIB_NAME)_PATH')'/$$f >> .shared ; \
		done; \
		echo "" >> .shared

$(LIB_STATIC):	$(LIB_OBJS) $(LIB_INCS) config.h
		$(RMF) $(LIB_STATIC)
		$(AR) $(LIB_STATIC) $(LIB_OBJS)
		$(RANLIB) $(LIB_STATIC)

test_asstorage.o: test_asstorage.c
		$(CC) $(CCFLAGS) $(EXTRA_DEFINES) -DTEST_ASSTORAGE $(INCLUDES) $(EXTRA_INCLUDES) -c asstorage.c -o test_asstorage.o

test_asstorage:	test_asstorage.o
		$(CC) test_asstorage.o $(USER_LD_FLAGS)  $(LIBRARIES_TEST) $(EXTRA_LIBRARIES) -o test_asstorage

test_asdraw.o:	draw.c
		$(CC) $(CCFLAGS) $(EXTRA_DEFINES) -DTEST_ASDRAW $(INCLUDES) $(EXTRA_INCLUDES) -c draw.c -o test_asdraw.o

test_asdraw:	test_asdraw.o
		$(CC) test_asdraw.o $(USER_LD_FLAGS) $(LIBRARIES_TEST) $(EXTRA_LIBRARIES) -o test_asdraw



.c.o:
		$(CC) $(CCFLAGS) $(EXTRA_DEFINES) $(INCLUDES) $(EXTRA_INCLUDES) -c $*.c -o $@

.c.s:
		$(CC) $(CCFLAGS) $(EXTRA_DEFINES) $(INCLUDES) $(EXTRA_INCLUDES) -S $*.c

#
# include dependency files if they exist
#
# this is merely a precaution, but as it does not work with ansi make
# we took it out
#ifneq ($(wildcard .depend),)
include .depend
#endif

$(LIB_SHARED_CYG): $(LIB_OBJS) $(LIB_INCS)
	$(CC) -shared -o $(LIB_SHARED_CYG) \
    		-Wl,--out-implib=$(LIB_SHARED_CYG_AR) \
    		-Wl,--export-all-symbols -Wl,--enable-auto-import \
    		-Wl,--whole-archive ${LIB_OBJS} \
    		$(USER_LD_FLAGS) -Wl,--no-whole-archive $(LIBRARIES)

$(LIB_SHARED): $(LIB_SHARED).$(LIBVERMAJOR)
	$(LN_S) -f $(LIB_SHARED).$(LIBVERMAJOR) $(LIB_SHARED)

$(LIB_SHARED).$(LIBVERMAJOR): $(LIB_SHARED).$(LIBVER)
	$(LN_S) -f $(LIB_SHARED).$(LIBVER) $(LIB_SHARED).$(LIBVERMAJOR)

$(LIB_SHARED).$(LIBVER): $(LIB_OBJS) $(LIB_INCS) config.h
	$(CC) -shared -Wl,-soname,$(LIB_SHARED_ROOT) -o $(LIB_SHARED).$(LIBVER) \
	 $(LIB_OBJS)

install.man:
		@if [ -d doc/man ] ; then \
			cd doc/man; \
			for f in `ls *.man`; do \
				page_name=`echo $$f| awk -F . '{print $$1}'`; \
	  			echo $(INSTALL_DATA) $$page_name.man $(AFTER_MAN_DIR)/$$page_name.1x; \
	  			$(INSTALL_DATA) $$page_name.man $(AFTER_MAN_DIR)/$$page_name.1x; \
			done; cd ../..; \
		fi
			

install.doc:
		@if [ -d doc/html ] ; then \
			if [ ! -d $(AFTER_DOC_DIR)/html ] ; then \
				$(MKDIR) -p $(AFTER_DOC_DIR)/html; \
			fi; \
			cd doc/html; \
			for f in `ls *.html`; do \
		  		echo $(INSTALL_DATA) $$f $(AFTER_DOC_DIR)/html/$$f; \
		  		$(INSTALL_DATA) $$f $(AFTER_DOC_DIR)/html/$$f; \
			done; \
			cd ../..; \
		fi

install:	install.lib


uninstall:	uninstall.lib

