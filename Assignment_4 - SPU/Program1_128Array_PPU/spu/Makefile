                                          
########################################################################
#			Target
########################################################################

PROGRAM_spu 	:= spu
LIBRARY_embed	:= spu.a

########################################################################
#			Local Defines
########################################################################

INCLUDE		:= -I ..
# Link with libm (math.h)
IMPORTS			= -lm

CFLAGS_gcc		= -std=gnu99
CC_OPT_LEVEL	= $(CC_OPT_LEVEL_DEBUG)

########################################################################
#			buildutils/make.footer
########################################################################

ifdef CELL_TOP
	include $(CELL_TOP)/buildutils/make.footer
else
	include /opt/cell/sdk/buildutils/make.footer
endif
