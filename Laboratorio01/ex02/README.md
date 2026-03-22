# 2. Gestor de tarefas pessoais
Construa uma aplicação de organização pessoal que permite gerir as tarefas pendentes de uma pessoa. Cada tarefa: é identificada por uma sequência de caracteres única que, por simplicidade, não pode conter espaços;
tem uma prioridade, definida por um inteiro entre 0 e 5 (5 é mais prioritária, 0 é menos prioritária).

A aplicação tem os seguintes comandos:
```bash
$ new <prioridade> <id-nova-tarefa>
```
> que insere a nova tarefa;

```bash
$ list <prioridade>
```
> que lista todas as tarefas com tarefa da prioridade indicada ou superior; a listagem deve estar ordenada por prioridade (mais prioritárias primeiro) e, entre tarefas igualmente prioritárias, por data de criação (mais recentes primeiro);
```bash
$ complete <id-nova-tarefa>
```
> que retira a tarefa indicada; caso a tarefa não exista, deve ser apresentada a mensagem de erro "TAREFA INEXISTENTE”.

>Sugestão: usar tantas listas quanto níveis de prioridade.