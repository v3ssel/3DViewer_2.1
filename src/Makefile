ARCHIVE = archive-3dviewer
BUILD = build

.PHONY: tests
all: install

install:
	mkdir -p ${BUILD}
	cd ${BUILD} && \
	cmake -G "Unix Makefiles" .. && \
	cmake --build . --target 3DViewer --config=Debug


uninstall: clean
	rm -rf 3DViewer libQGifImage.*


clean:
	rm -rf ${BUILD}


dist:
	mkdir -p ${ARCHIVE}
	cp -R 3DViewer/ ${ARCHIVE}/
	tar -cf ${ARCHIVE}.tar ${ARCHIVE}
	gzip -9f ${ARCHIVE}.tar
	rm -rf ${ARCHIVE}


tests:
	mkdir -p ${BUILD}
	cd ${BUILD} && \
	cmake -G "Unix Makefiles" .. && \
	cmake --build . --target Tests --config=Debug
	./Tests.exe


gcov_report: clean
	cd 3DViewer/Unit-tests && mkdir -p build && cd build && \
	cmake -DCMAKE_BUILD_TYPE=Coverage .. && cmake --build . --target all && \
	mv Tests ../ && cd .. && chmod +x Tests && ./Tests && \
	lcov -t "Tests" -o gcov_test.info --include */src/* -c -d . && \
	genhtml -o report/ gcov_test.info && \
	open ./report/index.html && \
	rm -rf ./*.gcno ./*.gcda ./gcov* build/

check:
	clang-format -n \
		core/*.cc \
		core/*/*.cc \
		core/*/*.h \