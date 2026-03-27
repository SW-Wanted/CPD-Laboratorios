# 🖥️ CPD-Terminal: Executor Paralelo de Processos

O **cpd-terminal** é um interpretador de comandos simplificado projetado para demonstrar conceitos de **Multiprocessamento** (Fork/Exec) e **Multithreading** (POSIX Threads) em ambientes Unix.

## 📝 O Desafio
Desenvolver um terminal capaz de lançar múltiplos programas em paralelo (background) e monitorar o tempo de execução de cada um através de uma tarefa monitora dedicada, garantindo a sincronização segura de dados.

---

## 🏗️ Arquitetura do Sistema
*   **Processo Pai (Principal)**: Responsável pela leitura de comandos e criação de processos filhos via `fork()`.
*   **Processo Filho**: Executa o programa solicitado via `execv()`.
*   **Thread Monitora**: Uma thread POSIX que executa em background, aguardando a terminação de processos (`wait`) e registrando estatísticas de tempo.
*   **Sincronização**: Utilização de `pthread_mutex` para evitar condições de corrida (race conditions) ao manipular a lista de processos partilhada.

---

## 🛠️ Como Utilizar

### Compilação Automática
O projeto utiliza um `Makefile` para gerir dependências:
```bash
make          # Gera o executável cpd-terminal
make test     # Gera o executavel fibonacci e testa o programa
make clean    # Limpa arquivos temporários
```

### Execução de Comandos
Você pode digitar comandos diretamente ou usar redirecionamento de arquivo:

```bash
# Manual
./cpd-terminal
$ /usr/bin/ls -l
$ fibonacci 10000
$ exit
```
```bash
# Via Script
./cpd-terminal < input.txt
```