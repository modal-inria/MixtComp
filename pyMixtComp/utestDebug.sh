cd debug && make runUtestPy && cd ..
MC_DETERMINISTIC=T debug/utest/runUtestPy --gtest_output=xml:reportPy.xml
