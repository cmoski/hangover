/*
 * Copyright 2017 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "ntdll.h"

#ifdef QEMU_DLL_GUEST

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, void *reserved)
{
    return TRUE;
}

#else

#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_ntdll);

const struct qemu_ops *qemu_ops;

static const syscall_handler dll_functions[] =
{
    qemu___C_specific_handler,
    qemu___isascii,
    qemu___iscsym,
    qemu___iscsymf,
    qemu___toascii,
    qemu__atoi64,
    qemu__i64toa,
    qemu__itoa,
    qemu__ltoa,
    qemu__memccpy,
    qemu__memicmp,
    qemu__splitpath,
    qemu__stricmp,
    qemu__strlwr,
    qemu__strnicmp,
    qemu__strupr,
    qemu__tolower,
    qemu__toupper,
    qemu__ui64toa,
    qemu__ultoa,
    qemu_atoi,
    qemu_atol,
    qemu_isalnum,
    qemu_isalpha,
    qemu_iscntrl,
    qemu_isdigit,
    qemu_isgraph,
    qemu_islower,
    qemu_isprint,
    qemu_ispunct,
    qemu_isspace,
    qemu_isupper,
    qemu_isxdigit,
    qemu_memchr,
    qemu_memcmp,
    qemu_memcpy,
    qemu_memmove,
    qemu_memset,
    qemu_RtlAddFunctionTable,
    qemu_RtlCaptureContext,
    qemu_RtlDecodePointer,
    qemu_RtlDeleteCriticalSection,
    qemu_RtlEncodePointer,
    qemu_RtlEnterCriticalSection,
    qemu_RtlInitializeCriticalSectionEx,
    qemu_RtlLeaveCriticalSection,
    qemu_RtlLookupFunctionEntry,
    qemu_RtlVirtualUnwind,
    qemu_strcat,
    qemu_strchr,
    qemu_strcmp,
    qemu_strcpy,
    qemu_strcspn,
    qemu_strlen,
    qemu_strncat,
    qemu_strncmp,
    qemu_strncpy,
    qemu_strpbrk,
    qemu_strrchr,
    qemu_strspn,
    qemu_strstr,
    qemu_strtol,
    qemu_strtoul,
    qemu_tolower,
    qemu_toupper,
    qemu_wcsrchr,
};

const WINAPI syscall_handler *qemu_dll_register(const struct qemu_ops *ops, uint32_t *dll_num)
{
    HMODULE ntdll;

    WINE_TRACE("Loading host-side ntdll wrapper.\n");

    ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll)
        WINE_ERR("ntdll.dll not loaded\n");

    p___isascii = (void *)GetProcAddress(ntdll, "__isascii");
    p___iscsym = (void *)GetProcAddress(ntdll, "__iscsym");
    p___iscsymf = (void *)GetProcAddress(ntdll, "__iscsymf");
    p___toascii = (void *)GetProcAddress(ntdll, "__toascii");
    p__atoi64 = (void *)GetProcAddress(ntdll, "_atoi64");
    p__i64toa = (void *)GetProcAddress(ntdll, "_i64toa");
    p__itoa = (void *)GetProcAddress(ntdll, "_itoa");
    p__ltoa = (void *)GetProcAddress(ntdll, "_ltoa");
    p__memccpy = (void *)GetProcAddress(ntdll, "_memccpy");
    p__memicmp = (void *)GetProcAddress(ntdll, "_memicmp");
    p__splitpath = (void *)GetProcAddress(ntdll, "_splitpath");
    p__stricmp = (void *)GetProcAddress(ntdll, "_stricmp");
    p__strlwr = (void *)GetProcAddress(ntdll, "_strlwr");
    p__strnicmp = (void *)GetProcAddress(ntdll, "_strnicmp");
    p__strupr = (void *)GetProcAddress(ntdll, "_strupr");
    p__tolower = (void *)GetProcAddress(ntdll, "_tolower");
    p__toupper = (void *)GetProcAddress(ntdll, "_toupper");
    p__ui64toa = (void *)GetProcAddress(ntdll, "_ui64toa");
    p__ultoa = (void *)GetProcAddress(ntdll, "_ultoa");
    p_atoi = (void *)GetProcAddress(ntdll, "atoi");
    p_atol = (void *)GetProcAddress(ntdll, "atol");
    p_isalnum = (void *)GetProcAddress(ntdll, "isalnum");
    p_isalpha = (void *)GetProcAddress(ntdll, "isalpha");
    p_iscntrl = (void *)GetProcAddress(ntdll, "iscntrl");
    p_isdigit = (void *)GetProcAddress(ntdll, "isdigit");
    p_isgraph = (void *)GetProcAddress(ntdll, "isgraph");
    p_islower = (void *)GetProcAddress(ntdll, "islower");
    p_isprint = (void *)GetProcAddress(ntdll, "isprint");
    p_ispunct = (void *)GetProcAddress(ntdll, "ispunct");
    p_isspace = (void *)GetProcAddress(ntdll, "isspace");
    p_isupper = (void *)GetProcAddress(ntdll, "isupper");
    p_isxdigit = (void *)GetProcAddress(ntdll, "isxdigit");
    p_memchr = (void *)GetProcAddress(ntdll, "memchr");
    p_memcmp = (void *)GetProcAddress(ntdll, "memcmp");
    p_memcpy = (void *)GetProcAddress(ntdll, "memcpy");
    p_memmove = (void *)GetProcAddress(ntdll, "memmove");
    p_memset = (void *)GetProcAddress(ntdll, "memset");
    p_strcat = (void *)GetProcAddress(ntdll, "strcat");
    p_strchr = (void *)GetProcAddress(ntdll, "strchr");
    p_strcmp = (void *)GetProcAddress(ntdll, "strcmp");
    p_strcpy = (void *)GetProcAddress(ntdll, "strcpy");
    p_strcspn = (void *)GetProcAddress(ntdll, "strcspn");
    p_strlen = (void *)GetProcAddress(ntdll, "strlen");
    p_strncat = (void *)GetProcAddress(ntdll, "strncat");
    p_strncmp = (void *)GetProcAddress(ntdll, "strncmp");
    p_strncpy = (void *)GetProcAddress(ntdll, "strncpy");
    p_strpbrk = (void *)GetProcAddress(ntdll, "strpbrk");
    p_strrchr = (void *)GetProcAddress(ntdll, "strrchr");
    p_strspn = (void *)GetProcAddress(ntdll, "strspn");
    p_strstr = (void *)GetProcAddress(ntdll, "strstr");
    p_strtol = (void *)GetProcAddress(ntdll, "strtol");
    p_strtoul = (void *)GetProcAddress(ntdll, "strtoul");
    p_tolower = (void *)GetProcAddress(ntdll, "tolower");
    p_toupper = (void *)GetProcAddress(ntdll, "toupper");
    p_wcsrchr = (void *)GetProcAddress(ntdll, "wcsrchr");

    qemu_ops = ops;
    *dll_num = QEMU_CURRENT_DLL;

    return dll_functions;
}

#endif
