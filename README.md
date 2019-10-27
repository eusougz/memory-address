
LABORATÓRIO DE SISTEMAS OPERACIONAIS - CEFET MG
Professor: Bruno

Alunos: Guilherme Giacomin e Uriel Braga


* Verificando endereços lógicos e físico (fork e memória compartilhada)
*** Descrição:

Implemente um procedimento que recebe, como parâmetro, o endereço lógico de uma variável 
e exibe o endereço físico correspondente.

Implemente um programa que: - declara uma variável (ex. int a;) - cria um novo processo 
fork) - exibe os endereços lógicos e físicos da variável nos dois processos (pai e filho). 


A variável poderá/deverá ter endereços lógicos distintos em cada processo? A variável 
poderá/deverá ter endereços físicos distintos em cada processo? Implemente e explique.

*** Respostas

A variável poderá/deverá ter endereços lógicos distintos em cada processo?

A variável possui o mesmo endereço lógico em cada processo. Isso se deve ao fato dos endereços lógicos do processo serem definidos em tempo de compilação. Com isso, no momento que o programa é compilado, a variável já possui seu endereço lógico definido e, indipendente de processos filhos,

A variável poderá/deverá ter endereços físicos distintos em cada processo? 

Os endereços físicos são definidos em tempo de execução já que a alocação de processos na memória utilizando tabela de páginas tem como principal característica a paginação por demanda, ou seja, somente no momento que ele precisar, ele vai procurar qual frame físico é correspondente ao endereço da tabela de páginas (contido no endereço lógico).
