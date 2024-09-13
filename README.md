# Explanation of Input Data

5 -> number of words that will be added
demais 5 bastante numeroso demasiado abundante excessivo -> word addes / number of synonyms / synonyms
facil 2 simples ed -> word addes / number of synonyms / synonyms
elegante 3 natural descomplicado trivial -> word addes / number of synonyms / synonyms
nada 4 zero vazio osso nulo -> word addes / number of synonyms / synonyms
trabalho 3 atividade tarefa missao -> word addes / number of synonyms / synonyms
3 -> number of queries
facil
demais
zero

# Explanation of OutPut Data

[elegante->nada->facil] -> the path taken in the AVL tree to find the word facil
simples,ed -> synonyms of the word facil
[elegante->demais] -> the path taken in the AVL tree to find the word demais
bastante,numeroso,demasiado,abundante,excessivo -> synonyms of the word demais
[elegante->nada->trabalho->?] -> the word was not found in the tree.

- -> this line indicates that there are no synonyms to display
