cd release/ && make runUtestJMC && cd ..
MC_DETERMINISTIC=T release/utest/runUtestJMC --gtest_output=xml:reportJMC.xml
