# Gleamex

Regex Library for Gleam

Gleamex é uma biblioteca para a linguagem Gleam que se propõe a fornecer suporte a expressões regulares à linguagem.

## Instalação

### Dependências

A biblioteca Gleamex foi testada na versão 1.2.1 do Gleam, logo se recomenda que seja usada essa versão.

Para a instalação do Gleam, acesse a documentação oficial: [Link](https://gleam.run/getting-started/installing/)

Para realizar o build da biblioteca, é necessário ter as seguintes dependências:
* GCC
* make

No Ubuntu, é possível instala-las com o seguinte comando:
```sh
sudo apt-get update && sudo apt-get install gcc make -y
```

### Dependências do gleam
```
gleam_stdlib = "0.38.0"
```

### Dependências para os testes unitários (opcional)

Os testes unitários do Gleamex foram implementados em Pytest. Para executa-los, é necessário ter instalado o Python 3.11 e as seguites dependências:

```
matplotlib==3.9.0
pytest==7.4.3
```

Aviso: Outras versões do Python e das dependências dos testes não foram testadas.

### Build da biblioteca Gleamex

```sh
cd gleamex
```

```sh
make
```

### Problema com "erl_nif.h"
Caso haja problemas com o #include "erl_nif.h" durante a compilação, acesse o arquivo _Makefile_ e modifique a linha 13 ERLANG_INCLUDE_DIR.

Rode o seguinte comando:

```sh
find /home -name erl_nif.h
```
Copie o caminho terminado em *usr/include/erl_nif.h* e substitua *apenas* o caminho da pasta (só até o include) no comando a seguir como no exemplo para adicionar o arquivo no PATH:

Exemplo
```
echo 'export C_INCLUDE_PATH=/home/linuxbrew/.linuxbrew/Cellar/erlang/26.2.5/lib/erlang/usr/include' >> ~/.bashrc
source ~/.bashrc
```
Comando
```
echo 'export C_INCLUDE_PATH=SUBSTITUA_AQUI' >> ~/.bashrc
source ~/.bashrc
```

Após esse passo, tente novamente executar o  comando Make.

<!-- ```Makefile
ERLANG_INCLUDE_DIR=/home/linuxbrew/.linuxbrew/Cellar/erlang/26.2.5/lib/erlang/usr/include 
``` -->

## Uso (exemplo)

Para rodar um exemplo de uso, abra o terminal dentro da pasta "examples"

```sh
cd examples/
```

Inicie então a execução do gleam

```sh
gleam run
```

Note que o comando *Make* copia automaticamente a pasta *_build* para o diretório *examples/*. Para utilizar a biblioteca Gleamex com outros projetos Gleam é necessário copiar a pasta *_build* para a pasta raiz do novo projeto Gleam. 

Em futuras atualizações do Gleam, a maneira para incluir o Gleamex é passível de mudanças.

## Funções do Gleamex

### Match

Para usar a função match, os parâmetros de entrada são os seguintes:

* A strign da regex.
* String que se deseja testar. 

E o retorno é o seguinte:
* Valor booleano que é verdadeiro para quando há o match da string testada ou falso quando não há.

```javascript
let match_result = gleamex.match(regex_pattern, test_string)
```


### Search

Para usar a função search, os parâmetros de entrada são os seguintes:

* A strign da regex. 
* String que se deseja realizar a busca. 
* O index do limite inferiro da busca (incluindo o próprio)
* O index do limite superior da busca (excluindo o próprio)

E o retorno é o seguinte:
* Valor booleano que é verdadeiro para quando há o match para o intervalo ou falso quando não há.

```javascript
let search_result = gleamex.search(regex_pattern, test_string, start_index, end_index)
```
### Iterator

(Ainda não implementado)

### Replace

(Ainda não implementado)

## Documentação completa

[link]()
