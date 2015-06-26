#include <stdlib.h>
#include <psp2/types.h>

void make_delta1(int *delta1, SceUInt8 *pat, SceInt32 patlen);
int is_prefix(SceUInt8 *word, int wordlen, int pos);
int suffix_length(SceUInt8 *word, int wordlen, int pos);
void make_delta2(int *delta2, SceUInt8 *pat, SceInt32 patlen);
SceUInt8* boyer_moore (SceUInt8 *string, SceUInt32 stringlen, SceUInt8 *pat, SceUInt32 patlen);
