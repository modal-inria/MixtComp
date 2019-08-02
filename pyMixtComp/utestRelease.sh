cd release && make runUtestPy && cd ..
MC_DETERMINISTIC=T release/utest/runUtestPy --gtest_output=xml:reportPy.xml
