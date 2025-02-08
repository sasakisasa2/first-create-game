//
// Game.cpp
//

#include "pch.h"
#include "dxtk.h"
#include "Scenes\SceneFactory.h"

#if DXTK_D3D9
#pragma comment(lib, "d3d9.lib")
#endif

using Microsoft::WRL::ComPtr;
using namespace DirectX;

dxtk* DXTK = nullptr;
INPUTSYSTEM InputSystem;

const D3D12_RESOURCE_STATES GameBase::m_resourceState[AdapterCount]
{ D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COMMON };

GameBase::GameBase() :
    m_window(nullptr),
    m_outputWidth (static_cast<int>(screen::width )),
    m_outputHeight(static_cast<int>(screen::height)),
    m_adapterCount(0),
    m_venderId(0),
    m_backBufferIndex(0),
    m_backBufferFormat(DXGI_FORMAT_UNKNOWN),
    m_depthBufferFormat(DXGI_FORMAT_UNKNOWN),
#if DXTK_MULTIGPU
    m_crossAdapterTextureSupport(FALSE),
    m_crossAdapterFenceValue(0),
#endif
    m_fenceValues{},
#if DXTK_MULTIGPU
    m_renderFenceValue(0),
#endif
#if DXTK_D3D9
    m_d3d12UploadFenceValue(0),
    m_d3d12Footprint{},
#endif
    m_retryAudio(false),
    m_dxtk(nullptr)
{
    DX::ThrowIfFailed(DXGIDeclareAdapterRemovalSupport());
    SetCurrentDirectory(L"Assets");
}

GameBase::~GameBase()
{
    m_scene.reset();

    //if (m_audioEngine)
    //{
    //    m_audioEngine->Suspend();
    //}

    // Ensure that the GPU is no longer referencing resources that are about to be destroyed.
    if (m_devices[PrimaryAdapter].CommandQueue && m_fence && m_devices[PrimaryAdapter].FenceEvent.IsValid())
        WaitForGpu();
}

// Initialize the Direct3D resources required to run.
void GameBase::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth  = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    std::wstring window_title = GAME_TITLE;
#ifdef _DEBUG
    window_title += L" (";
#if   !DXTK_D3D11 && !DXTK_DWRITE && !DXTK_D3D9
    window_title += L"DX12";
#elif (DXTK_D3D11 && DXTK_DWRITE) && DXTK_D3D9
    window_title += L"DX12&11on12&9";
#elif DXTK_D3D11 || DXTK_DWRITE
    window_title += L"DX12&11on12";
#else
    window_title += L"DX12&9";
#endif
#if !DXTK_DEBUGLAYER && DXTK_sRGB
    window_title += L":sRGB";
#else
    window_title += L":RGB";
#endif
    window_title += L")";
#endif
    SetWindowText(m_window, window_title.c_str());

#if DXTK_KEYBOARD
    m_keyboard = std::make_unique<DirectX::Keyboard>();
#endif
#if DXTK_MOUSE
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);
    InputSystem.Mouse.mouse = mouse.get();
#endif
#if DXTK_GAMEPAD
    wi::gamepad::Initialize();
#endif

    CreateDevice();
    CreateResources();
#if DXTK_D3D11 || DXTK_DWRITE
    D3D11CreateDevice();
#endif
#if DXTK_D3D9
    D3D9CreateDevice();
#endif

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);
    m_retryAudio  = false;

//#if DXTK_AMP
//    m_primaryAccelerator = std::make_unique<concurrency::accelerator>();
//    m_secondaryAccelerator = std::make_unique<concurrency::accelerator>();
//
//    *m_primaryAccelerator = concurrency::accelerator::get_auto_selection_view().get_accelerator();
//    *m_secondaryAccelerator = *m_primaryAccelerator;
//
//    std::vector<concurrency::accelerator> gpus = concurrency::accelerator::get_all();
//    const auto git_end = gpus.rend();
//    for (auto git = gpus.rbegin(); git != git_end; ++git) {
//        if (git->is_emulated)
//            continue;
//        if (*m_secondaryAccelerator != *git) {
//            *m_secondaryAccelerator = *git;
//            break;
//        }
//    }
//#endif

    m_dxtk = dxtk::GetInstance();
    m_dxtk->Attach(this);
    DXTK = m_dxtk;

    // TODO: Set the first scene.
    m_scene = SceneFactory::CreateScene(NextScene::StartScene);
    m_scene->Start();
}

// Executes the basic game loop.
void GameBase::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void GameBase::Update(DX::StepTimer const& timer)
{
    if (m_retryAudio)
    {
        m_retryAudio = false;
        if (m_audioEngine->Reset())
        {
            // TODO: restart any looped sounds here
            m_scene->OnRestartSound();
        }
    }
    else if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            m_retryAudio = true;
        }
    }

    // Update input devices.
#if DXTK_KEYBOARD
    InputSystem.Keyboard.isPressed = m_keyboard->GetState();
    InputSystem.Keyboard.tracker.Update(InputSystem.Keyboard.isPressed);
#endif
#if DXTK_MOUSE
    InputSystem.Mouse.isPressed = m_mouse->GetState();
    InputSystem.Mouse.was.Update(InputSystem.Mouse.isPressed);
#endif
#if DXTK_GAMEPAD
    wi::gamepad::Update();
#endif

    m_dxtk->Time.deltaTime = float(timer.GetElapsedSeconds());
    const NextScene nextScene = m_scene->Update(m_dxtk->Time.deltaTime);
    if (nextScene != NextScene::Continue)
    {
        m_scene.reset();
        m_scene = SceneFactory::CreateScene(nextScene);
        m_scene->Start();
    }
}

// Draws the scene.
void GameBase::Render()
{
    m_scene->Render();
    Present();
}

// Reset command list and allocator.
void GameBase::ResetCommand(const UINT adapter)
{
    // Reset command list and allocator.
    DX::ThrowIfFailed(m_devices[adapter].CommandAllocators[m_backBufferIndex]->Reset());
    DX::ThrowIfFailed(m_devices[adapter].CommandList->Reset(m_devices[adapter].CommandAllocators[m_backBufferIndex].Get(), nullptr));

    // Transition the render target into the correct state to allow for drawing into it.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_devices[adapter].RenderTargets[m_backBufferIndex].Get(),
        m_resourceState[adapter], D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    m_devices[adapter].CommandList->ResourceBarrier(1, &barrier);
}

// Helper method to prepare the command list for rendering and clear the back buffers.
void GameBase::ClearRenderTarget(const UINT adapter, const DirectX::XMVECTORF32& color)
{
    // Clear the views.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
        m_devices[adapter].RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        static_cast<INT>(m_backBufferIndex), m_devices[adapter].RTVDescriptorSize
    );
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDescriptor(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    m_devices[adapter].CommandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    m_devices[adapter].CommandList->ClearRenderTargetView(rtvDescriptor, color, 0, nullptr);
    m_devices[adapter].CommandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    static const D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
    static const D3D12_RECT  scissorRect = { 0, 0, static_cast<LONG>(m_outputWidth), static_cast<LONG>(m_outputHeight) };
    m_devices[adapter].CommandList->RSSetViewports(1, &viewport);
    m_devices[adapter].CommandList->RSSetScissorRects(1, &scissorRect);
}

// Submits the command list to the GPU
void GameBase::ExecuteCommandList(const UINT adapter)
{
    // Transition the render target to the state that allows it to be presented to the display.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_devices[adapter].RenderTargets[m_backBufferIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, m_resourceState[adapter]
    );
    m_devices[adapter].CommandList->ResourceBarrier(1, &barrier);

    // Send the command list off to the GPU for processing.
    DX::ThrowIfFailed(m_devices[adapter].CommandList->Close());
    m_devices[adapter].CommandQueue.Get()->ExecuteCommandLists(
        1, CommandListCast(m_devices[adapter].CommandList.GetAddressOf())
    );
}

// Submits the command lists to the GPU
void GameBase::ExecuteCommandLists(const UINT adapter, UINT NumCommandLists,
                                   ID3D12GraphicsCommandList* const* ppCommandLists)
{
    // Transition the render target to the state that allows it to be presented to the display.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_devices[adapter].RenderTargets[m_backBufferIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, m_resourceState[adapter]
    );
    m_devices[adapter].CommandList->ResourceBarrier(1, &barrier);

    // Send the command list off to the GPU for processing.
    for (UINT index = 0; index < NumCommandLists; ++index)
        DX::ThrowIfFailed(ppCommandLists[index]->Close());

    m_devices[adapter].CommandQueue.Get()->ExecuteCommandLists(
        NumCommandLists, CommandListCast(ppCommandLists)
    );
}

#if DXTK_MULTIGPU
// Command list to copy the render target to the shared heap on the secondary adapter.
ID3D12Resource* GameBase::PopulateCopyCommandList()
{
    // Reset the copy command allocator and command list.
    DX::ThrowIfFailed(m_copyCommandAllocators[m_backBufferIndex]->Reset());
    DX::ThrowIfFailed(m_copyCommandList->Reset(m_copyCommandAllocators[m_backBufferIndex].Get(), nullptr));

    // Copy the intermediate render target to the cross-adapter shared resource.
    // Transition barriers are not required since there are fences guarding against
    // concurrent read/write access to the shared heap.
    if (m_crossAdapterTextureSupport)
    {
        // If cross-adapter row-major textures are supported by the adapter,
        // simply copy the texture into the cross-adapter texture.
        auto resource = m_devices[SecondaryAdapter].CrossAdapterResources[m_backBufferIndex].Get();

        m_copyCommandList->CopyResource(
            resource, m_devices[SecondaryAdapter].RenderTargets[m_backBufferIndex].Get()
        );
        DX::ThrowIfFailed(m_copyCommandList->Close());

        return resource;
    }
    else
    {
        // If cross-adapter row-major textures are not supported by the adapter,
        // the texture will be copied over as a buffer so that the texture row
        // pitch can be explicitly managed.

        // Copy the intermediate render target into the shared buffer using the
        // memory layout prescribed by the render target.
        {
            D3D12_RESOURCE_DESC renderTargetDesc = m_devices[SecondaryAdapter].RenderTargets[m_backBufferIndex]->GetDesc();
            D3D12_PLACED_SUBRESOURCE_FOOTPRINT renderTargetLayout;
            m_devices[SecondaryAdapter].Device->GetCopyableFootprints(&renderTargetDesc, 0, 1, 0, &renderTargetLayout, nullptr, nullptr, nullptr);

            CD3DX12_TEXTURE_COPY_LOCATION dest(m_devices[SecondaryAdapter].CrossAdapterResources[m_backBufferIndex].Get(), renderTargetLayout);
            CD3DX12_TEXTURE_COPY_LOCATION src(m_devices[SecondaryAdapter].RenderTargets[m_backBufferIndex].Get(), 0);
            CD3DX12_BOX box(0, 0, m_outputWidth, m_outputHeight);
            m_copyCommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, &box);

            DX::ThrowIfFailed(m_copyCommandList->Close());
        }

        // Copy the buffer in the shared heap into a texture that the primary
        // adapter can sample from.
        {
            D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                m_primaryAdapterTextures[m_backBufferIndex].Get(),
                D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                D3D12_RESOURCE_STATE_COPY_DEST);
            m_devices[PrimaryAdapter].CommandList->ResourceBarrier(1, &barrier);

            // Copy the shared buffer contents into the texture using the memory
            // layout prescribed by the texture.
            D3D12_RESOURCE_DESC primaryAdapterTexture = m_primaryAdapterTextures[m_backBufferIndex]->GetDesc();
            D3D12_PLACED_SUBRESOURCE_FOOTPRINT textureLayout;
            m_devices[PrimaryAdapter].Device->GetCopyableFootprints(&primaryAdapterTexture, 0, 1, 0, &textureLayout, nullptr, nullptr, nullptr);

            auto resource = m_primaryAdapterTextures[m_backBufferIndex].Get();
            CD3DX12_TEXTURE_COPY_LOCATION dest(resource, 0);
            CD3DX12_TEXTURE_COPY_LOCATION src(m_devices[PrimaryAdapter].CrossAdapterResources[m_backBufferIndex].Get(), textureLayout);
            CD3DX12_BOX box(0, 0, m_outputWidth, m_outputHeight);
            m_devices[PrimaryAdapter].CommandList->CopyTextureRegion(&dest, 0, 0, 0, &src, &box);

            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            m_devices[PrimaryAdapter].CommandList->ResourceBarrier(1, &barrier);

            return resource;
        }
    }
    // return nullptr;
}

//  Submits the copy command list to the GPU
void GameBase::ExecuteCopyCommandList()
{
    m_copyCommandQueue.Get()->ExecuteCommandLists(
        1, CommandListCast(m_copyCommandList.GetAddressOf())
    );
}
#endif

// Presents the back buffer contents to the screen.
void GameBase::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
        MoveToNextFrame();
    }

    m_devices[PrimaryAdapter].GraphicsMemory->Commit(m_devices[PrimaryAdapter].CommandQueue.Get());
#if DXTK_MULTIGPU
    if (m_devices[SecondaryAdapter].Device)
    {
        m_devices[SecondaryAdapter].GraphicsMemory->Commit(m_devices[SecondaryAdapter].CommandQueue.Get());
    }
#endif
}

// Message handlers
void GameBase::OnActivated()
{
    // TODO: Game is becoming active window.
#if DXTK_KEYBOARD
    InputSystem.Keyboard.tracker.Reset();
#endif
}

void GameBase::OnDeactivated()
{
    // TODO: Game is becoming background window.

}

void GameBase::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    m_audioEngine->Suspend();
}

void GameBase::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
    m_audioEngine->Resume();

}

// Properties
void GameBase::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x180).
    width  = static_cast<int>(screen::width);
    height = static_cast<int>(screen::height);
}

// These are the resources that depend on the device.
void GameBase::CreateDevice()
{
    DWORD dxgiFactoryFlags = 0;

#if defined(_DEBUG) && DXTK_DEBUGLAYER
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }

        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
    }
#endif

    // Adapter 0 is the adapter that Presents frames to the display. It is assigned as
    // the "primary" adapter in this library.
    // Adapter 1 is an additional GPU that the app can take advantage of, but it does
    // not own the presentation step. It is assigned as the "secondary" adapter
    // in this library.
    DX::ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
    {
        const D3D_FEATURE_LEVEL FEATURE_LEVEL = D3D_FEATURE_LEVEL_11_0;

        ComPtr<IDXGIFactory6> factory6;
        ComPtr<IDXGIAdapter1> adapter;
        m_adapterCount = 0;

        auto CheckAndCreateDevice = [&]
        {
            DXGI_ADAPTER_DESC1 desc;
            DX::ThrowIfFailed(adapter->GetDesc1(&desc));
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                return;
            }

            // Create the DX12 API device object.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, IID_PPV_ARGS(m_devices[m_adapterCount].Device.ReleaseAndGetAddressOf()))))
            {
                if (m_adapterCount == 0)
                    m_venderId = desc.VendorId;
                ++m_adapterCount;
            }
        };

        if (SUCCEEDED(m_dxgiFactory->QueryInterface(IID_PPV_ARGS(factory6.GetAddressOf()))))
        {
            UINT adapterIndex = 0;
            while (true) {
                const HRESULT hr = factory6->EnumAdapterByGpuPreference(
                    adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                    IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())
                );
                if (hr == DXGI_ERROR_NOT_FOUND)
                    break;

                CheckAndCreateDevice();
                if (m_devices[AdapterCount - 1].Device != nullptr)
                    break;

                ++adapterIndex;
            }
        }
        else
        {
            UINT adapterIndex = 0;
            while (true) {
                const HRESULT hr = m_dxgiFactory->EnumAdapters1(
                    adapterIndex, adapter.ReleaseAndGetAddressOf()
                );
                if (hr == DXGI_ERROR_NOT_FOUND)
                    break;

                CheckAndCreateDevice();
                if (m_devices[AdapterCount - 1].Device != nullptr)
                    break;

                ++adapterIndex;
            }
        }

        if (!m_adapterCount)
        {
            ComPtr<IDXGIAdapter1> primaryAdapter;
            if (FAILED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(primaryAdapter.GetAddressOf()))))
            {
                throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
            }
            if (FAILED(D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, IID_PPV_ARGS(m_devices[m_adapterCount].Device.ReleaseAndGetAddressOf()))))
            {
                throw std::exception("No Direct3D 12 device found");
            }
            ++m_adapterCount;

            ComPtr<IDXGIAdapter1> secondaryAdapter;
            if (SUCCEEDED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(secondaryAdapter.GetAddressOf()))))
            {
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, IID_PPV_ARGS(m_devices[m_adapterCount].Device.ReleaseAndGetAddressOf()))))
                {
                    ++m_adapterCount;
                }
            }
        }
    }

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
#if DXTK_MULTIGPU
    for (UINT adapter = 0; adapter < m_adapterCount; ++adapter)
    {
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandQueue(
            &queueDesc,
            IID_PPV_ARGS(m_devices[adapter].CommandQueue.ReleaseAndGetAddressOf())
        ));
    }
#else
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommandQueue(
        &queueDesc,
        IID_PPV_ARGS(m_devices[PrimaryAdapter].CommandQueue.ReleaseAndGetAddressOf())
    ));

    // Create the compute command queue.
    if (m_devices[SecondaryAdapter].Device)
    {
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateCommandQueue(
            &queueDesc,
            IID_PPV_ARGS(m_devices[SecondaryAdapter].CommandQueue.ReleaseAndGetAddressOf())
        ));
    }
#endif

    // Create depth stencil views.
    D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc{};
    dsvDescriptorHeapDesc.NumDescriptors = 1;
    dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(m_dsvDescriptorHeap.ReleaseAndGetAddressOf())));

    // Create a command allocator and descriptor heaps.
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
    rtvDescriptorHeapDesc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    // PrimaryAdapter
    {
        const int adapter = PrimaryAdapter;

        // Create descriptor heaps for render target views and depth stencil views.
        rtvDescriptorHeapDesc.NumDescriptors = c_swapBufferCount + 1;
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateDescriptorHeap(
            &rtvDescriptorHeapDesc,
            IID_PPV_ARGS(m_devices[adapter].RTVDescriptorHeap.ReleaseAndGetAddressOf())
        ));
        m_devices[adapter].RTVDescriptorSize = m_devices[adapter].Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // Create a command allocator for each back buffer that will be rendered to.
        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(m_devices[adapter].CommandAllocators[frame].ReleaseAndGetAddressOf())
            ));
        }

        // Create a command list for recording graphics commands.
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_DIRECT,
            m_devices[adapter].CommandAllocators[0].Get(), nullptr,
            IID_PPV_ARGS(m_devices[adapter].CommandList.ReleaseAndGetAddressOf())
        ));
        DX::ThrowIfFailed(m_devices[adapter].CommandList->Close());
    }

    // Create a fence for tracking GPU execution progress.
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateFence(m_fenceValues[m_backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));
    ++m_fenceValues[m_backBufferIndex];

    // SecondaryAdapter
#if DXTK_MULTIGPU
    if (m_devices[SecondaryAdapter].Device)
    {
        const int adapter = SecondaryAdapter;
        // Create descriptor heaps for render target views and depth stencil views.
        rtvDescriptorHeapDesc.NumDescriptors = c_swapBufferCount;
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateDescriptorHeap(
            &rtvDescriptorHeapDesc,
            IID_PPV_ARGS(m_devices[adapter].RTVDescriptorHeap.ReleaseAndGetAddressOf())
        ));
        m_devices[adapter].RTVDescriptorSize = m_devices[adapter].Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_copyCommandQueue.ReleaseAndGetAddressOf())));
        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(m_devices[adapter].CommandAllocators[frame].ReleaseAndGetAddressOf())
            ));
            DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_COPY,
                IID_PPV_ARGS(m_copyCommandAllocators[frame].ReleaseAndGetAddressOf())));
        }

        // Create a command list for recording graphics commands.
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_DIRECT,
            m_devices[adapter].CommandAllocators[0].Get(), nullptr,
            IID_PPV_ARGS(m_devices[adapter].CommandList.ReleaseAndGetAddressOf())
        ));
        DX::ThrowIfFailed(m_devices[adapter].CommandList->Close());


        // Fence used by the secondary adapter to signal its copy queue that it has completed rendering.
        // When this is signaled, the secondary adapter's copy queue can begin copying to the cross-adapter shared resource.
        DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_renderFence.ReleaseAndGetAddressOf())
        ));

        // Cross-adapter shared fence used by both adapters.
        // Used by the secondary adapter to signal the primary adapter that it has completed copying to the cross-adapter shared resource.
        // When this is signaled, the prmary adapter can begin its work.
        DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateFence(
            0, D3D12_FENCE_FLAG_SHARED | D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER,
            IID_PPV_ARGS(m_devices[SecondaryAdapter].CrossAdapterFence.ReleaseAndGetAddressOf())
        ));

        // For now, require GENERIC_ALL access.
        HANDLE fenceHandle = nullptr;
        DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateSharedHandle(
            m_devices[SecondaryAdapter].CrossAdapterFence.Get(),
            nullptr,
            GENERIC_ALL,
            nullptr,
            &fenceHandle)
        );

        HRESULT openSharedHandleResult = m_devices[PrimaryAdapter].Device->OpenSharedHandle(
            fenceHandle, IID_PPV_ARGS(m_devices[PrimaryAdapter].CrossAdapterFence.ReleaseAndGetAddressOf())
        );

        // We can close the handle after opening the cross-adapter shared fence.
        CloseHandle(fenceHandle);

        DX::ThrowIfFailed(openSharedHandleResult);
    }

    // Create an event handle to use for frame synchronization.
    for (UINT adapter = 0; adapter < m_adapterCount; ++adapter)
    {
        m_devices[adapter].FenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
        if (!m_devices[adapter].FenceEvent.IsValid())
        {
            throw std::exception("CreateEvent");
        }

        m_devices[adapter].GraphicsMemory = std::make_unique<GraphicsMemory>(m_devices[adapter].Device.Get());
    }
#else
    {
        if (!m_devices[SecondaryAdapter].Device)
            m_devices[SecondaryAdapter].Device = m_devices[PrimaryAdapter].Device;

        const int adapter = SecondaryAdapter;

        // Create a compute command allocator for each back buffer that will be rendered to.
        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_COMPUTE,
                IID_PPV_ARGS(m_devices[adapter].CommandAllocators[frame].ReleaseAndGetAddressOf())
            ));
        }

        // Create a compute command list for recording graphics commands.
        DX::ThrowIfFailed(m_devices[adapter].Device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_COMPUTE,
            m_devices[adapter].CommandAllocators[0].Get(), nullptr,
            IID_PPV_ARGS(m_devices[adapter].CommandList.ReleaseAndGetAddressOf())
        ));
        DX::ThrowIfFailed(m_devices[adapter].CommandList->Close());
    }

    // Create an event handle to use for frame synchronization.
    m_devices[PrimaryAdapter].FenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
    if (!m_devices[PrimaryAdapter].FenceEvent.IsValid())
    {
        throw std::exception("CreateEvent");
    }

    m_devices[PrimaryAdapter].GraphicsMemory = std::make_unique<GraphicsMemory>(m_devices[PrimaryAdapter].Device.Get());
#endif
}

// Allocate all memory resources.
void GameBase::CreateResources()
{
    // Wait until all previous GPU work is complete.
    WaitForGpu();

    // Release resources that are tied to the swap chain and update fence values.
    for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
    {
        m_fenceValues[frame] = m_fenceValues[m_backBufferIndex];
    }

    const DXGI_FORMAT backBufferFormat  = DXGI_FORMAT_R8G8B8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
    const UINT backBufferWidth  = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(c_swapBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.Width  = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = c_swapBufferCount;
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Scaling    = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode  = DXGI_ALPHA_MODE_IGNORE;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a swap chain for the window.
        ComPtr<IDXGISwapChain1> swapChain;
        DX::ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
            m_devices[PrimaryAdapter].CommandQueue.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            swapChain.GetAddressOf()
        ));

        DX::ThrowIfFailed(swapChain.As(&m_swapChain));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        DX::ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the back buffers for this window which will be the final render targets
    // and create render target views for each of them.
    // PrimaryAdapter
    {
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
#if !DXTK_DEBUGLAYER && DXTK_sRGB
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
#else
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            DX::ThrowIfFailed(m_swapChain->GetBuffer(
                frame,
                IID_PPV_ARGS(m_devices[PrimaryAdapter].RenderTargets[frame].GetAddressOf())
            ));

            //wchar_t name[25] = {};
            //swprintf_s(name, L"Render target 0-%u", frame);
            //m_devices[PrimaryAdapter].RenderTargets[frame]->SetName(name);

            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
                m_devices[PrimaryAdapter].RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                static_cast<INT>(frame), m_devices[PrimaryAdapter].RTVDescriptorSize);
            m_devices[PrimaryAdapter].Device->CreateRenderTargetView(
                m_devices[PrimaryAdapter].RenderTargets[frame].Get(), &rtvDesc, rtvDescriptor
            );
        }
    }

#if DXTK_MULTIGPU
    // SecondaryAdapter
    if (m_devices[SecondaryAdapter].Device)
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
        m_swapChain->GetDesc1(&swapChainDesc);

        const CD3DX12_RESOURCE_DESC renderTargetDesc = CD3DX12_RESOURCE_DESC::Tex2D(
            swapChainDesc.Format,
            swapChainDesc.Width,
            swapChainDesc.Height,
            1u, 1u,
            swapChainDesc.SampleDesc.Count,
            swapChainDesc.SampleDesc.Quality,
            D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
            D3D12_TEXTURE_LAYOUT_UNKNOWN, 0u);

        const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        const CD3DX12_CLEAR_VALUE clearValue(swapChainDesc.Format, ClearColor);

        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
#pragma warning(push)
#pragma warning(disable:4238)
            DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAG_NONE,
                &renderTargetDesc,
                D3D12_RESOURCE_STATE_COMMON,
                &clearValue,
                IID_PPV_ARGS(m_devices[SecondaryAdapter].RenderTargets[frame].ReleaseAndGetAddressOf())));
#pragma warning(pop)
        }

        // Create cross-adapter shared resources on the secondary adapter, and open the shared handles on the primary adapter.
        {
            // Check whether shared row-major textures can be directly sampled by the
            // primary adapter. Support of this feature (or the lack thereof) will
            // determine our sharing strategy for the resource in question.
            D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
            DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CheckFeatureSupport(
                D3D12_FEATURE_D3D12_OPTIONS,
                reinterpret_cast<void*>(&options),
                sizeof(options))
            );
            m_crossAdapterTextureSupport = options.CrossAdapterRowMajorTextureSupported;

            UINT64 textureSize = 0;
            D3D12_RESOURCE_DESC crossAdapterDesc;
            if (m_crossAdapterTextureSupport)
            {
                // If cross-adapter row-major textures are supported by the adapter,
                // then they can be sampled directly.
                crossAdapterDesc = renderTargetDesc;
                crossAdapterDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER;
                crossAdapterDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                D3D12_RESOURCE_ALLOCATION_INFO textureInfo = m_devices[SecondaryAdapter].Device->GetResourceAllocationInfo(0, 1, &crossAdapterDesc);
                textureSize = textureInfo.SizeInBytes;
            }
            else
            {
                // If cross-adapter row-major textures are not supported by the adapter,
                // then they will be shared as buffers and then copied to a destination
                // texture on the secondary adapter.
                auto Align = [](UINT size, UINT alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)
                { return (size + alignment - 1) & ~(alignment - 1); };

                D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
                m_devices[SecondaryAdapter].Device->GetCopyableFootprints(&renderTargetDesc, 0, 1, 0, &layout, nullptr, nullptr, nullptr);
                textureSize = Align(layout.Footprint.RowPitch * layout.Footprint.Height);

                // Create a buffer with the same layout as the render target texture.
                crossAdapterDesc = CD3DX12_RESOURCE_DESC::Buffer(textureSize, D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER);
            }

            // Create a heap that will be shared by both adapters.
            CD3DX12_HEAP_DESC heapDesc(
                textureSize * c_swapBufferCount,
                D3D12_HEAP_TYPE_DEFAULT,
                0,
                D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);

            DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateHeap(
                &heapDesc,
                IID_PPV_ARGS(m_devices[SecondaryAdapter].CrossAdapterResourceHeap.ReleaseAndGetAddressOf())
            ));

            HANDLE heapHandle = nullptr;
            DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreateSharedHandle(
                m_devices[SecondaryAdapter].CrossAdapterResourceHeap.Get(),
                nullptr,
                GENERIC_ALL,
                nullptr,
                &heapHandle));

            HRESULT openSharedHandleResult = m_devices[PrimaryAdapter].Device->OpenSharedHandle(
                heapHandle, IID_PPV_ARGS(m_devices[PrimaryAdapter].CrossAdapterResourceHeap.ReleaseAndGetAddressOf())
            );

            // We can close the handle after opening the cross-adapter shared resource.
            CloseHandle(heapHandle);

            DX::ThrowIfFailed(openSharedHandleResult);

            // Create placed resources for each frame per adapter in the shared heap.
            for (UINT frame = 0; frame < c_swapBufferCount; frame++)
            {
                DX::ThrowIfFailed(m_devices[SecondaryAdapter].Device->CreatePlacedResource(
                    m_devices[SecondaryAdapter].CrossAdapterResourceHeap.Get(),
                    textureSize * frame,
                    &crossAdapterDesc,
                    D3D12_RESOURCE_STATE_COPY_DEST,
                    nullptr,
                    IID_PPV_ARGS(m_devices[SecondaryAdapter].CrossAdapterResources[frame].ReleaseAndGetAddressOf())
                ));

                DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreatePlacedResource(
                    m_devices[PrimaryAdapter].CrossAdapterResourceHeap.Get(),
                    textureSize * frame,
                    &crossAdapterDesc,
                    m_crossAdapterTextureSupport ? D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COPY_SOURCE,
                    nullptr,
                    IID_PPV_ARGS(m_devices[PrimaryAdapter].CrossAdapterResources[frame].ReleaseAndGetAddressOf())
                ));

                if (!m_crossAdapterTextureSupport)
                {
                    // If the secondary adapter's render target must be shared as a buffer,
                    // create a texture resource to copy it into on the primary adapter.
#pragma warning(push)
#pragma warning(disable:4238)
                    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommittedResource(
                        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                        D3D12_HEAP_FLAG_NONE,
                        &renderTargetDesc,
                        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                        nullptr,
                        IID_PPV_ARGS(m_primaryAdapterTextures[frame].ReleaseAndGetAddressOf())
                    ));
#pragma warning(pop)
                }
            }
        }
    }
#endif

    // Reset the index to the current back buffer.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
    // on this surface.
    CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        depthBufferFormat,
        backBufferWidth,
        backBufferHeight,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    depthOptimizedClearValue.Format = depthBufferFormat;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommittedResource(
        &depthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(m_depthStencil.ReleaseAndGetAddressOf())
    ));

    //m_depthStencil->SetName(L"Depth stencil");

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthBufferFormat;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    m_devices[PrimaryAdapter].Device->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    m_backBufferFormat  = backBufferFormat;
    m_depthBufferFormat = depthBufferFormat;
}

#if DXTK_D3D11 || DXTK_DWRITE
// These are the resources that depend on the Direct3D11 device.
void GameBase::D3D11CreateDevice()
{
    // Create an 11 device wrapped around the 12 device and share 12's command queue.
    UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;   // Required for Direct2D interoperability with Direct3D resources.
#ifdef _DEBUG
    d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    DX::ThrowIfFailed(D3D11On12CreateDevice(
        m_devices[PrimaryAdapter].Device.Get(),
        d3d11DeviceFlags,
        nullptr,
        0,
        reinterpret_cast<IUnknown**>(m_devices[PrimaryAdapter].CommandQueue.GetAddressOf()),
        1,
        0,
        m_d3d11Device.ReleaseAndGetAddressOf(),
        m_d3d11DeviceContext.ReleaseAndGetAddressOf(),
        nullptr
    ));
    DX::ThrowIfFailed(m_d3d11Device.As(&m_d3d11on12Device));

#if DXTK_DWRITE
    // Create the Direct2D.
    ComPtr<ID2D1Factory3> d2dfactory;
    D2D1_FACTORY_OPTIONS factoryOptions = {};
    DX::ThrowIfFailed(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        __uuidof(ID2D1Factory3),
        &factoryOptions,
        &d2dfactory
    ));

    // Query the 11On12 device from the 11 device.
    ComPtr<IDXGIDevice> dxgiDevice;
    DX::ThrowIfFailed(m_d3d11on12Device.As(&dxgiDevice));
    DX::ThrowIfFailed(d2dfactory->CreateDevice(dxgiDevice.Get(), m_d2dDevice.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_dWriteFactory));

    // Query the window's dpi settings.
    const float dpi = static_cast<float>(GetDpiForWindow(m_window));
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpi, dpi
    );

    for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
    {
        m_d2dRenderTargets  [frame].Reset();
        m_wrappedBackBuffers[frame].Reset();

        // Create a wrapped 11On12 resource of this back buffer. Since we are 
        // rendering all D3D12 content first and then all D2D content, we specify 
        // the In resource state as RENDER_TARGET - because D3D12 will have last 
        // used it in this state - and the Out resource state as PRESENT. When 
        // ReleaseWrappedResources() is called on the 11On12 device, the resource 
        // will be transitioned to the PRESENT state.
        D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
        DX::ThrowIfFailed(m_d3d11on12Device->CreateWrappedResource(
            m_devices[PrimaryAdapter].RenderTargets[frame].Get(),
            &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(m_wrappedBackBuffers[frame].GetAddressOf())
        ));

        // Create a render target for D2D to draw directly to this back buffer.
        ComPtr<IDXGISurface> surface;
        DX::ThrowIfFailed(m_wrappedBackBuffers[frame].As(&surface));
        DX::ThrowIfFailed(m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
            surface.Get(),
            &bitmapProperties,
            m_d2dRenderTargets[frame].GetAddressOf()
        ));
    }
#endif
}
#endif

#if DXTK_D3D9
// These are the resources that depend on the Direct3D9 device.
void GameBase::D3D9CreateDevice()
{
    //D3D9ON12_ARGS args{};
    //args.Enable9On12      = TRUE;
    //args.pD3D12Device     = m_devices[PrimaryAdapter].Device.Get();
    //args.ppD3D12Queues[0] = nullptr;    //m_d3d12UploadCommandQueue.Get();
    //args.NumQueues        = 0;          //1;
    //DX::ThrowIfFailed(Direct3DCreate9On12Ex(D3D_SDK_VERSION, &args, 1, m_d3d9.ReleaseAndGetAddressOf()));

    // Direct3D9 Object
    DX::ThrowIfFailed(Direct3DCreate9Ex(D3D_SDK_VERSION, m_d3d9.ReleaseAndGetAddressOf()));

    //D3DADAPTER_IDENTIFIER9 adapter_id;
    //m_d3d9->GetAdapterIdentifier(0, 0, &adapter_id);
    //if (adapter_id.VendorId == m_venderId) {
    //    D3D9ON12_ARGS args{};
    //    args.Enable9On12 = TRUE;
    //    DX::ThrowIfFailed(Direct3DCreate9On12Ex(D3D_SDK_VERSION, &args, 1, m_d3d9.ReleaseAndGetAddressOf()));
    //}

    // Direct3DDevice9 Object
    D3DPRESENT_PARAMETERS params{};
    params.BackBufferWidth        = m_outputWidth;
    params.BackBufferHeight       = m_outputHeight;
    params.BackBufferFormat       = D3DFMT_UNKNOWN;
    params.BackBufferCount        = 1;
    params.EnableAutoDepthStencil = TRUE;
    params.AutoDepthStencilFormat = D3DFMT_D24S8;
    params.hDeviceWindow          = m_window;
    params.MultiSampleType        = D3DMULTISAMPLE_NONE;
    params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    params.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT;
    params.Windowed               = TRUE;

    const DWORD   behavior[] = { D3DCREATE_MIXED_VERTEXPROCESSING    | D3DCREATE_MULTITHREADED,
                                 D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED };
    HRESULT hr = E_FAIL;
    for (int i = 0; i < _countof(behavior); i++) {
        hr = m_d3d9->CreateDeviceEx(
            0, D3DDEVTYPE_HAL, m_window,
            behavior[i], &params, nullptr,
            m_d3d9Device.ReleaseAndGetAddressOf()
        );
        if (hr == D3D_OK)
            break;
    }
    DX::ThrowIfFailed(hr);

    // Direct3DDevice9on12 Object
//  m_d3d9Device->QueryInterface(IID_PPV_ARGS(m_device9on12.ReleaseAndGetAddressOf()));

    // Render Target
    DX::ThrowIfFailed(D3DXCreateTexture(
        m_d3d9Device.Get(),
        m_outputWidth, m_outputHeight, 1,
        D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        m_d3d9RenderTexture.ReleaseAndGetAddressOf()
    ));
    DX::ThrowIfFailed(m_d3d9RenderTexture->GetSurfaceLevel(
        0, m_d3d9RenderSurface.ReleaseAndGetAddressOf()
    ));
    DX::ThrowIfFailed(m_d3d9Device->SetRenderTarget(0, m_d3d9RenderSurface.Get()));

    DX::ThrowIfFailed(m_d3d9Device->CreateOffscreenPlainSurface(
        m_outputWidth, m_outputHeight, D3DFMT_A8R8G8B8,
        D3DPOOL_SYSTEMMEM,
        m_d3d9CopySurface.ReleaseAndGetAddressOf(), nullptr
    ));

    m_d3d9Device->Clear(
        0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0
    );

    auto tex_desc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_B8G8R8A8_UNORM,
        static_cast<UINT64>(m_outputWidth),
        static_cast<UINT>  (m_outputHeight)
    );
    CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &tex_desc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_GRAPHICS_PPV_ARGS(m_d3d12RenderResource.ReleaseAndGetAddressOf()
    )));

    UINT64 total_bytes = 0;
    m_devices[PrimaryAdapter].Device->GetCopyableFootprints(
        &tex_desc, 0, 1, 0, &m_d3d12Footprint, nullptr, nullptr, &total_bytes
    );
 
    D3D12_RESOURCE_DESC up_desc{};
    up_desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    up_desc.Width            = total_bytes;
    up_desc.Height           = 1;
    up_desc.DepthOrArraySize = 1;
    up_desc.MipLevels        = 1;
    up_desc.SampleDesc.Count = 1;
    up_desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    heapProperties           = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &up_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_GRAPHICS_PPV_ARGS(m_d3d12UploadResource.ReleaseAndGetAddressOf()
    )));

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_COPY;
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommandQueue(
        &queueDesc,
        IID_PPV_ARGS(m_d3d12UploadCommandQueue.ReleaseAndGetAddressOf())
    ));

    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_COPY,
        IID_PPV_ARGS(m_d3d12UploadCommandAllocator.ReleaseAndGetAddressOf())
    ));

    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateCommandList(
        0, D3D12_COMMAND_LIST_TYPE_COPY,
        m_d3d12UploadCommandAllocator.Get(), nullptr,
        IID_PPV_ARGS(m_d3d12UploadCommandList.ReleaseAndGetAddressOf())
    ));
    DX::ThrowIfFailed(m_d3d12UploadCommandList->Close());

    DX::ThrowIfFailed(m_devices[PrimaryAdapter].Device->CreateFence(
        m_d3d12UploadFenceValue,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(m_d3d12UploadFence.ReleaseAndGetAddressOf())
    ));

    //m_d3d9Device->GetRenderTargetData(m_d3d9RenderSurface.Get(), m_d3d9CopySurface.Get());

    //D3DLOCKED_RECT lock;
    //m_d3d9CopySurface->LockRect(&lock, nullptr, D3DLOCK_READONLY);

    //ResourceUploadBatch resourceUploadBatch(m_d3d12UploadDevice.Get());
    //resourceUploadBatch.Begin();

    //D3D12_SUBRESOURCE_DATA subresource{};
    //subresource.pData      = lock.pBits;
    //subresource.RowPitch   = lock.Pitch;
    //subresource.SlicePitch = lock.Pitch * m_outputHeight;
    //DX::ThrowIfFailed(CreateTextureFromMemory(
    //    m_devices[PrimaryAdapter].Device.Get(), resourceUploadBatch,
    //    m_outputWidth, m_outputHeight, DXGI_FORMAT_B8G8R8A8_UNORM,
    //    subresource, m_d3d12RenderResource.ReleaseAndGetAddressOf(),
    //    false, D3D12_RESOURCE_STATE_COMMON
    //));

    //auto uploadResourcesFinished = resourceUploadBatch.End(m_d3d12UploadCommandQueue.Get());
    //uploadResourcesFinished.wait();

    //m_d3d9CopySurface->UnlockRect();
}

void GameBase::D3D9CreateShaderResourceView(const D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor)
{
    CreateShaderResourceView(
        m_devices[PrimaryAdapter].Device.Get(),
        m_d3d12RenderResource.Get(),
        srvDescriptor
    );
}

void GameBase::D3D9UpdateResource()
{
    //auto resource = m_d3d12RenderResource.Get();

    m_d3d9Device->GetRenderTargetData(m_d3d9RenderSurface.Get(), m_d3d9CopySurface.Get());

    void* upload_res;
    m_d3d12UploadResource->Map(0, nullptr, &upload_res);
    D3DLOCKED_RECT lock;
    m_d3d9CopySurface->LockRect(&lock, nullptr, D3DLOCK_READONLY);
    CopyMemory(upload_res, lock.pBits, lock.Pitch * m_outputHeight);
    m_d3d9CopySurface->UnlockRect();
    m_d3d12UploadResource->Unmap(0, nullptr);

    m_d3d12UploadCommandAllocator->Reset();
    m_d3d12UploadCommandList->Reset(m_d3d12UploadCommandAllocator.Get(), nullptr);

    static D3D12_TEXTURE_COPY_LOCATION tex_dst{};
    tex_dst.pResource        = m_d3d12RenderResource.Get();
    tex_dst.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

    static D3D12_TEXTURE_COPY_LOCATION tex_src;
    tex_src.pResource        = m_d3d12UploadResource.Get();
    tex_src.Type             = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    tex_src.SubresourceIndex = 0;
    tex_src.PlacedFootprint  = m_d3d12Footprint;
    m_d3d12UploadCommandList->CopyTextureRegion(&tex_dst, 0, 0, 0, &tex_src, nullptr);

    m_d3d12UploadCommandList->Close();
    m_d3d12UploadCommandQueue->ExecuteCommandLists(
        1, CommandListCast(m_d3d12UploadCommandList.GetAddressOf())
    );

    ++m_d3d12UploadFenceValue;
    m_d3d12UploadCommandQueue->Signal(m_d3d12UploadFence.Get(), m_d3d12UploadFenceValue);

    //D3DLOCKED_RECT lock;
    //m_d3d9CopySurface->LockRect(&lock, nullptr, D3DLOCK_READONLY);

    //ResourceUploadBatch resourceUploadBatch(m_d3d12UploadDevice.Get());
    //resourceUploadBatch.Begin();

    //D3D12_SUBRESOURCE_DATA subresource{};
    //subresource.pData      = lock.pBits;
    //subresource.RowPitch   = lock.Pitch;
    //subresource.SlicePitch = lock.Pitch * m_outputHeight;

    //resourceUploadBatch.Upload    (resource, 0, &subresource, 1);
    //resourceUploadBatch.Transition(
    //    resource, 
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    D3D12_RESOURCE_STATE_COMMON
    //);

    //auto uploadResourcesFinished = resourceUploadBatch.End(m_d3d12UploadCommandQueue.Get());
    //uploadResourcesFinished.wait();

    //m_d3d9CopySurface->UnlockRect();
}

void GameBase::D3D9WaitUpdate()
{
    if (m_d3d12UploadFence->GetCompletedValue() >= m_d3d12UploadFenceValue)
        return;

    HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!fenceEvent)
        return;
    m_d3d12UploadFence->SetEventOnCompletion(m_d3d12UploadFenceValue, fenceEvent);
    WaitForSingleObject(fenceEvent, INFINITE);
    CloseHandle(fenceEvent);
}
#endif

void GameBase::WaitForGpu() noexcept
{
    //if (!m_devices[PrimaryAdapter].CommandQueue || !m_fence || !m_devices[PrimaryAdapter].FenceEvent.IsValid())
    //{
    //    return;
    //}

    // Schedule a Signal command in the GPU queue.
    UINT64 fenceValue = m_fenceValues[m_backBufferIndex];
    if (FAILED(m_devices[PrimaryAdapter].CommandQueue.Get()->Signal(m_fence.Get(), fenceValue)))
    {
        return;
    }

    // Wait until the Signal has been processed.
    if (FAILED(m_fence->SetEventOnCompletion(fenceValue, m_devices[PrimaryAdapter].FenceEvent.Get())))
    {
        return;
    }
    WaitForSingleObjectEx(m_devices[PrimaryAdapter].FenceEvent.Get(), INFINITE, FALSE);

    // Increment the fence value for the current frame.
    ++m_fenceValues[m_backBufferIndex];
}

void GameBase::MoveToNextFrame()
{
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = m_fenceValues[m_backBufferIndex];
    DX::ThrowIfFailed(m_devices[PrimaryAdapter].CommandQueue.Get()->Signal(m_fence.Get(), currentFenceValue));

    // Update the back buffer index.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_fence->GetCompletedValue() < m_fenceValues[m_backBufferIndex])
    {
        DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIndex], m_devices[PrimaryAdapter].FenceEvent.Get()));
        WaitForSingleObjectEx(m_devices[PrimaryAdapter].FenceEvent.Get(), INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    m_fenceValues[m_backBufferIndex] = currentFenceValue + 1;
}

void GameBase::OnDeviceLost()
{
    // TODO: Perform Direct3D resource cleanup.
    m_scene->OnDeviceLost();

#if DXTK_D3D9
    m_d3d12RenderResource.Reset();
    m_d3d9CopySurface.Reset();
    m_d3d9RenderSurface.Reset();
    m_d3d9RenderTexture.Reset();
    m_d3d9Device.Reset();
#endif

#if DXTK_DWRITE
    for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
    {
        m_d2dRenderTargets[frame].Reset();
        m_wrappedBackBuffers[frame].Reset();
    }

    m_dWriteFactory.Reset();
    m_d2dDeviceContext.Reset();
    m_d2dDevice.Reset();
#endif

#if DXTK_D3D11
    m_d3d11DeviceContext.Reset();
    m_d3d11on12Device.Reset();
    m_d3d11Device.Reset();
#endif

#if DXTK_MULTIGPU
    if (m_devices[SecondaryAdapter].Device)
    {
        m_renderFence.Reset();

        m_crossAdapterTextureSupport = FALSE;
        for (UINT adapter = 0; adapter < m_adapterCount; ++adapter)
        {
            m_devices[adapter].CrossAdapterFence.Reset();
            for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
            {
                m_devices[adapter].CrossAdapterResources[frame].Reset();
            }
            m_devices[adapter].CrossAdapterResourceHeap.Reset();
        }

        m_copyCommandList.Reset();
        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            m_primaryAdapterTextures[frame].Reset();
            m_copyCommandAllocators[frame].Reset();
        }
        m_copyCommandQueue.Reset();
    }
#endif

    m_fence.Reset();
    m_dsvDescriptorHeap.Reset();
    m_depthStencil.Reset();
    m_swapChain.Reset();

    for (UINT adapter = 0; adapter < m_adapterCount; ++adapter)
    {
        m_devices[adapter].GraphicsMemory.reset();
        m_devices[adapter].RTVDescriptorHeap.Reset();

        for (UINT frame = 0; frame < c_swapBufferCount; ++frame)
        {
            m_devices[adapter].RenderTargets[frame].Reset();
            m_devices[adapter].CommandAllocators[frame].Reset();
        }

        m_devices[adapter].CommandList.Reset();
        m_devices[adapter].CommandQueue.Reset();

        m_devices[adapter].Device.Reset();
    }

    m_dxgiFactory.Reset();

    CreateDevice();
    CreateResources();

    m_scene->LoadAssets();
}
