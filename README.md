# simple-linux-device-driver
Device Driver (módulo) que desliga o computador de maneira imediata ou programada mediante a uma combinação de teclas do teclado

Este trabalho foi realizado para a Disciplina de Sistemas Operacionais, onde o objetivo ter uma primeira experiência em programação no "Kernel Space". O módulo funciona em ambientes Linux.
Para adicionar o módulo ao sistema você precisa:
  <br>1- Na pasta onde se encontram os arquivos, fazer um make:
    <code>$ make</code>
  <br>2- Inserir o módulo:
    <code>$ sudo insmod keyshut.ko</code>
  <br>3- Para remover:
    <code>$ sudo rmmod keyshut.ko</code>
  <br>4- Você pode ver as mensagens que o módulo "printa" ao fazer:
    <code>$ dmesg | tail -1</code>
     Para ver a última mensagem
     
<br><br>Funcionamento:
  Depois de inserido o módulo, você pode usa-lo para desligar o seu computador (shutdown) de duas maneiras: imediata e programada.
  Para desligar de maneira imediata pressione as teclas SHIFT + C simultaneamente;
  Para desligar de maneira programada pressione as teclas SHIFT + T simultaneamente e, logo após escolha o tempo no seguinte formato:
    Ex: 2h15m12s, para desligar o computador em 2 horas, 15 minutos e 12 segundo, sendo que as horas e os minutos podem sem dispensados, mas não os segundos.
