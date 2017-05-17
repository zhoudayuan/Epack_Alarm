#ifndef __COMPILE_DEBUG__
#define __COMPILE_DEBUG__

// PRE_CSBK_DEBUG为1 有预载波，应答转发1次，
// PRE_CSBK_DEBUG为0 有预载波，应答转发4次，
#define  DEL_PRE_CSBK_DEBUG              0
#define  ODP_SEND_INF_DEBUG              (!DEL_PRE_CSBK_DEBUG)

#endif