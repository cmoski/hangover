/*
 * Copyright 2008-2012 Henri Verbeet for CodeWeavers
 * Copyright 2017 André Hentschel
 * Copyright 2018 Stefan Dösinger for CodeWeavers
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

#define COBJMACROS

#include <windows.h>
#include <stdio.h>

#include "thunk/qemu_windows.h"

#include "windows-user-services.h"
#include "dll_list.h"

#ifdef QEMU_DLL_GUEST

#include <d3d11.h>
#include <debug.h>

#include <initguid.h>

DEFINE_GUID(IID_ID3D11Device2, 0x9d06dffa, 0xd1e5, 0x4d07, 0x83,0xa8, 0x1b,0xb1,0x23,0xf2,0xf8,0x41);
DEFINE_GUID(IID_ID3D11Device1, 0xa04bfb29, 0x08ef, 0x43d6, 0xa4,0x9c, 0xa9,0xbd,0xbd,0xcb,0xe6,0x86);

#else

#include <d3d11_2.h>
#include <wine/debug.h>

#endif

#include "thunk/qemu_d3d11.h"

#include "qemudxgi.h"
#include "qemu_d3d11.h"

WINE_DEFAULT_DEBUG_CHANNEL(qemu_d3d11);

struct qemu_d3d11_immediate_context_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t out;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_device_context *impl_from_ID3D11DeviceContext1(ID3D11DeviceContext1 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_device_context, ID3D11DeviceContext1_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_QueryInterface(ID3D11DeviceContext1 *iface, REFIID riid, void **out)
{
    struct qemu_d3d11_immediate_context_QueryInterface call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_QUERYINTERFACE);
    call.iface = (ULONG_PTR)context;
    call.riid = (ULONG_PTR)riid;
    call.out = (ULONG_PTR)out;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_QueryInterface *c = (struct qemu_d3d11_immediate_context_QueryInterface *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_QueryInterface(context->host, QEMU_G2H(c->riid), QEMU_G2H(c->out));
}

#endif

struct qemu_d3d11_immediate_context_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_immediate_context_AddRef(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_AddRef call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_ADDREF);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_AddRef *c = (struct qemu_d3d11_immediate_context_AddRef *)call;
    struct qemu_d3d11_device_context *context;

    WINE_TRACE("\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_AddRef(context->host);
}

#endif

struct qemu_d3d11_immediate_context_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_immediate_context_Release(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_Release call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RELEASE);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_Release(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Release *c = (struct qemu_d3d11_immediate_context_Release *)call;
    struct qemu_d3d11_device_context *context;

    /* As long as we only support the device-embedded immediate context we don't have to do anything here. */
    WINE_TRACE("\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_Release(context->host);
}

#endif

struct qemu_d3d11_immediate_context_GetDevice
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t device;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GetDevice(ID3D11DeviceContext1 *iface, ID3D11Device **device)
{
    struct qemu_d3d11_immediate_context_GetDevice call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);
    struct qemu_d3d11_device *device_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETDEVICE);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    device_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D11Device *)&device_impl->ID3D11Device2_iface;
}

#else

void qemu_d3d11_immediate_context_GetDevice(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetDevice *c = (struct qemu_d3d11_immediate_context_GetDevice *)call;
    struct qemu_d3d11_device_context *context;
    ID3D11Device *device;

    WINE_TRACE("\n");
    context = QEMU_G2H(c->iface);
    c->device = QEMU_H2G(CONTAINING_RECORD(context, struct qemu_d3d11_device, immediate_context));

    /* For refcounting. */
    ID3D11DeviceContext1_GetDevice(context->host, &device);
}

#endif

struct qemu_d3d11_immediate_context_GetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_GetPrivateData(ID3D11DeviceContext1 *iface, REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_immediate_context_GetPrivateData call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETPRIVATEDATA);
    call.iface = (ULONG_PTR)context;
    call.guid = (ULONG_PTR)guid;
    call.data_size = (ULONG_PTR)data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_GetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetPrivateData *c = (struct qemu_d3d11_immediate_context_GetPrivateData *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_GetPrivateData(context->host, QEMU_G2H(c->guid), QEMU_G2H(c->data_size), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_immediate_context_SetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_SetPrivateData(ID3D11DeviceContext1 *iface, REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_immediate_context_SetPrivateData call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SETPRIVATEDATA);
    call.iface = (ULONG_PTR)context;
    call.guid = (ULONG_PTR)guid;
    call.data_size = data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_SetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SetPrivateData *c = (struct qemu_d3d11_immediate_context_SetPrivateData *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_SetPrivateData(context->host, QEMU_G2H(c->guid), c->data_size, QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_immediate_context_SetPrivateDataInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_SetPrivateDataInterface(ID3D11DeviceContext1 *iface, REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_immediate_context_SetPrivateDataInterface call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SETPRIVATEDATAINTERFACE);
    call.iface = (ULONG_PTR)context;
    call.guid = (ULONG_PTR)guid;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_SetPrivateDataInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SetPrivateDataInterface *c = (struct qemu_d3d11_immediate_context_SetPrivateDataInterface *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_SetPrivateDataInterface(context->host, QEMU_G2H(c->guid), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_immediate_context_VSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_VSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_VSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_PSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_PSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSSetShaderResources *c = (struct qemu_d3d11_immediate_context_PSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_PSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetShader(ID3D11DeviceContext1 *iface, ID3D11PixelShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_PSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSSetShader *c = (struct qemu_d3d11_immediate_context_PSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_PSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_PSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSSetSamplers *c = (struct qemu_d3d11_immediate_context_PSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_VSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetShader(ID3D11DeviceContext1 *iface, ID3D11VertexShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_VSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSSetShader *c = (struct qemu_d3d11_immediate_context_VSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_DrawIndexed
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t index_count;
    uint64_t start_index_location;
    uint64_t base_vertex_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexed(ID3D11DeviceContext1 *iface, UINT index_count, UINT start_index_location, INT base_vertex_location)
{
    struct qemu_d3d11_immediate_context_DrawIndexed call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWINDEXED);
    call.iface = (ULONG_PTR)context;
    call.index_count = index_count;
    call.start_index_location = start_index_location;
    call.base_vertex_location = base_vertex_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawIndexed(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawIndexed *c = (struct qemu_d3d11_immediate_context_DrawIndexed *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawIndexed(context->host, c->index_count, c->start_index_location, c->base_vertex_location);
}

#endif

struct qemu_d3d11_immediate_context_Draw
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t vertex_count;
    uint64_t start_vertex_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_Draw(ID3D11DeviceContext1 *iface, UINT vertex_count, UINT start_vertex_location)
{
    struct qemu_d3d11_immediate_context_Draw call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAW);
    call.iface = (ULONG_PTR)context;
    call.vertex_count = vertex_count;
    call.start_vertex_location = start_vertex_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_Draw(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Draw *c = (struct qemu_d3d11_immediate_context_Draw *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_Draw(context->host, c->vertex_count, c->start_vertex_location);
}

#endif

struct qemu_d3d11_immediate_context_Map
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t subresource_idx;
    uint64_t map_type;
    uint64_t map_flags;
    uint64_t mapped_subresource;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_Map(ID3D11DeviceContext1 *iface, ID3D11Resource *resource, UINT subresource_idx, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE *mapped_subresource)
{
    struct qemu_d3d11_immediate_context_Map call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_MAP);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;
    call.subresource_idx = subresource_idx;
    call.map_type = map_type;
    call.map_flags = map_flags;
    call.mapped_subresource = (ULONG_PTR)mapped_subresource;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_Map(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Map *c = (struct qemu_d3d11_immediate_context_Map *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_Map(context->host, QEMU_G2H(c->resource), c->subresource_idx, c->map_type, c->map_flags, QEMU_G2H(c->mapped_subresource));
}

#endif

struct qemu_d3d11_immediate_context_Unmap
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t subresource_idx;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_Unmap(ID3D11DeviceContext1 *iface, ID3D11Resource *resource, UINT subresource_idx)
{
    struct qemu_d3d11_immediate_context_Unmap call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_UNMAP);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;
    call.subresource_idx = subresource_idx;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_Unmap(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Unmap *c = (struct qemu_d3d11_immediate_context_Unmap *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_Unmap(context->host, QEMU_G2H(c->resource), c->subresource_idx);
}

#endif

struct qemu_d3d11_immediate_context_PSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_PSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_PSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_IASetInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetInputLayout(ID3D11DeviceContext1 *iface, ID3D11InputLayout *input_layout)
{
    struct qemu_d3d11_immediate_context_IASetInputLayout call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IASETINPUTLAYOUT);
    call.iface = (ULONG_PTR)context;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IASetInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IASetInputLayout *c = (struct qemu_d3d11_immediate_context_IASetInputLayout *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IASetInputLayout(context->host, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d11_immediate_context_IASetVertexBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t strides;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetVertexBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers, const UINT *strides, const UINT *offsets)
{
    struct qemu_d3d11_immediate_context_IASetVertexBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IASETVERTEXBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.strides = (ULONG_PTR)strides;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IASetVertexBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IASetVertexBuffers *c = (struct qemu_d3d11_immediate_context_IASetVertexBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IASetVertexBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->strides), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d11_immediate_context_IASetIndexBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t format;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetIndexBuffer(ID3D11DeviceContext1 *iface, ID3D11Buffer *buffer, DXGI_FORMAT format, UINT offset)
{
    struct qemu_d3d11_immediate_context_IASetIndexBuffer call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IASETINDEXBUFFER);
    call.iface = (ULONG_PTR)context;
    call.buffer = (ULONG_PTR)buffer;
    call.format = format;
    call.offset = offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IASetIndexBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IASetIndexBuffer *c = (struct qemu_d3d11_immediate_context_IASetIndexBuffer *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IASetIndexBuffer(context->host, QEMU_G2H(c->buffer), c->format, c->offset);
}

#endif

struct qemu_d3d11_immediate_context_DrawIndexedInstanced
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t instance_index_count;
    uint64_t instance_count;
    uint64_t start_index_location;
    uint64_t base_vertex_location;
    uint64_t start_instance_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexedInstanced(ID3D11DeviceContext1 *iface, UINT instance_index_count, UINT instance_count, UINT start_index_location, INT base_vertex_location, UINT start_instance_location)
{
    struct qemu_d3d11_immediate_context_DrawIndexedInstanced call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWINDEXEDINSTANCED);
    call.iface = (ULONG_PTR)context;
    call.instance_index_count = instance_index_count;
    call.instance_count = instance_count;
    call.start_index_location = start_index_location;
    call.base_vertex_location = base_vertex_location;
    call.start_instance_location = start_instance_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawIndexedInstanced(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawIndexedInstanced *c = (struct qemu_d3d11_immediate_context_DrawIndexedInstanced *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawIndexedInstanced(context->host, c->instance_index_count, c->instance_count, c->start_index_location, c->base_vertex_location, c->start_instance_location);
}

#endif

struct qemu_d3d11_immediate_context_DrawInstanced
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t instance_vertex_count;
    uint64_t instance_count;
    uint64_t start_vertex_location;
    uint64_t start_instance_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawInstanced(ID3D11DeviceContext1 *iface, UINT instance_vertex_count, UINT instance_count, UINT start_vertex_location, UINT start_instance_location)
{
    struct qemu_d3d11_immediate_context_DrawInstanced call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWINSTANCED);
    call.iface = (ULONG_PTR)context;
    call.instance_vertex_count = instance_vertex_count;
    call.instance_count = instance_count;
    call.start_vertex_location = start_vertex_location;
    call.start_instance_location = start_instance_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawInstanced(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawInstanced *c = (struct qemu_d3d11_immediate_context_DrawInstanced *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawInstanced(context->host, c->instance_vertex_count, c->instance_count, c->start_vertex_location, c->start_instance_location);
}

#endif

struct qemu_d3d11_immediate_context_GSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_GSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_GSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_GSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetShader(ID3D11DeviceContext1 *iface, ID3D11GeometryShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_GSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSSetShader *c = (struct qemu_d3d11_immediate_context_GSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_IASetPrimitiveTopology
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t topology;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetPrimitiveTopology(ID3D11DeviceContext1 *iface, D3D11_PRIMITIVE_TOPOLOGY topology)
{
    struct qemu_d3d11_immediate_context_IASetPrimitiveTopology call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IASETPRIMITIVETOPOLOGY);
    call.iface = (ULONG_PTR)context;
    call.topology = topology;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IASetPrimitiveTopology(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IASetPrimitiveTopology *c = (struct qemu_d3d11_immediate_context_IASetPrimitiveTopology *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IASetPrimitiveTopology(context->host, c->topology);
}

#endif

struct qemu_d3d11_immediate_context_VSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_VSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSSetShaderResources *c = (struct qemu_d3d11_immediate_context_VSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_VSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_VSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSSetSamplers *c = (struct qemu_d3d11_immediate_context_VSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_Begin
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t asynchronous;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_Begin(ID3D11DeviceContext1 *iface, ID3D11Asynchronous *asynchronous)
{
    struct qemu_d3d11_immediate_context_Begin call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_BEGIN);
    call.iface = (ULONG_PTR)context;
    call.asynchronous = (ULONG_PTR)asynchronous;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_Begin(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Begin *c = (struct qemu_d3d11_immediate_context_Begin *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_Begin(context->host, QEMU_G2H(c->asynchronous));
}

#endif

struct qemu_d3d11_immediate_context_End
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t asynchronous;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_End(ID3D11DeviceContext1 *iface, ID3D11Asynchronous *asynchronous)
{
    struct qemu_d3d11_immediate_context_End call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_END);
    call.iface = (ULONG_PTR)context;
    call.asynchronous = (ULONG_PTR)asynchronous;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_End(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_End *c = (struct qemu_d3d11_immediate_context_End *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_End(context->host, QEMU_G2H(c->asynchronous));
}

#endif

struct qemu_d3d11_immediate_context_GetData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t asynchronous;
    uint64_t data;
    uint64_t data_size;
    uint64_t data_flags;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_GetData(ID3D11DeviceContext1 *iface, ID3D11Asynchronous *asynchronous, void *data, UINT data_size, UINT data_flags)
{
    struct qemu_d3d11_immediate_context_GetData call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETDATA);
    call.iface = (ULONG_PTR)context;
    call.asynchronous = (ULONG_PTR)asynchronous;
    call.data = (ULONG_PTR)data;
    call.data_size = data_size;
    call.data_flags = data_flags;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_GetData(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetData *c = (struct qemu_d3d11_immediate_context_GetData *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_GetData(context->host, QEMU_G2H(c->asynchronous), QEMU_G2H(c->data), c->data_size, c->data_flags);
}

#endif

struct qemu_d3d11_immediate_context_SetPredication
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t predicate;
    uint64_t value;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_SetPredication(ID3D11DeviceContext1 *iface, ID3D11Predicate *predicate, BOOL value)
{
    struct qemu_d3d11_immediate_context_SetPredication call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SETPREDICATION);
    call.iface = (ULONG_PTR)context;
    call.predicate = (ULONG_PTR)predicate;
    call.value = value;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_SetPredication(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SetPredication *c = (struct qemu_d3d11_immediate_context_SetPredication *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_SetPredication(context->host, QEMU_G2H(c->predicate), c->value);
}

#endif

struct qemu_d3d11_immediate_context_GSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_GSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSSetShaderResources *c = (struct qemu_d3d11_immediate_context_GSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_GSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_GSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSSetSamplers *c = (struct qemu_d3d11_immediate_context_GSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_OMSetRenderTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetRenderTargets(ID3D11DeviceContext1 *iface, UINT render_target_view_count, ID3D11RenderTargetView *const *render_target_views, ID3D11DepthStencilView *depth_stencil_view)
{
    struct qemu_d3d11_immediate_context_OMSetRenderTargets call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMSETRENDERTARGETS);
    call.iface = (ULONG_PTR)context;
    call.render_target_view_count = render_target_view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMSetRenderTargets(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMSetRenderTargets *c = (struct qemu_d3d11_immediate_context_OMSetRenderTargets *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMSetRenderTargets(context->host, c->render_target_view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view));
}

#endif

struct qemu_d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
    uint64_t unordered_access_view_start_slot;
    uint64_t unordered_access_view_count;
    uint64_t unordered_access_views;
    uint64_t initial_counts;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext1 *iface, UINT render_target_view_count, ID3D11RenderTargetView *const *render_target_views, ID3D11DepthStencilView *depth_stencil_view, UINT unordered_access_view_start_slot, UINT unordered_access_view_count, ID3D11UnorderedAccessView *const *unordered_access_views, const UINT *initial_counts)
{
    struct qemu_d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMSETRENDERTARGETSANDUNORDEREDACCESSVIEWS);
    call.iface = (ULONG_PTR)context;
    call.render_target_view_count = render_target_view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;
    call.unordered_access_view_start_slot = unordered_access_view_start_slot;
    call.unordered_access_view_count = unordered_access_view_count;
    call.unordered_access_views = (ULONG_PTR)unordered_access_views;
    call.initial_counts = (ULONG_PTR)initial_counts;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews *c = (struct qemu_d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews(context->host, c->render_target_view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view), c->unordered_access_view_start_slot, c->unordered_access_view_count, QEMU_G2H(c->unordered_access_views), QEMU_G2H(c->initial_counts));
}

#endif

struct qemu_d3d11_immediate_context_OMSetBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t blend_state;
    uint64_t blend_factor;
    uint64_t sample_mask;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetBlendState(ID3D11DeviceContext1 *iface, ID3D11BlendState *blend_state, const float blend_factor[4], UINT sample_mask)
{
    struct qemu_d3d11_immediate_context_OMSetBlendState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMSETBLENDSTATE);
    call.iface = (ULONG_PTR)context;
    call.blend_state = (ULONG_PTR)blend_state;
    call.blend_factor = (ULONG_PTR)blend_factor;
    call.sample_mask = sample_mask;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMSetBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMSetBlendState *c = (struct qemu_d3d11_immediate_context_OMSetBlendState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMSetBlendState(context->host, QEMU_G2H(c->blend_state), QEMU_G2H(c->blend_factor), c->sample_mask);
}

#endif

struct qemu_d3d11_immediate_context_OMSetDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_state;
    uint64_t stencil_ref;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetDepthStencilState(ID3D11DeviceContext1 *iface, ID3D11DepthStencilState *depth_stencil_state, UINT stencil_ref)
{
    struct qemu_d3d11_immediate_context_OMSetDepthStencilState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMSETDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)context;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;
    call.stencil_ref = stencil_ref;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMSetDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMSetDepthStencilState *c = (struct qemu_d3d11_immediate_context_OMSetDepthStencilState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMSetDepthStencilState(context->host, QEMU_G2H(c->depth_stencil_state), c->stencil_ref);
}

#endif

struct qemu_d3d11_immediate_context_SOSetTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_SOSetTargets(ID3D11DeviceContext1 *iface, UINT buffer_count, ID3D11Buffer *const *buffers, const UINT *offsets)
{
    struct qemu_d3d11_immediate_context_SOSetTargets call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SOSETTARGETS);
    call.iface = (ULONG_PTR)context;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_SOSetTargets(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SOSetTargets *c = (struct qemu_d3d11_immediate_context_SOSetTargets *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_SOSetTargets(context->host, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d11_immediate_context_DrawAuto
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawAuto(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_DrawAuto call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWAUTO);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawAuto(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawAuto *c = (struct qemu_d3d11_immediate_context_DrawAuto *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawAuto(context->host);
}

#endif

struct qemu_d3d11_immediate_context_DrawIndexedInstancedIndirect
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexedInstancedIndirect(ID3D11DeviceContext1 *iface, ID3D11Buffer *buffer, UINT offset)
{
    struct qemu_d3d11_immediate_context_DrawIndexedInstancedIndirect call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWINDEXEDINSTANCEDINDIRECT);
    call.iface = (ULONG_PTR)context;
    call.buffer = (ULONG_PTR)buffer;
    call.offset = offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawIndexedInstancedIndirect(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawIndexedInstancedIndirect *c = (struct qemu_d3d11_immediate_context_DrawIndexedInstancedIndirect *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawIndexedInstancedIndirect(context->host, QEMU_G2H(c->buffer), c->offset);
}

#endif

struct qemu_d3d11_immediate_context_DrawInstancedIndirect
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawInstancedIndirect(ID3D11DeviceContext1 *iface, ID3D11Buffer *buffer, UINT offset)
{
    struct qemu_d3d11_immediate_context_DrawInstancedIndirect call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DRAWINSTANCEDINDIRECT);
    call.iface = (ULONG_PTR)context;
    call.buffer = (ULONG_PTR)buffer;
    call.offset = offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DrawInstancedIndirect(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DrawInstancedIndirect *c = (struct qemu_d3d11_immediate_context_DrawInstancedIndirect *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DrawInstancedIndirect(context->host, QEMU_G2H(c->buffer), c->offset);
}

#endif

struct qemu_d3d11_immediate_context_Dispatch
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t thread_group_count_x;
    uint64_t thread_group_count_y;
    uint64_t thread_group_count_z;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_Dispatch(ID3D11DeviceContext1 *iface, UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z)
{
    struct qemu_d3d11_immediate_context_Dispatch call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DISPATCH);
    call.iface = (ULONG_PTR)context;
    call.thread_group_count_x = thread_group_count_x;
    call.thread_group_count_y = thread_group_count_y;
    call.thread_group_count_z = thread_group_count_z;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_Dispatch(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Dispatch *c = (struct qemu_d3d11_immediate_context_Dispatch *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_Dispatch(context->host, c->thread_group_count_x, c->thread_group_count_y, c->thread_group_count_z);
}

#endif

struct qemu_d3d11_immediate_context_DispatchIndirect
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DispatchIndirect(ID3D11DeviceContext1 *iface, ID3D11Buffer *buffer, UINT offset)
{
    struct qemu_d3d11_immediate_context_DispatchIndirect call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DISPATCHINDIRECT);
    call.iface = (ULONG_PTR)context;
    call.buffer = (ULONG_PTR)buffer;
    call.offset = offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DispatchIndirect(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DispatchIndirect *c = (struct qemu_d3d11_immediate_context_DispatchIndirect *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DispatchIndirect(context->host, QEMU_G2H(c->buffer), c->offset);
}

#endif

struct qemu_d3d11_immediate_context_RSSetState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetState(ID3D11DeviceContext1 *iface, ID3D11RasterizerState *rasterizer_state)
{
    struct qemu_d3d11_immediate_context_RSSetState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSSETSTATE);
    call.iface = (ULONG_PTR)context;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSSetState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSSetState *c = (struct qemu_d3d11_immediate_context_RSSetState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSSetState(context->host, QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d11_immediate_context_RSSetViewports
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t viewport_count;
    uint64_t viewports;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetViewports(ID3D11DeviceContext1 *iface, UINT viewport_count, const D3D11_VIEWPORT *viewports)
{
    struct qemu_d3d11_immediate_context_RSSetViewports call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSSETVIEWPORTS);
    call.iface = (ULONG_PTR)context;
    call.viewport_count = viewport_count;
    call.viewports = (ULONG_PTR)viewports;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSSetViewports(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSSetViewports *c = (struct qemu_d3d11_immediate_context_RSSetViewports *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSSetViewports(context->host, c->viewport_count, QEMU_G2H(c->viewports));
}

#endif

struct qemu_d3d11_immediate_context_RSSetScissorRects
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rect_count;
    uint64_t rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetScissorRects(ID3D11DeviceContext1 *iface, UINT rect_count, const D3D11_RECT *rects)
{
    struct qemu_d3d11_immediate_context_RSSetScissorRects call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSSETSCISSORRECTS);
    call.iface = (ULONG_PTR)context;
    call.rect_count = rect_count;
    call.rects = (ULONG_PTR)rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSSetScissorRects(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSSetScissorRects *c = (struct qemu_d3d11_immediate_context_RSSetScissorRects *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSSetScissorRects(context->host, c->rect_count, QEMU_G2H(c->rects));
}

#endif

struct qemu_d3d11_immediate_context_CopySubresourceRegion
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t dst_subresource_idx;
    uint64_t dst_x;
    uint64_t dst_y;
    uint64_t dst_z;
    uint64_t src_resource;
    uint64_t src_subresource_idx;
    uint64_t src_box;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CopySubresourceRegion(ID3D11DeviceContext1 *iface, ID3D11Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z, ID3D11Resource *src_resource, UINT src_subresource_idx, const D3D11_BOX *src_box)
{
    struct qemu_d3d11_immediate_context_CopySubresourceRegion call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_COPYSUBRESOURCEREGION);
    call.iface = (ULONG_PTR)context;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.dst_subresource_idx = dst_subresource_idx;
    call.dst_x = dst_x;
    call.dst_y = dst_y;
    call.dst_z = dst_z;
    call.src_resource = (ULONG_PTR)src_resource;
    call.src_subresource_idx = src_subresource_idx;
    call.src_box = (ULONG_PTR)src_box;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CopySubresourceRegion(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CopySubresourceRegion *c = (struct qemu_d3d11_immediate_context_CopySubresourceRegion *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CopySubresourceRegion(context->host, QEMU_G2H(c->dst_resource), c->dst_subresource_idx, c->dst_x, c->dst_y, c->dst_z, QEMU_G2H(c->src_resource), c->src_subresource_idx, QEMU_G2H(c->src_box));
}

#endif

struct qemu_d3d11_immediate_context_CopyResource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t src_resource;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CopyResource(ID3D11DeviceContext1 *iface, ID3D11Resource *dst_resource, ID3D11Resource *src_resource)
{
    struct qemu_d3d11_immediate_context_CopyResource call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_COPYRESOURCE);
    call.iface = (ULONG_PTR)context;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.src_resource = (ULONG_PTR)src_resource;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CopyResource(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CopyResource *c = (struct qemu_d3d11_immediate_context_CopyResource *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CopyResource(context->host, QEMU_G2H(c->dst_resource), QEMU_G2H(c->src_resource));
}

#endif

struct qemu_d3d11_immediate_context_UpdateSubresource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t subresource_idx;
    uint64_t box;
    uint64_t data;
    uint64_t row_pitch;
    uint64_t depth_pitch;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_UpdateSubresource(ID3D11DeviceContext1 *iface, ID3D11Resource *resource, UINT subresource_idx, const D3D11_BOX *box, const void *data, UINT row_pitch, UINT depth_pitch)
{
    struct qemu_d3d11_immediate_context_UpdateSubresource call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_UPDATESUBRESOURCE);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;
    call.subresource_idx = subresource_idx;
    call.box = (ULONG_PTR)box;
    call.data = (ULONG_PTR)data;
    call.row_pitch = row_pitch;
    call.depth_pitch = depth_pitch;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_UpdateSubresource(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_UpdateSubresource *c = (struct qemu_d3d11_immediate_context_UpdateSubresource *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_UpdateSubresource(context->host, QEMU_G2H(c->resource), c->subresource_idx, QEMU_G2H(c->box), QEMU_G2H(c->data), c->row_pitch, c->depth_pitch);
}

#endif

struct qemu_d3d11_immediate_context_CopyStructureCount
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_buffer;
    uint64_t dst_offset;
    uint64_t src_view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CopyStructureCount(ID3D11DeviceContext1 *iface, ID3D11Buffer *dst_buffer, UINT dst_offset, ID3D11UnorderedAccessView *src_view)
{
    struct qemu_d3d11_immediate_context_CopyStructureCount call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_COPYSTRUCTURECOUNT);
    call.iface = (ULONG_PTR)context;
    call.dst_buffer = (ULONG_PTR)dst_buffer;
    call.dst_offset = dst_offset;
    call.src_view = (ULONG_PTR)src_view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CopyStructureCount(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CopyStructureCount *c = (struct qemu_d3d11_immediate_context_CopyStructureCount *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CopyStructureCount(context->host, QEMU_G2H(c->dst_buffer), c->dst_offset, QEMU_G2H(c->src_view));
}

#endif

struct qemu_d3d11_immediate_context_ClearRenderTargetView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view;
    uint64_t color_rgba;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearRenderTargetView(ID3D11DeviceContext1 *iface, ID3D11RenderTargetView *render_target_view, const float color_rgba[4])
{
    struct qemu_d3d11_immediate_context_ClearRenderTargetView call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARRENDERTARGETVIEW);
    call.iface = (ULONG_PTR)context;
    call.render_target_view = (ULONG_PTR)render_target_view;
    call.color_rgba = (ULONG_PTR)color_rgba;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearRenderTargetView(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearRenderTargetView *c = (struct qemu_d3d11_immediate_context_ClearRenderTargetView *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearRenderTargetView(context->host, QEMU_G2H(c->render_target_view), QEMU_G2H(c->color_rgba));
}

#endif

struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewUint
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t unordered_access_view;
    uint64_t values;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearUnorderedAccessViewUint(ID3D11DeviceContext1 *iface, ID3D11UnorderedAccessView *unordered_access_view, const UINT values[4])
{
    struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewUint call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARUNORDEREDACCESSVIEWUINT);
    call.iface = (ULONG_PTR)context;
    call.unordered_access_view = (ULONG_PTR)unordered_access_view;
    call.values = (ULONG_PTR)values;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearUnorderedAccessViewUint(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewUint *c = (struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewUint *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearUnorderedAccessViewUint(context->host, QEMU_G2H(c->unordered_access_view), QEMU_G2H(c->values));
}

#endif

struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewFloat
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t unordered_access_view;
    uint64_t values;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearUnorderedAccessViewFloat(ID3D11DeviceContext1 *iface, ID3D11UnorderedAccessView *unordered_access_view, const float values[4])
{
    struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewFloat call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARUNORDEREDACCESSVIEWFLOAT);
    call.iface = (ULONG_PTR)context;
    call.unordered_access_view = (ULONG_PTR)unordered_access_view;
    call.values = (ULONG_PTR)values;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearUnorderedAccessViewFloat(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewFloat *c = (struct qemu_d3d11_immediate_context_ClearUnorderedAccessViewFloat *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearUnorderedAccessViewFloat(context->host, QEMU_G2H(c->unordered_access_view), QEMU_G2H(c->values));
}

#endif

struct qemu_d3d11_immediate_context_ClearDepthStencilView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_view;
    uint64_t flags;
    uint64_t depth;
    uint64_t stencil;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearDepthStencilView(ID3D11DeviceContext1 *iface, ID3D11DepthStencilView *depth_stencil_view, UINT flags, FLOAT depth, UINT8 stencil)
{
    struct qemu_d3d11_immediate_context_ClearDepthStencilView call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARDEPTHSTENCILVIEW);
    call.iface = (ULONG_PTR)context;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;
    call.flags = flags;
    call.depth = depth;
    call.stencil = stencil;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearDepthStencilView(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearDepthStencilView *c = (struct qemu_d3d11_immediate_context_ClearDepthStencilView *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearDepthStencilView(context->host, QEMU_G2H(c->depth_stencil_view), c->flags, c->depth, c->stencil);
}

#endif

struct qemu_d3d11_immediate_context_GenerateMips
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GenerateMips(ID3D11DeviceContext1 *iface, ID3D11ShaderResourceView *view)
{
    struct qemu_d3d11_immediate_context_GenerateMips call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GENERATEMIPS);
    call.iface = (ULONG_PTR)context;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GenerateMips(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GenerateMips *c = (struct qemu_d3d11_immediate_context_GenerateMips *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GenerateMips(context->host, QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_immediate_context_SetResourceMinLOD
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t min_lod;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_SetResourceMinLOD(ID3D11DeviceContext1 *iface, ID3D11Resource *resource, FLOAT min_lod)
{
    struct qemu_d3d11_immediate_context_SetResourceMinLOD call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SETRESOURCEMINLOD);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;
    call.min_lod = min_lod;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_SetResourceMinLOD(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SetResourceMinLOD *c = (struct qemu_d3d11_immediate_context_SetResourceMinLOD *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_SetResourceMinLOD(context->host, QEMU_G2H(c->resource), c->min_lod);
}

#endif

struct qemu_d3d11_immediate_context_GetResourceMinLOD
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
};

#ifdef QEMU_DLL_GUEST

static FLOAT STDMETHODCALLTYPE d3d11_immediate_context_GetResourceMinLOD(ID3D11DeviceContext1 *iface, ID3D11Resource *resource)
{
    struct qemu_d3d11_immediate_context_GetResourceMinLOD call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETRESOURCEMINLOD);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_GetResourceMinLOD(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetResourceMinLOD *c = (struct qemu_d3d11_immediate_context_GetResourceMinLOD *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_GetResourceMinLOD(context->host, QEMU_G2H(c->resource));
}

#endif

struct qemu_d3d11_immediate_context_ResolveSubresource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t dst_subresource_idx;
    uint64_t src_resource;
    uint64_t src_subresource_idx;
    uint64_t format;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ResolveSubresource(ID3D11DeviceContext1 *iface, ID3D11Resource *dst_resource, UINT dst_subresource_idx, ID3D11Resource *src_resource, UINT src_subresource_idx, DXGI_FORMAT format)
{
    struct qemu_d3d11_immediate_context_ResolveSubresource call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RESOLVESUBRESOURCE);
    call.iface = (ULONG_PTR)context;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.dst_subresource_idx = dst_subresource_idx;
    call.src_resource = (ULONG_PTR)src_resource;
    call.src_subresource_idx = src_subresource_idx;
    call.format = format;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ResolveSubresource(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ResolveSubresource *c = (struct qemu_d3d11_immediate_context_ResolveSubresource *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ResolveSubresource(context->host, QEMU_G2H(c->dst_resource), c->dst_subresource_idx, QEMU_G2H(c->src_resource), c->src_subresource_idx, c->format);
}

#endif

struct qemu_d3d11_immediate_context_ExecuteCommandList
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t command_list;
    uint64_t restore_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ExecuteCommandList(ID3D11DeviceContext1 *iface, ID3D11CommandList *command_list, BOOL restore_state)
{
    struct qemu_d3d11_immediate_context_ExecuteCommandList call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_EXECUTECOMMANDLIST);
    call.iface = (ULONG_PTR)context;
    call.command_list = (ULONG_PTR)command_list;
    call.restore_state = restore_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ExecuteCommandList(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ExecuteCommandList *c = (struct qemu_d3d11_immediate_context_ExecuteCommandList *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ExecuteCommandList(context->host, QEMU_G2H(c->command_list), c->restore_state);
}

#endif

struct qemu_d3d11_immediate_context_HSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_HSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSSetShaderResources *c = (struct qemu_d3d11_immediate_context_HSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_HSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetShader(ID3D11DeviceContext1 *iface, ID3D11HullShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_HSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSSetShader *c = (struct qemu_d3d11_immediate_context_HSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_HSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_HSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSSetSamplers *c = (struct qemu_d3d11_immediate_context_HSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_HSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_HSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_HSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_DSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_DSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSSetShaderResources *c = (struct qemu_d3d11_immediate_context_DSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_DSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetShader(ID3D11DeviceContext1 *iface, ID3D11DomainShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_DSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSSetShader *c = (struct qemu_d3d11_immediate_context_DSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_DSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_DSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSSetSamplers *c = (struct qemu_d3d11_immediate_context_DSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_DSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_DSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_DSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_CSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct qemu_d3d11_immediate_context_CSSetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetShaderResources *c = (struct qemu_d3d11_immediate_context_CSSetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_CSSetUnorderedAccessViews
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
    uint64_t initial_counts;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetUnorderedAccessViews(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11UnorderedAccessView *const *views, const UINT *initial_counts)
{
    struct qemu_d3d11_immediate_context_CSSetUnorderedAccessViews call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETUNORDEREDACCESSVIEWS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;
    call.initial_counts = (ULONG_PTR)initial_counts;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetUnorderedAccessViews(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetUnorderedAccessViews *c = (struct qemu_d3d11_immediate_context_CSSetUnorderedAccessViews *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetUnorderedAccessViews(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views), QEMU_G2H(c->initial_counts));
}

#endif

struct qemu_d3d11_immediate_context_CSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetShader(ID3D11DeviceContext1 *iface, ID3D11ComputeShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct qemu_d3d11_immediate_context_CSSetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetShader *c = (struct qemu_d3d11_immediate_context_CSSetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), c->class_instance_count);
}

#endif

struct qemu_d3d11_immediate_context_CSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct qemu_d3d11_immediate_context_CSSetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetSamplers *c = (struct qemu_d3d11_immediate_context_CSSetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_CSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct qemu_d3d11_immediate_context_CSSetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetConstantBuffers *c = (struct qemu_d3d11_immediate_context_CSSetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_VSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_VSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_VSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_PSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_PSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSGetShaderResources *c = (struct qemu_d3d11_immediate_context_PSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_PSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetShader(ID3D11DeviceContext1 *iface, ID3D11PixelShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_PSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSGetShader *c = (struct qemu_d3d11_immediate_context_PSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_PSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_PSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSGetSamplers *c = (struct qemu_d3d11_immediate_context_PSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_VSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetShader(ID3D11DeviceContext1 *iface, ID3D11VertexShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_VSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSGetShader *c = (struct qemu_d3d11_immediate_context_VSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_PSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_PSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_PSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_IAGetInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetInputLayout(ID3D11DeviceContext1 *iface, ID3D11InputLayout **input_layout)
{
    struct qemu_d3d11_immediate_context_IAGetInputLayout call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IAGETINPUTLAYOUT);
    call.iface = (ULONG_PTR)context;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IAGetInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IAGetInputLayout *c = (struct qemu_d3d11_immediate_context_IAGetInputLayout *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IAGetInputLayout(context->host, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d11_immediate_context_IAGetVertexBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t strides;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetVertexBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *strides, UINT *offsets)
{
    struct qemu_d3d11_immediate_context_IAGetVertexBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IAGETVERTEXBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.strides = (ULONG_PTR)strides;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IAGetVertexBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IAGetVertexBuffers *c = (struct qemu_d3d11_immediate_context_IAGetVertexBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IAGetVertexBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->strides), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d11_immediate_context_IAGetIndexBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t format;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetIndexBuffer(ID3D11DeviceContext1 *iface, ID3D11Buffer **buffer, DXGI_FORMAT *format, UINT *offset)
{
    struct qemu_d3d11_immediate_context_IAGetIndexBuffer call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IAGETINDEXBUFFER);
    call.iface = (ULONG_PTR)context;
    call.buffer = (ULONG_PTR)buffer;
    call.format = (ULONG_PTR)format;
    call.offset = (ULONG_PTR)offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IAGetIndexBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IAGetIndexBuffer *c = (struct qemu_d3d11_immediate_context_IAGetIndexBuffer *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IAGetIndexBuffer(context->host, QEMU_G2H(c->buffer), QEMU_G2H(c->format), QEMU_G2H(c->offset));
}

#endif

struct qemu_d3d11_immediate_context_GSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_GSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_GSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_GSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetShader(ID3D11DeviceContext1 *iface, ID3D11GeometryShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_GSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSGetShader *c = (struct qemu_d3d11_immediate_context_GSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_IAGetPrimitiveTopology
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t topology;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetPrimitiveTopology(ID3D11DeviceContext1 *iface, D3D11_PRIMITIVE_TOPOLOGY *topology)
{
    struct qemu_d3d11_immediate_context_IAGetPrimitiveTopology call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_IAGETPRIMITIVETOPOLOGY);
    call.iface = (ULONG_PTR)context;
    call.topology = (ULONG_PTR)topology;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_IAGetPrimitiveTopology(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_IAGetPrimitiveTopology *c = (struct qemu_d3d11_immediate_context_IAGetPrimitiveTopology *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_IAGetPrimitiveTopology(context->host, QEMU_G2H(c->topology));
}

#endif

struct qemu_d3d11_immediate_context_VSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_VSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSGetShaderResources *c = (struct qemu_d3d11_immediate_context_VSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_VSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_VSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSGetSamplers *c = (struct qemu_d3d11_immediate_context_VSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_GetPredication
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t predicate;
    uint64_t value;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GetPredication(ID3D11DeviceContext1 *iface, ID3D11Predicate **predicate, BOOL *value)
{
    struct qemu_d3d11_immediate_context_GetPredication call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETPREDICATION);
    call.iface = (ULONG_PTR)context;
    call.predicate = (ULONG_PTR)predicate;
    call.value = (ULONG_PTR)value;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GetPredication(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetPredication *c = (struct qemu_d3d11_immediate_context_GetPredication *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GetPredication(context->host, QEMU_G2H(c->predicate), QEMU_G2H(c->value));
}

#endif

struct qemu_d3d11_immediate_context_GSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_GSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSGetShaderResources *c = (struct qemu_d3d11_immediate_context_GSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_GSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_GSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSGetSamplers *c = (struct qemu_d3d11_immediate_context_GSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_OMGetRenderTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetRenderTargets(ID3D11DeviceContext1 *iface, UINT render_target_view_count, ID3D11RenderTargetView **render_target_views, ID3D11DepthStencilView **depth_stencil_view)
{
    struct qemu_d3d11_immediate_context_OMGetRenderTargets call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMGETRENDERTARGETS);
    call.iface = (ULONG_PTR)context;
    call.render_target_view_count = render_target_view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMGetRenderTargets(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMGetRenderTargets *c = (struct qemu_d3d11_immediate_context_OMGetRenderTargets *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMGetRenderTargets(context->host, c->render_target_view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view));
}

#endif

struct qemu_d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
    uint64_t unordered_access_view_start_slot;
    uint64_t unordered_access_view_count;
    uint64_t unordered_access_views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext1 *iface, UINT render_target_view_count, ID3D11RenderTargetView **render_target_views, ID3D11DepthStencilView **depth_stencil_view, UINT unordered_access_view_start_slot, UINT unordered_access_view_count, ID3D11UnorderedAccessView **unordered_access_views)
{
    struct qemu_d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMGETRENDERTARGETSANDUNORDEREDACCESSVIEWS);
    call.iface = (ULONG_PTR)context;
    call.render_target_view_count = render_target_view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;
    call.unordered_access_view_start_slot = unordered_access_view_start_slot;
    call.unordered_access_view_count = unordered_access_view_count;
    call.unordered_access_views = (ULONG_PTR)unordered_access_views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews *c = (struct qemu_d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews(context->host, c->render_target_view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view), c->unordered_access_view_start_slot, c->unordered_access_view_count, QEMU_G2H(c->unordered_access_views));
}

#endif

struct qemu_d3d11_immediate_context_OMGetBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t blend_state;
    uint64_t blend_factor;
    uint64_t sample_mask;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetBlendState(ID3D11DeviceContext1 *iface, ID3D11BlendState **blend_state, FLOAT blend_factor[4], UINT *sample_mask)
{
    struct qemu_d3d11_immediate_context_OMGetBlendState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMGETBLENDSTATE);
    call.iface = (ULONG_PTR)context;
    call.blend_state = (ULONG_PTR)blend_state;
    call.blend_factor = (ULONG_PTR)blend_factor;
    call.sample_mask = (ULONG_PTR)sample_mask;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMGetBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMGetBlendState *c = (struct qemu_d3d11_immediate_context_OMGetBlendState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMGetBlendState(context->host, QEMU_G2H(c->blend_state), QEMU_G2H(c->blend_factor), QEMU_G2H(c->sample_mask));
}

#endif

struct qemu_d3d11_immediate_context_OMGetDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_state;
    uint64_t stencil_ref;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetDepthStencilState(ID3D11DeviceContext1 *iface, ID3D11DepthStencilState **depth_stencil_state, UINT *stencil_ref)
{
    struct qemu_d3d11_immediate_context_OMGetDepthStencilState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_OMGETDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)context;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;
    call.stencil_ref = (ULONG_PTR)stencil_ref;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_OMGetDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_OMGetDepthStencilState *c = (struct qemu_d3d11_immediate_context_OMGetDepthStencilState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_OMGetDepthStencilState(context->host, QEMU_G2H(c->depth_stencil_state), QEMU_G2H(c->stencil_ref));
}

#endif

struct qemu_d3d11_immediate_context_SOGetTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_SOGetTargets(ID3D11DeviceContext1 *iface, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_SOGetTargets call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SOGETTARGETS);
    call.iface = (ULONG_PTR)context;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_SOGetTargets(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SOGetTargets *c = (struct qemu_d3d11_immediate_context_SOGetTargets *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_SOGetTargets(context->host, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_RSGetState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetState(ID3D11DeviceContext1 *iface, ID3D11RasterizerState **rasterizer_state)
{
    struct qemu_d3d11_immediate_context_RSGetState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSGETSTATE);
    call.iface = (ULONG_PTR)context;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSGetState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSGetState *c = (struct qemu_d3d11_immediate_context_RSGetState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSGetState(context->host, QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d11_immediate_context_RSGetViewports
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t viewport_count;
    uint64_t viewports;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetViewports(ID3D11DeviceContext1 *iface, UINT *viewport_count, D3D11_VIEWPORT *viewports)
{
    struct qemu_d3d11_immediate_context_RSGetViewports call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSGETVIEWPORTS);
    call.iface = (ULONG_PTR)context;
    call.viewport_count = (ULONG_PTR)viewport_count;
    call.viewports = (ULONG_PTR)viewports;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSGetViewports(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSGetViewports *c = (struct qemu_d3d11_immediate_context_RSGetViewports *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSGetViewports(context->host, QEMU_G2H(c->viewport_count), QEMU_G2H(c->viewports));
}

#endif

struct qemu_d3d11_immediate_context_RSGetScissorRects
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rect_count;
    uint64_t rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetScissorRects(ID3D11DeviceContext1 *iface, UINT *rect_count, D3D11_RECT *rects)
{
    struct qemu_d3d11_immediate_context_RSGetScissorRects call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_RSGETSCISSORRECTS);
    call.iface = (ULONG_PTR)context;
    call.rect_count = (ULONG_PTR)rect_count;
    call.rects = (ULONG_PTR)rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_RSGetScissorRects(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_RSGetScissorRects *c = (struct qemu_d3d11_immediate_context_RSGetScissorRects *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_RSGetScissorRects(context->host, QEMU_G2H(c->rect_count), QEMU_G2H(c->rects));
}

#endif

struct qemu_d3d11_immediate_context_HSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_HSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSGetShaderResources *c = (struct qemu_d3d11_immediate_context_HSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_HSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetShader(ID3D11DeviceContext1 *iface, ID3D11HullShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_HSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSGetShader *c = (struct qemu_d3d11_immediate_context_HSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_HSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_HSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSGetSamplers *c = (struct qemu_d3d11_immediate_context_HSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_HSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_HSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_HSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_DSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_DSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSGetShaderResources *c = (struct qemu_d3d11_immediate_context_DSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_DSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetShader(ID3D11DeviceContext1 *iface, ID3D11DomainShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_DSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSGetShader *c = (struct qemu_d3d11_immediate_context_DSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_DSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_DSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSGetSamplers *c = (struct qemu_d3d11_immediate_context_DSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_DSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_DSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_DSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_CSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetShaderResources(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct qemu_d3d11_immediate_context_CSGetShaderResources call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetShaderResources *c = (struct qemu_d3d11_immediate_context_CSGetShaderResources *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetShaderResources(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_CSGetUnorderedAccessViews
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetUnorderedAccessViews(ID3D11DeviceContext1 *iface, UINT start_slot, UINT view_count, ID3D11UnorderedAccessView **views)
{
    struct qemu_d3d11_immediate_context_CSGetUnorderedAccessViews call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETUNORDEREDACCESSVIEWS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetUnorderedAccessViews(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetUnorderedAccessViews *c = (struct qemu_d3d11_immediate_context_CSGetUnorderedAccessViews *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetUnorderedAccessViews(context->host, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d11_immediate_context_CSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
    uint64_t class_instances;
    uint64_t class_instance_count;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetShader(ID3D11DeviceContext1 *iface, ID3D11ComputeShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct qemu_d3d11_immediate_context_CSGetShader call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETSHADER);
    call.iface = (ULONG_PTR)context;
    call.shader = (ULONG_PTR)shader;
    call.class_instances = (ULONG_PTR)class_instances;
    call.class_instance_count = (ULONG_PTR)class_instance_count;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetShader *c = (struct qemu_d3d11_immediate_context_CSGetShader *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetShader(context->host, QEMU_G2H(c->shader), QEMU_G2H(c->class_instances), QEMU_G2H(c->class_instance_count));
}

#endif

struct qemu_d3d11_immediate_context_CSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetSamplers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct qemu_d3d11_immediate_context_CSGetSamplers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETSAMPLERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetSamplers *c = (struct qemu_d3d11_immediate_context_CSGetSamplers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetSamplers(context->host, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d11_immediate_context_CSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetConstantBuffers(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct qemu_d3d11_immediate_context_CSGetConstantBuffers call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetConstantBuffers *c = (struct qemu_d3d11_immediate_context_CSGetConstantBuffers *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetConstantBuffers(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d11_immediate_context_ClearState
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearState(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_ClearState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARSTATE);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearState *c = (struct qemu_d3d11_immediate_context_ClearState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearState(context->host);
}

#endif

struct qemu_d3d11_immediate_context_Flush
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_Flush(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_Flush call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_FLUSH);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_Flush(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_Flush *c = (struct qemu_d3d11_immediate_context_Flush *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_Flush(context->host);
}

#endif

struct qemu_d3d11_immediate_context_GetType
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE d3d11_immediate_context_GetType(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_GetType call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETTYPE);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_GetType(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetType *c = (struct qemu_d3d11_immediate_context_GetType *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_GetType(context->host);
}

#endif

struct qemu_d3d11_immediate_context_GetContextFlags
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static UINT STDMETHODCALLTYPE d3d11_immediate_context_GetContextFlags(ID3D11DeviceContext1 *iface)
{
    struct qemu_d3d11_immediate_context_GetContextFlags call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GETCONTEXTFLAGS);
    call.iface = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_GetContextFlags(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GetContextFlags *c = (struct qemu_d3d11_immediate_context_GetContextFlags *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_GetContextFlags(context->host);
}

#endif

struct qemu_d3d11_immediate_context_FinishCommandList
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t restore;
    uint64_t command_list;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_FinishCommandList(ID3D11DeviceContext1 *iface, BOOL restore, ID3D11CommandList **command_list)
{
    struct qemu_d3d11_immediate_context_FinishCommandList call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_FINISHCOMMANDLIST);
    call.iface = (ULONG_PTR)context;
    call.restore = restore;
    call.command_list = (ULONG_PTR)command_list;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_immediate_context_FinishCommandList(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_FinishCommandList *c = (struct qemu_d3d11_immediate_context_FinishCommandList *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DeviceContext1_FinishCommandList(context->host, c->restore, QEMU_G2H(c->command_list));
}

#endif

struct qemu_d3d11_immediate_context_CopySubresourceRegion1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t dst_subresource_idx;
    uint64_t dst_x;
    uint64_t dst_y;
    uint64_t dst_z;
    uint64_t src_resource;
    uint64_t src_subresource_idx;
    uint64_t src_box;
    uint64_t flags;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CopySubresourceRegion1(ID3D11DeviceContext1 *iface, ID3D11Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z, ID3D11Resource *src_resource, UINT src_subresource_idx, const D3D11_BOX *src_box, UINT flags)
{
    struct qemu_d3d11_immediate_context_CopySubresourceRegion1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_COPYSUBRESOURCEREGION1);
    call.iface = (ULONG_PTR)context;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.dst_subresource_idx = dst_subresource_idx;
    call.dst_x = dst_x;
    call.dst_y = dst_y;
    call.dst_z = dst_z;
    call.src_resource = (ULONG_PTR)src_resource;
    call.src_subresource_idx = src_subresource_idx;
    call.src_box = (ULONG_PTR)src_box;
    call.flags = flags;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CopySubresourceRegion1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CopySubresourceRegion1 *c = (struct qemu_d3d11_immediate_context_CopySubresourceRegion1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CopySubresourceRegion1(context->host, QEMU_G2H(c->dst_resource), c->dst_subresource_idx, c->dst_x, c->dst_y, c->dst_z, QEMU_G2H(c->src_resource), c->src_subresource_idx, QEMU_G2H(c->src_box), c->flags);
}

#endif

struct qemu_d3d11_immediate_context_UpdateSubresource1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t subresource_idx;
    uint64_t box;
    uint64_t data;
    uint64_t row_pitch;
    uint64_t depth_pitch;
    uint64_t flags;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_UpdateSubresource1(ID3D11DeviceContext1 *iface, ID3D11Resource *resource, UINT subresource_idx, const D3D11_BOX *box, const void *data, UINT row_pitch, UINT depth_pitch, UINT flags)
{
    struct qemu_d3d11_immediate_context_UpdateSubresource1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_UPDATESUBRESOURCE1);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;
    call.subresource_idx = subresource_idx;
    call.box = (ULONG_PTR)box;
    call.data = (ULONG_PTR)data;
    call.row_pitch = row_pitch;
    call.depth_pitch = depth_pitch;
    call.flags = flags;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_UpdateSubresource1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_UpdateSubresource1 *c = (struct qemu_d3d11_immediate_context_UpdateSubresource1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_UpdateSubresource1(context->host, QEMU_G2H(c->resource), c->subresource_idx, QEMU_G2H(c->box), QEMU_G2H(c->data), c->row_pitch, c->depth_pitch, c->flags);
}

#endif

struct qemu_d3d11_immediate_context_DiscardResource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DiscardResource(ID3D11DeviceContext1 *iface, ID3D11Resource *resource)
{
    struct qemu_d3d11_immediate_context_DiscardResource call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DISCARDRESOURCE);
    call.iface = (ULONG_PTR)context;
    call.resource = (ULONG_PTR)resource;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DiscardResource(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DiscardResource *c = (struct qemu_d3d11_immediate_context_DiscardResource *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DiscardResource(context->host, QEMU_G2H(c->resource));
}

#endif

struct qemu_d3d11_immediate_context_DiscardView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DiscardView(ID3D11DeviceContext1 *iface, ID3D11View *view)
{
    struct qemu_d3d11_immediate_context_DiscardView call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DISCARDVIEW);
    call.iface = (ULONG_PTR)context;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DiscardView(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DiscardView *c = (struct qemu_d3d11_immediate_context_DiscardView *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DiscardView(context->host, QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_immediate_context_VSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_VSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_VSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_HSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_HSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_HSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_DSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_DSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_DSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_GSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_GSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_GSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_PSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_PSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_PSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_CSSetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer * const *buffers, const UINT *first_constant, const UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_CSSetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSSETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSSetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSSetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_CSSetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSSetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_VSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_VSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_VSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_VSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_VSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_VSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_VSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_HSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_HSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_HSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_HSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_HSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_HSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_HSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_DSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_DSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_DSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_GSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_GSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_GSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_GSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_GSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_GSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_GSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_PSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_PSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_PSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_PSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_PSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_PSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_PSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_CSGetConstantBuffers1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t first_constant;
    uint64_t num_constants;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetConstantBuffers1(ID3D11DeviceContext1 *iface, UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *first_constant, UINT *num_constants)
{
    struct qemu_d3d11_immediate_context_CSGetConstantBuffers1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CSGETCONSTANTBUFFERS1);
    call.iface = (ULONG_PTR)context;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.first_constant = (ULONG_PTR)first_constant;
    call.num_constants = (ULONG_PTR)num_constants;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_CSGetConstantBuffers1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_CSGetConstantBuffers1 *c = (struct qemu_d3d11_immediate_context_CSGetConstantBuffers1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_CSGetConstantBuffers1(context->host, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->first_constant), QEMU_G2H(c->num_constants));
}

#endif

struct qemu_d3d11_immediate_context_SwapDeviceContextState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t state;
    uint64_t prev_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_SwapDeviceContextState(ID3D11DeviceContext1 *iface, ID3DDeviceContextState *state, ID3DDeviceContextState **prev_state)
{
    struct qemu_d3d11_immediate_context_SwapDeviceContextState call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_SWAPDEVICECONTEXTSTATE);
    call.iface = (ULONG_PTR)context;
    call.state = (ULONG_PTR)state;
    call.prev_state = (ULONG_PTR)prev_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_SwapDeviceContextState(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_SwapDeviceContextState *c = (struct qemu_d3d11_immediate_context_SwapDeviceContextState *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_SwapDeviceContextState(context->host, QEMU_G2H(c->state), QEMU_G2H(c->prev_state));
}

#endif

struct qemu_d3d11_immediate_context_ClearView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view;
    uint64_t color;
    uint64_t rect;
    uint64_t num_rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearView(ID3D11DeviceContext1 *iface, ID3D11View *view, const FLOAT color[4], const D3D11_RECT *rect, UINT num_rects)
{
    struct qemu_d3d11_immediate_context_ClearView call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_CLEARVIEW);
    call.iface = (ULONG_PTR)context;
    call.view = (ULONG_PTR)view;
    call.color = (ULONG_PTR)color;
    call.rect = (ULONG_PTR)rect;
    call.num_rects = num_rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_ClearView(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_ClearView *c = (struct qemu_d3d11_immediate_context_ClearView *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_ClearView(context->host, QEMU_G2H(c->view), QEMU_G2H(c->color), QEMU_G2H(c->rect), c->num_rects);
}

#endif

struct qemu_d3d11_immediate_context_DiscardView1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view;
    uint64_t rects;
    uint64_t num_rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_immediate_context_DiscardView1(ID3D11DeviceContext1 *iface, ID3D11View *view, const D3D11_RECT *rects, UINT num_rects)
{
    struct qemu_d3d11_immediate_context_DiscardView1 call;
    struct qemu_d3d11_device_context *context = impl_from_ID3D11DeviceContext1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_IMMEDIATE_CONTEXT_DISCARDVIEW1);
    call.iface = (ULONG_PTR)context;
    call.view = (ULONG_PTR)view;
    call.rects = (ULONG_PTR)rects;
    call.num_rects = num_rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_immediate_context_DiscardView1(struct qemu_syscall *call)
{
    struct qemu_d3d11_immediate_context_DiscardView1 *c = (struct qemu_d3d11_immediate_context_DiscardView1 *)call;
    struct qemu_d3d11_device_context *context;

    WINE_FIXME("Unverified!\n");
    context = QEMU_G2H(c->iface);

    ID3D11DeviceContext1_DiscardView1(context->host, QEMU_G2H(c->view), QEMU_G2H(c->rects), c->num_rects);
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_QueryInterface(ID3D11Device2 *iface, REFIID riid, void **out)
{
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, out);
}

static ULONG STDMETHODCALLTYPE d3d11_device_AddRef(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d11_device_Release(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    return IUnknown_Release(device->outer_unk);
}

#endif

struct qemu_d3d11_device_CreateBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t data;
    uint64_t buffer;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateBuffer(ID3D11Device2 *iface, const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Buffer **buffer)
{
    struct qemu_d3d11_device_CreateBuffer call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEBUFFER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.data = (ULONG_PTR)data;
    call.buffer = (ULONG_PTR)buffer;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateBuffer *c = (struct qemu_d3d11_device_CreateBuffer *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateBuffer(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->data), QEMU_G2H(c->buffer));
}

#endif

struct qemu_d3d11_device_CreateTexture1D
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t data;
    uint64_t texture;
    uint64_t dxgi_surface;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture1D(ID3D11Device2 *iface, const D3D11_TEXTURE1D_DESC *desc,
        const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture1D **texture)
{
    struct qemu_d3d11_device_CreateTexture1D call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    struct qemu_d3d11_texture *obj;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATETEXTURE1D);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);
    if (FAILED(call.super.iret))
        return call.super.iret;

    obj = (struct qemu_d3d11_texture *)(ULONG_PTR)call.texture;
    qemu_d3d11_texture_guest_init(obj, device, 1, call.dxgi_surface);
    *texture = &obj->ID3D11Texture1D_iface;

    return call.super.iret;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture1D(ID3D10Device1 *iface,
        const D3D10_TEXTURE1D_DESC *desc, const D3D10_SUBRESOURCE_DATA *data, ID3D10Texture1D **texture)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    D3D11_TEXTURE1D_DESC d3d11_desc;
    ID3D11Texture1D *tex11;
    HRESULT hr;

    WINE_TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.ArraySize = desc->ArraySize;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    hr = d3d11_device_CreateTexture1D(&device->ID3D11Device2_iface, &d3d11_desc,
            (const D3D11_SUBRESOURCE_DATA *)data, &tex11);
    if (FAILED(hr))
        return hr;

    hr = ID3D11Texture2D_QueryInterface(tex11, &IID_ID3D10Texture1D, (void **)texture);
    ID3D11Texture2D_Release(tex11);
    return hr;
}

#else

void qemu_d3d11_device_CreateTexture1D(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateTexture1D *c = (struct qemu_d3d11_device_CreateTexture1D *)call;
    struct qemu_d3d11_device *device;
    struct qemu_d3d11_texture *obj;
    ID3D11Texture1D *host;
    D3D11_SUBRESOURCE_DATA stack, *data = &stack;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);
#if GUEST_BIT == HOST_BIT
    data = QEMU_G2H(c->data);
#else
    if (c->data)
        D3D11_SUBRESOURCE_DATA_g2h(data, QEMU_G2H(c->data));
    else
        data = NULL;
#endif

    c->super.iret = ID3D11Device2_CreateTexture1D(device->host_d3d11, QEMU_G2H(c->desc), data, &host);
    if (FAILED(c->super.iret))
        return;

    c->super.iret = qemu_d3d11_texture_create((ID3D11Resource *)host, device, &c->dxgi_surface, 1, &obj);
    if (FAILED(c->super.iret))
        ID3D11Texture1D_Release(host);

    c->texture = QEMU_H2G(obj);
}

#endif

struct qemu_d3d11_device_CreateTexture2D
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t data;
    uint64_t texture;
    uint64_t dxgi_surface;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture2D(ID3D11Device2 *iface, const D3D11_TEXTURE2D_DESC *desc,
        const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture2D **texture)
{
    struct qemu_d3d11_device_CreateTexture2D call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    struct qemu_d3d11_texture *obj;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATETEXTURE2D);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);
    if (FAILED(call.super.iret))
        return call.super.iret;

    obj = (struct qemu_d3d11_texture *)(ULONG_PTR)call.texture;
    qemu_d3d11_texture_guest_init(obj, device, 2, call.dxgi_surface);
    *texture = &obj->ID3D11Texture2D_iface;

    return call.super.iret;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture2D(ID3D10Device1 *iface, const D3D10_TEXTURE2D_DESC *desc,
        const D3D10_SUBRESOURCE_DATA *data, ID3D10Texture2D **texture)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    ID3D11Texture2D *tex11;
    HRESULT hr;
    D3D11_TEXTURE2D_DESC d3d11_desc;

    WINE_TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.Height = desc->Height;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.ArraySize = desc->ArraySize;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.SampleDesc = desc->SampleDesc;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    hr = d3d11_device_CreateTexture2D(&device->ID3D11Device2_iface, &d3d11_desc,
            (const D3D11_SUBRESOURCE_DATA *)data, &tex11);
    if (FAILED(hr))
        return hr;

    hr = ID3D11Texture2D_QueryInterface(tex11, &IID_ID3D10Texture2D, (void **)texture);
    ID3D11Texture2D_Release(tex11);
    return hr;
}

#else

void qemu_d3d11_device_CreateTexture2D(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateTexture2D *c = (struct qemu_d3d11_device_CreateTexture2D *)call;
    struct qemu_d3d11_device *device;
    struct qemu_d3d11_texture *obj;
    ID3D11Texture2D *host;
    D3D11_SUBRESOURCE_DATA stack, *data = &stack;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);
#if GUEST_BIT == HOST_BIT
    data = QEMU_G2H(c->data);
#else
    if (c->data)
        D3D11_SUBRESOURCE_DATA_g2h(data, QEMU_G2H(c->data));
    else
        data = NULL;
#endif

    c->super.iret = ID3D11Device2_CreateTexture2D(device->host_d3d11, QEMU_G2H(c->desc), data, &host);
    if (FAILED(c->super.iret))
        return;

    c->super.iret = qemu_d3d11_texture_create((ID3D11Resource *)host, device, &c->dxgi_surface, 2, &obj);
    if (FAILED(c->super.iret))
        ID3D11Texture2D_Release(host);

    c->texture = QEMU_H2G(obj);
}

#endif

struct qemu_d3d11_device_CreateTexture3D
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t data;
    uint64_t texture;
    uint64_t dxgi_surface;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture3D(ID3D11Device2 *iface, const D3D11_TEXTURE3D_DESC *desc,
        const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture3D **texture)
{
    struct qemu_d3d11_device_CreateTexture3D call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    struct qemu_d3d11_texture *obj;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATETEXTURE3D);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);
    if (FAILED(call.super.iret))
        return call.super.iret;

    obj = (struct qemu_d3d11_texture *)(ULONG_PTR)call.texture;
    qemu_d3d11_texture_guest_init(obj, device, 3, call.dxgi_surface);
    *texture = &obj->ID3D11Texture3D_iface;

    return call.super.iret;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture3D(ID3D10Device1 *iface, const D3D10_TEXTURE3D_DESC *desc,
        const D3D10_SUBRESOURCE_DATA *data, ID3D10Texture3D **texture)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    D3D11_TEXTURE3D_DESC d3d11_desc;
    ID3D11Texture3D *tex11;
    HRESULT hr;

    WINE_TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.Height = desc->Height;
    d3d11_desc.Depth = desc->Depth;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    hr = d3d11_device_CreateTexture3D(&device->ID3D11Device2_iface, &d3d11_desc,
            (const D3D11_SUBRESOURCE_DATA *)data, &tex11);
    if (FAILED(hr))
        return hr;

    hr = ID3D11Texture3D_QueryInterface(tex11, &IID_ID3D10Texture3D, (void **)texture);
    ID3D11Texture3D_Release(tex11);
    return hr;
}

#else

void qemu_d3d11_device_CreateTexture3D(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateTexture3D *c = (struct qemu_d3d11_device_CreateTexture3D *)call;
    struct qemu_d3d11_device *device;
    struct qemu_d3d11_texture *obj;
    ID3D11Texture3D *host;
    D3D11_SUBRESOURCE_DATA stack, *data = &stack;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);
#if GUEST_BIT == HOST_BIT
    data = QEMU_G2H(c->data);
#else
    if (c->data)
        D3D11_SUBRESOURCE_DATA_g2h(data, QEMU_G2H(c->data));
    else
        data = NULL;
#endif

    c->super.iret = ID3D11Device2_CreateTexture3D(device->host_d3d11, QEMU_G2H(c->desc), data, &host);
    if (FAILED(c->super.iret))
        return;

    c->super.iret = qemu_d3d11_texture_create((ID3D11Resource *)host, device, &c->dxgi_surface, 3, &obj);
    if (FAILED(c->super.iret))
        ID3D11Texture3D_Release(host);

    c->texture = QEMU_H2G(obj);
}

#endif

struct qemu_d3d11_device_CreateShaderResourceView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateShaderResourceView(ID3D11Device2 *iface, ID3D11Resource *resource, const D3D11_SHADER_RESOURCE_VIEW_DESC *desc, ID3D11ShaderResourceView **view)
{
    struct qemu_d3d11_device_CreateShaderResourceView call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATESHADERRESOURCEVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateShaderResourceView(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateShaderResourceView *c = (struct qemu_d3d11_device_CreateShaderResourceView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateShaderResourceView(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_device_CreateUnorderedAccessView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateUnorderedAccessView(ID3D11Device2 *iface, ID3D11Resource *resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *desc, ID3D11UnorderedAccessView **view)
{
    struct qemu_d3d11_device_CreateUnorderedAccessView call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEUNORDEREDACCESSVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateUnorderedAccessView(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateUnorderedAccessView *c = (struct qemu_d3d11_device_CreateUnorderedAccessView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateUnorderedAccessView(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_device_CreateRenderTargetView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateRenderTargetView(ID3D11Device2 *iface, ID3D11Resource *resource, const D3D11_RENDER_TARGET_VIEW_DESC *desc, ID3D11RenderTargetView **view)
{
    struct qemu_d3d11_device_CreateRenderTargetView call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATERENDERTARGETVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateRenderTargetView(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateRenderTargetView *c = (struct qemu_d3d11_device_CreateRenderTargetView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateRenderTargetView(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_device_CreateDepthStencilView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDepthStencilView(ID3D11Device2 *iface, ID3D11Resource *resource, const D3D11_DEPTH_STENCIL_VIEW_DESC *desc, ID3D11DepthStencilView **view)
{
    struct qemu_d3d11_device_CreateDepthStencilView call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEPTHSTENCILVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDepthStencilView(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDepthStencilView *c = (struct qemu_d3d11_device_CreateDepthStencilView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDepthStencilView(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d11_device_CreateInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t element_descs;
    uint64_t element_count;
    uint64_t shader_byte_code;
    uint64_t shader_byte_code_length;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateInputLayout(ID3D11Device2 *iface, const D3D11_INPUT_ELEMENT_DESC *element_descs, UINT element_count, const void *shader_byte_code, SIZE_T shader_byte_code_length, ID3D11InputLayout **input_layout)
{
    struct qemu_d3d11_device_CreateInputLayout call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEINPUTLAYOUT);
    call.iface = (ULONG_PTR)device;
    call.element_descs = (ULONG_PTR)element_descs;
    call.element_count = element_count;
    call.shader_byte_code = (ULONG_PTR)shader_byte_code;
    call.shader_byte_code_length = shader_byte_code_length;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateInputLayout *c = (struct qemu_d3d11_device_CreateInputLayout *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateInputLayout(device->host_d3d11, QEMU_G2H(c->element_descs), c->element_count, QEMU_G2H(c->shader_byte_code), c->shader_byte_code_length, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d11_device_CreateVertexShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateVertexShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11VertexShader **shader)
{
    struct qemu_d3d11_device_CreateVertexShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEVERTEXSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateVertexShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateVertexShader *c = (struct qemu_d3d11_device_CreateVertexShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateVertexShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateGeometryShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateGeometryShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11GeometryShader **shader)
{
    struct qemu_d3d11_device_CreateGeometryShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEGEOMETRYSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateGeometryShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateGeometryShader *c = (struct qemu_d3d11_device_CreateGeometryShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateGeometryShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateGeometryShaderWithStreamOutput
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t so_entries;
    uint64_t entry_count;
    uint64_t buffer_strides;
    uint64_t strides_count;
    uint64_t rasterizer_stream;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateGeometryShaderWithStreamOutput(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, const D3D11_SO_DECLARATION_ENTRY *so_entries, UINT entry_count, const UINT *buffer_strides, UINT strides_count, UINT rasterizer_stream, ID3D11ClassLinkage *class_linkage, ID3D11GeometryShader **shader)
{
    struct qemu_d3d11_device_CreateGeometryShaderWithStreamOutput call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.so_entries = (ULONG_PTR)so_entries;
    call.entry_count = entry_count;
    call.buffer_strides = (ULONG_PTR)buffer_strides;
    call.strides_count = strides_count;
    call.rasterizer_stream = rasterizer_stream;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateGeometryShaderWithStreamOutput(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateGeometryShaderWithStreamOutput *c = (struct qemu_d3d11_device_CreateGeometryShaderWithStreamOutput *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateGeometryShaderWithStreamOutput(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->so_entries), c->entry_count, QEMU_G2H(c->buffer_strides), c->strides_count, c->rasterizer_stream, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreatePixelShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreatePixelShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11PixelShader **shader)
{
    struct qemu_d3d11_device_CreatePixelShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEPIXELSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreatePixelShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreatePixelShader *c = (struct qemu_d3d11_device_CreatePixelShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreatePixelShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateHullShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateHullShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11HullShader **shader)
{
    struct qemu_d3d11_device_CreateHullShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEHULLSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateHullShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateHullShader *c = (struct qemu_d3d11_device_CreateHullShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateHullShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateDomainShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDomainShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11DomainShader **shader)
{
    struct qemu_d3d11_device_CreateDomainShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDOMAINSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDomainShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDomainShader *c = (struct qemu_d3d11_device_CreateDomainShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDomainShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateComputeShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t class_linkage;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateComputeShader(ID3D11Device2 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11ComputeShader **shader)
{
    struct qemu_d3d11_device_CreateComputeShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATECOMPUTESHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.class_linkage = (ULONG_PTR)class_linkage;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateComputeShader(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateComputeShader *c = (struct qemu_d3d11_device_CreateComputeShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateComputeShader(device->host_d3d11, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->class_linkage), QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d11_device_CreateClassLinkage
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t class_linkage;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateClassLinkage(ID3D11Device2 *iface, ID3D11ClassLinkage **class_linkage)
{
    struct qemu_d3d11_device_CreateClassLinkage call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATECLASSLINKAGE);
    call.iface = (ULONG_PTR)device;
    call.class_linkage = (ULONG_PTR)class_linkage;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateClassLinkage(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateClassLinkage *c = (struct qemu_d3d11_device_CreateClassLinkage *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateClassLinkage(device->host_d3d11, QEMU_G2H(c->class_linkage));
}

#endif

struct qemu_d3d11_device_CreateBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t blend_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateBlendState(ID3D11Device2 *iface, const D3D11_BLEND_DESC *desc, ID3D11BlendState **blend_state)
{
    struct qemu_d3d11_device_CreateBlendState call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEBLENDSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.blend_state = (ULONG_PTR)blend_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateBlendState *c = (struct qemu_d3d11_device_CreateBlendState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateBlendState(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->blend_state));
}

#endif

struct qemu_d3d11_device_CreateDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t depth_stencil_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDepthStencilState(ID3D11Device2 *iface, const D3D11_DEPTH_STENCIL_DESC *desc, ID3D11DepthStencilState **depth_stencil_state)
{
    struct qemu_d3d11_device_CreateDepthStencilState call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDepthStencilState *c = (struct qemu_d3d11_device_CreateDepthStencilState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDepthStencilState(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->depth_stencil_state));
}

#endif

struct qemu_d3d11_device_CreateRasterizerState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateRasterizerState(ID3D11Device2 *iface, const D3D11_RASTERIZER_DESC *desc, ID3D11RasterizerState **rasterizer_state)
{
    struct qemu_d3d11_device_CreateRasterizerState call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATERASTERIZERSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateRasterizerState(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateRasterizerState *c = (struct qemu_d3d11_device_CreateRasterizerState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateRasterizerState(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d11_device_CreateSamplerState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t sampler_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateSamplerState(ID3D11Device2 *iface, const D3D11_SAMPLER_DESC *desc, ID3D11SamplerState **sampler_state)
{
    struct qemu_d3d11_device_CreateSamplerState call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATESAMPLERSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.sampler_state = (ULONG_PTR)sampler_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateSamplerState(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateSamplerState *c = (struct qemu_d3d11_device_CreateSamplerState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateSamplerState(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->sampler_state));
}

#endif

struct qemu_d3d11_device_CreateQuery
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t query;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateQuery(ID3D11Device2 *iface, const D3D11_QUERY_DESC *desc, ID3D11Query **query)
{
    struct qemu_d3d11_device_CreateQuery call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEQUERY);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.query = (ULONG_PTR)query;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateQuery(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateQuery *c = (struct qemu_d3d11_device_CreateQuery *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateQuery(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->query));
}

#endif

struct qemu_d3d11_device_CreatePredicate
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t predicate;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreatePredicate(ID3D11Device2 *iface, const D3D11_QUERY_DESC *desc, ID3D11Predicate **predicate)
{
    struct qemu_d3d11_device_CreatePredicate call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEPREDICATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.predicate = (ULONG_PTR)predicate;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreatePredicate(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreatePredicate *c = (struct qemu_d3d11_device_CreatePredicate *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreatePredicate(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->predicate));
}

#endif

struct qemu_d3d11_device_CreateCounter
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t counter;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateCounter(ID3D11Device2 *iface, const D3D11_COUNTER_DESC *desc, ID3D11Counter **counter)
{
    struct qemu_d3d11_device_CreateCounter call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATECOUNTER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.counter = (ULONG_PTR)counter;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateCounter(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateCounter *c = (struct qemu_d3d11_device_CreateCounter *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateCounter(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->counter));
}

#endif

struct qemu_d3d11_device_CreateDeferredContext
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
    uint64_t context;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDeferredContext(ID3D11Device2 *iface, UINT flags, ID3D11DeviceContext **context)
{
    struct qemu_d3d11_device_CreateDeferredContext call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEFERREDCONTEXT);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;
    call.context = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDeferredContext(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDeferredContext *c = (struct qemu_d3d11_device_CreateDeferredContext *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDeferredContext(device->host_d3d11, c->flags, QEMU_G2H(c->context));
}

#endif

struct qemu_d3d11_device_OpenSharedResource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t riid;
    uint64_t out;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_OpenSharedResource(ID3D11Device2 *iface, HANDLE resource, REFIID riid, void **out)
{
    struct qemu_d3d11_device_OpenSharedResource call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_OPENSHAREDRESOURCE);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.riid = (ULONG_PTR)riid;
    call.out = (ULONG_PTR)out;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_OpenSharedResource(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_OpenSharedResource *c = (struct qemu_d3d11_device_OpenSharedResource *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_OpenSharedResource(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->riid), QEMU_G2H(c->out));
}

#endif

struct qemu_d3d11_device_CheckFormatSupport
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t format;
    uint64_t format_support;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckFormatSupport(ID3D11Device2 *iface, DXGI_FORMAT format, UINT *format_support)
{
    struct qemu_d3d11_device_CheckFormatSupport call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKFORMATSUPPORT);
    call.iface = (ULONG_PTR)device;
    call.format = format;
    call.format_support = (ULONG_PTR)format_support;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CheckFormatSupport(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckFormatSupport *c = (struct qemu_d3d11_device_CheckFormatSupport *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CheckFormatSupport(device->host_d3d11, c->format, QEMU_G2H(c->format_support));
}

#endif

struct qemu_d3d11_device_CheckMultisampleQualityLevels
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t format;
    uint64_t sample_count;
    uint64_t quality_level_count;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckMultisampleQualityLevels(ID3D11Device2 *iface, DXGI_FORMAT format, UINT sample_count, UINT *quality_level_count)
{
    struct qemu_d3d11_device_CheckMultisampleQualityLevels call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKMULTISAMPLEQUALITYLEVELS);
    call.iface = (ULONG_PTR)device;
    call.format = format;
    call.sample_count = sample_count;
    call.quality_level_count = (ULONG_PTR)quality_level_count;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CheckMultisampleQualityLevels(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckMultisampleQualityLevels *c = (struct qemu_d3d11_device_CheckMultisampleQualityLevels *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CheckMultisampleQualityLevels(device->host_d3d11, c->format, c->sample_count, QEMU_G2H(c->quality_level_count));
}

#endif

struct qemu_d3d11_device_CheckCounterInfo
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t info;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_device_CheckCounterInfo(ID3D11Device2 *iface, D3D11_COUNTER_INFO *info)
{
    struct qemu_d3d11_device_CheckCounterInfo call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKCOUNTERINFO);
    call.iface = (ULONG_PTR)device;
    call.info = (ULONG_PTR)info;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_device_CheckCounterInfo(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckCounterInfo *c = (struct qemu_d3d11_device_CheckCounterInfo *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D11Device2_CheckCounterInfo(device->host_d3d11, QEMU_G2H(c->info));
}

#endif

struct qemu_d3d11_device_CheckCounter
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t type;
    uint64_t active_counter_count;
    uint64_t name;
    uint64_t name_length;
    uint64_t units;
    uint64_t units_length;
    uint64_t description;
    uint64_t description_length;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckCounter(ID3D11Device2 *iface, const D3D11_COUNTER_DESC *desc, D3D11_COUNTER_TYPE *type, UINT *active_counter_count, char *name, UINT *name_length, char *units, UINT *units_length, char *description, UINT *description_length)
{
    struct qemu_d3d11_device_CheckCounter call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKCOUNTER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.type = (ULONG_PTR)type;
    call.active_counter_count = (ULONG_PTR)active_counter_count;
    call.name = (ULONG_PTR)name;
    call.name_length = (ULONG_PTR)name_length;
    call.units = (ULONG_PTR)units;
    call.units_length = (ULONG_PTR)units_length;
    call.description = (ULONG_PTR)description;
    call.description_length = (ULONG_PTR)description_length;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CheckCounter(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckCounter *c = (struct qemu_d3d11_device_CheckCounter *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CheckCounter(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->type), QEMU_G2H(c->active_counter_count), QEMU_G2H(c->name), QEMU_G2H(c->name_length), QEMU_G2H(c->units), QEMU_G2H(c->units_length), QEMU_G2H(c->description), QEMU_G2H(c->description_length));
}

#endif

struct qemu_d3d11_device_CheckFeatureSupport
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t feature;
    uint64_t feature_support_data;
    uint64_t feature_support_data_size;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckFeatureSupport(ID3D11Device2 *iface, D3D11_FEATURE feature, void *feature_support_data, UINT feature_support_data_size)
{
    struct qemu_d3d11_device_CheckFeatureSupport call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKFEATURESUPPORT);
    call.iface = (ULONG_PTR)device;
    call.feature = feature;
    call.feature_support_data = (ULONG_PTR)feature_support_data;
    call.feature_support_data_size = feature_support_data_size;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CheckFeatureSupport(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckFeatureSupport *c = (struct qemu_d3d11_device_CheckFeatureSupport *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CheckFeatureSupport(device->host_d3d11, c->feature, QEMU_G2H(c->feature_support_data), c->feature_support_data_size);
}

#endif

struct qemu_d3d11_device_GetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_GetPrivateData(ID3D11Device2 *iface, REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_device_GetPrivateData call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETPRIVATEDATA);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data_size = (ULONG_PTR)data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_GetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetPrivateData *c = (struct qemu_d3d11_device_GetPrivateData *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_GetPrivateData(device->host_d3d11, QEMU_G2H(c->guid), QEMU_G2H(c->data_size), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_device_SetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_SetPrivateData(ID3D11Device2 *iface, REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_device_SetPrivateData call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_SETPRIVATEDATA);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data_size = data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_SetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_SetPrivateData *c = (struct qemu_d3d11_device_SetPrivateData *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_SetPrivateData(device->host_d3d11, QEMU_G2H(c->guid), c->data_size, QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_device_SetPrivateDataInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_SetPrivateDataInterface(ID3D11Device2 *iface, REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_device_SetPrivateDataInterface call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_SETPRIVATEDATAINTERFACE);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_SetPrivateDataInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_SetPrivateDataInterface *c = (struct qemu_d3d11_device_SetPrivateDataInterface *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_SetPrivateDataInterface(device->host_d3d11, QEMU_G2H(c->guid), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d11_device_GetFeatureLevel
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static D3D_FEATURE_LEVEL STDMETHODCALLTYPE d3d11_device_GetFeatureLevel(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device_GetFeatureLevel call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETFEATURELEVEL);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_GetFeatureLevel(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetFeatureLevel *c = (struct qemu_d3d11_device_GetFeatureLevel *)call;
    struct qemu_d3d11_device *device;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_GetFeatureLevel(device->host_d3d11);
}

#endif

struct qemu_d3d11_device_GetCreationFlags
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static UINT STDMETHODCALLTYPE d3d11_device_GetCreationFlags(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device_GetCreationFlags call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETCREATIONFLAGS);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_GetCreationFlags(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetCreationFlags *c = (struct qemu_d3d11_device_GetCreationFlags *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_GetCreationFlags(device->host_d3d11);
}

#endif

struct qemu_d3d11_device_GetDeviceRemovedReason
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_GetDeviceRemovedReason(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device_GetDeviceRemovedReason call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETDEVICEREMOVEDREASON);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_GetDeviceRemovedReason(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetDeviceRemovedReason *c = (struct qemu_d3d11_device_GetDeviceRemovedReason *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_GetDeviceRemovedReason(device->host_d3d11);
}

#endif

struct qemu_d3d11_device_GetImmediateContext
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t context;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_device_GetImmediateContext(ID3D11Device2 *iface,
        ID3D11DeviceContext **immediate_context)
{
    struct qemu_d3d11_device_GetImmediateContext call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);
    struct qemu_d3d11_device_context *context;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETIMMEDIATECONTEXT);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    context = (struct qemu_d3d11_device_context *)(ULONG_PTR)call.context;
    *immediate_context = &context->ID3D11DeviceContext1_iface;
}

static void STDMETHODCALLTYPE d3d11_device_GetImmediateContext1(ID3D11Device2 *iface, ID3D11DeviceContext1 **context)
{
    struct qemu_d3d11_device_GetImmediateContext call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETIMMEDIATECONTEXT1);
    call.iface = (ULONG_PTR)device;
    call.context = (ULONG_PTR)context;

    qemu_syscall(&call.super);
}

static void STDMETHODCALLTYPE d3d11_device_GetImmediateContext2(ID3D11Device2 *iface, ID3D11DeviceContext2 **context)
{
    struct qemu_d3d11_device_GetImmediateContext call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETIMMEDIATECONTEXT2);
    call.iface = (ULONG_PTR)device;
    call.context = (ULONG_PTR)context;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_device_GetImmediateContext(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetImmediateContext *c = (struct qemu_d3d11_device_GetImmediateContext *)call;
    struct qemu_d3d11_device *device;
    ID3D11DeviceContext1 *host;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);
    c->context = QEMU_H2G(&device->immediate_context);

    /* For refcounting */
    ID3D11Device2_GetImmediateContext(device->host_d3d11, (ID3D11DeviceContext **)&host);
    if (host != device->immediate_context.host)
        WINE_ERR("Got context %p, expected %p.\n", host, device->immediate_context.host);
}

void qemu_d3d11_device_GetImmediateContext1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetImmediateContext *c = (struct qemu_d3d11_device_GetImmediateContext *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D11Device2_GetImmediateContext1(device->host_d3d11, QEMU_G2H(c->context));
}

void qemu_d3d11_device_GetImmediateContext2(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetImmediateContext *c = (struct qemu_d3d11_device_GetImmediateContext *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D11Device2_GetImmediateContext2(device->host_d3d11, QEMU_G2H(c->context));
}

#endif

struct qemu_d3d11_device_SetExceptionMode
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_SetExceptionMode(ID3D11Device2 *iface, UINT flags)
{
    struct qemu_d3d11_device_SetExceptionMode call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_SETEXCEPTIONMODE);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_SetExceptionMode(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_SetExceptionMode *c = (struct qemu_d3d11_device_SetExceptionMode *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_SetExceptionMode(device->host_d3d11, c->flags);
}

#endif

struct qemu_d3d11_device_GetExceptionMode
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static UINT STDMETHODCALLTYPE d3d11_device_GetExceptionMode(ID3D11Device2 *iface)
{
    struct qemu_d3d11_device_GetExceptionMode call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETEXCEPTIONMODE);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_GetExceptionMode(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetExceptionMode *c = (struct qemu_d3d11_device_GetExceptionMode *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_GetExceptionMode(device->host_d3d11);
}

#endif

struct qemu_d3d11_device_CreateDeferredContext1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
    uint64_t context;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDeferredContext1(ID3D11Device2 *iface, UINT flags, ID3D11DeviceContext1 **context)
{
    struct qemu_d3d11_device_CreateDeferredContext1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEFERREDCONTEXT1);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;
    call.context = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDeferredContext1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDeferredContext1 *c = (struct qemu_d3d11_device_CreateDeferredContext1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDeferredContext1(device->host_d3d11, c->flags, QEMU_G2H(c->context));
}

#endif

struct qemu_d3d11_device_CreateBlendState1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateBlendState1(ID3D11Device2 *iface, const D3D11_BLEND_DESC1 *desc, ID3D11BlendState1 **state)
{
    struct qemu_d3d11_device_CreateBlendState1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEBLENDSTATE1);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.state = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateBlendState1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateBlendState1 *c = (struct qemu_d3d11_device_CreateBlendState1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateBlendState1(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->state));
}

#endif

struct qemu_d3d11_device_CreateRasterizerState1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateRasterizerState1(ID3D11Device2 *iface, const D3D11_RASTERIZER_DESC1 *desc, ID3D11RasterizerState1 **state)
{
    struct qemu_d3d11_device_CreateRasterizerState1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATERASTERIZERSTATE1);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.state = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateRasterizerState1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateRasterizerState1 *c = (struct qemu_d3d11_device_CreateRasterizerState1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateRasterizerState1(device->host_d3d11, QEMU_G2H(c->desc), QEMU_G2H(c->state));
}

#endif

struct qemu_d3d11_device_CreateDeviceContextState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
    uint64_t feature_levels;
    uint64_t feature_levels_count;
    uint64_t sdk_version;
    uint64_t emulated_interface;
    uint64_t chosen_feature_level;
    uint64_t state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDeviceContextState(ID3D11Device2 *iface, UINT flags, const D3D_FEATURE_LEVEL *feature_levels, UINT feature_levels_count, UINT sdk_version, REFIID emulated_interface, D3D_FEATURE_LEVEL *chosen_feature_level, ID3DDeviceContextState **state)
{
    struct qemu_d3d11_device_CreateDeviceContextState call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEVICECONTEXTSTATE);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;
    call.feature_levels = (ULONG_PTR)feature_levels;
    call.feature_levels_count = feature_levels_count;
    call.sdk_version = sdk_version;
    call.emulated_interface = (ULONG_PTR)emulated_interface;
    call.chosen_feature_level = (ULONG_PTR)chosen_feature_level;
    call.state = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDeviceContextState(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDeviceContextState *c = (struct qemu_d3d11_device_CreateDeviceContextState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDeviceContextState(device->host_d3d11, c->flags, QEMU_G2H(c->feature_levels), c->feature_levels_count, c->sdk_version, QEMU_G2H(c->emulated_interface), QEMU_G2H(c->chosen_feature_level), QEMU_G2H(c->state));
}

#endif

struct qemu_d3d11_device_OpenSharedResource1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t handle;
    uint64_t riid;
    uint64_t resource;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_OpenSharedResource1(ID3D11Device2 *iface, HANDLE handle, REFIID riid, void **resource)
{
    struct qemu_d3d11_device_OpenSharedResource1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_OPENSHAREDRESOURCE1);
    call.iface = (ULONG_PTR)device;
    call.handle = (ULONG_PTR)handle;
    call.riid = (ULONG_PTR)riid;
    call.resource = (ULONG_PTR)resource;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_OpenSharedResource1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_OpenSharedResource1 *c = (struct qemu_d3d11_device_OpenSharedResource1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_OpenSharedResource1(device->host_d3d11, QEMU_G2H(c->handle), QEMU_G2H(c->riid), QEMU_G2H(c->resource));
}

#endif

struct qemu_d3d11_device_OpenSharedResourceByName
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t name;
    uint64_t access;
    uint64_t riid;
    uint64_t resource;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_OpenSharedResourceByName(ID3D11Device2 *iface, const WCHAR *name, DWORD access, REFIID riid, void **resource)
{
    struct qemu_d3d11_device_OpenSharedResourceByName call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_OPENSHAREDRESOURCEBYNAME);
    call.iface = (ULONG_PTR)device;
    call.name = (ULONG_PTR)name;
    call.access = access;
    call.riid = (ULONG_PTR)riid;
    call.resource = (ULONG_PTR)resource;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_OpenSharedResourceByName(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_OpenSharedResourceByName *c = (struct qemu_d3d11_device_OpenSharedResourceByName *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_OpenSharedResourceByName(device->host_d3d11, QEMU_G2H(c->name), c->access, QEMU_G2H(c->riid), QEMU_G2H(c->resource));
}

#endif

struct qemu_d3d11_device_CreateDeferredContext2
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
    uint64_t context;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDeferredContext2(ID3D11Device2 *iface, UINT flags, ID3D11DeviceContext2 **context)
{
    struct qemu_d3d11_device_CreateDeferredContext2 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CREATEDEFERREDCONTEXT2);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;
    call.context = (ULONG_PTR)context;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CreateDeferredContext2(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CreateDeferredContext2 *c = (struct qemu_d3d11_device_CreateDeferredContext2 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CreateDeferredContext2(device->host_d3d11, c->flags, QEMU_G2H(c->context));
}

#endif

struct qemu_d3d11_device_GetResourceTiling
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t tile_count;
    uint64_t mip_desc;
    uint64_t tile_shape;
    uint64_t subresource_tiling_count;
    uint64_t first_subresource_tiling;
    uint64_t subresource_tiling;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_device_GetResourceTiling(ID3D11Device2 *iface, ID3D11Resource *resource, UINT *tile_count, D3D11_PACKED_MIP_DESC *mip_desc, D3D11_TILE_SHAPE *tile_shape, UINT *subresource_tiling_count, UINT first_subresource_tiling, D3D11_SUBRESOURCE_TILING *subresource_tiling)
{
    struct qemu_d3d11_device_GetResourceTiling call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_GETRESOURCETILING);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.tile_count = (ULONG_PTR)tile_count;
    call.mip_desc = (ULONG_PTR)mip_desc;
    call.tile_shape = (ULONG_PTR)tile_shape;
    call.subresource_tiling_count = (ULONG_PTR)subresource_tiling_count;
    call.first_subresource_tiling = first_subresource_tiling;
    call.subresource_tiling = (ULONG_PTR)subresource_tiling;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_device_GetResourceTiling(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_GetResourceTiling *c = (struct qemu_d3d11_device_GetResourceTiling *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D11Device2_GetResourceTiling(device->host_d3d11, QEMU_G2H(c->resource), QEMU_G2H(c->tile_count), QEMU_G2H(c->mip_desc), QEMU_G2H(c->tile_shape), QEMU_G2H(c->subresource_tiling_count), c->first_subresource_tiling, QEMU_G2H(c->subresource_tiling));
}

#endif

struct qemu_d3d11_device_CheckMultisampleQualityLevels1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t format;
    uint64_t sample_count;
    uint64_t flags;
    uint64_t quality_level_count;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckMultisampleQualityLevels1(ID3D11Device2 *iface, DXGI_FORMAT format, UINT sample_count, UINT flags, UINT *quality_level_count)
{
    struct qemu_d3d11_device_CheckMultisampleQualityLevels1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D11Device2(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEVICE_CHECKMULTISAMPLEQUALITYLEVELS1);
    call.iface = (ULONG_PTR)device;
    call.format = format;
    call.sample_count = sample_count;
    call.flags = flags;
    call.quality_level_count = (ULONG_PTR)quality_level_count;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_device_CheckMultisampleQualityLevels1(struct qemu_syscall *call)
{
    struct qemu_d3d11_device_CheckMultisampleQualityLevels1 *c = (struct qemu_d3d11_device_CheckMultisampleQualityLevels1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D11Device2_CheckMultisampleQualityLevels1(device->host_d3d11, c->format, c->sample_count, c->flags, QEMU_G2H(c->quality_level_count));
}

#endif

struct qemu_d3d_device_inner_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t out;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_device *impl_from_IUnknown(IUnknown *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_device, IUnknown_inner);
}

static HRESULT STDMETHODCALLTYPE d3d_device_inner_QueryInterface(IUnknown *iface, REFIID riid, void **out)
{
    struct qemu_d3d_device_inner_QueryInterface call;
    struct qemu_d3d11_device *device = impl_from_IUnknown(iface);

    WINE_TRACE("iface %p, riid %s, out %p.\n", iface, wine_dbgstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_ID3D11Device2)
            || IsEqualGUID(riid, &IID_ID3D11Device1)
            || IsEqualGUID(riid, &IID_ID3D11Device)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        *out = &device->ID3D11Device2_iface;
    }
    else if (IsEqualGUID(riid, &IID_ID3D10Device1)
            || IsEqualGUID(riid, &IID_ID3D10Device))
    {
        *out = &device->ID3D10Device1_iface;
    }
    else if (IsEqualGUID(riid, &IID_ID3D10Multithread))
    {
        *out = &device->ID3D10Multithread_iface;
    }
    else if (IsEqualGUID(riid, &IID_IQemuD3D11Device))
    {
        *out = &device->IQemuD3D11Device_iface;
    }
    else
    {
        /* Test if we're supposed to know about it. */
        call.super.id = QEMU_SYSCALL_ID(CALL_D3D_DEVICE_INNER_QUERYINTERFACE);
        call.iface = (ULONG_PTR)device;
        call.riid = (ULONG_PTR)riid;

        qemu_syscall(&call.super);

        *out = NULL;
        return E_NOINTERFACE;
    }

    IUnknown_AddRef((IUnknown *)*out);
    return S_OK;
}

#else

void qemu_d3d_device_inner_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d_device_inner_QueryInterface *c = (struct qemu_d3d_device_inner_QueryInterface *)call;
    struct qemu_d3d11_device *device;
    IUnknown *out;

    WINE_TRACE("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = IUnknown_QueryInterface(device->host_d3d11, QEMU_G2H(c->riid), (void **)&out);
    if (SUCCEEDED(c->super.iret))
    {
        WINE_FIXME("Host device returned IID %s which this wrapper does not know about.\n",
                wine_dbgstr_guid(QEMU_G2H(c->riid)));
        IUnknown_Release(out);
    }
}

#endif

struct qemu_d3d_device_inner_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d_device_inner_AddRef(IUnknown *iface)
{
    struct qemu_d3d_device_inner_AddRef call;
    struct qemu_d3d11_device *device = impl_from_IUnknown(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D_DEVICE_INNER_ADDREF);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d_device_inner_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d_device_inner_AddRef *c = (struct qemu_d3d_device_inner_AddRef *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Did not expect this to be called\n");
    c->super.iret = 2;
}

#endif

struct qemu_d3d_device_inner_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d_device_inner_Release(IUnknown *iface)
{
    struct qemu_d3d_device_inner_Release call;
    struct qemu_d3d11_device *device = impl_from_IUnknown(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D_DEVICE_INNER_RELEASE);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d_device_inner_Release(struct qemu_syscall *call)
{
    struct qemu_d3d_device_inner_Release *c = (struct qemu_d3d_device_inner_Release *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Did not expect this to be called\n");
    c->super.iret = 1;
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_QueryInterface(ID3D10Device1 *iface, REFIID riid,
        void **ppv)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, ppv);
}

static ULONG STDMETHODCALLTYPE d3d10_device_AddRef(ID3D10Device1 *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d10_device_Release(ID3D10Device1 *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_Release(device->outer_unk);
}

#endif

struct qemu_d3d10_device_VSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSSetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct qemu_d3d10_device_VSSetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSSetConstantBuffers *c = (struct qemu_d3d10_device_VSSetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSSetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_PSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSSetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct qemu_d3d10_device_PSSetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSSetShaderResources *c = (struct qemu_d3d10_device_PSSetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSSetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_PSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSSetShader(ID3D10Device1 *iface, ID3D10PixelShader *shader)
{
    struct qemu_d3d10_device_PSSetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSSETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSSetShader *c = (struct qemu_d3d10_device_PSSetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSSetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_PSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSSetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct qemu_d3d10_device_PSSetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSSETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSSetSamplers *c = (struct qemu_d3d10_device_PSSetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSSetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_VSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSSetShader(ID3D10Device1 *iface, ID3D10VertexShader *shader)
{
    struct qemu_d3d10_device_VSSetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSSETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSSetShader *c = (struct qemu_d3d10_device_VSSetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSSetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_DrawIndexed
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t index_count;
    uint64_t start_index_location;
    uint64_t base_vertex_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_DrawIndexed(ID3D10Device1 *iface, UINT index_count, UINT start_index_location, INT base_vertex_location)
{
    struct qemu_d3d10_device_DrawIndexed call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_DRAWINDEXED);
    call.iface = (ULONG_PTR)device;
    call.index_count = index_count;
    call.start_index_location = start_index_location;
    call.base_vertex_location = base_vertex_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_DrawIndexed(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_DrawIndexed *c = (struct qemu_d3d10_device_DrawIndexed *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_DrawIndexed(device->host_d3d10, c->index_count, c->start_index_location, c->base_vertex_location);
}

#endif

struct qemu_d3d10_device_Draw
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t vertex_count;
    uint64_t start_vertex_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_Draw(ID3D10Device1 *iface, UINT vertex_count, UINT start_vertex_location)
{
    struct qemu_d3d10_device_Draw call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_DRAW);
    call.iface = (ULONG_PTR)device;
    call.vertex_count = vertex_count;
    call.start_vertex_location = start_vertex_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_Draw(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_Draw *c = (struct qemu_d3d10_device_Draw *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_Draw(device->host_d3d10, c->vertex_count, c->start_vertex_location);
}

#endif

struct qemu_d3d10_device_PSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSSetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct qemu_d3d10_device_PSSetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSSetConstantBuffers *c = (struct qemu_d3d10_device_PSSetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSSetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_IASetInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IASetInputLayout(ID3D10Device1 *iface, ID3D10InputLayout *input_layout)
{
    struct qemu_d3d10_device_IASetInputLayout call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IASETINPUTLAYOUT);
    call.iface = (ULONG_PTR)device;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IASetInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IASetInputLayout *c = (struct qemu_d3d10_device_IASetInputLayout *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IASetInputLayout(device->host_d3d10, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d10_device_IASetVertexBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t strides;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IASetVertexBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers, const UINT *strides, const UINT *offsets)
{
    struct qemu_d3d10_device_IASetVertexBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IASETVERTEXBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.strides = (ULONG_PTR)strides;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IASetVertexBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IASetVertexBuffers *c = (struct qemu_d3d10_device_IASetVertexBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IASetVertexBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->strides), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d10_device_IASetIndexBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t format;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IASetIndexBuffer(ID3D10Device1 *iface, ID3D10Buffer *buffer, DXGI_FORMAT format, UINT offset)
{
    struct qemu_d3d10_device_IASetIndexBuffer call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IASETINDEXBUFFER);
    call.iface = (ULONG_PTR)device;
    call.buffer = (ULONG_PTR)buffer;
    call.format = format;
    call.offset = offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IASetIndexBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IASetIndexBuffer *c = (struct qemu_d3d10_device_IASetIndexBuffer *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IASetIndexBuffer(device->host_d3d10, QEMU_G2H(c->buffer), c->format, c->offset);
}

#endif

struct qemu_d3d10_device_DrawIndexedInstanced
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t instance_index_count;
    uint64_t instance_count;
    uint64_t start_index_location;
    uint64_t base_vertex_location;
    uint64_t start_instance_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_DrawIndexedInstanced(ID3D10Device1 *iface, UINT instance_index_count, UINT instance_count, UINT start_index_location, INT base_vertex_location, UINT start_instance_location)
{
    struct qemu_d3d10_device_DrawIndexedInstanced call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_DRAWINDEXEDINSTANCED);
    call.iface = (ULONG_PTR)device;
    call.instance_index_count = instance_index_count;
    call.instance_count = instance_count;
    call.start_index_location = start_index_location;
    call.base_vertex_location = base_vertex_location;
    call.start_instance_location = start_instance_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_DrawIndexedInstanced(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_DrawIndexedInstanced *c = (struct qemu_d3d10_device_DrawIndexedInstanced *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_DrawIndexedInstanced(device->host_d3d10, c->instance_index_count, c->instance_count, c->start_index_location, c->base_vertex_location, c->start_instance_location);
}

#endif

struct qemu_d3d10_device_DrawInstanced
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t instance_vertex_count;
    uint64_t instance_count;
    uint64_t start_vertex_location;
    uint64_t start_instance_location;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_DrawInstanced(ID3D10Device1 *iface, UINT instance_vertex_count, UINT instance_count, UINT start_vertex_location, UINT start_instance_location)
{
    struct qemu_d3d10_device_DrawInstanced call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_DRAWINSTANCED);
    call.iface = (ULONG_PTR)device;
    call.instance_vertex_count = instance_vertex_count;
    call.instance_count = instance_count;
    call.start_vertex_location = start_vertex_location;
    call.start_instance_location = start_instance_location;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_DrawInstanced(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_DrawInstanced *c = (struct qemu_d3d10_device_DrawInstanced *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_DrawInstanced(device->host_d3d10, c->instance_vertex_count, c->instance_count, c->start_vertex_location, c->start_instance_location);
}

#endif

struct qemu_d3d10_device_GSSetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSSetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct qemu_d3d10_device_GSSetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSSETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSSetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSSetConstantBuffers *c = (struct qemu_d3d10_device_GSSetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSSetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_GSSetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSSetShader(ID3D10Device1 *iface, ID3D10GeometryShader *shader)
{
    struct qemu_d3d10_device_GSSetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSSETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSSetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSSetShader *c = (struct qemu_d3d10_device_GSSetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSSetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_IASetPrimitiveTopology
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t topology;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IASetPrimitiveTopology(ID3D10Device1 *iface, D3D10_PRIMITIVE_TOPOLOGY topology)
{
    struct qemu_d3d10_device_IASetPrimitiveTopology call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IASETPRIMITIVETOPOLOGY);
    call.iface = (ULONG_PTR)device;
    call.topology = topology;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IASetPrimitiveTopology(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IASetPrimitiveTopology *c = (struct qemu_d3d10_device_IASetPrimitiveTopology *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IASetPrimitiveTopology(device->host_d3d10, c->topology);
}

#endif

struct qemu_d3d10_device_VSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSSetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct qemu_d3d10_device_VSSetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSSetShaderResources *c = (struct qemu_d3d10_device_VSSetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSSetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_VSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSSetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct qemu_d3d10_device_VSSetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSSETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSSetSamplers *c = (struct qemu_d3d10_device_VSSetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSSetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_SetPredication
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t predicate;
    uint64_t value;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_SetPredication(ID3D10Device1 *iface, ID3D10Predicate *predicate, BOOL value)
{
    struct qemu_d3d10_device_SetPredication call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SETPREDICATION);
    call.iface = (ULONG_PTR)device;
    call.predicate = (ULONG_PTR)predicate;
    call.value = value;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_SetPredication(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SetPredication *c = (struct qemu_d3d10_device_SetPredication *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_SetPredication(device->host_d3d10, QEMU_G2H(c->predicate), c->value);
}

#endif

struct qemu_d3d10_device_GSSetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSSetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct qemu_d3d10_device_GSSetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSSETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSSetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSSetShaderResources *c = (struct qemu_d3d10_device_GSSetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSSetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_GSSetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSSetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct qemu_d3d10_device_GSSetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSSETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSSetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSSetSamplers *c = (struct qemu_d3d10_device_GSSetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSSetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_OMSetRenderTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMSetRenderTargets(ID3D10Device1 *iface, UINT render_target_view_count, ID3D10RenderTargetView *const *render_target_views, ID3D10DepthStencilView *depth_stencil_view)
{
    struct qemu_d3d10_device_OMSetRenderTargets call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMSETRENDERTARGETS);
    call.iface = (ULONG_PTR)device;
    call.render_target_view_count = render_target_view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMSetRenderTargets(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMSetRenderTargets *c = (struct qemu_d3d10_device_OMSetRenderTargets *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMSetRenderTargets(device->host_d3d10, c->render_target_view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view));
}

#endif

struct qemu_d3d10_device_OMSetBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t blend_state;
    uint64_t blend_factor;
    uint64_t sample_mask;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMSetBlendState(ID3D10Device1 *iface, ID3D10BlendState *blend_state, const float blend_factor[4], UINT sample_mask)
{
    struct qemu_d3d10_device_OMSetBlendState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMSETBLENDSTATE);
    call.iface = (ULONG_PTR)device;
    call.blend_state = (ULONG_PTR)blend_state;
    call.blend_factor = (ULONG_PTR)blend_factor;
    call.sample_mask = sample_mask;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMSetBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMSetBlendState *c = (struct qemu_d3d10_device_OMSetBlendState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMSetBlendState(device->host_d3d10, QEMU_G2H(c->blend_state), QEMU_G2H(c->blend_factor), c->sample_mask);
}

#endif

struct qemu_d3d10_device_OMSetDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_state;
    uint64_t stencil_ref;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMSetDepthStencilState(ID3D10Device1 *iface, ID3D10DepthStencilState *depth_stencil_state, UINT stencil_ref)
{
    struct qemu_d3d10_device_OMSetDepthStencilState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMSETDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)device;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;
    call.stencil_ref = stencil_ref;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMSetDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMSetDepthStencilState *c = (struct qemu_d3d10_device_OMSetDepthStencilState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMSetDepthStencilState(device->host_d3d10, QEMU_G2H(c->depth_stencil_state), c->stencil_ref);
}

#endif

struct qemu_d3d10_device_SOSetTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t target_count;
    uint64_t targets;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_SOSetTargets(ID3D10Device1 *iface, UINT target_count, ID3D10Buffer *const *targets, const UINT *offsets)
{
    struct qemu_d3d10_device_SOSetTargets call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SOSETTARGETS);
    call.iface = (ULONG_PTR)device;
    call.target_count = target_count;
    call.targets = (ULONG_PTR)targets;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_SOSetTargets(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SOSetTargets *c = (struct qemu_d3d10_device_SOSetTargets *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_SOSetTargets(device->host_d3d10, c->target_count, QEMU_G2H(c->targets), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d10_device_DrawAuto
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_DrawAuto(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_DrawAuto call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_DRAWAUTO);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_DrawAuto(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_DrawAuto *c = (struct qemu_d3d10_device_DrawAuto *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_DrawAuto(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_RSSetState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSSetState(ID3D10Device1 *iface, ID3D10RasterizerState *rasterizer_state)
{
    struct qemu_d3d10_device_RSSetState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSSETSTATE);
    call.iface = (ULONG_PTR)device;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSSetState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSSetState *c = (struct qemu_d3d10_device_RSSetState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSSetState(device->host_d3d10, QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d10_device_RSSetViewports
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t viewport_count;
    uint64_t viewports;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSSetViewports(ID3D10Device1 *iface, UINT viewport_count, const D3D10_VIEWPORT *viewports)
{
    struct qemu_d3d10_device_RSSetViewports call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSSETVIEWPORTS);
    call.iface = (ULONG_PTR)device;
    call.viewport_count = viewport_count;
    call.viewports = (ULONG_PTR)viewports;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSSetViewports(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSSetViewports *c = (struct qemu_d3d10_device_RSSetViewports *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSSetViewports(device->host_d3d10, c->viewport_count, QEMU_G2H(c->viewports));
}

#endif

struct qemu_d3d10_device_RSSetScissorRects
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rect_count;
    uint64_t rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSSetScissorRects(ID3D10Device1 *iface, UINT rect_count, const D3D10_RECT *rects)
{
    struct qemu_d3d10_device_RSSetScissorRects call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSSETSCISSORRECTS);
    call.iface = (ULONG_PTR)device;
    call.rect_count = rect_count;
    call.rects = (ULONG_PTR)rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSSetScissorRects(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSSetScissorRects *c = (struct qemu_d3d10_device_RSSetScissorRects *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSSetScissorRects(device->host_d3d10, c->rect_count, QEMU_G2H(c->rects));
}

#endif

struct qemu_d3d10_device_CopySubresourceRegion
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t dst_subresource_idx;
    uint64_t dst_x;
    uint64_t dst_y;
    uint64_t dst_z;
    uint64_t src_resource;
    uint64_t src_subresource_idx;
    uint64_t src_box;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_CopySubresourceRegion(ID3D10Device1 *iface, ID3D10Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z, ID3D10Resource *src_resource, UINT src_subresource_idx, const D3D10_BOX *src_box)
{
    struct qemu_d3d10_device_CopySubresourceRegion call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_COPYSUBRESOURCEREGION);
    call.iface = (ULONG_PTR)device;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.dst_subresource_idx = dst_subresource_idx;
    call.dst_x = dst_x;
    call.dst_y = dst_y;
    call.dst_z = dst_z;
    call.src_resource = (ULONG_PTR)src_resource;
    call.src_subresource_idx = src_subresource_idx;
    call.src_box = (ULONG_PTR)src_box;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_CopySubresourceRegion(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CopySubresourceRegion *c = (struct qemu_d3d10_device_CopySubresourceRegion *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_CopySubresourceRegion(device->host_d3d10, QEMU_G2H(c->dst_resource), c->dst_subresource_idx, c->dst_x, c->dst_y, c->dst_z, QEMU_G2H(c->src_resource), c->src_subresource_idx, QEMU_G2H(c->src_box));
}

#endif

struct qemu_d3d10_device_CopyResource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t src_resource;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_CopyResource(ID3D10Device1 *iface, ID3D10Resource *dst_resource, ID3D10Resource *src_resource)
{
    struct qemu_d3d10_device_CopyResource call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_COPYRESOURCE);
    call.iface = (ULONG_PTR)device;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.src_resource = (ULONG_PTR)src_resource;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_CopyResource(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CopyResource *c = (struct qemu_d3d10_device_CopyResource *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_CopyResource(device->host_d3d10, QEMU_G2H(c->dst_resource), QEMU_G2H(c->src_resource));
}

#endif

struct qemu_d3d10_device_UpdateSubresource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t subresource_idx;
    uint64_t box;
    uint64_t data;
    uint64_t row_pitch;
    uint64_t depth_pitch;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_UpdateSubresource(ID3D10Device1 *iface, ID3D10Resource *resource, UINT subresource_idx, const D3D10_BOX *box, const void *data, UINT row_pitch, UINT depth_pitch)
{
    struct qemu_d3d10_device_UpdateSubresource call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_UPDATESUBRESOURCE);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.subresource_idx = subresource_idx;
    call.box = (ULONG_PTR)box;
    call.data = (ULONG_PTR)data;
    call.row_pitch = row_pitch;
    call.depth_pitch = depth_pitch;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_UpdateSubresource(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_UpdateSubresource *c = (struct qemu_d3d10_device_UpdateSubresource *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_UpdateSubresource(device->host_d3d10, QEMU_G2H(c->resource), c->subresource_idx, QEMU_G2H(c->box), QEMU_G2H(c->data), c->row_pitch, c->depth_pitch);
}

#endif

struct qemu_d3d10_device_ClearRenderTargetView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t render_target_view;
    uint64_t color_rgba;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_ClearRenderTargetView(ID3D10Device1 *iface, ID3D10RenderTargetView *render_target_view, const float color_rgba[4])
{
    struct qemu_d3d10_device_ClearRenderTargetView call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CLEARRENDERTARGETVIEW);
    call.iface = (ULONG_PTR)device;
    call.render_target_view = (ULONG_PTR)render_target_view;
    call.color_rgba = (ULONG_PTR)color_rgba;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_ClearRenderTargetView(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_ClearRenderTargetView *c = (struct qemu_d3d10_device_ClearRenderTargetView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_ClearRenderTargetView(device->host_d3d10, QEMU_G2H(c->render_target_view), QEMU_G2H(c->color_rgba));
}

#endif

struct qemu_d3d10_device_ClearDepthStencilView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_view;
    uint64_t flags;
    uint64_t depth;
    uint64_t stencil;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_ClearDepthStencilView(ID3D10Device1 *iface, ID3D10DepthStencilView *depth_stencil_view, UINT flags, FLOAT depth, UINT8 stencil)
{
    struct qemu_d3d10_device_ClearDepthStencilView call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CLEARDEPTHSTENCILVIEW);
    call.iface = (ULONG_PTR)device;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;
    call.flags = flags;
    call.depth = depth;
    call.stencil = stencil;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_ClearDepthStencilView(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_ClearDepthStencilView *c = (struct qemu_d3d10_device_ClearDepthStencilView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_ClearDepthStencilView(device->host_d3d10, QEMU_G2H(c->depth_stencil_view), c->flags, c->depth, c->stencil);
}

#endif

struct qemu_d3d10_device_GenerateMips
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GenerateMips(ID3D10Device1 *iface, ID3D10ShaderResourceView *view)
{
    struct qemu_d3d10_device_GenerateMips call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GENERATEMIPS);
    call.iface = (ULONG_PTR)device;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GenerateMips(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GenerateMips *c = (struct qemu_d3d10_device_GenerateMips *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GenerateMips(device->host_d3d10, QEMU_G2H(c->view));
}

#endif

struct qemu_d3d10_device_ResolveSubresource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t dst_resource;
    uint64_t dst_subresource_idx;
    uint64_t src_resource;
    uint64_t src_subresource_idx;
    uint64_t format;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_ResolveSubresource(ID3D10Device1 *iface, ID3D10Resource *dst_resource, UINT dst_subresource_idx, ID3D10Resource *src_resource, UINT src_subresource_idx, DXGI_FORMAT format)
{
    struct qemu_d3d10_device_ResolveSubresource call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RESOLVESUBRESOURCE);
    call.iface = (ULONG_PTR)device;
    call.dst_resource = (ULONG_PTR)dst_resource;
    call.dst_subresource_idx = dst_subresource_idx;
    call.src_resource = (ULONG_PTR)src_resource;
    call.src_subresource_idx = src_subresource_idx;
    call.format = format;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_ResolveSubresource(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_ResolveSubresource *c = (struct qemu_d3d10_device_ResolveSubresource *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_ResolveSubresource(device->host_d3d10, QEMU_G2H(c->dst_resource), c->dst_subresource_idx, QEMU_G2H(c->src_resource), c->src_subresource_idx, c->format);
}

#endif

struct qemu_d3d10_device_VSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSGetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct qemu_d3d10_device_VSGetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSGetConstantBuffers *c = (struct qemu_d3d10_device_VSGetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSGetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_PSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSGetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct qemu_d3d10_device_PSGetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSGetShaderResources *c = (struct qemu_d3d10_device_PSGetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSGetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_PSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSGetShader(ID3D10Device1 *iface, ID3D10PixelShader **shader)
{
    struct qemu_d3d10_device_PSGetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSGETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSGetShader *c = (struct qemu_d3d10_device_PSGetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSGetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_PSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSGetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct qemu_d3d10_device_PSGetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSGETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSGetSamplers *c = (struct qemu_d3d10_device_PSGetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSGetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_VSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSGetShader(ID3D10Device1 *iface, ID3D10VertexShader **shader)
{
    struct qemu_d3d10_device_VSGetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSGETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSGetShader *c = (struct qemu_d3d10_device_VSGetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSGetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_PSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_PSGetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct qemu_d3d10_device_PSGetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_PSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_PSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_PSGetConstantBuffers *c = (struct qemu_d3d10_device_PSGetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_PSGetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_IAGetInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IAGetInputLayout(ID3D10Device1 *iface, ID3D10InputLayout **input_layout)
{
    struct qemu_d3d10_device_IAGetInputLayout call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IAGETINPUTLAYOUT);
    call.iface = (ULONG_PTR)device;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IAGetInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IAGetInputLayout *c = (struct qemu_d3d10_device_IAGetInputLayout *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IAGetInputLayout(device->host_d3d10, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d10_device_IAGetVertexBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t strides;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IAGetVertexBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers, UINT *strides, UINT *offsets)
{
    struct qemu_d3d10_device_IAGetVertexBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IAGETVERTEXBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.strides = (ULONG_PTR)strides;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IAGetVertexBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IAGetVertexBuffers *c = (struct qemu_d3d10_device_IAGetVertexBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IAGetVertexBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->strides), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d10_device_IAGetIndexBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer;
    uint64_t format;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IAGetIndexBuffer(ID3D10Device1 *iface, ID3D10Buffer **buffer, DXGI_FORMAT *format, UINT *offset)
{
    struct qemu_d3d10_device_IAGetIndexBuffer call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IAGETINDEXBUFFER);
    call.iface = (ULONG_PTR)device;
    call.buffer = (ULONG_PTR)buffer;
    call.format = (ULONG_PTR)format;
    call.offset = (ULONG_PTR)offset;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IAGetIndexBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IAGetIndexBuffer *c = (struct qemu_d3d10_device_IAGetIndexBuffer *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IAGetIndexBuffer(device->host_d3d10, QEMU_G2H(c->buffer), QEMU_G2H(c->format), QEMU_G2H(c->offset));
}

#endif

struct qemu_d3d10_device_GSGetConstantBuffers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t buffer_count;
    uint64_t buffers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSGetConstantBuffers(ID3D10Device1 *iface, UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct qemu_d3d10_device_GSGetConstantBuffers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSGETCONSTANTBUFFERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSGetConstantBuffers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSGetConstantBuffers *c = (struct qemu_d3d10_device_GSGetConstantBuffers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSGetConstantBuffers(device->host_d3d10, c->start_slot, c->buffer_count, QEMU_G2H(c->buffers));
}

#endif

struct qemu_d3d10_device_GSGetShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSGetShader(ID3D10Device1 *iface, ID3D10GeometryShader **shader)
{
    struct qemu_d3d10_device_GSGetShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSGETSHADER);
    call.iface = (ULONG_PTR)device;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSGetShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSGetShader *c = (struct qemu_d3d10_device_GSGetShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSGetShader(device->host_d3d10, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_IAGetPrimitiveTopology
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t topology;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_IAGetPrimitiveTopology(ID3D10Device1 *iface, D3D10_PRIMITIVE_TOPOLOGY *topology)
{
    struct qemu_d3d10_device_IAGetPrimitiveTopology call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_IAGETPRIMITIVETOPOLOGY);
    call.iface = (ULONG_PTR)device;
    call.topology = (ULONG_PTR)topology;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_IAGetPrimitiveTopology(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_IAGetPrimitiveTopology *c = (struct qemu_d3d10_device_IAGetPrimitiveTopology *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_IAGetPrimitiveTopology(device->host_d3d10, QEMU_G2H(c->topology));
}

#endif

struct qemu_d3d10_device_VSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSGetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct qemu_d3d10_device_VSGetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSGetShaderResources *c = (struct qemu_d3d10_device_VSGetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSGetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_VSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_VSGetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct qemu_d3d10_device_VSGetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_VSGETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_VSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_VSGetSamplers *c = (struct qemu_d3d10_device_VSGetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_VSGetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_GetPredication
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t predicate;
    uint64_t value;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GetPredication(ID3D10Device1 *iface, ID3D10Predicate **predicate, BOOL *value)
{
    struct qemu_d3d10_device_GetPredication call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETPREDICATION);
    call.iface = (ULONG_PTR)device;
    call.predicate = (ULONG_PTR)predicate;
    call.value = (ULONG_PTR)value;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GetPredication(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetPredication *c = (struct qemu_d3d10_device_GetPredication *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GetPredication(device->host_d3d10, QEMU_G2H(c->predicate), QEMU_G2H(c->value));
}

#endif

struct qemu_d3d10_device_GSGetShaderResources
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t view_count;
    uint64_t views;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSGetShaderResources(ID3D10Device1 *iface, UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct qemu_d3d10_device_GSGetShaderResources call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSGETSHADERRESOURCES);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.view_count = view_count;
    call.views = (ULONG_PTR)views;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSGetShaderResources(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSGetShaderResources *c = (struct qemu_d3d10_device_GSGetShaderResources *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSGetShaderResources(device->host_d3d10, c->start_slot, c->view_count, QEMU_G2H(c->views));
}

#endif

struct qemu_d3d10_device_GSGetSamplers
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t start_slot;
    uint64_t sampler_count;
    uint64_t samplers;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GSGetSamplers(ID3D10Device1 *iface, UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct qemu_d3d10_device_GSGetSamplers call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GSGETSAMPLERS);
    call.iface = (ULONG_PTR)device;
    call.start_slot = start_slot;
    call.sampler_count = sampler_count;
    call.samplers = (ULONG_PTR)samplers;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GSGetSamplers(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GSGetSamplers *c = (struct qemu_d3d10_device_GSGetSamplers *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GSGetSamplers(device->host_d3d10, c->start_slot, c->sampler_count, QEMU_G2H(c->samplers));
}

#endif

struct qemu_d3d10_device_OMGetRenderTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t view_count;
    uint64_t render_target_views;
    uint64_t depth_stencil_view;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMGetRenderTargets(ID3D10Device1 *iface, UINT view_count, ID3D10RenderTargetView **render_target_views, ID3D10DepthStencilView **depth_stencil_view)
{
    struct qemu_d3d10_device_OMGetRenderTargets call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMGETRENDERTARGETS);
    call.iface = (ULONG_PTR)device;
    call.view_count = view_count;
    call.render_target_views = (ULONG_PTR)render_target_views;
    call.depth_stencil_view = (ULONG_PTR)depth_stencil_view;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMGetRenderTargets(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMGetRenderTargets *c = (struct qemu_d3d10_device_OMGetRenderTargets *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMGetRenderTargets(device->host_d3d10, c->view_count, QEMU_G2H(c->render_target_views), QEMU_G2H(c->depth_stencil_view));
}

#endif

struct qemu_d3d10_device_OMGetBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t blend_state;
    uint64_t blend_factor;
    uint64_t sample_mask;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMGetBlendState(ID3D10Device1 *iface, ID3D10BlendState **blend_state, FLOAT blend_factor[4], UINT *sample_mask)
{
    struct qemu_d3d10_device_OMGetBlendState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMGETBLENDSTATE);
    call.iface = (ULONG_PTR)device;
    call.blend_state = (ULONG_PTR)blend_state;
    call.blend_factor = (ULONG_PTR)blend_factor;
    call.sample_mask = (ULONG_PTR)sample_mask;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMGetBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMGetBlendState *c = (struct qemu_d3d10_device_OMGetBlendState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMGetBlendState(device->host_d3d10, QEMU_G2H(c->blend_state), QEMU_G2H(c->blend_factor), QEMU_G2H(c->sample_mask));
}

#endif

struct qemu_d3d10_device_OMGetDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t depth_stencil_state;
    uint64_t stencil_ref;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_OMGetDepthStencilState(ID3D10Device1 *iface, ID3D10DepthStencilState **depth_stencil_state, UINT *stencil_ref)
{
    struct qemu_d3d10_device_OMGetDepthStencilState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OMGETDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)device;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;
    call.stencil_ref = (ULONG_PTR)stencil_ref;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_OMGetDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OMGetDepthStencilState *c = (struct qemu_d3d10_device_OMGetDepthStencilState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_OMGetDepthStencilState(device->host_d3d10, QEMU_G2H(c->depth_stencil_state), QEMU_G2H(c->stencil_ref));
}

#endif

struct qemu_d3d10_device_SOGetTargets
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t buffer_count;
    uint64_t buffers;
    uint64_t offsets;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_SOGetTargets(ID3D10Device1 *iface, UINT buffer_count, ID3D10Buffer **buffers, UINT *offsets)
{
    struct qemu_d3d10_device_SOGetTargets call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SOGETTARGETS);
    call.iface = (ULONG_PTR)device;
    call.buffer_count = buffer_count;
    call.buffers = (ULONG_PTR)buffers;
    call.offsets = (ULONG_PTR)offsets;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_SOGetTargets(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SOGetTargets *c = (struct qemu_d3d10_device_SOGetTargets *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_SOGetTargets(device->host_d3d10, c->buffer_count, QEMU_G2H(c->buffers), QEMU_G2H(c->offsets));
}

#endif

struct qemu_d3d10_device_RSGetState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSGetState(ID3D10Device1 *iface, ID3D10RasterizerState **rasterizer_state)
{
    struct qemu_d3d10_device_RSGetState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSGETSTATE);
    call.iface = (ULONG_PTR)device;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSGetState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSGetState *c = (struct qemu_d3d10_device_RSGetState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSGetState(device->host_d3d10, QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d10_device_RSGetViewports
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t viewport_count;
    uint64_t viewports;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSGetViewports(ID3D10Device1 *iface, UINT *viewport_count, D3D10_VIEWPORT *viewports)
{
    struct qemu_d3d10_device_RSGetViewports call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSGETVIEWPORTS);
    call.iface = (ULONG_PTR)device;
    call.viewport_count = (ULONG_PTR)viewport_count;
    call.viewports = (ULONG_PTR)viewports;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSGetViewports(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSGetViewports *c = (struct qemu_d3d10_device_RSGetViewports *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSGetViewports(device->host_d3d10, QEMU_G2H(c->viewport_count), QEMU_G2H(c->viewports));
}

#endif

struct qemu_d3d10_device_RSGetScissorRects
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t rect_count;
    uint64_t rects;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_RSGetScissorRects(ID3D10Device1 *iface, UINT *rect_count, D3D10_RECT *rects)
{
    struct qemu_d3d10_device_RSGetScissorRects call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_RSGETSCISSORRECTS);
    call.iface = (ULONG_PTR)device;
    call.rect_count = (ULONG_PTR)rect_count;
    call.rects = (ULONG_PTR)rects;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_RSGetScissorRects(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_RSGetScissorRects *c = (struct qemu_d3d10_device_RSGetScissorRects *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_RSGetScissorRects(device->host_d3d10, QEMU_G2H(c->rect_count), QEMU_G2H(c->rects));
}

#endif

struct qemu_d3d10_device_GetDeviceRemovedReason
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_GetDeviceRemovedReason(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_GetDeviceRemovedReason call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETDEVICEREMOVEDREASON);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_GetDeviceRemovedReason(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetDeviceRemovedReason *c = (struct qemu_d3d10_device_GetDeviceRemovedReason *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_GetDeviceRemovedReason(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_SetExceptionMode
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t flags;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_SetExceptionMode(ID3D10Device1 *iface, UINT flags)
{
    struct qemu_d3d10_device_SetExceptionMode call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SETEXCEPTIONMODE);
    call.iface = (ULONG_PTR)device;
    call.flags = flags;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_SetExceptionMode(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SetExceptionMode *c = (struct qemu_d3d10_device_SetExceptionMode *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_SetExceptionMode(device->host_d3d10, c->flags);
}

#endif

struct qemu_d3d10_device_GetExceptionMode
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static UINT STDMETHODCALLTYPE d3d10_device_GetExceptionMode(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_GetExceptionMode call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETEXCEPTIONMODE);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_GetExceptionMode(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetExceptionMode *c = (struct qemu_d3d10_device_GetExceptionMode *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_GetExceptionMode(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_GetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_GetPrivateData(ID3D10Device1 *iface, REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d10_device_GetPrivateData call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETPRIVATEDATA);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data_size = (ULONG_PTR)data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_GetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetPrivateData *c = (struct qemu_d3d10_device_GetPrivateData *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_GetPrivateData(device->host_d3d10, QEMU_G2H(c->guid), QEMU_G2H(c->data_size), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d10_device_SetPrivateData
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data_size;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_SetPrivateData(ID3D10Device1 *iface, REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d10_device_SetPrivateData call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SETPRIVATEDATA);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data_size = data_size;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_SetPrivateData(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SetPrivateData *c = (struct qemu_d3d10_device_SetPrivateData *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_SetPrivateData(device->host_d3d10, QEMU_G2H(c->guid), c->data_size, QEMU_G2H(c->data));
}

#endif

struct qemu_d3d10_device_SetPrivateDataInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t guid;
    uint64_t data;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_SetPrivateDataInterface(ID3D10Device1 *iface, REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d10_device_SetPrivateDataInterface call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SETPRIVATEDATAINTERFACE);
    call.iface = (ULONG_PTR)device;
    call.guid = (ULONG_PTR)guid;
    call.data = (ULONG_PTR)data;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_SetPrivateDataInterface(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SetPrivateDataInterface *c = (struct qemu_d3d10_device_SetPrivateDataInterface *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_SetPrivateDataInterface(device->host_d3d10, QEMU_G2H(c->guid), QEMU_G2H(c->data));
}

#endif

struct qemu_d3d10_device_ClearState
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_ClearState(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_ClearState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CLEARSTATE);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_ClearState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_ClearState *c = (struct qemu_d3d10_device_ClearState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_ClearState(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_Flush
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_Flush(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_Flush call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_FLUSH);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_Flush(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_Flush *c = (struct qemu_d3d10_device_Flush *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_Flush(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_CreateBuffer
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t data;
    uint64_t buffer;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBuffer(ID3D10Device1 *iface, const D3D10_BUFFER_DESC *desc, const D3D10_SUBRESOURCE_DATA *data, ID3D10Buffer **buffer)
{
    struct qemu_d3d10_device_CreateBuffer call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEBUFFER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.data = (ULONG_PTR)data;
    call.buffer = (ULONG_PTR)buffer;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateBuffer(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateBuffer *c = (struct qemu_d3d10_device_CreateBuffer *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateBuffer(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->data), QEMU_G2H(c->buffer));
}

#endif

struct qemu_d3d10_device_CreateShaderResourceView1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateShaderResourceView1(ID3D10Device1 *iface, ID3D10Resource *resource, const D3D10_SHADER_RESOURCE_VIEW_DESC1 *desc, ID3D10ShaderResourceView1 **view)
{
    struct qemu_d3d10_device_CreateShaderResourceView1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATESHADERRESOURCEVIEW1);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateShaderResourceView1(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateShaderResourceView1 *c = (struct qemu_d3d10_device_CreateShaderResourceView1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateShaderResourceView1(device->host_d3d10, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d10_device_CreateShaderResourceView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateShaderResourceView(ID3D10Device1 *iface, ID3D10Resource *resource, const D3D10_SHADER_RESOURCE_VIEW_DESC *desc, ID3D10ShaderResourceView **view)
{
    struct qemu_d3d10_device_CreateShaderResourceView call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATESHADERRESOURCEVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateShaderResourceView(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateShaderResourceView *c = (struct qemu_d3d10_device_CreateShaderResourceView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateShaderResourceView(device->host_d3d10, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d10_device_CreateRenderTargetView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateRenderTargetView(ID3D10Device1 *iface, ID3D10Resource *resource, const D3D10_RENDER_TARGET_VIEW_DESC *desc, ID3D10RenderTargetView **view)
{
    struct qemu_d3d10_device_CreateRenderTargetView call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATERENDERTARGETVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateRenderTargetView(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateRenderTargetView *c = (struct qemu_d3d10_device_CreateRenderTargetView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateRenderTargetView(device->host_d3d10, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d10_device_CreateDepthStencilView
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource;
    uint64_t desc;
    uint64_t view;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateDepthStencilView(ID3D10Device1 *iface, ID3D10Resource *resource, const D3D10_DEPTH_STENCIL_VIEW_DESC *desc, ID3D10DepthStencilView **view)
{
    struct qemu_d3d10_device_CreateDepthStencilView call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEDEPTHSTENCILVIEW);
    call.iface = (ULONG_PTR)device;
    call.resource = (ULONG_PTR)resource;
    call.desc = (ULONG_PTR)desc;
    call.view = (ULONG_PTR)view;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateDepthStencilView(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateDepthStencilView *c = (struct qemu_d3d10_device_CreateDepthStencilView *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateDepthStencilView(device->host_d3d10, QEMU_G2H(c->resource), QEMU_G2H(c->desc), QEMU_G2H(c->view));
}

#endif

struct qemu_d3d10_device_CreateInputLayout
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t element_descs;
    uint64_t element_count;
    uint64_t shader_byte_code;
    uint64_t shader_byte_code_length;
    uint64_t input_layout;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateInputLayout(ID3D10Device1 *iface, const D3D10_INPUT_ELEMENT_DESC *element_descs, UINT element_count, const void *shader_byte_code, SIZE_T shader_byte_code_length, ID3D10InputLayout **input_layout)
{
    struct qemu_d3d10_device_CreateInputLayout call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEINPUTLAYOUT);
    call.iface = (ULONG_PTR)device;
    call.element_descs = (ULONG_PTR)element_descs;
    call.element_count = element_count;
    call.shader_byte_code = (ULONG_PTR)shader_byte_code;
    call.shader_byte_code_length = shader_byte_code_length;
    call.input_layout = (ULONG_PTR)input_layout;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateInputLayout(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateInputLayout *c = (struct qemu_d3d10_device_CreateInputLayout *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateInputLayout(device->host_d3d10, QEMU_G2H(c->element_descs), c->element_count, QEMU_G2H(c->shader_byte_code), c->shader_byte_code_length, QEMU_G2H(c->input_layout));
}

#endif

struct qemu_d3d10_device_CreateVertexShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateVertexShader(ID3D10Device1 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D10VertexShader **shader)
{
    struct qemu_d3d10_device_CreateVertexShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEVERTEXSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateVertexShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateVertexShader *c = (struct qemu_d3d10_device_CreateVertexShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateVertexShader(device->host_d3d10, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_CreateGeometryShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateGeometryShader(ID3D10Device1 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D10GeometryShader **shader)
{
    struct qemu_d3d10_device_CreateGeometryShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEGEOMETRYSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateGeometryShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateGeometryShader *c = (struct qemu_d3d10_device_CreateGeometryShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateGeometryShader(device->host_d3d10, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_CreateGeometryShaderWithStreamOutput
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t output_stream_decls;
    uint64_t output_stream_decl_count;
    uint64_t output_stream_stride;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateGeometryShaderWithStreamOutput(ID3D10Device1 *iface, const void *byte_code, SIZE_T byte_code_length, const D3D10_SO_DECLARATION_ENTRY *output_stream_decls, UINT output_stream_decl_count, UINT output_stream_stride, ID3D10GeometryShader **shader)
{
    struct qemu_d3d10_device_CreateGeometryShaderWithStreamOutput call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.output_stream_decls = (ULONG_PTR)output_stream_decls;
    call.output_stream_decl_count = output_stream_decl_count;
    call.output_stream_stride = output_stream_stride;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateGeometryShaderWithStreamOutput(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateGeometryShaderWithStreamOutput *c = (struct qemu_d3d10_device_CreateGeometryShaderWithStreamOutput *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateGeometryShaderWithStreamOutput(device->host_d3d10, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->output_stream_decls), c->output_stream_decl_count, c->output_stream_stride, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_CreatePixelShader
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t byte_code;
    uint64_t byte_code_length;
    uint64_t shader;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreatePixelShader(ID3D10Device1 *iface, const void *byte_code, SIZE_T byte_code_length, ID3D10PixelShader **shader)
{
    struct qemu_d3d10_device_CreatePixelShader call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEPIXELSHADER);
    call.iface = (ULONG_PTR)device;
    call.byte_code = (ULONG_PTR)byte_code;
    call.byte_code_length = byte_code_length;
    call.shader = (ULONG_PTR)shader;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreatePixelShader(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreatePixelShader *c = (struct qemu_d3d10_device_CreatePixelShader *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreatePixelShader(device->host_d3d10, QEMU_G2H(c->byte_code), c->byte_code_length, QEMU_G2H(c->shader));
}

#endif

struct qemu_d3d10_device_CreateBlendState1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t blend_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBlendState1(ID3D10Device1 *iface, const D3D10_BLEND_DESC1 *desc, ID3D10BlendState1 **blend_state)
{
    struct qemu_d3d10_device_CreateBlendState1 call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEBLENDSTATE1);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.blend_state = (ULONG_PTR)blend_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateBlendState1(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateBlendState1 *c = (struct qemu_d3d10_device_CreateBlendState1 *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateBlendState1(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->blend_state));
}

#endif

struct qemu_d3d10_device_CreateBlendState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t blend_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBlendState(ID3D10Device1 *iface, const D3D10_BLEND_DESC *desc, ID3D10BlendState **blend_state)
{
    struct qemu_d3d10_device_CreateBlendState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEBLENDSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.blend_state = (ULONG_PTR)blend_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateBlendState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateBlendState *c = (struct qemu_d3d10_device_CreateBlendState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateBlendState(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->blend_state));
}

#endif

struct qemu_d3d10_device_CreateDepthStencilState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t depth_stencil_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateDepthStencilState(ID3D10Device1 *iface, const D3D10_DEPTH_STENCIL_DESC *desc, ID3D10DepthStencilState **depth_stencil_state)
{
    struct qemu_d3d10_device_CreateDepthStencilState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEDEPTHSTENCILSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.depth_stencil_state = (ULONG_PTR)depth_stencil_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateDepthStencilState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateDepthStencilState *c = (struct qemu_d3d10_device_CreateDepthStencilState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateDepthStencilState(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->depth_stencil_state));
}

#endif

struct qemu_d3d10_device_CreateRasterizerState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t rasterizer_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateRasterizerState(ID3D10Device1 *iface, const D3D10_RASTERIZER_DESC *desc, ID3D10RasterizerState **rasterizer_state)
{
    struct qemu_d3d10_device_CreateRasterizerState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATERASTERIZERSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.rasterizer_state = (ULONG_PTR)rasterizer_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateRasterizerState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateRasterizerState *c = (struct qemu_d3d10_device_CreateRasterizerState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateRasterizerState(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->rasterizer_state));
}

#endif

struct qemu_d3d10_device_CreateSamplerState
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t sampler_state;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateSamplerState(ID3D10Device1 *iface, const D3D10_SAMPLER_DESC *desc, ID3D10SamplerState **sampler_state)
{
    struct qemu_d3d10_device_CreateSamplerState call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATESAMPLERSTATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.sampler_state = (ULONG_PTR)sampler_state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateSamplerState(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateSamplerState *c = (struct qemu_d3d10_device_CreateSamplerState *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateSamplerState(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->sampler_state));
}

#endif

struct qemu_d3d10_device_CreateQuery
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t query;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateQuery(ID3D10Device1 *iface, const D3D10_QUERY_DESC *desc, ID3D10Query **query)
{
    struct qemu_d3d10_device_CreateQuery call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEQUERY);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.query = (ULONG_PTR)query;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateQuery(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateQuery *c = (struct qemu_d3d10_device_CreateQuery *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateQuery(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->query));
}

#endif

struct qemu_d3d10_device_CreatePredicate
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t predicate;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreatePredicate(ID3D10Device1 *iface, const D3D10_QUERY_DESC *desc, ID3D10Predicate **predicate)
{
    struct qemu_d3d10_device_CreatePredicate call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATEPREDICATE);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.predicate = (ULONG_PTR)predicate;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreatePredicate(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreatePredicate *c = (struct qemu_d3d10_device_CreatePredicate *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreatePredicate(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->predicate));
}

#endif

struct qemu_d3d10_device_CreateCounter
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t counter;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateCounter(ID3D10Device1 *iface, const D3D10_COUNTER_DESC *desc, ID3D10Counter **counter)
{
    struct qemu_d3d10_device_CreateCounter call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CREATECOUNTER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.counter = (ULONG_PTR)counter;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CreateCounter(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CreateCounter *c = (struct qemu_d3d10_device_CreateCounter *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CreateCounter(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->counter));
}

#endif

struct qemu_d3d10_device_CheckFormatSupport
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t format;
    uint64_t format_support;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckFormatSupport(ID3D10Device1 *iface, DXGI_FORMAT format, UINT *format_support)
{
    struct qemu_d3d10_device_CheckFormatSupport call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CHECKFORMATSUPPORT);
    call.iface = (ULONG_PTR)device;
    call.format = format;
    call.format_support = (ULONG_PTR)format_support;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CheckFormatSupport(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CheckFormatSupport *c = (struct qemu_d3d10_device_CheckFormatSupport *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CheckFormatSupport(device->host_d3d10, c->format, QEMU_G2H(c->format_support));
}

#endif

struct qemu_d3d10_device_CheckMultisampleQualityLevels
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t format;
    uint64_t sample_count;
    uint64_t quality_level_count;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckMultisampleQualityLevels(ID3D10Device1 *iface, DXGI_FORMAT format, UINT sample_count, UINT *quality_level_count)
{
    struct qemu_d3d10_device_CheckMultisampleQualityLevels call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CHECKMULTISAMPLEQUALITYLEVELS);
    call.iface = (ULONG_PTR)device;
    call.format = format;
    call.sample_count = sample_count;
    call.quality_level_count = (ULONG_PTR)quality_level_count;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CheckMultisampleQualityLevels(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CheckMultisampleQualityLevels *c = (struct qemu_d3d10_device_CheckMultisampleQualityLevels *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CheckMultisampleQualityLevels(device->host_d3d10, c->format, c->sample_count, QEMU_G2H(c->quality_level_count));
}

#endif

struct qemu_d3d10_device_CheckCounterInfo
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t counter_info;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_CheckCounterInfo(ID3D10Device1 *iface, D3D10_COUNTER_INFO *counter_info)
{
    struct qemu_d3d10_device_CheckCounterInfo call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CHECKCOUNTERINFO);
    call.iface = (ULONG_PTR)device;
    call.counter_info = (ULONG_PTR)counter_info;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_CheckCounterInfo(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CheckCounterInfo *c = (struct qemu_d3d10_device_CheckCounterInfo *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_CheckCounterInfo(device->host_d3d10, QEMU_G2H(c->counter_info));
}

#endif

struct qemu_d3d10_device_CheckCounter
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
    uint64_t type;
    uint64_t active_counters;
    uint64_t name;
    uint64_t name_length;
    uint64_t units;
    uint64_t units_length;
    uint64_t description;
    uint64_t description_length;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckCounter(ID3D10Device1 *iface, const D3D10_COUNTER_DESC *desc, D3D10_COUNTER_TYPE *type, UINT *active_counters, char *name, UINT *name_length, char *units, UINT *units_length, char *description, UINT *description_length)
{
    struct qemu_d3d10_device_CheckCounter call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_CHECKCOUNTER);
    call.iface = (ULONG_PTR)device;
    call.desc = (ULONG_PTR)desc;
    call.type = (ULONG_PTR)type;
    call.active_counters = (ULONG_PTR)active_counters;
    call.name = (ULONG_PTR)name;
    call.name_length = (ULONG_PTR)name_length;
    call.units = (ULONG_PTR)units;
    call.units_length = (ULONG_PTR)units_length;
    call.description = (ULONG_PTR)description;
    call.description_length = (ULONG_PTR)description_length;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_CheckCounter(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_CheckCounter *c = (struct qemu_d3d10_device_CheckCounter *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_CheckCounter(device->host_d3d10, QEMU_G2H(c->desc), QEMU_G2H(c->type), QEMU_G2H(c->active_counters), QEMU_G2H(c->name), QEMU_G2H(c->name_length), QEMU_G2H(c->units), QEMU_G2H(c->units_length), QEMU_G2H(c->description), QEMU_G2H(c->description_length));
}

#endif

struct qemu_d3d10_device_GetCreationFlags
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static UINT STDMETHODCALLTYPE d3d10_device_GetCreationFlags(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_GetCreationFlags call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETCREATIONFLAGS);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_GetCreationFlags(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetCreationFlags *c = (struct qemu_d3d10_device_GetCreationFlags *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_GetCreationFlags(device->host_d3d10);
}

#endif

struct qemu_d3d10_device_OpenSharedResource
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t resource_handle;
    uint64_t guid;
    uint64_t resource;
};

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_device_OpenSharedResource(ID3D10Device1 *iface, HANDLE resource_handle, REFIID guid, void **resource)
{
    struct qemu_d3d10_device_OpenSharedResource call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_OPENSHAREDRESOURCE);
    call.iface = (ULONG_PTR)device;
    call.resource_handle = (ULONG_PTR)resource_handle;
    call.guid = (ULONG_PTR)guid;
    call.resource = (ULONG_PTR)resource;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_OpenSharedResource(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_OpenSharedResource *c = (struct qemu_d3d10_device_OpenSharedResource *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_OpenSharedResource(device->host_d3d10, QEMU_G2H(c->resource_handle), QEMU_G2H(c->guid), QEMU_G2H(c->resource));
}

#endif

struct qemu_d3d10_device_SetTextFilterSize
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t width;
    uint64_t height;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_SetTextFilterSize(ID3D10Device1 *iface, UINT width, UINT height)
{
    struct qemu_d3d10_device_SetTextFilterSize call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_SETTEXTFILTERSIZE);
    call.iface = (ULONG_PTR)device;
    call.width = width;
    call.height = height;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_SetTextFilterSize(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_SetTextFilterSize *c = (struct qemu_d3d10_device_SetTextFilterSize *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_SetTextFilterSize(device->host_d3d10, c->width, c->height);
}

#endif

struct qemu_d3d10_device_GetTextFilterSize
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t width;
    uint64_t height;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_device_GetTextFilterSize(ID3D10Device1 *iface, UINT *width, UINT *height)
{
    struct qemu_d3d10_device_GetTextFilterSize call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETTEXTFILTERSIZE);
    call.iface = (ULONG_PTR)device;
    call.width = (ULONG_PTR)width;
    call.height = (ULONG_PTR)height;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_device_GetTextFilterSize(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetTextFilterSize *c = (struct qemu_d3d10_device_GetTextFilterSize *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Device1_GetTextFilterSize(device->host_d3d10, QEMU_G2H(c->width), QEMU_G2H(c->height));
}

#endif

struct qemu_d3d10_device_GetFeatureLevel
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static D3D10_FEATURE_LEVEL1 STDMETHODCALLTYPE d3d10_device_GetFeatureLevel(ID3D10Device1 *iface)
{
    struct qemu_d3d10_device_GetFeatureLevel call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Device(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEVICE_GETFEATURELEVEL);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_device_GetFeatureLevel(struct qemu_syscall *call)
{
    struct qemu_d3d10_device_GetFeatureLevel *c = (struct qemu_d3d10_device_GetFeatureLevel *)call;
    struct qemu_d3d11_device *device;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Device1_GetFeatureLevel(device->host_d3d10);
}

#endif

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_device *impl_from_ID3D10Multithread(ID3D10Multithread *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_device, ID3D10Multithread_iface);
}

static HRESULT STDMETHODCALLTYPE d3d10_multithread_QueryInterface(ID3D10Multithread *iface, REFIID iid, void **out)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    WINE_TRACE("iface %p, iid %s, out %p.\n", iface, wine_dbgstr_guid(iid), out);

    return IUnknown_QueryInterface(device->outer_unk, iid, out);
}

static ULONG STDMETHODCALLTYPE d3d10_multithread_AddRef(ID3D10Multithread *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    WINE_TRACE("iface %p.\n", iface);

    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d10_multithread_Release(ID3D10Multithread *iface)
{
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    WINE_TRACE("iface %p.\n", iface);

    return IUnknown_Release(device->outer_unk);
}

#endif

struct qemu_d3d10_multithread_Enter
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_multithread_Enter(ID3D10Multithread *iface)
{
    struct qemu_d3d10_multithread_Enter call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_MULTITHREAD_ENTER);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_multithread_Enter(struct qemu_syscall *call)
{
    struct qemu_d3d10_multithread_Enter *c = (struct qemu_d3d10_multithread_Enter *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Multithread_Enter(device->host_mt);
}

#endif

struct qemu_d3d10_multithread_Leave
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_multithread_Leave(ID3D10Multithread *iface)
{
    struct qemu_d3d10_multithread_Leave call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_MULTITHREAD_LEAVE);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_multithread_Leave(struct qemu_syscall *call)
{
    struct qemu_d3d10_multithread_Leave *c = (struct qemu_d3d10_multithread_Leave *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    ID3D10Multithread_Leave(device->host_mt);
}

#endif

struct qemu_d3d10_multithread_SetMultithreadProtected
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t protect;
};

#ifdef QEMU_DLL_GUEST

static BOOL STDMETHODCALLTYPE d3d10_multithread_SetMultithreadProtected(ID3D10Multithread *iface, BOOL protect)
{
    struct qemu_d3d10_multithread_SetMultithreadProtected call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_MULTITHREAD_SETMULTITHREADPROTECTED);
    call.iface = (ULONG_PTR)device;
    call.protect = protect;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_multithread_SetMultithreadProtected(struct qemu_syscall *call)
{
    struct qemu_d3d10_multithread_SetMultithreadProtected *c = (struct qemu_d3d10_multithread_SetMultithreadProtected *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Multithread_SetMultithreadProtected(device->host_mt, c->protect);
}

#endif

struct qemu_d3d10_multithread_GetMultithreadProtected
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static BOOL STDMETHODCALLTYPE d3d10_multithread_GetMultithreadProtected(ID3D10Multithread *iface)
{
    struct qemu_d3d10_multithread_GetMultithreadProtected call;
    struct qemu_d3d11_device *device = impl_from_ID3D10Multithread(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_MULTITHREAD_GETMULTITHREADPROTECTED);
    call.iface = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_multithread_GetMultithreadProtected(struct qemu_syscall *call)
{
    struct qemu_d3d10_multithread_GetMultithreadProtected *c = (struct qemu_d3d10_multithread_GetMultithreadProtected *)call;
    struct qemu_d3d11_device *device;

    WINE_FIXME("Unverified!\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = ID3D10Multithread_GetMultithreadProtected(device->host_mt);
}

#endif

struct qemu_wrap_implicit_surface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t host_dxgi;
    uint64_t texture;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_device *impl_from_IQemuD3D11Device(IQemuD3D11Device *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_device, IQemuD3D11Device_iface);
}

static HRESULT STDMETHODCALLTYPE qemu_device_QueryInterface(IQemuD3D11Device *iface, REFIID riid, void **out)
{
    struct qemu_d3d11_device *device = impl_from_IQemuD3D11Device(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, out);
}

static ULONG STDMETHODCALLTYPE qemu_device_AddRef(IQemuD3D11Device *iface)
{
    struct qemu_d3d11_device *device = impl_from_IQemuD3D11Device(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE qemu_device_Release(IQemuD3D11Device *iface)
{
    struct qemu_d3d11_device *device = impl_from_IQemuD3D11Device(iface);
    return IUnknown_Release(device->outer_unk);
}

static HRESULT STDMETHODCALLTYPE qemu_device_wrap_implicit_surface(IQemuD3D11Device *iface, uint64_t host,
        IUnknown **surface)
{
    struct qemu_wrap_implicit_surface call;
    struct qemu_d3d11_device *device = impl_from_IQemuD3D11Device(iface);
    struct qemu_d3d11_texture *texture;

    WINE_TRACE("Creating d3d11 texture for host DXGI surface %p.\n", (void *)(ULONG_PTR)host);

    call.super.id = QEMU_SYSCALL_ID(CALL_WRAP_IMPLICIT_SURFACE);
    call.iface = (ULONG_PTR)device;
    call.host_dxgi = host;
    qemu_syscall(&call.super);
    if (FAILED(call.super.iret))
        return call.super.iret;

    texture = (struct qemu_d3d11_texture *)(ULONG_PTR)call.texture;
    qemu_d3d11_texture_guest_init(texture, device, 2, host);
    *surface = (IUnknown *)&texture->ID3D11Texture2D_iface;
    IUnknown_AddRef(*surface);

    return S_OK;
}

static struct IQemuD3D11DeviceVtbl qemu_device_vtbl =
{
    /* IUnknown methods */
    qemu_device_QueryInterface,
    qemu_device_AddRef,
    qemu_device_Release,
    /* IQemuDXGIDeviceVtbl methods */
    qemu_device_wrap_implicit_surface,
};

static struct
{
    ID3D11DeviceContextVtbl vtbl1;
    void *CopySubresourceRegion1;
    void *UpdateSubresource1;
    void *DiscardResource;
    void *DiscardView;
    void *VSSetConstantBuffers1;
    void *HSSetConstantBuffers1;
    void *DSSetConstantBuffers1;
    void *GSSetConstantBuffers1;
    void *PSSetConstantBuffers1;
    void *CSSetConstantBuffers1;
    void *VSGetConstantBuffers1;
    void *HSGetConstantBuffers1;
    void *DSGetConstantBuffers1;
    void *GSGetConstantBuffers1;
    void *PSGetConstantBuffers1;
    void *CSGetConstantBuffers1;
    void *SwapDeviceContextState;
    void *ClearView;
    void *DiscardView1;
}
d3d11_immediate_context_vtbl =
{
    {
        /* IUnknown methods */
        d3d11_immediate_context_QueryInterface,
        d3d11_immediate_context_AddRef,
        d3d11_immediate_context_Release,
        /* ID3D11DeviceChild methods */
        d3d11_immediate_context_GetDevice,
        d3d11_immediate_context_GetPrivateData,
        d3d11_immediate_context_SetPrivateData,
        d3d11_immediate_context_SetPrivateDataInterface,
        /* ID3D11DeviceContext methods */
        d3d11_immediate_context_VSSetConstantBuffers,
        d3d11_immediate_context_PSSetShaderResources,
        d3d11_immediate_context_PSSetShader,
        d3d11_immediate_context_PSSetSamplers,
        d3d11_immediate_context_VSSetShader,
        d3d11_immediate_context_DrawIndexed,
        d3d11_immediate_context_Draw,
        d3d11_immediate_context_Map,
        d3d11_immediate_context_Unmap,
        d3d11_immediate_context_PSSetConstantBuffers,
        d3d11_immediate_context_IASetInputLayout,
        d3d11_immediate_context_IASetVertexBuffers,
        d3d11_immediate_context_IASetIndexBuffer,
        d3d11_immediate_context_DrawIndexedInstanced,
        d3d11_immediate_context_DrawInstanced,
        d3d11_immediate_context_GSSetConstantBuffers,
        d3d11_immediate_context_GSSetShader,
        d3d11_immediate_context_IASetPrimitiveTopology,
        d3d11_immediate_context_VSSetShaderResources,
        d3d11_immediate_context_VSSetSamplers,
        d3d11_immediate_context_Begin,
        d3d11_immediate_context_End,
        d3d11_immediate_context_GetData,
        d3d11_immediate_context_SetPredication,
        d3d11_immediate_context_GSSetShaderResources,
        d3d11_immediate_context_GSSetSamplers,
        d3d11_immediate_context_OMSetRenderTargets,
        d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews,
        d3d11_immediate_context_OMSetBlendState,
        d3d11_immediate_context_OMSetDepthStencilState,
        d3d11_immediate_context_SOSetTargets,
        d3d11_immediate_context_DrawAuto,
        d3d11_immediate_context_DrawIndexedInstancedIndirect,
        d3d11_immediate_context_DrawInstancedIndirect,
        d3d11_immediate_context_Dispatch,
        d3d11_immediate_context_DispatchIndirect,
        d3d11_immediate_context_RSSetState,
        d3d11_immediate_context_RSSetViewports,
        d3d11_immediate_context_RSSetScissorRects,
        d3d11_immediate_context_CopySubresourceRegion,
        d3d11_immediate_context_CopyResource,
        d3d11_immediate_context_UpdateSubresource,
        d3d11_immediate_context_CopyStructureCount,
        d3d11_immediate_context_ClearRenderTargetView,
        d3d11_immediate_context_ClearUnorderedAccessViewUint,
        d3d11_immediate_context_ClearUnorderedAccessViewFloat,
        d3d11_immediate_context_ClearDepthStencilView,
        d3d11_immediate_context_GenerateMips,
        d3d11_immediate_context_SetResourceMinLOD,
        d3d11_immediate_context_GetResourceMinLOD,
        d3d11_immediate_context_ResolveSubresource,
        d3d11_immediate_context_ExecuteCommandList,
        d3d11_immediate_context_HSSetShaderResources,
        d3d11_immediate_context_HSSetShader,
        d3d11_immediate_context_HSSetSamplers,
        d3d11_immediate_context_HSSetConstantBuffers,
        d3d11_immediate_context_DSSetShaderResources,
        d3d11_immediate_context_DSSetShader,
        d3d11_immediate_context_DSSetSamplers,
        d3d11_immediate_context_DSSetConstantBuffers,
        d3d11_immediate_context_CSSetShaderResources,
        d3d11_immediate_context_CSSetUnorderedAccessViews,
        d3d11_immediate_context_CSSetShader,
        d3d11_immediate_context_CSSetSamplers,
        d3d11_immediate_context_CSSetConstantBuffers,
        d3d11_immediate_context_VSGetConstantBuffers,
        d3d11_immediate_context_PSGetShaderResources,
        d3d11_immediate_context_PSGetShader,
        d3d11_immediate_context_PSGetSamplers,
        d3d11_immediate_context_VSGetShader,
        d3d11_immediate_context_PSGetConstantBuffers,
        d3d11_immediate_context_IAGetInputLayout,
        d3d11_immediate_context_IAGetVertexBuffers,
        d3d11_immediate_context_IAGetIndexBuffer,
        d3d11_immediate_context_GSGetConstantBuffers,
        d3d11_immediate_context_GSGetShader,
        d3d11_immediate_context_IAGetPrimitiveTopology,
        d3d11_immediate_context_VSGetShaderResources,
        d3d11_immediate_context_VSGetSamplers,
        d3d11_immediate_context_GetPredication,
        d3d11_immediate_context_GSGetShaderResources,
        d3d11_immediate_context_GSGetSamplers,
        d3d11_immediate_context_OMGetRenderTargets,
        d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews,
        d3d11_immediate_context_OMGetBlendState,
        d3d11_immediate_context_OMGetDepthStencilState,
        d3d11_immediate_context_SOGetTargets,
        d3d11_immediate_context_RSGetState,
        d3d11_immediate_context_RSGetViewports,
        d3d11_immediate_context_RSGetScissorRects,
        d3d11_immediate_context_HSGetShaderResources,
        d3d11_immediate_context_HSGetShader,
        d3d11_immediate_context_HSGetSamplers,
        d3d11_immediate_context_HSGetConstantBuffers,
        d3d11_immediate_context_DSGetShaderResources,
        d3d11_immediate_context_DSGetShader,
        d3d11_immediate_context_DSGetSamplers,
        d3d11_immediate_context_DSGetConstantBuffers,
        d3d11_immediate_context_CSGetShaderResources,
        d3d11_immediate_context_CSGetUnorderedAccessViews,
        d3d11_immediate_context_CSGetShader,
        d3d11_immediate_context_CSGetSamplers,
        d3d11_immediate_context_CSGetConstantBuffers,
        d3d11_immediate_context_ClearState,
        d3d11_immediate_context_Flush,
        d3d11_immediate_context_GetType,
        d3d11_immediate_context_GetContextFlags,
        d3d11_immediate_context_FinishCommandList,
    },
    /* ID3D11DeviceContext1 methods */
    d3d11_immediate_context_CopySubresourceRegion1,
    d3d11_immediate_context_UpdateSubresource1,
    d3d11_immediate_context_DiscardResource,
    d3d11_immediate_context_DiscardView,
    d3d11_immediate_context_VSSetConstantBuffers1,
    d3d11_immediate_context_HSSetConstantBuffers1,
    d3d11_immediate_context_DSSetConstantBuffers1,
    d3d11_immediate_context_GSSetConstantBuffers1,
    d3d11_immediate_context_PSSetConstantBuffers1,
    d3d11_immediate_context_CSSetConstantBuffers1,
    d3d11_immediate_context_VSGetConstantBuffers1,
    d3d11_immediate_context_HSGetConstantBuffers1,
    d3d11_immediate_context_DSGetConstantBuffers1,
    d3d11_immediate_context_GSGetConstantBuffers1,
    d3d11_immediate_context_PSGetConstantBuffers1,
    d3d11_immediate_context_CSGetConstantBuffers1,
    d3d11_immediate_context_SwapDeviceContextState,
    d3d11_immediate_context_ClearView,
    d3d11_immediate_context_DiscardView1,
};

static struct
{
    ID3D11DeviceVtbl vtbl1;
    /* ID3D11Device1 methods */
    void *GetImmediateContext1;
    void *CreateDeferredContext1;
    void *CreateBlendState1;
    void *CreateRasterizerState1;
    void *CreateDeviceContextState;
    void *OpenSharedResource1;
    void *OpenSharedResourceByName;
    /* ID3D11Device2 methods */
    void *GetImmediateContext2;
    void *CreateDeferredContext2;
    void *GetResourceTiling;
    void *CheckMultisampleQualityLevels1;
}
d3d11_device_vtbl =
{
    {
    /* IUnknown methods */
        d3d11_device_QueryInterface,
        d3d11_device_AddRef,
        d3d11_device_Release,
        /* ID3D11Device methods */
        d3d11_device_CreateBuffer,
        d3d11_device_CreateTexture1D,
        d3d11_device_CreateTexture2D,
        d3d11_device_CreateTexture3D,
        d3d11_device_CreateShaderResourceView,
        d3d11_device_CreateUnorderedAccessView,
        d3d11_device_CreateRenderTargetView,
        d3d11_device_CreateDepthStencilView,
        d3d11_device_CreateInputLayout,
        d3d11_device_CreateVertexShader,
        d3d11_device_CreateGeometryShader,
        d3d11_device_CreateGeometryShaderWithStreamOutput,
        d3d11_device_CreatePixelShader,
        d3d11_device_CreateHullShader,
        d3d11_device_CreateDomainShader,
        d3d11_device_CreateComputeShader,
        d3d11_device_CreateClassLinkage,
        d3d11_device_CreateBlendState,
        d3d11_device_CreateDepthStencilState,
        d3d11_device_CreateRasterizerState,
        d3d11_device_CreateSamplerState,
        d3d11_device_CreateQuery,
        d3d11_device_CreatePredicate,
        d3d11_device_CreateCounter,
        d3d11_device_CreateDeferredContext,
        d3d11_device_OpenSharedResource,
        d3d11_device_CheckFormatSupport,
        d3d11_device_CheckMultisampleQualityLevels,
        d3d11_device_CheckCounterInfo,
        d3d11_device_CheckCounter,
        d3d11_device_CheckFeatureSupport,
        d3d11_device_GetPrivateData,
        d3d11_device_SetPrivateData,
        d3d11_device_SetPrivateDataInterface,
        d3d11_device_GetFeatureLevel,
        d3d11_device_GetCreationFlags,
        d3d11_device_GetDeviceRemovedReason,
        d3d11_device_GetImmediateContext,
        d3d11_device_SetExceptionMode,
        d3d11_device_GetExceptionMode,
    },
    /* ID3D11Device1 methods */
    d3d11_device_GetImmediateContext1,
    d3d11_device_CreateDeferredContext1,
    d3d11_device_CreateBlendState1,
    d3d11_device_CreateRasterizerState1,
    d3d11_device_CreateDeviceContextState,
    d3d11_device_OpenSharedResource1,
    d3d11_device_OpenSharedResourceByName,
    /* ID3D11Device2 methods */
    d3d11_device_GetImmediateContext2,
    d3d11_device_CreateDeferredContext2,
    d3d11_device_GetResourceTiling,
    d3d11_device_CheckMultisampleQualityLevels1,
};

static struct ID3D10Device1Vtbl d3d10_device1_vtbl =
{
    /* IUnknown methods */
    d3d10_device_QueryInterface,
    d3d10_device_AddRef,
    d3d10_device_Release,
    /* ID3D10Device methods */
    d3d10_device_VSSetConstantBuffers,
    d3d10_device_PSSetShaderResources,
    d3d10_device_PSSetShader,
    d3d10_device_PSSetSamplers,
    d3d10_device_VSSetShader,
    d3d10_device_DrawIndexed,
    d3d10_device_Draw,
    d3d10_device_PSSetConstantBuffers,
    d3d10_device_IASetInputLayout,
    d3d10_device_IASetVertexBuffers,
    d3d10_device_IASetIndexBuffer,
    d3d10_device_DrawIndexedInstanced,
    d3d10_device_DrawInstanced,
    d3d10_device_GSSetConstantBuffers,
    d3d10_device_GSSetShader,
    d3d10_device_IASetPrimitiveTopology,
    d3d10_device_VSSetShaderResources,
    d3d10_device_VSSetSamplers,
    d3d10_device_SetPredication,
    d3d10_device_GSSetShaderResources,
    d3d10_device_GSSetSamplers,
    d3d10_device_OMSetRenderTargets,
    d3d10_device_OMSetBlendState,
    d3d10_device_OMSetDepthStencilState,
    d3d10_device_SOSetTargets,
    d3d10_device_DrawAuto,
    d3d10_device_RSSetState,
    d3d10_device_RSSetViewports,
    d3d10_device_RSSetScissorRects,
    d3d10_device_CopySubresourceRegion,
    d3d10_device_CopyResource,
    d3d10_device_UpdateSubresource,
    d3d10_device_ClearRenderTargetView,
    d3d10_device_ClearDepthStencilView,
    d3d10_device_GenerateMips,
    d3d10_device_ResolveSubresource,
    d3d10_device_VSGetConstantBuffers,
    d3d10_device_PSGetShaderResources,
    d3d10_device_PSGetShader,
    d3d10_device_PSGetSamplers,
    d3d10_device_VSGetShader,
    d3d10_device_PSGetConstantBuffers,
    d3d10_device_IAGetInputLayout,
    d3d10_device_IAGetVertexBuffers,
    d3d10_device_IAGetIndexBuffer,
    d3d10_device_GSGetConstantBuffers,
    d3d10_device_GSGetShader,
    d3d10_device_IAGetPrimitiveTopology,
    d3d10_device_VSGetShaderResources,
    d3d10_device_VSGetSamplers,
    d3d10_device_GetPredication,
    d3d10_device_GSGetShaderResources,
    d3d10_device_GSGetSamplers,
    d3d10_device_OMGetRenderTargets,
    d3d10_device_OMGetBlendState,
    d3d10_device_OMGetDepthStencilState,
    d3d10_device_SOGetTargets,
    d3d10_device_RSGetState,
    d3d10_device_RSGetViewports,
    d3d10_device_RSGetScissorRects,
    d3d10_device_GetDeviceRemovedReason,
    d3d10_device_SetExceptionMode,
    d3d10_device_GetExceptionMode,
    d3d10_device_GetPrivateData,
    d3d10_device_SetPrivateData,
    d3d10_device_SetPrivateDataInterface,
    d3d10_device_ClearState,
    d3d10_device_Flush,
    d3d10_device_CreateBuffer,
    d3d10_device_CreateTexture1D,
    d3d10_device_CreateTexture2D,
    d3d10_device_CreateTexture3D,
    d3d10_device_CreateShaderResourceView,
    d3d10_device_CreateRenderTargetView,
    d3d10_device_CreateDepthStencilView,
    d3d10_device_CreateInputLayout,
    d3d10_device_CreateVertexShader,
    d3d10_device_CreateGeometryShader,
    d3d10_device_CreateGeometryShaderWithStreamOutput,
    d3d10_device_CreatePixelShader,
    d3d10_device_CreateBlendState,
    d3d10_device_CreateDepthStencilState,
    d3d10_device_CreateRasterizerState,
    d3d10_device_CreateSamplerState,
    d3d10_device_CreateQuery,
    d3d10_device_CreatePredicate,
    d3d10_device_CreateCounter,
    d3d10_device_CheckFormatSupport,
    d3d10_device_CheckMultisampleQualityLevels,
    d3d10_device_CheckCounterInfo,
    d3d10_device_CheckCounter,
    d3d10_device_GetCreationFlags,
    d3d10_device_OpenSharedResource,
    d3d10_device_SetTextFilterSize,
    d3d10_device_GetTextFilterSize,
    d3d10_device_CreateShaderResourceView1,
    d3d10_device_CreateBlendState1,
    d3d10_device_GetFeatureLevel,
};

static struct IUnknownVtbl d3d_device_inner_unknown_vtbl =
{
    /* IUnknown methods */
    d3d_device_inner_QueryInterface,
    d3d_device_inner_AddRef,
    d3d_device_inner_Release,
};

static struct ID3D10MultithreadVtbl d3d10_multithread_vtbl =
{
    d3d10_multithread_QueryInterface,
    d3d10_multithread_AddRef,
    d3d10_multithread_Release,
    d3d10_multithread_Enter,
    d3d10_multithread_Leave,
    d3d10_multithread_SetMultithreadProtected,
    d3d10_multithread_GetMultithreadProtected,
};

void qemu_d3d11_device_guest_init(struct qemu_d3d11_device *device, void *outer_unknown)
{
    device->IUnknown_inner.lpVtbl = &d3d_device_inner_unknown_vtbl;
    device->ID3D11Device2_iface.lpVtbl = &d3d11_device_vtbl.vtbl1;
    device->ID3D10Device1_iface.lpVtbl = &d3d10_device1_vtbl;
    device->ID3D10Multithread_iface.lpVtbl = &d3d10_multithread_vtbl;
    device->IQemuD3D11Device_iface.lpVtbl = &qemu_device_vtbl;
    device->outer_unk = outer_unknown;
}

void qemu_d3d11_context_guest_init(struct qemu_d3d11_device_context *context)
{
    context->ID3D11DeviceContext1_iface.lpVtbl = &d3d11_immediate_context_vtbl.vtbl1;
}

#else

void qemu_wrap_implicit_surface(struct qemu_syscall *call)
{
    struct qemu_wrap_implicit_surface *c = (struct qemu_wrap_implicit_surface *)call;
    struct qemu_d3d11_device *device;
    struct qemu_d3d11_texture *texture;
    ID3D11Texture2D *host;
    uint64_t dummy;

    WINE_TRACE("\n");
    device = QEMU_G2H(c->iface);

    c->super.iret = IUnknown_QueryInterface((IUnknown *)QEMU_G2H(c->host_dxgi), &IID_ID3D11Texture2D, (void **)&host);
    if (FAILED(c->super.iret))
        WINE_ERR("Could not get ID3D11Texture2D interface.\n");

    c->super.iret = qemu_d3d11_texture_create((ID3D11Resource *)host, device, &dummy, 2, &texture);
    c->texture = QEMU_H2G(texture);

    ID3D11Texture2D_Release(host);
}

static inline struct qemu_d3d11_device *impl_from_priv_data(IUnknown *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_device, priv_data_iface);
}

struct qemu_d3d11_device *device_from_host(ID3D11Device2 *host)
{
    IUnknown *priv;
    DWORD size = sizeof(priv);
    HRESULT hr;

    hr = ID3D11Device2_GetPrivateData(host, &IID_d3d11_priv_data, &size, &priv);
    if (FAILED(hr))
        WINE_ERR("Failed to get private data from host surface %p.\n", host);

    IUnknown_Release(priv);
    return impl_from_priv_data(priv);
}

#endif