
#ifndef ATMO_VARIABLES_H
#define ATMO_VARIABLES_H

#ifdef __cplusplus
	extern "C"{
#endif

#define ATMO_VARIABLE(ELEMENT, NAME) ATMO_ ## ELEMENT ## _VARIABLE_ ## NAME

static ATMO_DriverInstanceHandle_t ATMO_EmbeddedStaticTextDisplay_VARIABLE_pageHandle;

#ifdef __cplusplus
}
#endif
#endif
