#! /bin/bash

total_code_lines=0
total_executed_lines=0
for source_file_gcno in src/CMakeFiles/CMakeTest_lib.dir/*.cpp.gcno
do
    source_file=${source_file_gcno%.gcno}
    #echo ${source_file}.cpp
    last_line=$(gcov ${source_file}.cpp | head -n 2 | tail -n 1)
    num_lines=$(echo $last_line | cut -d ' ' -f 4-)
    executed_percent=$(echo $last_line | cut -d ' ' -f 2 | cut -d ':' -f 2 | cut -d '%' -f 1)
    #echo $num_lines
    #echo $executed_percent
    executed_lines=$(awk -v ex_percent=$executed_percent -v n_lines=$num_lines 'BEGIN {printf "%.2f", ex_percent * n_lines / 100}' | cut -d '.' -f 1)
    #echo $executed_lines
    total_code_lines=$(($total_code_lines+$num_lines))
    total_executed_lines=$(($total_executed_lines+$executed_lines))
done

#echo 'Code coverage: ' $total_executed_lines '/' $total_code_lines
awk -v executed=$total_executed_lines -v total=$total_code_lines 'BEGIN {printf "Code coverage: %.2f%\n", executed / total * 100}'