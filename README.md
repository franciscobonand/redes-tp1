# Redes de Computadores - TP1

## Introdução
O Trabalho Prático 1 teve como proposta a implementação de um sistema Cliente-Servidor que utiliza o protocolo TCP para comunicação, ficando a cargo do usuário a utilização em IPv4 ou IPv6. O sistema desenvolvido permite a gestão de coordenadas (X, Y) que representam a localização geográfica de locais de vacinação, de forma que o usuário pode, por meio de comandos do Cliente, adicionar, remover e listar os locais cadastrados, assim como consultar qual o local de vacinação cadastrado que está mais próximo de um ponto (X, Y) informado (esse cálculo é feito com base na distância Euclidiana dos pontos).  

Neste documento será detalhada a usabilidade do Cliente, assim como os detalhes e decisões de implementação do Cliente e do Servidor, de forma a permitir o melhor entendimento dessas estruturas e abordar algumas dificuldades que surgiram durante a implementação.

## Restrições
Nesse tópico serão listadas as restrições da estrutura de dados que representa os locais de vacinação, assim como aquelas relacionadas à comunicação entre o Cliente e o Servidor:  
- Os valores de X e Y devem ser números inteiros entre 0 e 9999
- Um mesmo local (X, Y) não pode ser cadastrado mais de uma vez (não existem cadastros repetidos)
- Não podem haver mais de 50 locais cadastrados no sistema
- As mensagens enviadas entre Cliente-Servidor devem ter um tamanho máximo de 500 bytes
- As mensagens enviadas do Cliente para o Servidor devem possuir o caractere ‘\n’ para indicar o final de um comando válido
- Vários Clientes podem se conectar ao mesmo Servidor, porém a sua execução é sequencial (Clientes distintos não têm seus comandos executados em paralelo)
- Um Cliente é automaticamente encerrado caso envie um comando inválido para o Servidor

## Estrutura de dados
A estrutura utilizada para armazenamento das coordenadas cadastradas pelo usuário, denominada Locations, possui apenas dois elementos: um contador, que armazena quantos locais de vacinação estão cadastrados, e um arranjo de tamanho 50 de uma outra subestrutura denominada Coordinates. Essa segunda estrutura, por sua vez, é composta apenas por dois campos “X” e “Y”, que são os inteiros utilizados para representar as coordenadas de um local de vacinação.
                      

## Comandos
A comunicação Cliente-Servidor é feita por meio do uso de seis comandos distintos, sendo que quatro são voltados para a manutenção do registro dos locais de vacinação e os outros dois para a gestão do ciclo de vida do Cliente e do Servidor. A seguir estão listados os comandos e seus retornos esperados:

**add X Y**  
Cadastra os pontos (X, Y) informados no registro de locais de vacinação. Os possíveis retornos para esse comando são:  
- “X Y added”: ocorre quanto a coordenada informada é válida e foi cadastrada com sucesso
- “X Y already exists”: ocorre quando a coordenada informada é válida, porém já está cadastrada
- “limit exceeded”: ocorre quando já existem 50 locais distintos cadastrados
- “error”: ocorre quando o comando é inválido (por exemplo, quando X ou Y são inferiores a 0 ou superiores a 9999). Caso receba esse retorno, o Cliente é encerrado  

**rm X Y**  
Remove os pontos (X, Y) do registro de locais de vacinação. Os possíveis retornos para esse comando são:  
- “X Y removed”: ocorre quanto a coordenada informada é válida e foi removida com sucesso
- “X Y does not exist”: ocorre quando a coordenada informada é válida, porém esse registro não está cadastrado
- “error”: ocorre quando o comando é inválido (por exemplo, quando X ou Y são inferiores a 0 ou superiores a 9999). Caso receba esse retorno, o Cliente é encerrado  

**list**  
Lista todos os registros de locais de vacinação que encontram-se cadastrados. Os possíveis retornos para esse comando são:  
- “X1 Y1 X2 Y2 … Xn Yn ”: ocorre quando existem registros. Esse retorno representa todos os registros cadastrados, de (X1, Y1) até (Xn, Yn)
- “none”: ocorre quando não existem registros cadastrados
- “error”: ocorre quando o comando é inválido. Caso receba esse retorno, o Cliente é encerrado  

**query X Y**  
Retorna o conjunto de pontos (X, Y) mais próximo das coordenadas informadas no comando, baseando-se na distância Euclidiana entre os pontos. Os possíveis retornos para esse comando são:
- “X Y”: ocorre quando a coordenada informada é válida e existem registros cadastrados. Representa o ponto cadastrado mais próximo das coordenadas (X, Y) informadas pelo usuário
- “none”: ocorre quando não existem registros cadastrados
- “error”: ocorre quando o comando é inválido (por exemplo, quando X ou Y são inferiores a 0 ou superiores a 9999). Caso receba esse retorno, o Cliente é encerrado  

**exit**  
Utilizado para encerrar o Cliente atual. Após este comando, o Cliente é desconectado do Servidor e encerrado, e o Servidor torna-se disponível para tratar requisições de demais clientes.  

**kill**  
Utilizado para encerrar tanto o Cliente atual quanto o Servidor. Após esse comando, o Cliente é desconectado e encerrado, e o Servidor desligado.  

## Mensagens
Como dito anteriormente, a comunicação entre Cliente e Servidor é feita por meio do envio de mensagens de até 500 bytes e que possuam ‘\n’ para identificar o final de um comando. Nesse tópico, será abordado o recebimento e posterior tratamento dessas mensagens.

Primeiramente, é sabido que uma mensagem K enviada entre as estruturas do Cliente e do Servidor não será necessariamente recebida inteira de uma só vez. Ou seja, se o Cliente envia uma mensagem K para o Servidor pode ser que, ao realizar a primeira leitura dessa requisição, o Servidor só receba uma parte K1 da mensagem (onde K = K1 + K2 + … + Kn). Dessa forma torna-se necessário que o Servidor efetue diversas leituras, até que a mensagem seja lida por completo.

A fim de lidar com essa questão e realizar o número exato de leituras necessárias (tanto por parte do Cliente quanto por parte do Servidor), todas as mensagens enviadas no sistema possuem um prefixo do tipo “número-”, onde esse número representa o tamanho da mensagem que está sendo enviada. Dessa forma, a estrutura que recebe a mensagem sabe exatamente o tamanho da informação que deve ser lida, e, portanto, pode realizar leituras até que tenha lido uma quantidade igual ao número informado pelo prefixo. Um exemplo de mensagem nesse formato:  

`"29-add 123 123\nadd 321 321\nlist\n"`

## Desafios
Dentre os desafios encontrados durante a implementação do sistema Cliente-Servidor, como a solução descrita acima para o recebimento “parcelado” de mensagens, o que mais se destacou foi o tratamento dos comandos recebidos pelo Servidor, com o enfoque principalmente em casos que a mensagem enviada possui múltiplos comandos (como a mensagem ilustrada acima).

Fazer o parsing de uma string pareceu, no primeiro momento, algo simples e que possivelmente alguma biblioteca padrão da linguagem teria uma solução trivial. Todavia, fazer a divisão de uma string de comandos primeiro pelo caractere ‘\n’ para separar os diferentes comandos, e depois pelo caractere “ “ (espaço em branco) para separar as diferentes informações do comando provou ser definitivamente mais difícil que o esperado.

O uso de funções como strtok para fazer o parsing foi um pouco confuso e gerou diversos problemas devido ao seu funcionamento não-trivial quando comparado ao de uma função comum de split para strings implementada em outras linguagens de programação como Javascript, Python e Golang. Dessa forma, para realizar o tratamento e interpretação de comandos foram implementadas diversas funções auxiliares, que são responsáveis por fazer o parsing de strings, a conversão de strings para inteiros (e vice-versa),  “montar” as mensagens a serem retornadas pelo Servidor, dentre outros.

Além disso, na estrutura do Servidor optei por fazer funções que recebem a referência de um arranjo de caracteres para que aquele arranjo seja alterado e receba a mensagem a ser retornada para o Cliente. Isso foi feito dessa forma dado que fazer funções que retornem uma string acabava por gerar alguns bugs que não eram facilmente reproduzíveis e rastreáveis.

Por fim, foi adicionado um delay de 1 segundo entre as respostas do Servidor. Isso foi feito para lidar com bugs que não estavam fazendo muito sentido, visto que, apesar da implementação da comunicação entre Cliente e Servidor estar correta, por vezes o Cliente solicitava o cadastro de múltiplos locais de vacinação distintos e esses locais eram cadastrados, mas o Servidor não retornava as mensagens de confirmação de cadastro para todos os locais (mesmo mostrando em seus logs a efetivação de todos os cadastros). Dessa forma, viu-se necessária a adição desse delay de 1 segundo para que o Servidor enviasse corretamente todas as respostas ao Cliente. 

