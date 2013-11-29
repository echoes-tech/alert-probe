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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/conf.o \
	${OBJECTDIR}/src/format.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/sender.o \
	${OBJECTDIR}/src/signals.o \
	${OBJECTDIR}/tests_unit/utilUnitTest.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f6 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f7

# C Compiler Flags
CFLAGS=-pedantic -Wextra

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../addon/dist/Debug/${CND_PLATFORM} -L/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/dist/Release/${CND_PLATFORM} -L/usr/local/lib -Wl,-rpath,../addon/dist/Debug/${CND_PLATFORM} -Wl,-rpath,/opt/echoes-alert/probe/lib `pkg-config --libs glib-2.0 gobject-2.0 gio-2.0` -lpthread -lcunit -laddon  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/probe ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/src/conf.o: src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf.o src/conf.c

${OBJECTDIR}/src/format.o: src/format.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format.o src/format.c

${OBJECTDIR}/src/log.o: src/log.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log.o src/log.c

${OBJECTDIR}/src/sender.o: src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender.o src/sender.c

${OBJECTDIR}/src/signals.o: src/signals.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/signals.o src/signals.c

${OBJECTDIR}/tests_unit/utilUnitTest.o: tests_unit/utilUnitTest.c 
	${MKDIR} -p ${OBJECTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${OBJECTDIR}/tests_unit/utilUnitTest.o tests_unit/utilUnitTest.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests_unit/confcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests_unit/formatcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f6: ${TESTDIR}/tests_unit/logcunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f6 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests_unit/sendercunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f7: ${TESTDIR}/tests_unit/signalscunittest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f7 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests_unit/confcunittest.o: tests_unit/confcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${TESTDIR}/tests_unit/confcunittest.o tests_unit/confcunittest.c


${TESTDIR}/tests_unit/formatcunittest.o: tests_unit/formatcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra   `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests_unit/formatcunittest.o tests_unit/formatcunittest.c


${TESTDIR}/tests_unit/logcunittest.o: tests_unit/logcunittest.c 
	${MKDIR} -p ${TESTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${TESTDIR}/tests_unit/logcunittest.o tests_unit/logcunittest.c


${TESTDIR}/tests_unit/sendercunittest.o: tests_unit/sendercunittest.c 
	${MKDIR} -p ${TESTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${TESTDIR}/tests_unit/sendercunittest.o tests_unit/sendercunittest.c


${TESTDIR}/tests_unit/signalscunittest.o: tests_unit/signalscunittest.c 
	${MKDIR} -p ${TESTDIR}/tests_unit
	${RM} $@.d
	$(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -MMD -MP -MF $@.d -o ${TESTDIR}/tests_unit/signalscunittest.o tests_unit/signalscunittest.c


${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/src/conf_nomain.o: ${OBJECTDIR}/src/conf.o src/conf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/conf.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/conf_nomain.o src/conf.c;\
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
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/format_nomain.o src/format.c;\
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
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/log_nomain.o src/log.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/log.o ${OBJECTDIR}/src/log_nomain.o;\
	fi

${OBJECTDIR}/src/sender_nomain.o: ${OBJECTDIR}/src/sender.o src/sender.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/sender.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/sender_nomain.o src/sender.c;\
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
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/signals_nomain.o src/signals.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/signals.o ${OBJECTDIR}/src/signals_nomain.o;\
	fi

${OBJECTDIR}/tests_unit/utilUnitTest_nomain.o: ${OBJECTDIR}/tests_unit/utilUnitTest.o tests_unit/utilUnitTest.c 
	${MKDIR} -p ${OBJECTDIR}/tests_unit
	@NMOUTPUT=`${NM} ${OBJECTDIR}/tests_unit/utilUnitTest.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -Wall -Iinclude -I../addon/include -I/var/lib/jenkins/workspace/ea-probe_addon-${target}/label/${NODE_NAME}/include `pkg-config --cflags glib-2.0 gobject-2.0 gio-2.0`  -pedantic -Wextra -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/tests_unit/utilUnitTest_nomain.o tests_unit/utilUnitTest.c;\
	else  \
	    ${CP} ${OBJECTDIR}/tests_unit/utilUnitTest.o ${OBJECTDIR}/tests_unit/utilUnitTest_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f6 || true; \
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
