/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TDATA_H_RPCGEN
#define _TDATA_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct taggeddata {
	char done;
	int value;
};
typedef struct taggeddata taggeddata;

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_taggeddata (XDR *, taggeddata*);

#else /* K&R C */
extern bool_t xdr_taggeddata ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TDATA_H_RPCGEN */
