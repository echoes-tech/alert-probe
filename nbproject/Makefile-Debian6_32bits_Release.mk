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
CND_CONF=Debian6_32bits_Release
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
	${OBJECTDIR}/src/addon/addonSNMP.o \
	${OBJECTDIR}/src/search.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/sender.o \
	${OBJECTDIR}/src/addon/addonFile.o \
	${OBJECTDIR}/src/conf.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

# C Compiler Flags
CFLAGS=-m32 -DNDEBUG

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,/opt/echoes-alert/probe/lib `pkg-config --libs libssl gobject-2.0 json-glib-1.0` /usr/local/lib32/libmysqlclient.so.16  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: /usr/local/lib32/libmysqlclient.so.16

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

${OBJECTDIR}/src/addon/addonSNMP.o: src/addon/addonSNMP.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonSNMP.o src/addon/addonSNMP.c

${OBJECTDIR}/src/search.o: src/search.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/search.o src/search.c

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

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${TESTDIR}/tests/addon.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c}   -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/addon.o: tests/addon.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -MMD -MP -MF $@.d -o ${TESTDIR}/tests/addon.o tests/addon.c


${OBJECTDIR}/src/addon/addonMySQL_nomain.o: ${OBJECTDIR}/src/addon/addonMySQL.o src/addon/addonMySQL.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addonMySQL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonMySQL_nomain.o src/addon/addonMySQL.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addonMySQL.o ${OBJECTDIR}/src/addon/addonMySQL_nomain.o;\
	fi

${OBJECTDIR}/src/format_nomain.o: ${OBJECTDIR}/src/format.o src/format.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/format.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format_nomain.o src/format.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/format.o ${OBJECTDIR}/src/format_nomain.o;\
	fi

${OBJECTDIR}/src/addon/addonList_nomain.o: ${OBJECTDIR}/src/addon/addonList.o src/addon/addonList.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addonList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonList_nomain.o src/addon/addonList.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addonList.o ${OBJECTDIR}/src/addon/addonList_nomain.o;\
	fi

${OBJECTDIR}/src/log_nomain.o: ${OBJECTDIR}/src/log.o src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/log.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log_nomain.o src/log.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/log_nomain.o;\
	fi

${OBJECTDIR}/src/plugin_nomain.o: ${OBJECTDIR}/src/plugin.o src/plugin.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/plugin.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin_nomain.o src/plugin.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/plugin.o ${OBJECTDIR}/src/plugin_nomain.o;\
	fi

${OBJECTDIR}/src/addon/addon_nomain.o: ${OBJECTDIR}/src/addon/addon.o src/addon/addon.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addon.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addon_nomain.o src/addon/addon.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addon.o ${OBJECTDIR}/src/addon/addon_nomain.o;\
	fi

${OBJECTDIR}/src/addon/addonLog_nomain.o: ${OBJECTDIR}/src/addon/addonLog.o src/addon/addonLog.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addonLog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonLog_nomain.o src/addon/addonLog.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addonLog.o ${OBJECTDIR}/src/addon/addonLog_nomain.o;\
	fi

${OBJECTDIR}/src/addon/addonSNMP_nomain.o: ${OBJECTDIR}/src/addon/addonSNMP.o src/addon/addonSNMP.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addonSNMP.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonSNMP_nomain.o src/addon/addonSNMP.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addonSNMP.o ${OBJECTDIR}/src/addon/addonSNMP_nomain.o;\
	fi

${OBJECTDIR}/src/search_nomain.o: ${OBJECTDIR}/src/search.o src/search.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/search.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/search_nomain.o src/search.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/search.o ${OBJECTDIR}/src/search_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/src/sender_nomain.o: ${OBJECTDIR}/src/sender.o src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/sender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender_nomain.o src/sender.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/sender.o ${OBJECTDIR}/src/sender_nomain.o;\
	fi

${OBJECTDIR}/src/addon/addonFile_nomain.o: ${OBJECTDIR}/src/addon/addonFile.o src/addon/addonFile.c 
	${MKDIR} -p ${OBJECTDIR}/src/addon
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addon/addonFile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addon/addonFile_nomain.o src/addon/addonFile.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addon/addonFile.o ${OBJECTDIR}/src/addon/addonFile_nomain.o;\
	fi

${OBJECTDIR}/src/conf_nomain.o: ${OBJECTDIR}/src/conf.o src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/conf.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude `pkg-config --cflags libssl gobject-2.0 json-glib-1.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf_nomain.o src/conf.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/conf.o ${OBJECTDIR}/src/conf_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
