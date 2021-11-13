set(CTEST_SOURCE_DIRECTORY "src")
set(CTEST_BINARY_DIRECTORY "build")











ctest_start("Incremental")
ctest_test()
ctest_submit()