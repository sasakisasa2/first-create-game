//
// GameBase.h
//

#pragma once

#define GAME_TITLE      L"SEARCH STAR"

#define DXTK_FULLSCREEN 0   // 1...fullscreen mode
#define DXTK_KEYBOARD   1   // 1...use Keyboard
#define DXTK_MOUSE      0   // 1...use Mouse
#define DXTK_GAMEPAD    1   // 1...use GamePad

#define DXTK_sRGB       1   // 1...sRGB / 0...RGB
#define DXTK_DEBUGLAYER 0   // 1...use DebugLayer(Forced sRGB is disabled)
#define DXTK_MULTIGPU   0   // 1...use Heterogeneous Multiadapter
//#define DXTK_AMP      0   // 1...use AMP

#define DXTK_D3D11      0   // 1...use Direct3D11
#define DXTK_DWRITE     0   // 1...use Direct2D and DirectWrite
#define DXTK_D3D9       0   // 1...use Direct3D9
#define DXTK_DSHOW      0   // 1...use DirectShow

enum class screen : int {
    width = 1280,
    height = 720
};

#include "DX\StepTimer.h"

#if DXTK_D3D11 || DXTK_DWRITE
    #include <d3d11on12.h>
#endif
#if DXTK_DWRITE
    #include <d2d1_3.h>
    #include <shtypes.h>
    #include <dwrite.h>
#endif

#if DXTK_D3D9
    #include <d3d9on12.h>
    #include "DirectXTK9.h"
#endif

#if DXTK_DSHOW
    #include "DirectShow.h"
#endif

struct DirectX::XMVECTORF32;
class Scene;

enum {
    PrimaryAdapter = 0,
    SecondaryAdapter,
    AdapterCount
};

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class GameBase final
{
public:

    GameBase();
    ~GameBase();

    GameBase(GameBase&&) = default;
    GameBase& operator= (GameBase&&) = default;

    GameBase(GameBase const&) = delete;
    GameBase& operator= (GameBase const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:
    friend class dxtk;

    void Update(DX::StepTimer const& timer);
    void Render();

    void ResetCommand(const UINT adapter);
    void ClearRenderTarget(const UINT adapter, const DirectX::XMVECTORF32& color);

    void ExecuteCommandList(const UINT adapter);
    void ExecuteCommandLists(const UINT adapter, UINT NumCommandLists,
                             ID3D12GraphicsCommandList* const* ppCommandLists);
#if DXTK_MULTIGPU
    ID3D12Resource* PopulateCopyCommandList();
    void ExecuteCopyCommandList();

    inline void CommandQueueSignal(ID3D12CommandQueue* queue)
    {
        DX::ThrowIfFailed(queue->Signal(m_renderFence.Get(), m_renderFenceValue));
    }

    inline void CommandQueueWait(ID3D12CommandQueue* queue)
    {
        DX::ThrowIfFailed(queue->Wait(m_renderFence.Get(), m_renderFenceValue));
        ++m_renderFenceValue;
    }

    inline void CrossAdapterQueueSignal()
    {
        DX::ThrowIfFailed(
            m_copyCommandQueue->Signal(
                m_devices[SecondaryAdapter].CrossAdapterFence.Get(),
                m_crossAdapterFenceValue
            )
        );
    }

    inline void CrossAdapterQueueWait()
    {
        DX::ThrowIfFailed(
            m_devices[PrimaryAdapter].CommandQueue->Wait(
                m_devices[PrimaryAdapter].CrossAdapterFence.Get(),
                m_crossAdapterFenceValue
            )
        );
        ++m_crossAdapterFenceValue;
    }
#endif

    void Present();

    void CreateDevice();
    void CreateResources();
#if DXTK_D3D11 || DXTK_DWRITE
    void D3D11CreateDevice();
#endif
#if DXTK_D3D9
    void D3D9CreateDevice();
    void D3D9CreateShaderResourceView(const D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor);
    void D3D9UpdateResource();
    void D3D9WaitUpdate();
#endif

    void WaitForGpu() noexcept;
    void MoveToNextFrame();

    void OnDeviceLost();

    // Application state
    HWND                                              m_window;
    int                                               m_outputWidth;
    int                                               m_outputHeight;

    static const UINT                                 c_swapBufferCount = 3;
    static const D3D12_RESOURCE_STATES                m_resourceState[AdapterCount];

    Microsoft::WRL::ComPtr<IDXGIFactory4>             m_dxgiFactory;

    // Direct3D Objects
    struct D3D12OBJECTS {
        Microsoft::WRL::ComPtr<ID3D12Device>              Device;

        Microsoft::WRL::ComPtr<ID3D12CommandQueue>        CommandQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    CommandAllocators[c_swapBufferCount];
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

        Microsoft::WRL::ComPtr<ID3D12Resource>            RenderTargets[c_swapBufferCount];
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>      RTVDescriptorHeap;
        UINT                                              RTVDescriptorSize = 0;

        Microsoft::WRL::Wrappers::Event                   FenceEvent;

        std::unique_ptr<DirectX::GraphicsMemory>          GraphicsMemory;

#if DXTK_MULTIGPU
        Microsoft::WRL::ComPtr<ID3D12Heap>                CrossAdapterResourceHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource>            CrossAdapterResources[c_swapBufferCount];
        Microsoft::WRL::ComPtr<ID3D12Fence>               CrossAdapterFence;
#endif
    };
    D3D12OBJECTS                                      m_devices[AdapterCount];
    UINT                                              m_adapterCount;
    UINT                                              m_venderId;

#if DXTK_MULTIGPU
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>        m_copyCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    m_copyCommandAllocators[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_copyCommandList;

    BOOL                                              m_crossAdapterTextureSupport;
    Microsoft::WRL::ComPtr<ID3D12Resource>            m_primaryAdapterTextures[c_swapBufferCount];   // Only used if cross adapter texture support is unavailable.
    UINT64                                            m_crossAdapterFenceValue;
#endif

    Microsoft::WRL::ComPtr<ID3D12Fence>               m_fence;
    UINT64                                            m_fenceValues[c_swapBufferCount];

#if DXTK_MULTIGPU
    Microsoft::WRL::ComPtr<ID3D12Fence>               m_renderFence;
    UINT64                                            m_renderFenceValue;
#endif

    // Rendering resources
    Microsoft::WRL::ComPtr<IDXGISwapChain3>           m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource>            m_depthStencil;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>      m_dsvDescriptorHeap;

    UINT                                              m_backBufferIndex;
    DXGI_FORMAT                                       m_backBufferFormat;
    DXGI_FORMAT                                       m_depthBufferFormat;

#if DXTK_D3D11 || DXTK_DWRITE
    // Direct3D11 on 12 objects
    Microsoft::WRL::ComPtr<ID3D11Device>              m_d3d11Device;
    Microsoft::WRL::ComPtr<ID3D11On12Device>          m_d3d11on12Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>       m_d3d11DeviceContext;
#endif

#if DXTK_DWRITE
    // Direct2D objects
    Microsoft::WRL::ComPtr<ID2D1Device2>              m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext2>       m_d2dDeviceContext;
    Microsoft::WRL::ComPtr<IDWriteFactory>            m_dWriteFactory;

    Microsoft::WRL::ComPtr<ID3D11Resource>            m_wrappedBackBuffers[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID2D1Bitmap1>              m_d2dRenderTargets[c_swapBufferCount];
#endif

#if DXTK_D3D9
    // Direct3D9 on 12 Objects
    Microsoft::WRL::ComPtr<IDirect3D9Ex>              m_d3d9;
    Microsoft::WRL::ComPtr<IDirect3DDevice9Ex>        m_d3d9Device;
//  Microsoft::WRL::ComPtr<IDirect3DDevice9On12>      m_device9on12;
    Microsoft::WRL::ComPtr<IDirect3DTexture9>         m_d3d9RenderTexture;
    Microsoft::WRL::ComPtr<IDirect3DSurface9>         m_d3d9RenderSurface;
    Microsoft::WRL::ComPtr<IDirect3DSurface9>         m_d3d9CopySurface;

    Microsoft::WRL::ComPtr<ID3D12Resource>            m_d3d12RenderResource;
    Microsoft::WRL::ComPtr<ID3D12Resource>            m_d3d12UploadResource;
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT                m_d3d12Footprint;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue>        m_d3d12UploadCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    m_d3d12UploadCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_d3d12UploadCommandList;
    Microsoft::WRL::ComPtr<ID3D12Fence>               m_d3d12UploadFence;
    UINT64                                            m_d3d12UploadFenceValue;
#endif

    // Audio
    std::unique_ptr<DirectX::AudioEngine>             m_audioEngine;
    bool                                              m_retryAudio;

    // Game state
    DX::StepTimer                                     m_timer;

#if DXTK_KEYBOARD
    std::unique_ptr<DirectX::Keyboard>                m_keyboard;
#endif

#if DXTK_MOUSE
    std::unique_ptr<DirectX::Mouse>                   m_mouse;
#endif

#if DXTK_GAMEPAD
#endif

//#if DXTK_AMP
//    std::unique_ptr<concurrency::accelerator>         m_primaryAccelerator;
//    std::unique_ptr<concurrency::accelerator>         m_secondaryAccelerator;
//#endif

    std::unique_ptr<Scene>                            m_scene;
    dxtk*                                             m_dxtk;
};
