# 📚 Laboratório 01:

## 🎯 Objectivos do Laboratório

Este laboratório cobre:
- **Estruturas de Dados**: Implementação de listas ligadas e gestão dinâmica de memória
- **Debugging Avançado**: Domínio da ferramenta GDB para análise e resolução de problemas
- **Automatização**: Utilização de Makefiles para simplificar compilação
- **Processamento Paralelo**: Conceitos de fork/exec e sincronização de processos
- **Multithreading**: Uso de POSIX Threads para execução concorrente
- **Gestão de Memória**: Compreensão de segfaults e análise de core dumps

---

## 📋 Estrutura do Laboratório

```
Laboratorio01/
├── ex01/           # Implementação de Lista Ligada com Processos
├── ex02/           # Sistema de Gestão de Tarefas
├── desafio/        # Terminal Paralelo (Fork/Exec + Threads)
└── README.md       # Este arquivo
```

---

## 🔧 Ferramentas Utilizadas

| Ferramenta | Versão | Propósito |
| :--- | :--- | :--- |
| **GCC** | 11.x+ | Compilador C POSIX |
| **GDB** | 12.x+ | Debugger para análise de código |
| **Make** | 4.x+ | Automatização de compilação |
| **POSIX Threads** | pthread | Multithreading |
| **SO** | Linux/Unix | Ambiente de execução |

---

## 📖 [Exercício 01](./ex01/): Implementação de Lista Ligada

**Objectivo**: Implementar e manipular uma lista ligada que armazena informações de processos.

### Conceitos Aprendidos
- **Estruturas (structs)**: Organização de dados relacionados
- **Ponteiros**: Manipulação de memória dinâmica
- **Listas Ligadas**: Estrutura de dados fundamental
- **Alocação Dinâmica**: `malloc()` e `free()`

### O que Implementar
```c
typedef struct lst_iitem {
   int pid;              // Identificador único do processo
   time_t starttime;     // Hora de início
   time_t endtime;       // Hora de término
   struct lst_iitem *next;  // Ponteiro para próximo elemento
} lst_iitem_t;
```

### Funções Principais
- `lst_new()`: Criar uma nova lista vazia
- `lst_destroy()`: Liberar toda a memória da lista
- `insert_new_process()`: Adicionar novo processo à lista
- `update_terminated_process()`: Atualizar tempo de conclusão de um processo

### Compilação
```bash
cd ex01
make              # Compila com símbolos de depuração (-g)
./main            # Executa o programa
make clean        # Remove ficheiros temporários
```

### 🐛 Debugging com GDB
```bash
gdb ./main
(gdb) b main              # Define breakpoint na função main
(gdb) r                   # Começa execução
(gdb) p *item             # Inspeciona conteúdo da estrutura
(gdb) s                   # Entra dentro da próxima função
(gdb) n                   # Executa próxima linha (sem entrar em funções)
(gdb) continue            # Continua até próximo breakpoint
(gdb) bt                  # Mostra pilha de chamadas
(gdb) q                   # Sai do GDB
```

---

## 📋 [Exercício 02](./ex02/): Sistema de Gestão de Tarefas

**Objectivo**: Criar um gestor de tarefas com filtros por prioridade.

### Conceitos Aprendidos
- **Múltiplas Listas**: Um array de listas (uma por nível de prioridade)
- **Operações CRUD**: Create, Read, Update, Delete
- **Processamento de Strings**: Manipulação de IDs textuais
- **Ordenação por Prioridade**: Gestão de dados categorizados

### O que Implementar
```c
typedef struct s_task {
    char id[MAX_ID];      // Identificador único da tarefa (até 64 carateres)
    struct s_task *next;  // Ponteiro para próximo elemento
} t_task;

// Array de 6 listas: índice = nível de prioridade (0-5)
t_task *lists[MAX_PRIORITY];
```

### Interface de Comandos
| Comando | Descrição |
| :--- | :--- |
| `new <prioridade> <id>` | Cria nova tarefa (prioridade 0-5) |
| `list <prioridade>` | Lista tarefas com prioridade >= valor |
| `complete <id>` | Remove tarefa da lista |
| `quit` | Sai do programa |

### Exemplo de Uso
```bash
cd ex02
make
./main

$ new 5 TarefaUrgente
$ new 2 TarefaBaixa
$ list 3                    # Mostra tarefas com prioridade >= 3
$ complete TarefaUrgente
$ list 0                    # Mostra todas as tarefas restantes
$ quit
```

### Comportamento Esperado
```
$ new 5 Task1
$ new 5 Task2
$ new 3 Task3
$ list 4
[5] Task1 -> [5] Task2     # Ordenado por inserção

$ complete Task1
$ list 4
[5] Task2                  # Task1 foi removida

$ list 0
[5] Task2 -> [3] Task3     # Mostra todas (prioridade >= 0)
```

---

## 🚀 [Desafio](./desafio/): Terminal Paralelo (CPD-Terminal)

**Objectivo**: Criar um interpretador de comandos que executa programas em background com monitoramento de processos.

### Conceitos Avançados
- **Fork/Exec**: Criação de processos filhos (`fork()`, `execv()`)
- **POSIX Threads**: Execução concorrente com pthread
- **Sincronização**: Mutexes para evitar race conditions
- **Wait()**: Aguardar término de processos filhos
- **Tratamento de Sinais**: Lidar com processos filhos mortos

### Arquitetura do Sistema

```
┌─────────────────────────┐
│  Processo Principal     │
│  (Loop de Comandos)     │
└──────────────┬──────────┘
               │
       ┌───────┴────────┐
       │                │
   ┌───▼────┐      ┌───▼────────┐
   │ Fork() │      │ Thread      │
   │ Exec() │      │ Monitora    │
   │        │      │ wait()      │
   └────────┘      └────────────┘
       │                │
   ┌───▼───┐        ┌──▼──────┐
   │ Filho │        │ Lista   │
   │ Proc. │        │ Processos
   │       │        │        │
   └───────┘        └────────┘
```

### Componentes Principais

#### 1. **Estrutura de Dados (list.h)**
```c
typedef struct lst_iitem {
   int pid;                // ID do processo
   time_t starttime;       // Quando começou
   time_t endtime;         // Quando terminou
   struct lst_iitem *next; // Próximo elemento
} lst_iitem_t;

typedef struct {
   lst_iitem_t *first;     // Cabeça da lista
} list_t;
```

#### 2. **Leitor de Linha de Comando (commandlinereader.h)**
Parse de comandos como: `/usr/bin/ls -l`, `fibonacci 1000`

#### 3. **Processo Principal (main.c)**
- Loop infinito à espera de comandos
- Fork executa cada comando num processo filho
- Amostras o PID do novo processo
- Adiciona à lista de processos

#### 4. **Thread Monitora**
- Executa `wait()` para detectar processos filhos terminados
- Actualiza `endtime` na lista
- Utiliza mutex para sincronização
- Evita race conditions

### Compilação e Execução
```bash
cd desafio
make              # Compila cpd-terminal e fibonacci
./cpd-terminal    # Inicia o programa
```

### Exemplos de Uso
```bash
./cpd-terminal
$ /usr/bin/ls -l              # Executa ls v background
Processo criado com PID: 12345
$ fibonacci 10000              # Calcula fibonacci de forma paralela
Processo criado com PID: 12346
$ ps aux | grep fibonacci     # Vê processos em execução
$ wait                        # Aguarda todos os processos
```

### Testes Automáticos
```bash
make test

# Executa 4 testes:
# 1. Teste Paralelo: 3 fibonacci's simultâneos
# 2. Teste Sequencial: 1 fibonacci
# 3. Teste de Erro: Comando inválido
# 4. Teste Exit: Sair sem filhos
```

---

## 🐛 Guia Completo de GDB (Debugger)

### 1️⃣ Início e Controle de Execução
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `gdb ./programa` | - | Inicia o GDB com o executável especificado. |
| `run <args>` | `r` | Inicia a execução do programa do zero. |
| `continue` | `c` | Continua a execução após o programa ter parado num breakpoint. |
| `quit` | `q` | Sai do GDB. |
| `kill` | `k` | Interrompe a execução do programa atual sem sair do GDB. |

### 2️⃣ Gestão de Breakpoints (Pontos de Paragem)
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `break [local]` | `b` | Define um breakpoint. Ex: `b main`, `b list.c:36`. |
| `break [func]` | `b` | Define breakpoint numa função. Ex: `b insert_new_process`. |
| `info breakpoints` | `i b` | Lista todos os breakpoints ativos com números. |
| `delete [num]` | `d` | Remove um breakpoint específico pelo número. |
| `disable [num]` | - | Desativa um breakpoint sem o remover. |
| `enable [num]` | - | Reativa um breakpoint desativado. |
| `clear` | - | Limpa todos os breakpoints no local atual. |

#### Exemplo Prático:
```bash
gdb ./main
(gdb) b main                      # Breakpoint no início
(gdb) b list.c:44                 # Breakpoint na linha 44 de list.c
(gdb) b insert_new_process        # Breakpoint quando a função é chamada
(gdb) info breakpoints            # Mostra all breakpoints
(gdb) delete 1                    # Remove breakpoint 1
(gdb) disable 2                   # Desativa breakpoint 2 (mantém para depois)
(gdb) r                           # Começa execução até breakpoint
```

### 3️⃣ Navegação Passo-a-Passo
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `next` | `n` | Executa a próxima linha. **Pula** o interior das funções. |
| `step` | `s` | Executa a próxima linha. **Entra** dentro das funções. |
| `finish` | - | Sai da função actual e volta para quem a chamou. |
| `until [linha]` | - | Executa até chegar a uma linha específica. |

#### Exemplo Prático:
```bash
(gdb) s              # Entra dentro de insert_new_process()
(gdb) n              # Próxima linha sem entrar em funções
(gdb) s              # Entra em malloc() para ver internamente
(gdb) finish         # Sai de malloc() e volta
(gdb) until 50       # Executa até linha 50
```

### 4️⃣ Inspeção de Variáveis e Memória
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `print [var]` | `p` | Mostra o valor de uma variável. |
| `print *[ponteiro]` | `p *` | Dereferencia um ponteiro e mostra conteúdo. |
| `print &[var]` | `p &` | Mostra o endereço de memória de uma variável. |
| `print [ptr]->[field]` | - | Acessa campo de struct via ponteiro. |
| `print [var][índice]` | - | Acessa elemento de array. |
| `display [var]` | - | Mostra valor **automaticamente** em cada passo. |
| `undisplay [id]` | - | Remove variável da lista de display. |
| `list` | `l` | Mostra linhas de código fonte ao redor. |
| `watch [var]` | - | Para a execução quando `var` for alterada. |

#### Exemplo Prático:
```bash
(gdb) b insert_new_process
(gdb) r
(gdb) n               # Próxima linha
(gdb) p list          # Mostra valor de 'list'
(gdb) p *list         # Dereferencia 'list' (mostra estrutura)
(gdb) p &list         # Endereço de memória de 'list'
(gdb) p item->pid     # Acessa campo 'pid' através de 'item'
(gdb) p items[0]      # Primeiro elemento de array
(gdb) display item->pid  # Mostra pid automaticamente
(gdb) watch endtime   # Para se 'endtime' for modificado
```

### 5️⃣ Análise de Stack (Pilha de Chamadas) - **ESSENCIAL PARA SEGFAULTS**
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `backtrace` | `bt` | Mostra a "árvore" completa de chamadas até ao ponto actual. |
| `frame [num]` | `f` | Muda o contexto para uma função específica na pilha. |
| `info locals` | - | Lista **todas** as variáveis locais da função actual. |
| `info args` | - | Lista **todos** os argumentos passados para a função. |

#### Exemplo Prático:
```bash
(gdb) bt              # Mostra pilha completa:
                      # #0 update_terminated_process at list.c:50
                      # #1 main at main.c:25
                      # #2 __libc_start_main ... (runtime)
(gdb) f 1             # Muda para frame 1 (main)
(gdb) info locals     # Mostra todas as variáveis locais de main
(gdb) info args       # Mostra argumentos de main
(gdb) f 0             # Volta para frame 0 (update_terminated_process)
(gdb) p pid           # Mostra 'pid' no contexto actual
```

---

## ⚠️ Segmentation Fault (SEGFAULT) - Diagnóstico e Resolução

### O que é um Segmentation Fault?
Um **Segfault** ocorre quando o programa tenta aceder a uma posição de memória que não lhe pertence:
- Dereferenciação de ponteiro NULL
- Acesso a memória após `free()`
- Buffer overflow
- Stack overflow
- Acesso a array fora dos limites

### 🔍 Diagnosticando Segfaults com Core Dumps

#### Ativar Core Dumps
```bash
# Ver limite actual
ulimit -c

# Ativar core dumps (linux/unix)
ulimit -c unlimited

# Verificar (deve retornar "unlimited")
ulimit -c
```

#### Executar Programa e Gerar Core Dump
```bash
./main
# Segmentation fault (core dumped)

# Verificar se core foi criado
ls -la core
# -rw------- 1 user user 5242880 mar 23 10:45 core
```

#### Analisar Core Dump com GDB
```bash
gdb ./main core

# Dentro do GDB:
(gdb) bt              # Mostra onde o segfault aconteceu
(gdb) frame 0         # Vai para o frame onde crashou
(gdb) info locals     # Mostra valores das variáveis locais
(gdb) info args       # Mostra argumentos da função
(gdb) print *ptr      # Verifica se ponteiro é válido
```

#### Exemplo: Debuggando um Segfault
```bash
# Programa crashes
$ ./main
Segmentation fault (core dumped)

# Análise
$ gdb ./main core
(gdb) bt
#0 0x00005555555546d5 in update_terminated_process (list=0x0, pid=1234, endtime=1679567100) at list.c:50
#1 0x0000555555554789 in main () at main.c:35
#2 0x00007ffff7a05f90 in __libc_start_main ()

# O problema: 'list' é NULL!
(gdb) frame 0
(gdb) p list
$1 = (list_t *) 0x0      # Confirma: list é NULL

# Volta para main para ver o problema
(gdb) frame 1
(gdb) info locals        # Mostra como list foi inicializado errado
```

### Causas Comuns e Soluções

| Causa | Sintoma | Solução |
| :--- | :--- | :--- |
| Ponteiro NULL | `p *NULL` dereferencia NULL | Verificar `if (ptr != NULL)` |
| Memória liberada | Usar `free()` e depois usar | Não usar após `free()` |
| Array overflow | Acesso além dos limites | Verificar índices |
| Pilha cheia | Recursão infinita | Condição de paragem |
| Corrupção de heap | Comportamento aleatório | Verificar alocs/deallocs |

---

## 🔨 Automatização com Make

### 🎯 O que é Make?
Make é uma ferramenta que:
- Define **regras** de compilação
- Detecta **dependências** entre ficheiros
- Executa apenas o **necessário** (não recompila ficheiros inalterados)
- Simplifica comandos complexos

### Estrutura de um Makefile

```makefile
# Variáveis
CC = gcc              # Compilador
CFLAGS = -g -Wall    # Flags do compilador
LDFLAGS = -lpthread  # Flags do linker

# Targets (alvo para compilar)
target: dependencies
	commands

# Exemplo:
main: list.o main.o
	$(CC) -o main list.o main.o $(LDFLAGS)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

main.o: main.c list.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main
```

### Exemplos de Makefile

#### Exercício 01 (Simple)
```makefile
CC = gcc
CFLAGS = -g

main: list.o main.o
	$(CC) -o main list.o main.o

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

main.o: main.c list.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main
```

#### Desafio (Com Threads)
```makefile
CC = gcc
CFLAGS = -g -I ./includes

# Target principal
cpd-terminal: main.o list.o commandlinereader.o
	$(CC) -o cpd-terminal main.o list.o commandlinereader.o -lpthread

# Targets para object files
main.o: main.c ./includes/list.h ./includes/commandlinereader.h
	$(CC) $(CFLAGS) -c main.c

list.o: ./srcs/list.c ./includes/list.h
	$(CC) $(CFLAGS) -c ./srcs/list.c

commandlinereader.o: ./srcs/commandlinereader.c ./includes/commandlinereader.h
	$(CC) $(CFLAGS) -c ./srcs/commandlinereader.c

# Target de teste
fibonacci: ./srcs/fibonacci.c
	$(CC) $(CFLAGS) -o fibonacci ./srcs/fibonacci.c

test: cpd-terminal fibonacci
	./cpd-terminal < tests/input.txt

clean:
	rm -f *.o cpd-terminal fibonacci
```

### Comandos Make

| Comando | Resultado |
| :--- | :--- |
| `make` | Compila tudo (ou o que mudou) |
| `make clean` | Remove ficheiros compilados |
| `make test` | Compila e executa testes |
| `make TARGET` | Compila target específico |

### Como Funciona o Make

```
$ make main

# Make verifica:
# 1. Existe main?
#    Não → precisa compilar
# 2. Dependências: list.o e main.o
#    - list.o existe e list.c não mudou? Sim → skip
#    - main.o existe e main.c não mudou? Sim → skip
# 3. Se alguma dependência mudou, recompila main

# Resultado: Apenas ficheiros alterados são recompilados
```

### 💡 Boas Práticas
1. **Use variáveis** para compilador e flags
2. **Defina dependências** correctamente
3. **Adicione phony targets** como `clean`, `test`
4. **Use `-g`** flag para debugging (símbolos)
5. **Use `-Wall`** para ativar warnings

---

## 📚 Resumo de Conceitos Aprendidos

### Nível 1: Básico
✅ Compilação em C com GCC  
✅ Estruturas (structs) e alocação dinâmica de memória  
✅ Ponteiros e desreferenciação  
✅ Funções e passagem de parâmetros  

### Nível 2: Intermédio
✅ Listas ligadas (linked lists)  
✅ Operações CRUD em estruturas dinâmicas  
✅ Organização de código (headers e implementação)  
✅ Automatização com Makefiles  

### Nível 3: Avançado
✅ Debugging completo com GDB  
✅ Análise de stack e variáveis em tempo de execução  
✅ Identificação e resolução de memory leaks  
✅ Processamento paralelo com fork/exec  
✅ Multithreading com POSIX Threads  
✅ Sincronização com mutexes  
✅ Tratamento de core dumps  

---

## 🎓 Checklist de Domínio

Após completar este laboratório, deverá estar apto a:

### ✓ Estruturas de Dados
- [ ] Implementar listas ligadas do zero
- [ ] Compreender ponteiros e alocação dinâmica
- [ ] Evitar memory leaks
- [ ] Manipular múltiplas listas

### ✓ Debugging
- [ ] Usar GDB com confiança
- [ ] Definir e gerir breakpoints
- [ ] Analisar stack traces
- [ ] Inspecionar memória
- [ ] Diagnosticar segfaults com core dumps

### ✓ Build & Deploy
- [ ] Escrever Makefiles funcionais
- [ ] Compilar com flags corretas
- [ ] Gerir dependências
- [ ] Limpar automaticamente

### ✓ Programação Avançada
- [ ] Criar processos filhos com fork/exec
- [ ] Implementar threads POSIX
- [ ] Sincronizar com mutexes
- [ ] Monitorar processos filhos

---

## 🚀 Próximos Passos

1. **Valgrind**: Ferramenta para detectar memory leaks
   ```bash
   valgrind ./main
   ```

2. **AddressSanitizer**: Detector de erros de memória
   ```bash
   gcc -fsanitize=address -g main.c
   ```

3. **Optimization**: Compilar com `-O2` ou `-O3`
   ```bash
   gcc -O2 -o main main.c
   ```

4. **Profiling**: Analisar performance com `gprof`
   ```bash
   gcc -pg -o main main.c
   ./main
   gprof main gmon.out
   ```

---

## 📞 Referências Rápidas

### GDB Cheatsheet
```bash
gdb ./program
(gdb) b main              # Breakpoint
(gdb) r                   # Executa
(gdb) n                   # Next
(gdb) s                   # Step
(gdb) c                   # Continue
(gdb) p var               # Print
(gdb) bt                  # Backtrace
(gdb) q                   # Quit
```

### Sistema de Ficheiros Lab
```
ex01/  → Lista simples com processos
ex02/  → Arrays de listas com prioridades
desafio/ → Fork/Exec + Threads + Sincronização
```

### Compilação Rápida
```bash
# Com debugging
gcc -g -o main main.c list.c

# Com threads
gcc -g -o prog main.c -lpthread

# Com warnings
gcc -g -Wall -Wextra -o prog main.c

# Otimizado
gcc -O2 -o prog main.c
```

---

**Parabéns! Completou o Laboratório 01! 🎉**

Tem agora uma compreensão sólida de C, debugging, estruturas de dados e processamento de múltiplos processos.