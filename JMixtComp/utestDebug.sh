cd debug/ && make runUtestJMC && cd ..
MC_DETERMINISTIC=T debug/utest/runUtestJMC --gtest_output=xml:reportJMC.xml
