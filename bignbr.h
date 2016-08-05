#ifndef __BIGNBR_H__
#define __BIGNBR_H__

/* END OF NUMBER */
#define BIGNBR_EON 'E'

/* SIGN */
#define BIGNBR_SIGN 0

struct _bignbr
{
	unsigned int len;
	unsigned char *data;
};

typedef struct _bignbr bignbr;

/* Core */
void bignbr_fill (bignbr*, unsigned char*);
void bignbr_init (bignbr*, unsigned int, unsigned char*);
void bignbr_free (bignbr*);
void bignbr_print (bignbr*);
void bignbr_cpy (bignbr*, bignbr*);

bool bignbr_cmp_str (bignbr*, unsigned char*);

/* Checks */
bool bignbr_is_null (bignbr*);

void bignbr_set_negative (bignbr*, bool);
bool bignbr_is_negative (bignbr*);

bool bignbr_is_greater (bignbr*, bignbr*);

/* Arithmetic */
void bignbr_add (bignbr*, bignbr*);
void bignbr_sub (bignbr*, bignbr*);
void bignbr_mpl (bignbr*, bignbr*);

#endif /* __BIGNBR_H__ */
