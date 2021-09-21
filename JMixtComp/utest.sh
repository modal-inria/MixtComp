cd build/ && make runUtestJMC && cd ..
MC_DETERMINISTIC=T build/utest/runUtestJMC --gtest_output=xml:reportJMC.xml
