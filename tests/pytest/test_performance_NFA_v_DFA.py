import subprocess
import time
import matplotlib.pyplot as plt
import pytest
import os

# Diretórios e caminhos dos executáveis
BIN_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../bin"))
EXEC_DFA_PATH = os.path.join(BIN_DIR, "test_match_dfa_exec")
EXEC_NFA_PATH = os.path.join(BIN_DIR, "test_match_nfa_exec")

# Função para executar o teste do DFA e medir o tempo
def measure_time_dfa(regex_pattern, test_string):
    start_time = time.time()
    subprocess.run([EXEC_DFA_PATH, regex_pattern, test_string], stdout=subprocess.PIPE)
    end_time = time.time()
    return end_time - start_time

# Função para executar o teste do NFA e medir o tempo
def measure_time_nfa(regex_pattern, test_string):
    start_time = time.time()
    subprocess.run([EXEC_NFA_PATH, regex_pattern, test_string], stdout=subprocess.PIPE)
    end_time = time.time()
    return end_time - start_time

def main():
    ns = list(range(1, 51))
    times_nfa = []
    times_dfa = []
    regex_pattern = "(a?)a{"  # Pattern: a?*a* (this is a regex)
    test_string_prefix = "a"  # String: a*
    
    for n in ns:
        regex_pattern_n = regex_pattern + str(n) + "}"
        test_string_n = test_string_prefix * n
        
        # Measure time for NFA
        time_nfa = measure_time_nfa(regex_pattern_n, test_string_n)
        times_nfa.append(time_nfa)
        
        # Measure time for DFA
        time_dfa = measure_time_dfa(regex_pattern_n, test_string_n)
        times_dfa.append(time_dfa)
        
        print(f"n = {n}: NFA time = {time_nfa:.6f}s, DFA time = {time_dfa:.6f}s")

    nfa_times_s = [time * 1000 for time in times_nfa]
    dfa_times_s = [time * 1000 for time in times_dfa]
    
    # Plotting results
    plt.figure(figsize=(10, 6))
    plt.plot(ns, nfa_times_s, marker='o', linestyle='-', color='b', label='NFA')
    plt.plot(ns, dfa_times_s, marker='s', linestyle='--', color='r', label='DFA')
    plt.title('Time to match $a?^{n}a^{n}$ (regex) against $a^{n}$ (string)')
    plt.xlabel('n')
    plt.ylabel('Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('performance_comparison.png')
    plt.show()

if __name__ == "__main__":
    main()
