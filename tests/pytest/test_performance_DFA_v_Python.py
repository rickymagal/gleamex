import subprocess
import time
import matplotlib.pyplot as plt
import pytest
import re
import os

# Diretórios e caminhos dos executáveis
BIN_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../bin"))
EXEC_DFA_PATH = os.path.join(BIN_DIR, "test_match_dfa_exec")
EXEC_NFA_PATH = os.path.join(BIN_DIR, "test_match_nfa_exec")


def measure_time_dfa(regex_pattern, test_string):
    start_time = time.time()
    subprocess.run([EXEC_DFA_PATH, regex_pattern, test_string], stdout=subprocess.PIPE)
    end_time = time.time()
    return end_time - start_time


def measure_time_re(regex, string):
    start_time = time.perf_counter()
    match = re.match(regex, string)
    end_time = time.perf_counter()
    return end_time - start_time


def main():
    ns = list(range(1, 31))
    times_dfa = []
    times_re = []
    regex_pattern = "(a?)a{"  # Pattern: a?*a* (this is a regex)
    test_string_prefix = "a"  # String: a*
    
    for n in ns:
        regex_pattern_n = 'a?' * n + 'a' * n
        test_string_n = 'a' * n
        
        # Measure time for NFA
        time_re = measure_time_re(regex_pattern_n, test_string_n)
        times_re.append(time_re)
        
        # Measure time for DFA
        time_dfa = measure_time_dfa(regex_pattern_n, test_string_n)
        times_dfa.append(time_dfa)
        
        print(f"n = {n}: RE time = {time_re:.6f}s, DFA time = {time_dfa:.6f}s")
    
    # Plotting results
    plt.figure(figsize=(10, 6))
    plt.plot(ns, times_re, marker='o', linestyle='-', color='b', label='re (Python stdlib)')
    plt.plot(ns, times_dfa, marker='s', linestyle='--', color='r', label='Gleamex')
    plt.title('Time to match $a?^{n}a^{n}$ (regex) against $a^{n}$ (string)')
    plt.xlabel('n')
    plt.ylabel('Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('performance_comparison_Gleamex_v_Python.png')
    plt.show()

if __name__ == "__main__":
    main()
