#ifndef __BIGNBR_H__
#define __BIGNBR_H__

/* END OF NUMBER */
#define BIGNBR_EON 'E'

/* SIGN */
#define BIGNBR_SIGN 0

#define BIGNBR_GETNBR(a) ((a) == (BIGNBR_EON) ? (0) : (a))

struct _bignbr
{
	unsigned int len;
	char *data;
};

typedef struct _bignbr bignbr;

/* Core */
void bignbr_init (bignbr*, unsigned int, char*);
void bignbr_free (bignbr*);
void bignbr_cpy (bignbr*, bignbr*);
void bignbr_cat_digit (bignbr*, const char);
void bignbr_fill (bignbr*, const char*);
void bignbr_print (const bignbr*);
unsigned int bignbr_get_eon_pos (const bignbr*);

/* Check */
bool bignbr_cmp_str (const bignbr*, const char*);
bool bignbr_is_null (const bignbr*);

void bignbr_set_negative (bignbr*, const bool);
bool bignbr_is_negative (const bignbr*);

bool bignbr_is_greater (const bignbr*, const bignbr*);

/* Arithmetic */
void bignbr_add (bignbr*, bignbr*);
void bignbr_sub (bignbr*, bignbr*);
void bignbr_mpl (bignbr*, bignbr*);
void bignbr_div (bignbr*, bignbr*, bignbr*, bignbr*);

#endif /* __BIGNBR_H__ */
