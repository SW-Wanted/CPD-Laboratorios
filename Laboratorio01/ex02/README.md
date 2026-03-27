# Exercício 02: Gestor de Tarefas Pessoais

Criar um sistema que gerencie tarefas identificadas por IDs únicos, com níveis de prioridade de 0 (mínima) a 5 (máxima). O sistema deve suportar:
1.  **Inserção** de novas tarefas.
2.  **Listagem** filtrada por prioridade (ordenada da mais prioritária para a menos prioritária).
3.  **Conclusão** de tarefas com remoção da lista.

---

## 🚀 Comandos

| Comando | Descrição |
| :--- | :--- |
| `new <prioridade> <id>` | Adiciona uma tarefa ao nível especificado (0-5). |
| `list <prioridade>` | Lista tarefas com nível igual ou superior ao informado. |
| `complete <id>` | Finaliza e remove a tarefa. Retorna erro se o ID não existir. |

## 💡 Exemplo de Uso
```bash
$ new 5 UrgentTask
$ new 2 LowPriorityTask
$ list 3
$ complete UrgentTask
$ quit