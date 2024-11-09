Definição de escopo do que iremos fazer

########### NOME DO ARQUIVO .txt ###########
data no formato: DDMMAAAA.txt
exemplos: 08112024.txt

ver se em time.h tem como validar data (ver se a data realmente existe)
se não tiver, dia válido: 1 a 31, mês válido: 1 a 12, ano: <= ano corrente (pelo menos isso)
	
########### LAYOUT DO ARQUIVO .txt ###########
status;data;sala;hora_inicio;hora_fim;monitor_s/n;CPF;nome;celular;prof_responsavel;evento

usar como delimitador dos campos o ponto e vírgula, muito usado em arquivos bancários, melhor visualmente para ler no arquivo txt
					
########### DEFINIÇÃO DOS CAMPOS ###########
					
> status = ativo / cancelado / modificado (linha só será lida se o status != cancelado)
			- quando for cancelar um agendamento troca o status de ativo para cancelado
			- quando for alterar um agendamento altera o status de ativo/modificado para modificado 
					(segurança, saber que alguém alterou o agendamento inicial)
			- cancelados nunca serão mostrados, fica no arquivo como histórico

> data = formato: DDMMAAAA (por segurança guardar também a data dentro do arquivo, facilita mostrar a data do agendamento)

> sala = formato: definir em arquivo a parte, lista de salas que podem ser agendadas
Seria uma dropdown list para selecionar (prático)
Evita agendar a mesma sala duas vezes caso coloquem nomes levemente diferentes
Nesse projeto o escopo não inclui cadastramento de salas (tempo limitado)

> hora_inicio = no tela: hora início: [__:__] (h/min) / hora fim: [__:__] (h/min)
				só pode digitar números 
				hora: 01 até 24 / minuto: 1 até 59
				guardar no arquivo em horário militar, ex: 1250

> hora_fim = garantira que seja maior que hora_inicio

> monitor_s/n = se responder 's' (sim), precisa preencher o campo de prof_responsavel
				se 'n' (nao), gravar branco (espaço vazio) no prof_responsavel (não deixar digitar no agendamento)
		
> CPF = formato no tela: [___.___.___-__]
		formato no arquivo: 11 characteres numericos, ex: 123123123-12
		se existir uma rotina de digito verificador de CPF na internet podemos usar (validar CPF = professor vai gostar)

> nome = nome da pessoa que requisitou a sala, limite de caracteres 60, tem que ser diferente de branco (tem que preencher)

> celular = formato na tela: (__) _.___.___   
			formato no arquivo: 123123456789
			justificativa para o DDD: existem pessoas com celular DDD diferente de 081 (outros estados)
			por exemplo, pedro meu amigo é de espirito santo e nunca mudou o DDD

> prof_responsavel = nome do professor do responsavel, limite de caracteres 60, tem que ser diferente de branco (tem que preencher) se monitor_s/n for igual a 's'

> evento = nome/descrição curta do evento, limite de caracteres 100, tem que ser diferente de branco (tem que preencher)
			se a pessoa digitar somente um caracter pedir pra preencher direito, pelo menos 10 caracteres
			tem que dizer o motivo do agendamento
			(exemplo: um ponto (invalido), dois pontos (invalido), até 9 caracteres (invalido), monitoria (invalido, não disse de que)

########### GUI ###########
Ainda falta definir:
Basicamente tudo relacionado a GUI com o GTK
- Tela de agendamento
- Tela para consulta (Mostrar eventos de uma data/sala)
        ver detalhes / alterar / cancelar (tela de agendamento
Já organizei algumas sugestões, depois mostro
  
