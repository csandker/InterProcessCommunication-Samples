

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for Interface1-Implicit.idl:
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

#include "Interface1-Implicit_h.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   65                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _Interface12DImplicit_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } Interface12DImplicit_MIDL_TYPE_FORMAT_STRING;

typedef struct _Interface12DImplicit_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } Interface12DImplicit_MIDL_PROC_FORMAT_STRING;

typedef struct _Interface12DImplicit_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } Interface12DImplicit_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};



extern const Interface12DImplicit_MIDL_TYPE_FORMAT_STRING Interface12DImplicit__MIDL_TypeFormatString;
extern const Interface12DImplicit_MIDL_PROC_FORMAT_STRING Interface12DImplicit__MIDL_ProcFormatString;
extern const Interface12DImplicit_MIDL_EXPR_FORMAT_STRING Interface12DImplicit__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: Example1, ver. 1.0,
   GUID={0x9510b60a,0x2eac,0x43fc,{0x80,0x77,0xaa,0xef,0xbd,0xf3,0x75,0x2b}} */

 extern const MIDL_STUBLESS_PROXY_INFO Example1_ProxyInfo;
handle_t hImplicitBinding;


static const RPC_CLIENT_INTERFACE Example1___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x9510b60a,0x2eac,0x43fc,{0x80,0x77,0xaa,0xef,0xbd,0xf3,0x75,0x2b}},{1,0}},
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
    /* [string][in] */ const char *pszOutput)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&Example1_ProxyInfo,
                  0,
                  0,
                  pszOutput);
    return ( int  )_RetVal.Simple;
    
}


void Shutdown( void)
{

    NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&Example1_ProxyInfo,
                  1,
                  0,
                  0);
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const Interface12DImplicit_MIDL_PROC_FORMAT_STRING Interface12DImplicit__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Output */

			0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pszOutput */

/* 26 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 28 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 30 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 32 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 34 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 36 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Shutdown */

/* 38 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/* 40 */	NdrFcLong( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x1 ),	/* 1 */
/* 46 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 54 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };

static const Interface12DImplicit_MIDL_TYPE_FORMAT_STRING Interface12DImplicit__MIDL_TypeFormatString =
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
    38
    };



#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for Interface1-Implicit.idl:
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
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
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
        (NDR64_UINT32) 66 /* 0x42 */,    /* primitive handle */ /* IsIntrepreted */
        (NDR64_UINT32) 0 /* 0x0 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */
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
        (NDR64_UINT32) 786498 /* 0xc0042 */,    /* primitive handle */ /* IsIntrepreted, ClientMustSize, HasReturn */
        (NDR64_UINT32) 16 /* 0x10 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 2 /* 0x2 */,
        (NDR64_UINT16) 0 /* 0x0 */
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
        0 /* 0x0 */,   /* Stack offset */
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
        8 /* 0x8 */,   /* Stack offset */
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
    &hImplicitBinding,
    0,
    0,
    0,
    0,
    Interface12DImplicit__MIDL_TypeFormatString.Format,
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
    Interface12DImplicit__MIDL_ProcFormatString.Format,
    Example1_FormatStringOffsetTable,
    Interface12DImplicit__MIDL_TypeFormatString.Format,
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
    Interface12DImplicit__MIDL_ProcFormatString.Format,
    Example1_FormatStringOffsetTable,
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)Example1_SyntaxInfo
    
    };

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

