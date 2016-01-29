#ifndef __MISC_H__
#define __MISC_H__

#ifdef __HCS12__
#define ALLIGN_SIZE  (1)
#endif


#ifndef ALLIGN_SIZE
#define ALLIGN_SIZE  (4)
#endif


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)      (sizeof(a)/sizeof(a[0]))
#endif

#ifndef ALLIGN_SIZEOF
#define ALLIGN_SIZEOF(s)   (((sizeof(s) +  ALLIGN_SIZE - 1) / ALLIGN_SIZE) * ALLIGN_SIZE)
#endif

#endif
