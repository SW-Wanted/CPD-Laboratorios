# Laboratório 01

## Lista de comandos do Debugger gdb 
### 1. Início e Controle de Execução
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `gdb ./programa` | - | Inicia o GDB com o executável especificado. |
| `run` | `r` | Inicia a execução do programa do zero. |
| `continue` | `c` | Continua a execução após o programa ter parado num breakpoint. |
| `quit` | `q` | Sai do GDB. |
| `kill` | `k` | Interrompe a execução do programa atual sem sair do GDB. |

### 2. Gestão de Breakpoints (Pontos de Paragem)
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `break [local]` | `b` | Define um breakpoint. Ex: `b main`, `b list.c:36`. |
| `info breakpoints` | `i b` | Lista todos os breakpoints ativos e seus números de identificação. |
| `delete [num]` | `d` | Remove um breakpoint específico pelo número. |
| `disable [num]` | - | Desativa um breakpoint sem o remover. |
| `clear` | - | Limpa todos os breakpoints no local atual. |

### 3. Navegação Passo-a-Passo
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `next` | `n` | Executa a próxima linha. **Pula** o interior das funções (executa-as por inteiro). |
| `step` | `s` | Executa a próxima linha. **Entra** dentro das funções para depuração interna. |
| `finish` | - | Termina a execução da função atual e volta para quem a chamou. |
| `until [linha]` | - | Executa até chegar a uma linha específica (útil para sair de loops). |

### 4. Inspeção de Variáveis e Memória
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `print [var]` | `p` | Mostra o valor de uma variável no momento atual. |
| `print *[ponteiro]` | `p *` | Mostra o conteúdo da memória para onde o ponteiro aponta (desreferenciação). |
| `display [var]` | - | Mostra o valor da variável **automaticamente** em cada passo (`step/next`). |
| `undisplay [id]` | - | Remove uma variável da lista de display automático. |
| `list` | `l` | Mostra as linhas de código fonte ao redor da posição atual. |
| `watch [var]` | - | Para a execução sempre que o valor da variável `var` for alterado. |

### 5. Análise de Stack (Pilha de Chamadas) - Essencial para SegFaults
| Comando | Atalho | Descrição |
| :--- | :--- | :--- |
| `backtrace` | `bt` | Mostra a "árvore" de chamadas de funções que levaram ao ponto atual. |
| `frame [num]` | `f` | Muda o contexto para uma função específica na pilha (visto no `bt`). |
| `info locals` | - | Lista todas as variáveis locais da função no frame atual. |
| `info args` | - | Lista os argumentos passados para a função no frame atual. |

## 🛠 Ferramentas
* Linguagem C
* GCC (Compiler)
* GDB (Debugger)
* Make (Automation)
* Ambiente Linux/Ubuntu