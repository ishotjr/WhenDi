#include "atmosphere_elementSetup.h"

#ifdef __cplusplus
	extern "C"{
#endif

void ATMO_ElementSetup() {
	ATMO_Value_t inOutValue;
	ATMO_InitValue(&inOutValue);
	EmbeddedNxpRpkUserButtons_setup(&inOutValue, &inOutValue);
	EmbeddedIconLinesDisplay_setup(&inOutValue, &inOutValue);
	Interval_setup(&inOutValue, &inOutValue);
}

#ifdef __cplusplus
}
#endif
