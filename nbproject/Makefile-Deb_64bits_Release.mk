#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=distcc
CXX=distcc
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Deb_64bits_Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/addon/addonMySQL.o \
	${OBJECTDIR}/src/format.o \
	${OBJECTDIR}/src/addon/addonList.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/plugin.o \
	${OBJECTDIR}/src/addon/addon.o \
	${OBJECTDIR}/src/addon/addonLog.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/sender.o \
	${OBJECTDIR}/src/addon/addonFile.o \
	${OBJECTDIR}/src/conf.o


# C Compiler Flags
CFLAGS=-m64 -DNDEBUG

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,/opt/echoes-alert/probe/lib `pkg-config --libs libssl gobject-2.0 json-glib-1.0` -lmysqlclient  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/addon/addonMySQL.o: src/addon/addonMySQL.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonMySQL.o src/addon/addonMySQL.c

${OBJECTDIR}/src/format.o: src/format.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format.o src/format.c

${OBJECTDIR}/src/addon/addonList.o: src/addon/addonList.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonList.o src/addon/addonList.c

${OBJECTDIR}/src/log.o: src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log.o src/log.c

${OBJECTDIR}/src/plugin.o: src/plugin.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin.o src/plugin.c

${OBJECTDIR}/src/addon/addon.o: src/addon/addon.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addon.o src/addon/addon.c

${OBJECTDIR}/src/addon/addonLog.o: src/addon/addonLog.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonLog.o src/addon/addonLog.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/src/sender.o: src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender.o src/sender.c

${OBJECTDIR}/src/addon/addonFile.o: src/addon/addonFile.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonFile.o src/addon/addonFile.c

${OBJECTDIR}/src/conf.o: src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf.o src/conf.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
