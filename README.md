# CPP_Standard_Parser
Parsing C++ Standard definitions and generating graph description (for graph visualization)

IDE:

NetBeans IDE 8.1

Features:

DOT-file generation (for https://dreampuf.github.io/GraphvizOnline)

Tests:

99_all_gen.sh -> 99_all_run.sh -> 99_all_run.log - test definitions separately

99_all_to_one_gen.sh -> 99_all_to_one_run.sh + 99_all_to_one.txt -> 99_all_to_one_run.log - test definitions "all in one"

cpp_standard_parcer -f 99_all_to_one.txt -gen - generate graph (dot format)

Result:

cpp_std_graph.svg
c18_std_graph.svg
