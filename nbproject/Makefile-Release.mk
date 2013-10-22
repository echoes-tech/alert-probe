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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/addonList.o \
	${OBJECTDIR}/src/conf.o \
	${OBJECTDIR}/src/format.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/plugin.o \
	${OBJECTDIR}/src/sender.o \
	${OBJECTDIR}/src/signals.o \
	${OBJECTDIR}/tests/utilUnitTest.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f6 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f7

# C Compiler Flags
CFLAGS=-DNDEBUG

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Laddon/dist/Release/${CND_PLATFORM} -L/usr/local/lib -Wl,-rpath,/opt/echoes-alert/probe/lib/addons -Wl,-rpath,/opt/echoes-alert/probe/lib -Wl,-rpath,lib/addons `pkg-config --libs gobject-2.0 json-glib-1.0` -lpthread -laddon -lcunit  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe ${OBJECTFILES} ${LDLIBSOPTIONS} -s

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/src/addonList.o: src/addonList.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addonList.o src/addonList.c

${OBJECTDIR}/src/conf.o: src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf.o src/conf.c

${OBJECTDIR}/src/format.o: src/format.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format.o src/format.c

${OBJECTDIR}/src/log.o: src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log.o src/log.c

${OBJECTDIR}/src/plugin.o: src/plugin.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin.o src/plugin.c

${OBJECTDIR}/src/sender.o: src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender.o src/sender.c

${OBJECTDIR}/src/signals.o: src/signals.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/signals.o src/signals.c

${OBJECTDIR}/tests/utilUnitTest.o: tests/utilUnitTest.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${OBJECTDIR}/tests/utilUnitTest.o tests/utilUnitTest.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/addoncunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/confcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/formatcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests/logcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/plugincunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/sendercunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests/signalscunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/addoncunittest.o: tests/addoncunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/addoncunittest.o tests/addoncunittest.c


${TESTDIR}/tests/confcunittest.o: tests/confcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/confcunittest.o tests/confcunittest.c


${TESTDIR}/tests/formatcunittest.o: tests/formatcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/formatcunittest.o tests/formatcunittest.c


${TESTDIR}/tests/logcunittest.o: tests/logcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/logcunittest.o tests/logcunittest.c


${TESTDIR}/tests/plugincunittest.o: tests/plugincunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/plugincunittest.o tests/plugincunittest.c


${TESTDIR}/tests/sendercunittest.o: tests/sendercunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/sendercunittest.o tests/sendercunittest.c


${TESTDIR}/tests/signalscunittest.o: tests/signalscunittest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -MMD -MP -MF $@.d -o ${TESTDIR}/tests/signalscunittest.o tests/signalscunittest.c


${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/src/addonList_nomain.o: ${OBJECTDIR}/src/addonList.o src/addonList.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/addonList.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/addonList_nomain.o src/addonList.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/addonList.o ${OBJECTDIR}/src/addonList_nomain.o;\
	fi

${OBJECTDIR}/src/conf_nomain.o: ${OBJECTDIR}/src/conf.o src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/conf.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf_nomain.o src/conf.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/conf.o ${OBJECTDIR}/src/conf_nomain.o;\
	fi

${OBJECTDIR}/src/format_nomain.o: ${OBJECTDIR}/src/format.o src/format.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/format.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format_nomain.o src/format.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/format.o ${OBJECTDIR}/src/format_nomain.o;\
	fi

${OBJECTDIR}/src/log_nomain.o: ${OBJECTDIR}/src/log.o src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/log.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log_nomain.o src/log.c;\
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
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/plugin_nomain.o src/plugin.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/plugin.o ${OBJECTDIR}/src/plugin_nomain.o;\
	fi

${OBJECTDIR}/src/sender_nomain.o: ${OBJECTDIR}/src/sender.o src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/sender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender_nomain.o src/sender.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/sender.o ${OBJECTDIR}/src/sender_nomain.o;\
	fi

${OBJECTDIR}/src/signals_nomain.o: ${OBJECTDIR}/src/signals.o src/signals.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/signals.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/signals_nomain.o src/signals.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/signals.o ${OBJECTDIR}/src/signals_nomain.o;\
	fi

${OBJECTDIR}/tests/utilUnitTest_nomain.o: ${OBJECTDIR}/tests/utilUnitTest.o tests/utilUnitTest.c 
	${MKDIR} -p ${OBJECTDIR}/tests
	@NMOUTPUT=`${NM} ${OBJECTDIR}/tests/utilUnitTest.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O3 -s -Iinclude -Iaddon/include `pkg-config --cflags gobject-2.0 json-glib-1.0`  -DNDEBUG -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/tests/utilUnitTest_nomain.o tests/utilUnitTest.c;\
	else  \
	    ${CP} ${OBJECTDIR}/tests/utilUnitTest.o ${OBJECTDIR}/tests/utilUnitTest_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f7 || true; \
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
