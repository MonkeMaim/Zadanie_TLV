/**
 * @file tlv.h
 * @brief Biblioteka obsługi protokołu TLV (Type-Length-Value) dla interfejsu UART.
 * @details Zawiera definicje statusów oraz interfejsy funkcji do kodowania 
 *          i dekodowania ramek komunikatów.
 */

#ifndef TLV_H
#define TLV_H

#include <stdint.h>
#include <stddef.h>
#include "msg.h"    // Dołączamy struktury z zadania rekrutacyjnego

/**
 * @brief Kody statusu zwracane przez funkcje biblioteki TLV.
 */
typedef enum {
    TLV_OK = 0,                   /**< Operacja zakończona sukcesem */
    TLV_ERROR_NULL_PTR,           /**< Błąd: wykryto wskaźnik NULL */
    TLV_ERROR_BUFFER_TOO_SMALL,   /**< Błąd: bufor docelowy jest zbyt mały */
    TLV_ERROR_INVALID_LENGTH      /**< Błąd: nieprawidłowa długość ramki lub payloadu */
} TLV_Status_t;

/**
 * @brief Koduje strukturę wiadomości do surowego bufora bajtów w formacie TLV.
 * 
 * @param[in]  packet       Wskaźnik na strukturę wejściową do zakodowania.
 * @param[out] out_buffer   Wskaźnik na bufor, do którego zostaną zapisane bajty ramki.
 * @param[in]  buffer_size  Całkowity, dostępny rozmiar bufora docelowego w bajtach.
 * @param[out] out_len      Wskaźnik na zmienną, w której zostanie zapisana faktyczna liczba zakodowanych bajtów.
 * @return     TLV_Status_t Status wykonania operacji (TLV_OK w przypadku sukcesu).
 */
TLV_Status_t TLV_Encode(const packet_t *packet, uint8_t *out_buffer, size_t buffer_size, size_t *out_len);

/**
 * @brief Dekoduje surowy bufor bajtów TLV do struktury wiadomości.
 * 
 * @param[in]  in_buffer    Wskaźnik na bufor zawierający surowe dane odebrane z interfejsu.
 * @param[in]  buffer_size  Rozmiar bufora wejściowego w bajtach.
 * @param[out] out_packet   Wskaźnik na strukturę docelową, do której zostaną wypakowane dane.
 * @return     TLV_Status_t Status wykonania operacji (TLV_OK w przypadku sukcesu).
 */
TLV_Status_t TLV_Decode(const uint8_t *in_buffer, size_t buffer_size, packet_t *out_packet);

#endif // TLV_H