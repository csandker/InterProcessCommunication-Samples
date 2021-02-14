

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for Interface1-Explicit.idl:
    Oicf, W3, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, app_config, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "Interface1-Explicit_h.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   73                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _Interface12DExplicit_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } Interface12DExplicit_MIDL_TYPE_FORMAT_STRING;

typedef struct _Interface12DExplicit_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } Interface12DExplicit_MIDL_PROC_FORMAT_STRING;

typedef struct _Interface12DExplicit_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } Interface12DExplicit_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};



extern const Interface12DExplicit_MIDL_TYPE_FORMAT_STRING Interface12DExplicit__MIDL_TypeFormatString;
extern const Interface12DExplicit_MIDL_PROC_FORMAT_STRING Interface12DExplicit__MIDL_ProcFormatString;
extern const Interface12DExplicit_MIDL_EXPR_FORMAT_STRING Interface12DExplicit__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: Example1, ver. 1.0,
   GUID={0xd6b1ad2b,0xb550,0x4729,{0xb6,0xc2,0x16,0x51,0xf5,0x84,0x80,0xc3}} */

 extern const MIDL_STUBLESS_PROXY_INFO Example1_ProxyInfo;


static const RPC_CLIENT_INTERFACE Example1___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xd6b1ad2b,0xb550,0x4729,{0xb6,0xc2,0x16,0x51,0xf5,0x84,0x80,0xc3}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    &Example1_ProxyInfo,
    0x02000000
    };
RPC_IF_HANDLE Example1_v1_0_c_ifspec = (RPC_IF_HANDLE)& Example1___RpcClientInterface;

extern const MIDL_STUB_DESC Example1_StubDesc;

static RPC_BINDING_HANDLE Example1__MIDL_AutoBindHandle;


int Output( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ const char *pszOutput)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&Example1_ProxyInfo,
                  0,
                  0,
                  hBinding,
                  pszOutput);
    return ( int  )_RetVal.Simple;
    
}


void Shutdown( 
    /* [in] */ handle_t hBinding)
{

    NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&Example1_ProxyInfo,
                  1,
                  0,
                  hBinding);
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const Interface12DExplicit_MIDL_PROC_FORMAT_STRING Interface12DExplicit__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Output */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x8 ),	/* 8 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 20 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pszOutput */

/* 30 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 32 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 34 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Shutdown */

/* 42 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x1 ),	/* 1 */
/* 50 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 52 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 54 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 62 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };

static const Interface12DExplicit_MIDL_TYPE_FORMAT_STRING Interface12DExplicit__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short Example1_FormatStringOffsetTable[] =
    {
    0,
    42
    };



#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for Interface1-Explicit.idl:
    Oicf, W3, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, app_config, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)




#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"


typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
}
__midl_frag6_t;
extern const __midl_frag6_t __midl_frag6;

typedef 
NDR64_FORMAT_CHAR
__midl_frag5_t;
extern const __midl_frag5_t __midl_frag5;

typedef 
struct _NDR64_CONFORMANT_STRING_FORMAT
__midl_frag4_t;
extern const __midl_frag4_t __midl_frag4;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag3_t;
extern const __midl_frag3_t __midl_frag3;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag2_t;
extern const __midl_frag2_t __midl_frag2;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag6_t __midl_frag6 =
{ 
/* Shutdown */
    { 
    /* Shutdown */      /* procedure Shutdown */
        (NDR64_UINT32) 16777280 /* 0x1000040 */,    /* explicit handle */ /* IsIntrepreted, HasExtensions */
        (NDR64_UINT32) 8 /* 0x8 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    }
};

static const __midl_frag5_t __midl_frag5 =
0x5    /* FC64_INT32 */;

static const __midl_frag4_t __midl_frag4 =
{ 
/* *char */
    { 
    /* *char */
        0x63,    /* FC64_CONF_CHAR_STRING */
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT16) 1 /* 0x1 */
    }
};

static const __midl_frag3_t __midl_frag3 =
{ 
/* *char */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag4
};

static const __midl_frag2_t __midl_frag2 =
{ 
/* Output */
    { 
    /* Output */      /* procedure Output */
        (NDR64_UINT32) 17563712 /* 0x10c0040 */,    /* explicit handle */ /* IsIntrepreted, ClientMustSize, HasReturn, HasExtensions */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 2 /* 0x2 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* pszOutput */      /* parameter pszOutput */
        &__midl_frag4,
        { 
        /* pszOutput */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* int */      /* parameter int */
        &__midl_frag5,
        { 
        /* int */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;


#include "poppack.h"


static const FormatInfoRef Example1_Ndr64ProcTable[] =
    {
    &__midl_frag2,
    &__midl_frag6
    };


static const MIDL_STUB_DESC Example1_StubDesc = 
    {
    (void *)& Example1___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &Example1__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    Interface12DExplicit__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    (void *)& Example1_ProxyInfo,   /* proxy/server info */
    0
    };

static const MIDL_SYNTAX_INFO Example1_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    Interface12DExplicit__MIDL_ProcFormatString.Format,
    Example1_FormatStringOffsetTable,
    Interface12DExplicit__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) Example1_Ndr64ProcTable,
    0,
    0,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO Example1_ProxyInfo =
    {
    &Example1_StubDesc,
    Interface12DExplicit__MIDL_ProcFormatString.Format,
    Example1_FormatStringOffsetTable,
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)Example1_SyntaxInfo
    
    };

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

