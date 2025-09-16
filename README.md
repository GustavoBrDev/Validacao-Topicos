# Sobre o Repositório

O repositório consta com a 'resposta' da prova de validação do componente curricular de **Tópicos Especiais em Programação I**. Esta foi realizada pelo aluno *Gustavo Stinghen* no dia **16/09/2025**.

## Sobre o Enunciado

Foi escolhido o primeiro enunciado, que diz:

> Desenvolva um sistema de automação residencial onde
dispositivos (como lâmpadas, ventiladores ou relés) podem ser
controlados remotamente via interface web. O NodeMCU atua
como o controlador dos dispositivos e se comunica com o servidor
Django via MQTT. A interface web permite ligar/desligar
dispositivos e visualizar o status atual de cada um.

## Estrutura

O repositório consta com esse READ ME, o arquivo de Server (express), os módulos necessários e toda a interface Web. Além disso no diretório **"Arduino"** há a presença do código desenvolvido no Wokwi juntamente com o json do circuito.

## Acessando o Wowki

Em tese, o código estará disponível no [Wowki](https://wokwi.com/projects/442281521046333441), mas já presenciei casos que o mesmo não ocorreu. No pior dos casos, favor me contatar pois possuo uma cópia do mesmo.

## Rodando em Ambiente local

Primeirmente prossiga até o Wowki e crie um novo projeto com o ESP32 ou utilize do link para o mesmo:

https://wokwi.com/projects/new/esp32.

Copie e cole o conteudo do arquivo **sketch.json** em **diagram.json** para montar o circuito. Em seguida, cole o conteudo do arquivo **codigo.cpp** na janela ao lado. 

Após isso prossiga para **Library Manager** e adicione (botão azul) as seguintes bibliotecas:

* PubSubClient
* Wifi

*Atente-se aos nomes, eles devem ser idênticos*

Após isso, realize o download do arquivo web (todo o repositório) e abra na IDE que preferir. Acesse via terminal o diretório atual e rode o servidor com o comando:

```bash
node server.js
```

O servidor express iniciará na porta **3000**. Após isso, utilize o **Live Server** ou outra extensão para visualizar a estética do aplicativo.

Se todos os passos prosseguirem corretamente, você verá o ESP se comunicando com a Web e vice-versa.