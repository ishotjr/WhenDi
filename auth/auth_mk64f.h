#include "../atmo/core.h"

/**
 * @brief This whole module is designed to be a thread safe wrapper directly around the NXP A100x Interface
 * 
 */
uint8_t ATMO_MK64F_Auth_Init();
uint8_t ATMO_MK64F_Auth_GetUid(uint8_t *uid);
uint8_t ATMO_MK64F_Auth_GetCert(uint8_t *cert);
uint8_t ATMO_MK64F_Auth_SetChallenge(uint8_t *challenge);
uint8_t ATMO_MK64F_Auth_GetResponse(uint8_t *response);
uint8_t ATMO_MK64F_Auth_Authenticate(uint8_t *challenge, uint8_t *response);