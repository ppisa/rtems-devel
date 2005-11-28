#ifndef _SYSTEM_DEF_H_
#define _SYSTEM_DEF_H_

#include <inttypes.h>

#define VER_CODE(major,minor,patch) (major*0x10000+minor*0x100+patch)
/* Software version */
#define SW_VER_ID	"APPFOO"
#define SW_VER_MAJOR	0
#define SW_VER_MINOR	1
#define SW_VER_PATCH	0
#define SW_VER_CODE	VER_CODE(SW_VER_MAJOR,SW_VER_MINOR,SW_VER_PATCH)
/* Hardware version */
#define HW_VER_ID	"SOMEBOARD"
#define HW_VER_MAJOR	0
#define HW_VER_MINOR	1
#define HW_VER_PATCH	0
#define HW_VER_CODE	VER_CODE(HW_VER_MAJOR,HW_VER_MINOR,HW_VER_PATCH)
/* Version of mechanical  */
#define MECH_VER_ID     "SOMESTUFF"
#define MECH_VER_MAJOR  0
#define MECH_VER_MINOR  1
#define MECH_VER_PATCH  0
#define MECH_VER_CODE	VER_CODE(MECH_VER_MAJOR,MECH_VER_MINOR,MECH_VER_PATCH)

#endif /* _SYSTEM_DEF_H_ */
