/**
 * @file tlv.c
 * @brief Implementacja funkcji kodowania i dekodowania protokołu TLV.
 * @details Zawiera procedury serializacji struktury do surowego bufora bajtów
 *          oraz bezpiecznej deserializacji i walidacji danych wejściowych.
 */

#include "tlv.h"

/**
 * @brief Koduje strukturę pakietu do surowego bufora bajtów w formacie TLV.
 */
TLV_Status_t TLV_Encode(const packet_t *packet, uint8_t *out_buffer, size_t buffer_size, size_t *out_len) {
    // Programowanie obronne: sprawdzenie czy wskaźniki nie są NULL-ami
    if (packet == NULL || out_buffer == NULL || out_len == NULL) {
        return TLV_ERROR_NULL_PTR;
    }

    // Obliczenie wymaganego rozmiaru: nagłówek (2 bajty: Typ + Długość) + payload
    size_t required_size = 2 + packet->payload_length;

    // Sprawdzenie, czy bufor docelowy pomieści całą ramkę
    if (buffer_size < required_size) {
        return TLV_ERROR_BUFFER_TOO_SMALL;
    }

    // Serializacja nagłówka (Type oraz Length)
    out_buffer[0] = packet->message_id;
    out_buffer[1] = packet->payload_length;

    // Kopiowanie danych payloadu bajt po bajcie
    for (uint8_t i = 0; i < packet->payload_length; i++) {
        out_buffer[2 + i] = packet->payload[i];
    }

    // Zwrócenie faktycznej liczby zapisanych bajtów
    *out_len = required_size;

    return TLV_OK;
}

/**
 * @brief Dekoduje surowy bufor bajtów TLV do struktury wiadomości.
 */
TLV_Status_t TLV_Decode(const uint8_t *in_buffer, size_t buffer_size, packet_t *out_packet) {
    // Programowanie obronne: sprawdzenie wskaźników
    if (in_buffer == NULL || out_packet == NULL) {
        return TLV_ERROR_NULL_PTR;
    }

    // Ramka musi mieć przynajmniej 2 bajty na nagłówek (Type + Length)
    if (buffer_size < 2) {
        return TLV_ERROR_BUFFER_TOO_SMALL;
    }

    // Odczyt nagłówka z surowego bufora
    out_packet->message_id = in_buffer[0];
    out_packet->payload_length = in_buffer[1];

    // Walidacja bezpieczeństwa: czy długość payloadu nie przekracza zdefiniowanego limitu
    if (out_packet->payload_length > TLV_MAX_PAYLOAD_SIZE) {
        return TLV_ERROR_INVALID_LENGTH;
    }

    // Sprawdzenie, czy bufor zawiera tyle bajtów, ile zadeklarowano w polu Length
    if (buffer_size < (size_t)(2 + out_packet->payload_length)) {
        return TLV_ERROR_BUFFER_TOO_SMALL;
    }

    // Przepisywanie danych payloadu do struktury docelowej
    for (uint8_t i = 0; i < out_packet->payload_length; i++) {
        out_packet->payload[i] = in_buffer[2 + i];
    }

    return TLV_OK;
}