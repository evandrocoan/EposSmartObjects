
Projeto "Controle integrado e distribuído de luminárias"

Os requisitos funcionais do sistema poderiam ser os seguintes:

- Permitir que a luminária possa cadastrar-se, informando seu ID e os serviços que ela executa (controle de intensidade de cada cor, etc) a um servidor, e saber quando ela já foi cadastrada nesse servidor. Isso deve permitir que posteriormente ela receba mensagens com comandos.

- Através de mensagens recebidas, controlar a cor e a intensidade do brilho de uma luminária de LEDs RGB (usando PWM em hardware), fazendo essas intensidades fixas ou mudando com o tempo

- Através de mensagens recebidas, cadastrar uma configuração de cores e intensidades para cada possível usuário, e aplicar essa configuração quando um usuário está presente (informação recebida via mensagem)

- Permitir que se a mensagem de comando recebida não for para esta luminária, reencaminhar a mensagem para outros objetos inteligentes

 

Como requisitos não-funcionais, acho que tempos, principalmente:

- Ser implementado com EPOS 1.1 na placa EposMotesII

 

Acho que vocês precisarão de 3 EposMotesII: Dois para controlar luminárias diferentes e um para funcionar como gateway ou para indicar a presença de uma pessoa.
