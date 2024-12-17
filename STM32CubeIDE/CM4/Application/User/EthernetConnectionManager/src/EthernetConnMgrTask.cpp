#include "lwip.h"
#include <string.h>
#include "ethernetif.h"
#include "lwip/tcp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "EthernetConnMgrTask.h"


extern QueueHandle_t queueToEthernetConnMgr;

#define SERVER_PORT 5000           // Port serwera

static struct tcp_pcb *tcp_client_pcb;
static char received_message[100] = {0};
static uint8_t connection_active = 0;

/* Funkcja wywoływana przy odbiorze danych */
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Serwer zamknął połączenie
        tcp_close(tpcb);
        connection_active = 0;
        return ERR_OK;
    }

    /* Kopiuj dane do zmiennej */
    if (p->len < sizeof(received_message)) {
        strncpy(received_message, (char *)p->payload, p->len);
        received_message[p->len] = '\0';
        // Odebrano wiadomość, zapisana do zmiennej received_message
    }

    /* Zwalnij bufor */
    tcp_recved(tpcb, p->len);
    pbuf_free(p);
    return ERR_OK;
}

/* Funkcja wywoływana przy wysłaniu danych */
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    // Dane wysłane
    return ERR_OK;
}

/* Funkcja wywoływana po połączeniu z serwerem */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        // Błąd połączenia
        tcp_close(tpcb);
        connection_active = 0;
        return err;
    }

    // Połączono z serwerem TCP
    connection_active = 1;

    /* Ustaw funkcje odbioru i wysłania */
    tcp_recv(tpcb, tcp_client_recv);
    tcp_sent(tpcb, tcp_client_sent);

    return ERR_OK;
}

void StartTask_EthernetConnMgr(void *argument) {
    MX_LWIP_Init();

    ip_addr_t server_ip;
    err_t err;

    IP4_ADDR(&server_ip, 192, 168, 1, 10);  // Adres serwera

    /* Infinite loop */
    for (;;) {
        if (!connection_active) {
            /* Utwórz nowe PCB */
            tcp_client_pcb = tcp_new();
            if (tcp_client_pcb == NULL) {
                // Nie udało się utworzyć PCB TCP
                osDelay(5000);
                continue;
            }
            /* Ustaw timeout dla PCB */
            tcp_poll(tcp_client_pcb, NULL, 4);

            /* Połącz z serwerem */
            err = tcp_connect(tcp_client_pcb, &server_ip, SERVER_PORT, tcp_client_connected);
            if (err != ERR_OK) {
                // Błąd podczas łączenia
                tcp_close(tcp_client_pcb);
                osDelay(5000);
                continue;
            }

            // Próba połączenia z serwerem
        } else {
            /* Wyślij wiadomość co 100 ms */
            const char *message = "Zapytanie od STM32\n";
            err = tcp_write(tcp_client_pcb, message, strlen(message), TCP_WRITE_FLAG_COPY);
            if (err == ERR_OK) {
                tcp_output(tcp_client_pcb);
                // Wiadomość wysłana
            } else {
                // Błąd wysyłania danych
            }
        }

        osDelay(100);  // Czekaj 100 ms
    }
}


EthernetConnectionManagerTask::EthernetConnectionManagerTask() : event_bus(EventBus(queueToEthernetConnMgr, EVENT_CLIENT_ETHERNET_CONNECTION_MANAGER)) {}

void EthernetConnectionManagerTask::OnEvent(EEventType event, UMessageData msg, EEventClient eventSender, [[maybe_unused]] EEventClient eventReceiver)
{
//	switch(event)
//	{
//		case ASYNC_REQUEST_CALCULATE:
//			break;
//		default:
//			break;
//	}
}

void EthernetConnectionManagerTask::process()
{
	event_bus.receive([this](EEventType event, UMessageData msg, EEventClient eventSender, EEventClient eventReceiver){this->OnEvent(event, msg, eventSender, eventReceiver);});
}
