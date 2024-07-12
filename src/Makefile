ARCHIVE = archive-3dviewer
BUILD = build

ifeq ($(shell uname), Darwin)
	MACCOMPILER = -DCMAKE_CXX_COMPILER="/usr/local/bin/g++"
endif

.PHONY: tests
all: install

install:
	mkdir -p ${BUILD}
	cd ${BUILD} && \
	cmake -G "Unix Makefiles" .. && \
	cmake --build . --target 3DViewer --config=Release


uninstall: clean
	rm -rf 3DViewer ViewerTests *.dll *.so


clean:
	rm -rf ${BUILD}


dist: uninstall
	mkdir -p ${ARCHIVE}
	cp -R . ${ARCHIVE}/
	tar -cf ${ARCHIVE}.tar ${ARCHIVE}
	gzip -9f ${ARCHIVE}.tar
	rm -rf ${ARCHIVE}


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
