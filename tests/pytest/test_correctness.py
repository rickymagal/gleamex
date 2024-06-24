import subprocess
import os
import pytest

# Diretórios e caminhos dos executáveis
BIN_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../bin"))
EXEC_DFA_PATH = os.path.join(BIN_DIR, "test_match_dfa_exec")
EXEC_NFA_PATH = os.path.join(BIN_DIR, "test_match_nfa_exec")

# Função para executar o teste do DFA
def run_test_match_dfa(regex_pattern, test_string):
    result = subprocess.run([EXEC_DFA_PATH, regex_pattern, test_string], stdout=subprocess.PIPE)
    return result.returncode == 0

# Função para executar o teste do NFA
def run_test_match_nfa(regex_pattern, test_string):
    result = subprocess.run([EXEC_NFA_PATH, regex_pattern, test_string], stdout=subprocess.PIPE)
    return result.returncode == 0

@pytest.mark.parametrize(
    "regex_pattern, test_string, expected_result",
    [
        # Casos mais complexos
        ("((a|b)*c)+", "abababcabababc", True),  # Sequências repetidas com concatenação final
        ("((a|b)*c)+", "ababab", False),  # Falta o caractere 'c' no final
        ("(a|b)*|(c|d)+", "ababab", True),  # Combinação de alternância e repetição
        ("(a|b)*|(c|d)+", "cdcdcd", True),  # Apenas repetição
        ("(a|b)*|(c|d)+", "efef", False),  # Nenhum padrão correspondente
        ("((a|b)+c)?d", "ac", False),  # Falta 'd' após padrão opcional
        ("((a|b)+c)?d", "abcbc", False),  # Não corresponde ao padrão completo
        ("(a|(b|c)*d)+", "abbbbcd", True),  # Sequência complexa de alternância e repetição
        ("(a|(b|c)*d)+", "ad", True),  # Alternância simples seguida de repetição
        ("(a|(b|c)*d)+", "abbbbbbbcd", True),  # Repetição mais longa seguida de 'd'
        ("(a|(b|c)*d)+", "abbbbbbbce", False),  # Não termina com 'd'
        ("((a|b)+(c|d)?)?", "abc", True),  # Padrão opcional com padrão interno
        ("((a|b)+(c|d)?)?", "ab", True),  # Apenas padrão opcional
        ("((a|b)+(c|d)?)?", "ef", False),  # Não corresponde a nenhum padrão
        ("((a|b)?c)*d", "abcbc", False),  # Não termina com 'd'
        ("((a|b)?c)*d", "abcacbd", False),  # Não corresponde ao padrão completo
        ("(a|b)+|(c|d)*", "ef", False),  # Nenhum padrão correspondente
        ("((a|b)|c)+", "abababc", True),  # Combinação de alternância e repetição
        ("((a|b)|c)+", "abc", True),  # Alternância simples seguida de repetição
        ("((a|b)|c)+", "def", False),  # Não corresponde a nenhum padrão
        ("((a|b)(c|d)?)+", "ac", True),  # Padrão alternativo opcional repetido
        ("((a|b)(c|d)?)+", "ab", True),  # Padrão alternativo repetido sem opcional
        ("((a|b)(c|d)?)+", "abcd", False),  # Não corresponde ao padrão completo
        ("((a|b)(c|d)?)+", "ef", False),  # Não corresponde a nenhum padrão
        ("((a|b)(c|d)*)+", "abcccc", True),  # Padrão alternativo seguido de repetição
        ("((a|b)(c|d)*)+", "abcd", True),  # Padrão alternativo seguido de repetição
        ("((a|b)(c|d)*)+", "ef", False),  # Não corresponde a nenhum padrão
        ("(a|(b|(c|d)+)?)*", "abbbbcd", True),  # Padrão aninhado com repetição opcional
        ("(a|(b|(c|d)+)?)*", "ad", True),  # Alternância simples seguida de padrão aninhado
        ("(a|(b|(c|d)+)?)*", "abbbbbbbcd", True),  # Repetição mais longa seguida de padrão aninhado
        ("(a|(b|(c|d)+)?)*", "abbbbbbbce", False),  # Não corresponde ao padrão completo
        ("((a|b)*|(c|d)+)?", "ababab", True),  # Padrão opcional com alternância e repetição
        ("((a|b)*|(c|d)+)?", "cdcdcd", True),  # Apenas repetição
        ("((a|b)*|(c|d)+)?", "efef", False),  # Nenhum padrão correspondente
        ("((a|b)*|(c|d)+)?", "", True),  # Padrão opcional vazio
        ("((a|b)|c)?(d|e|f)+", "d", True),  # Padrão aninhado com repetição
        ("((a|b)|c)?(d|e|f)+", "a", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)+", "def", True),  # Padrão aninhado com repetição
        ("((a|b)|c)?(d|e|f)+", "adef", True),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)+", "", False),  # Não corresponde a nenhum padrão
        ("((a|b)|c)?(d|e|f)*", "", True),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "f", True),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "abc", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)*", "ababdefef", False),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "ababdefeg", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)?", "", True),  # Padrão aninhado opcional
        ("((a|b)|c)?(d|e|f)?", "d", True),  # Padrão aninhado opcional
        ("((a|b)|c)?(d|e|f)?", "abc", False),  # Não corresponde ao padrão completo

         # Casos com erros sintáticos
        ("((a|b)", "ab", False),  # Parêntese aberto sem fechamento
        ("(a|b)|", "a", False),  # Alternância sem operando à direita
        ("a|b)", "a", False),  # Parêntese fechado sem abertura correspondente
        ("(a|b", "a", False),  # Parêntese aberto sem fechamento
        ("((a|b)))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)", "a", False),  # Parêntese aberto sem fechamento
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("a|b|", "a", False),  # Alternância sem operando à direita
        ("(a|b|", "a", False),  # Parêntese aberto sem fechamento
        ("a|b)|c", "a", False),  # Parêntese fechado sem abertura correspondente
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("(a|b))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("(a|b)|c|", "a", False),  # Alternância sem operando à direita
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("a|b)|c", "a", False),  # Parêntese fechado sem abertura correspondente
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("(a|b))", "a", False),  # Parêntese fechado sem abertura correspondente
        ("(a|b))", "a", False),  # Parêntese fechado sem abertura correspondente
    ]
)

        

def test_match_dfa(regex_pattern, test_string, expected_result):
    assert run_test_match_dfa(regex_pattern, test_string) == expected_result

@pytest.mark.parametrize(
    "regex_pattern, test_string, expected_result",
    [
        # Casos mais complexos
        ("((a|b)*c)+", "abababcabababc", True),  # Sequências repetidas com concatenação final
        ("((a|b)*c)+", "ababab", False),  # Falta o caractere 'c' no final
        ("(a|b)*|(c|d)+", "ababab", True),  # Combinação de alternância e repetição
        ("(a|b)*|(c|d)+", "cdcdcd", True),  # Apenas repetição
        ("(a|b)*|(c|d)+", "efef", False),  # Nenhum padrão correspondente
        ("((a|b)+c)?d", "ac", False),  # Falta 'd' após padrão opcional
        ("((a|b)+c)?d", "abcbc", False),  # Não corresponde ao padrão completo
        ("(a|(b|c)*d)+", "abbbbcd", True),  # Sequência complexa de alternância e repetição
        ("(a|(b|c)*d)+", "ad", True),  # Alternância simples seguida de repetição
        ("(a|(b|c)*d)+", "abbbbbbbcd", True),  # Repetição mais longa seguida de 'd'
        ("(a|(b|c)*d)+", "abbbbbbbce", False),  # Não termina com 'd'
        ("((a|b)+(c|d)?)?", "abc", True),  # Padrão opcional com padrão interno
        ("((a|b)+(c|d)?)?", "ab", True),  # Apenas padrão opcional
        ("((a|b)+(c|d)?)?", "ef", False),  # Não corresponde a nenhum padrão
        ("((a|b)?c)*d", "abcbc", False),  # Não termina com 'd'
        ("((a|b)?c)*d", "abcacbd", False),  # Não corresponde ao padrão completo
        ("(a|b)+|(c|d)*", "ef", False),  # Nenhum padrão correspondente
        ("((a|b)|c)+", "abababc", True),  # Combinação de alternância e repetição
        ("((a|b)|c)+", "abc", True),  # Alternância simples seguida de repetição
        ("((a|b)|c)+", "def", False),  # Não corresponde a nenhum padrão
        ("((a|b)(c|d)?)+", "ac", True),  # Padrão alternativo opcional repetido
        ("((a|b)(c|d)?)+", "ab", True),  # Padrão alternativo repetido sem opcional
        ("((a|b)(c|d)?)+", "abcd", False),  # Não corresponde ao padrão completo
        ("((a|b)(c|d)?)+", "ef", False),  # Não corresponde a nenhum padrão
        ("((a|b)(c|d)*)+", "abcccc", True),  # Padrão alternativo seguido de repetição
        ("((a|b)(c|d)*)+", "abcd", True),  # Padrão alternativo seguido de repetição
        ("((a|b)(c|d)*)+", "ef", False),  # Não corresponde a nenhum padrão4
        
        ("(a|(b|(c|d)+)?)*", "abbbbbbbce", False),  # Não corresponde ao padrão completo
        ("((a|b)*|(c|d)+)?", "ababab", True),  # Padrão opcional com alternância e repetição
        ("((a|b)*|(c|d)+)?", "cdcdcd", True),  # Apenas repetição
        ("((a|b)*|(c|d)+)?", "efef", False),  # Nenhum padrão correspondente
        ("((a|b)*|(c|d)+)?", "", True),  # Padrão opcional vazio
        ("((a|b)|c)?(d|e|f)+", "d", True),  # Padrão aninhado com repetição
        ("((a|b)|c)?(d|e|f)+", "a", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)+", "def", True),  # Padrão aninhado com repetição
        ("((a|b)|c)?(d|e|f)+", "adef", True),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)+", "", False),  # Não corresponde a nenhum padrão
        ("((a|b)|c)?(d|e|f)*", "", True),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "f", True),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "abc", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)*", "ababdefef", False),  # Padrão aninhado opcional com repetição
        ("((a|b)|c)?(d|e|f)*", "ababdefeg", False),  # Não corresponde ao padrão completo
        ("((a|b)|c)?(d|e|f)?", "", True),  # Padrão aninhado opcional
        ("((a|b)|c)?(d|e|f)?", "d", True),  # Padrão aninhado opcional
        ("((a|b)|c)?(d|e|f)?", "abc", False),  # Não corresponde ao padrão completo

         # Casos com erros sintáticos
        ("((a|b)", "ab", False),  # Parêntese aberto sem fechamento
        ("(a|b)|", "a", False),  # Alternância sem operando à direita
        ("a|b)", "a", False),  # Parêntese fechado sem abertura correspondente
        ("(a|b", "a", False),  # Parêntese aberto sem fechamento
        ("((a|b)))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)", "a", False),  # Parêntese aberto sem fechamento
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("a|b|", "a", False),  # Alternância sem operando à direita
        ("(a|b|", "a", False),  # Parêntese aberto sem fechamento
        ("a|b)|c", "a", False),  # Parêntese fechado sem abertura correspondente
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("(a|b))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("(a|b)|c|", "a", False),  # Alternância sem operando à direita
        ("((a|b)|c", "a", False),  # Parêntese aberto sem fechamento
        ("a|b)|c", "a", False),  # Parêntese fechado sem abertura correspondente
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("((a|b)|c))", "a", False),  # Parênteses extras sem correspondência
        ("(a|b))", "a", False),  # Parêntese fechado sem abertura correspondente
        ("(a|b))", "a", False),  # Parêntese fechado sem abertura correspondente
    ]
)


def test_match_nfa(regex_pattern, test_string, expected_result):
    assert run_test_match_nfa(regex_pattern, test_string) == expected_result
    
if __name__ == "__main__":
    pytest.main()
