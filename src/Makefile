ARCHIVE = archive-3dviewer
BUILD = build

ifeq ($(shell uname), Darwin)
	MACCOMPILER = -DCMAKE_CXX_COMPILER="/usr/local/bin/g++"
	OPEN = open
endif

.PHONY: tests
all: dist install tests check dvi

install:
	mkdir -p ${BUILD}
	cd ${BUILD} && \
	cmake -G "Unix Makefiles" .. && \
	cmake --build . --target 3DViewer --config=Release


uninstall: clean
	rm -rf 3DViewer ViewerTests *.dll *.so $(ARCHIVE).tar.gz


clean:
	rm -rf ${BUILD}


dist: uninstall
	mkdir -p ${ARCHIVE}
	cp -R core/ \
		  controller/ \
		  gui/ \
		  QGifImage/ \
		  tests/ \
		  CMakeLists.txt \
		  Makefile \
		  README.md \
		  ${ARCHIVE}/
	tar -cf ${ARCHIVE}.tar ${ARCHIVE}
	gzip -9f ${ARCHIVE}.tar
	rm -rf ${ARCHIVE}


dvi:
	$(OPEN)	./README.md


tests:
	mkdir -p ${BUILD}
	cd ${BUILD} && \
	cmake $(MACCOMPILER) -G "Unix Makefiles" .. && \
	cmake --build . --target ViewerTests --config=RelWithDebInfo
	./ViewerTests


check:
	clang-format -n \
		core/*.cc \
		core/*/*.cc \
		core/*/*.h \
