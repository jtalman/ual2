#              $Id: GNUmakefile,v 1.14 2005/03/11 12:01:03 ganis Exp $

#------------------------------------------------------------------------------#
#                       C o m m o n   V a r i a b l e s                        #
#------------------------------------------------------------------------------#
  
include ../GNUmake.env

MORELIBS = -L$(LIBDIR) -lXrdOuc

LIBDEPS = $(LIBDIR)/libXrdOuc.a

#------------------------------------------------------------------------------#
#                             C o m p o n e n t s                              #
#------------------------------------------------------------------------------#
  
SOURCES = \
        XrdSecClient.cc        \
        XrdSecPManager.cc      \
        XrdSecProtocolhost.cc  \
        XrdSecServer.vv        \
        XrdSectestClient.cc    \
        XrdSectestServer.cc    \
  
OBJSLIB = \
        $(OBJDIR)/XrdSecClient.o        \
        $(OBJDIR)/XrdSecPManager.o      \
        $(OBJDIR)/XrdSecProtocolhost.o  \
        $(OBJDIR)/XrdSecServer.o

OBJECTA = \
        $(OBJDIR)/XrdSectestClient.o

OBJECTB = \
        $(OBJDIR)/XrdSectestServer.o

OBJECTS = $(OBJSLIB) $(OBJECTA) $(OBJECTB)

LIBRARY = $(LIBDIR)/libXrdSec.$(TYPESHLIB)
LIBARCH = $(LIBDIR)/libXrdSec.a

TSTBINA = $(BINDIR)/testclient
TSTBINB = $(BINDIR)/testserver

TARGETS = $(LIBARCH) $(LIBRARY) $(TSTBINA) $(TSTBINB)

#------------------------------------------------------------------------------#
#                           S e a r c h   P a t h s                            #
#------------------------------------------------------------------------------#

vpath XrdOuc% ../XrdOuc

#------------------------------------------------------------------------------#
#                          I n i t i a l   R u l e s                           #
#------------------------------------------------------------------------------#
 
include ../GNUmake.options

anything: $(TARGETS)
	@echo Make XrdSec done.

#------------------------------------------------------------------------------#
#                           D e p e n d e n c i e s                            #
#------------------------------------------------------------------------------#

$(LIBARCH): $(OBJSLIB)
	@echo Creating archive $(LIBARCH) 
	$(ECHO)rm -f $(LIBARCH)
	$(ECHO)ar -rc $(LIBARCH) $(OBJSLIB)
	$(ECHO)ranlib $(LIBARCH)
	$(ECHO)if [ "x$(SUNCACHE)" != "x" ]; then \
	   if [ "$(TYPE)" = "SunOS" -a "$(CC)" = "CC" ]; then \
	      ar -rc $(LIBARCH) $(OBJDIR)$(SUNCACHE)/*/*.o; \
	   fi;\
	fi

$(LIBRARY): $(OBJSLIB) $(LIBDEPS)
	@echo Creating shared library $(LIBRARY) 
	$(ECHO)if [ "x$(TYPE)" = "xcygwin" ]; then \
	   $(CC) $(OBJSLIB) $(LDSO) $(MORELIBS) $(LIBS) \
		-Wl,--out-implib=$(LIBRARY).a -o $(LIBRARY); \
	else \
	   $(CC) $(OBJSLIB) $(LDSO) $(MORELIBS) $(LIBS) -o $(LIBRARY); \
	fi

$(TSTBINA): $(OBJECTA) $(LIBDEPS)
	@echo Creating executable $(TSTBINA) 
	$(ECHO)$(LD) $(LDOP) $(OBJECTA) $(BINLIBS) $(MORELIBS) -lXrdSec -lXrdOuc $(LIBS) -o $(TSTBINA)

$(TSTBINB): $(OBJECTB) $(LIBDEPS)
	@echo Creating executable $(TSTBINB) 
	$(ECHO)$(LD) $(LDOP) $(OBJECTB) $(BINLIBS) $(MORELIBS) -lXrdSec -lXrdOuc $(LIBS) -o $(TSTBINB)

$(OBJDIR)/XrdSecClient.o: XrdOucPthread.hh XrdSecPManager.hh XrdSecInterface.hh \
                          XrdOucErrInfo.hh XrdSecClient.cc   XrdSecEntity.hh
	@echo Compiling XrdSecClient.cc 
	$(ECHO)$(CC) -c $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/XrdSecClient.o XrdSecClient.cc

$(OBJDIR)/XrdSecPManager.o: XrdSecInterface.hh XrdSecPManager.hh \
                            XrdSecProtocolhost.hh \
                            XrdOucErrInfo.hh   XrdOucPthread.hh \
                            XrdSecPManager.cc
	@echo Compiling XrdSecPManager.cc 
	$(ECHO)$(CC) -c $(CFLAGS32) $(INCLUDE) -o $(OBJDIR)/XrdSecPManager.o XrdSecPManager.cc

$(OBJDIR)/XrdSecProtocolhost.o: XrdSecProtocolhost.cc XrdSecProtocolhost.hh \
                                XrdSecInterface.hh    XrdSecEntity.hh
	@echo Compiling XrdSecProtocolhost.cc
	$(ECHO)$(CC) -c $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/XrdSecProtocolhost.o XrdSecProtocolhost.cc
$(OBJDIR)/XrdSecServer.o: XrdSecServer.cc       XrdSecServer.hh \
                          XrdOucErrInfo.hh      XrdOucError.hh \
                          XrdOucLogger.hh       XrdOucStream.hh \
                          XrdSecInterface.hh    XrdSecPManager.hh \
                          XrdSecTrace.hh
	@echo Compiling XrdSecServer.cc 
	$(ECHO)$(CC) -c $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/XrdSecServer.o XrdSecServer.cc

$(OBJDIR)/XrdSectestClient.o:   XrdSectestClient.cc XrdSecInterface.hh \
                                XrdSecEntity.hh
	@echo Compiling XrdSectestClient.cc 
	$(ECHO)$(CC) -c $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/XrdSectestClient.o XrdSectestClient.cc

$(OBJDIR)/XrdSectestServer.o: XrdSecInterface.hh XrdSecEntity.hh XrdOucLogger.hh \
                              XrdSectestServer.cc
	@echo Compiling XrdSectestServer.cc 
	$(ECHO)$(CC) -c $(CFLAGS) $(INCLUDE) -o $(OBJDIR)/XrdSectestServer.o XrdSectestServer.cc
