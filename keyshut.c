#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/reboot.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
 
 
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Esdras de Lima Chaves <esdraslchaves@gmail.com>");
MODULE_DESCRIPTION("Modulo que desliga o computador de maneira imediata ou programada a partir de teclas pressionadas");
 
 
 
// Definições de quais índices do vetor tecla[] representam quais teclas
#define T_shift 0
#define T_c 1
#define T_t 2
 
 
// Definição dos scancodes das teclas "chaves"
#define SHIFT 0x2A
#define C 0x2E
#define T 0x14
 
 
// Definição das váriaveis de "transformação" do scancode e status
#define scan_const 0x7F
#define stat_const 0x80
 
 
// Definição das teclas que representam segundos, minutos e horas
#define seg 0x1F
#define mnt 0x32
#define hor 0x23
 
 
// Definição das teclas numéricas
#define k0 0x0b
#define k1 0x02
#define k2 0x03
#define k3 0x04
#define k4 0x05
#define k5 0x06
#define k6 0x07
#define k7 0x08
#define k8 0x09
#define k9 0x0A
 
 
// Declaração da estrutura do timer a ser utilizado
static struct timer_list my_timer;
 
 
// Declaração e inicialização de variáveis de controle
int tecla[3] = {0, 0, 0};
int tempo = 0;
int tempoaux = 0;
int set_timer = 0;
 
 
 
// Procedimento que desliga o computador
void computer_shutdown(void) {
    orderly_poweroff(true);
}
 
 
/*
*  Procedimento que executa quando o timer é setado,
*  impede que o desligamento programado seja chamado novamente e
*  programa o computar para desligar
*/
void my_timer_callback(unsigned long data) {
    computer_shutdown();
}
 
 
// Handler de interrupção do teclado
irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    static unsigned char scancode;
    unsigned char status;
 
    status = inb(0x64);
    scancode = inb(0x60);
 
 
    // Guarda quais teclas estão sendo pressionadas
    if((scancode  & scan_const) == SHIFT  && (scancode & stat_const) == 0)
        tecla[T_shift] = 1;
 
    if((scancode  & scan_const) == SHIFT && (scancode & stat_const) != 0)
        tecla[T_shift] = 0;
 
    if((scancode  & scan_const) == C && (scancode & stat_const) == 0)
    tecla[T_c] = 1;
 
    if((scancode  & scan_const) == C && (scancode & stat_const) != 0)
    tecla[T_c] = 0;
 
    if((scancode  & scan_const) == T && (scancode & stat_const) == 0)
    tecla[T_t] = 1;
 
    if((scancode  & scan_const) == T && (scancode & stat_const) != 0)
    tecla[T_t] = 0;
 
 
   
 
    // Lê o tempo desejado
    if (set_timer == 1 && (scancode & stat_const) == 0) {
    
        switch(scancode & scan_const) {
            case k0:
        	tempoaux *= 10;
            break;
        
	    case k1:
        	tempoaux *= 10;
        	tempoaux += 1;
            break;
    
	    case k2:
	        tempoaux *= 10;
	        tempoaux += 2;
            break;
    
	    case k3:
	        tempoaux *= 10;
	        tempoaux += 3;
            break;
    
	    case k4:
	        tempoaux *= 10;
	        tempoaux += 4;
            break;
        
	    case k5:
        	tempoaux *= 10;
        	tempoaux += 5;
            break;
    
	    case k6:
	        tempoaux *= 10;
	        tempoaux += 6;
            break;
    
	    case k7:
	        tempoaux *= 10;
	        tempoaux += 7;
            break;
        
	    case k8:
        	tempoaux *= 10;
	        tempoaux += 8;
            break;
   
   	    case k9:
	        tempoaux *= 10;
	        tempoaux += 9;
            break;
    
	    case seg:
 	    	tempo += tempoaux * 1000;
 	        tempoaux = 0;
		set_timer = -1;      
        	setup_timer(&my_timer, my_timer_callback, 0);           
        	mod_timer(&my_timer, jiffies + msecs_to_jiffies(tempo));
            break;

            case mnt:       
	        tempo += tempoaux * 60000;
        	tempoaux = 0;
            break;
        
	    case hor:
        	tempo = tempoaux * 3600000;
        	tempoaux = 0;
            break;
        
	    default:
        	set_timer = 0;
            break;
 
        }
 
    }
 
 
    // Verifica se é um desligamento imediato ou programado
    if(tecla[T_shift] == 1 && tecla[T_c] == 1 && tecla[T_t] == 0)
    	computer_shutdown();
    else if(set_timer == 0 && tecla[T_shift] == 1 && tecla[T_c] == 0 && tecla[T_t] == 1){
    	printk(KERN_INFO "Teste");
    	set_timer = 1;
    }
 
    return (irq_handler_t)IRQ_HANDLED;
}
 
 
 
 
// Função chamada quando módulo é inserido no kernel (insmod)
static int __init keyshut_init(void) {
    printk(KERN_INFO "keyshut device has been registered\n");
    request_irq(1, (irq_handler_t) irq_handler, IRQF_SHARED, "keyboard_stats", (void *) (irq_handler));
 
    return 0;
}


// Procedimento chamado quando o módulo é removido do kernel (rmmod)
static void __exit keyshut_exit(void) {
    try_to_del_timer_sync (&my_timer);
    free_irq(1, (void *) (irq_handler));
    printk(KERN_INFO "keyshut device has been unregistered \n");
}


// Define as função (callbacks) a serem chamadas no init e no exit
module_init(keyshut_init);
module_exit(keyshut_exit)
