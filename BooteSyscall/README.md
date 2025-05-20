# Boot e syscall

Este é um texto em construção (2025-05-13) mas uso como material da disciplina.

## Introdução

Processadores comerciais, como Intel Core e ARM-CortexM são bem mais complexos que os processadores apresentados para fins didáticos, em especial no que tange ao gerenciamento de memória e acessos a dispositivos (periféricos)

... dois eventos no uso dos computadores atraem curiosidade: o *boot* e as *chamadas de sistema*.

No *boot* - quando o computador é ligado - o que ocorre até que o sistema operacional seja executado e o usuário possa usar o console?

Quando um programa de usuário é executado, frequentemente funções do sistema operacional são chamadas, por exemplo, quando o programa imprime uma mensagem na tela, quando lê/escreve em um arquivo, quando envia/recebe dados pela rede. Nessas situações, o programa faz uma chamada de sistema (syscall)... o que exatamente é isso?

## Boot

Durante o *boot*, assim que o processador inicia, ele ou lê informação de configuração e passa a executar um programa no endereço real 0x0 ou *simplesmente* executa um programa no endereço real 0x0.

Detalhes:
  - Intel: (referência não oficial) https://superuser.com/questions/344954/what-mode-do-modern-64-bit-intel-chip-pcs-run-the-boot-sector-in#:~:text=The%20boot%20loader%20starts%20execution,%22%20(ring%200)%20mode https://en.wikipedia.org/wiki/X86_assembly_language#Execution_modes 
  - ARM: (referência oficial) https://developer.arm.com/documentation/ka001193/latest/ , https://developer.arm.com/documentation/102412/0103/Privilege-and-Exception-levels/Exception-levels
  
O endereço 0x0 contém o a primeira instrução do programa  *Universal Extensible Firmware Interface* ou UEFI. Em computadores mais antigos o programa é *Basic Input Output System* ou BIOS. Esse programa é armazenado em ROM ou em memória FLASH.  UEFI tem muitos usos alternativos e características controversas (https://en.wikipedia.org/wiki/UEFI). No escopo deste texto, importa que este é o programa que é executado primeiro no computador, que o processador é iniciado em um modo com alguns privilégios (ié acesso ao gerenciamento de memória e ao registrador de estado do processador). Esses privilégios na execução de programas são dependentes de arquitetura, sendo que na arquitetura Intel usa-se *protection ring* e na arquitetura ARM usa-se *privilege*. Ambos são maneiras de limitar o acesso a partes do sistema (mais em: https://en.wikipedia.org/wiki/Protection_ring , https://developer.arm.com/documentation/dui0646/c/The-Cortex-M7-Processor/Programmers-model/Processor-mode-and-privilege-levels-for-software-execution , ). Por fim, o sistema operacional é executado, ao menos em parte, em modo privilegiado.

## syscall

https://developer.arm.com/documentation/102374/0102/System-calls
https://github.com/0xAX/linux-insides/blob/master/SysCall/linux-syscall-1.md
https://github.com/0xAX/linux-insides/tree/master


## Malware

https://www.kaspersky.com.br/resource-center/threats/virus-removal-for-android
https://en.wikipedia.org/wiki/Linux_malware
https://www.techtarget.com/searchsecurity/definition/RAT-remote-access-Trojan
https://www.avast.com/c-new-computer-viruses#:~:text=8Base%20tends%20to%20target%20businesses,executed%20without%20any%20further%20interaction.
