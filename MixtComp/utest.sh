cd build/ && make runUtest && cd ..
MC_DETERMINISTIC=T build/utest/runUtest --gtest_output=xml:report.xml
